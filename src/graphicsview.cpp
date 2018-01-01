#include <QDesktopServices>
#include <QApplication>
#include <QSettings>
#include <QDir>
#include "graphicsview.h"
#include "globalvariables.h"
#include "svgcache.h"
#include <QtDebug>



// note internal resolution is 800*600 for positions of items

void GraphicsView::createHighScoreCounter()
{
    highScoreCounter_ = new QLCDNumber();
    highScoreCounter_->resize(highScoreCounter_->sizeHint() * 3);
    highScoreCounter_->setSegmentStyle(QLCDNumber::Flat);

    highScoreCounter_->setStyleSheet("color: black;"
                            "background-color: transparent;"
                                     );
    highScoreCounter_->setFrameStyle(QFrame::NoFrame);

    QGraphicsProxyWidget* proxy = scene()->addWidget(highScoreCounter_);
    proxy->setZValue(100.0);


}

GraphicsView::GraphicsView( QWidget * parent)
		: QGraphicsView(parent)
{
    playerVehicle = Q_NULLPTR;
    hitpointsBar_ = Q_NULLPTR;
    highScoreCounter_ = Q_NULLPTR;
    mainMenu_ = Q_NULLPTR;
    mainMenuProxy_ = Q_NULLPTR;
	pixmapCaching_ = false;
	mainLoopCounter_ = 1;
    doubleBackToExitPressedOnce_ = false;

	gameState = new GameState;
	readSettings();

	// disable scroll bars
	this->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	this->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

	// optimizations
	this->setCacheMode(QGraphicsView::CacheBackground);

	this->setOptimizationFlags(QGraphicsView::DontClipPainter
										|QGraphicsView::DontSavePainterState
										|QGraphicsView::DontAdjustForAntialiasing);

    QGraphicsScene * scene = new QGraphicsScene(0.0,0.0,800.0,600.0,this);
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	scene->setBackgroundBrush(createBackgroundGradient());
	setSceneRect(scene->sceneRect());

	//scene->addRect(sceneRect());


	graphicsEngine = new GraphicsEngine(this);
	graphicsEngine->setScene(scene);
	graphicsEngine->setPixmapCaching(pixmapCaching_);
	hitpointsBar_ = graphicsEngine->createHitpointsBarAt(QPointF(512,512));



	//scene->addRect(graphicsEngine->levelRect());

	setStyleSheet("QGraphicsView { border: none }");

    this->setScene(scene);

	createPlayerVehicle();

	createScriptProxy();

    createHighScoreCounter();

	createMainMenu();

#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
    createSoftButtons();
#endif

    adjustSoftButtonPositions(0);


    // you can adjust the gamespeed by changing TimeoutInterval
    // on windows the game has a "half speed" bug
#ifdef Q_OS_WIN32
    timerTimeoutInterval_ = 8;
#else
    timerTimeoutInterval_ = 40;
#endif

	timer = new QBasicTimer;
    timer->start(timerTimeoutInterval_,this); // starting global timer here

	// initial phase, upcoming "enemy phases" are automatically shown by the script
	graphicsEngine->showText(tr("Phase ") + QString::number(gameState->phase()),3000);
}



void GraphicsView::resizeEvent(QResizeEvent *event)
{
	QGraphicsView::resizeEvent(event);
    fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);

	int width = event->size().width();
	int height= event->size().height();

	this->setMaximumHeight(width*600/800); // mustn't be higher than sceneRect

	qreal widthSceneCoords = width* (600.0/height);
	qreal borderSceneRectDist = (widthSceneCoords -800)/2.0;

    if(hitpointsBar_ != Q_NULLPTR && hitpointsBar_->graphicsProxyWidget() != Q_NULLPTR)
	{
        hitpointsBar_->graphicsProxyWidget()->setPos(800.0 + borderSceneRectDist -256,32.0);
	}
    if(highScoreCounter_ != Q_NULLPTR && highScoreCounter_->graphicsProxyWidget() != Q_NULLPTR)
	{
        highScoreCounter_->graphicsProxyWidget()->setPos(QPointF(0 - borderSceneRectDist, 32.0));
	}

    adjustSoftButtonPositions(borderSceneRectDist);

}

void GraphicsView::timerEvent(QTimerEvent* event)
{
	// pause
	if((mainMenuProxy_ && mainMenuProxy_->isVisible()))
		return;

    scene()->advance();

	if(scriptProxy)
		scriptProxy->timerEvent(event);

	graphicsEngine->processExplosions();
	graphicsEngine->processProjectiles();
	graphicsEngine->processLevelRect();
	updateTopLevel();

	//mainLoop();
}

void GraphicsView::setPaused(bool b)
{
    if(!timer || !graphicsEngine || b == isPaused())
        return;

        if(isPaused())
        {
        // TimeoutInterval is a global variable
            timer->start(timerTimeoutInterval_,this);
            graphicsEngine->hideText();
        }
        else
        {
            timer->stop();
            graphicsEngine->showText(tr("Pause"));
        }
}

bool GraphicsView::isPaused()
{
    return !timer->isActive();
}

void GraphicsView::keyPressEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())
    {
        event->ignore();
        return;
    }

	// pause game
	switch(event->key())
	{
	case Qt::Key_Pause:
	{
        setPaused(!isPaused());
		break;
	}
	case Qt::Key_K:
	{
        qDebug() << scene()->items().size();

        if(scriptProxy != Q_NULLPTR )
		{
			scriptProxy->disconnect();
			scriptProxy->deleteLater();
		}
		else
			createScriptProxy();

		break;
	}
#ifdef Q_OS_ANDROID
    case Qt::Key_Back:
    {
        if(playerVehicle  == Q_NULLPTR || doubleBackToExitPressedOnce_)
        {
           this->close(); // back to menu
        }

        graphicsEngine->showText(tr("Press again to Exit"));
        doubleBackToExitPressedOnce_ = true;
        QTimer::singleShot(2000,this,&GraphicsView::hideDoublePressToExit);
        break;
    }
#endif
	default:
		{
				emit signalKeyPress(event->key());
		}
	}
}

#ifdef Q_OS_ANDROID
void GraphicsView::hideDoublePressToExit()
{
    graphicsEngine->hideText();
    doubleBackToExitPressedOnce_ = false;
}
#endif

void GraphicsView::keyReleaseEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())
    {
        event->ignore();
        return;
    }

	// quits the application
	if(event->key() == Qt::Key_Escape)
	{
		if(!mainMenuProxy_)
			return;

		mainMenu_->setCurrentIndex(0); // mainMenu
			mainMenuProxy_->show();

		return;
	}
    else if(event->key() == Qt::Key_Back)
    {
        event->accept();
        return;
    }
    emit signalKeyRelease(event->key());
}

void GraphicsView::createScriptProxy()
{

    scriptProxy = new JSProxy(this);
	connect(this,SIGNAL(signalKeyPress(int)),scriptProxy,SIGNAL(signalKeyPress(int)));
	connect(this,SIGNAL(signalKeyRelease(int)),scriptProxy,SIGNAL(signalKeyRelease(int)));

    scriptProxy->newQObjectWithName(gameState,"gameState");
    scriptProxy->newQObjectWithName(graphicsEngine,"graphicsEngine");

    scriptProxy->newQObjectWithName(playerVehicle,"PlayerVehicle");

    scriptProxy->evaluateFile( ":control.js");
    scriptProxy->evaluateFile(":ai.js");
    scriptProxy->evaluateFile(":emitter.js");
    scriptProxy->evaluateFile(":mainloop.js");

}

void GraphicsView::createPlayerVehicle()
{
	QPointF pos(100,0.44*600);

	playerVehicle = new Vehicle;
	playerVehicle->setFile(":images2/yellowBattleship.svg");
	playerVehicle->setPos(pos);
	playerVehicle->setDecceleration(QPointF(0.05,0));
	playerVehicle->setDiplomacy(1); // 1 is player's diplomacy
	playerVehicle->setMaximumVelocity(QPointF(1.5,1.5));
	playerVehicle->setZValue(3.0);
    playerVehicle->setHitpoints(10);

    connect(playerVehicle,&Vehicle::destroyed,[this]{setPaused(true);});
    scene()->addItem(playerVehicle);
}

QGradient GraphicsView::createBackgroundGradient() const
{
	QColor skyTop(0x71,0x9f,0xce);
	QColor skyBottom(0xb9,0xd0,0xe7);
	QColor seaTop(0x33,0x65,0xa2);
	QColor seaBottom(0x70,0x9e,0xcf);

	QLinearGradient gradient;
	gradient.setSpread(QGradient::RepeatSpread);
	gradient.setStart(0.5,0.0);
	gradient.setFinalStop(0.5,1.0);
	QGradientStops stops;
	stops << QGradientStop(0.0, skyBottom);
	stops << QGradientStop(0.5, skyTop.lighter());
	stops << QGradientStop(0.51, seaTop);
	stops << QGradientStop(1.0, seaBottom);
	gradient.setStops(stops);

	gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
	return gradient;
}

void GraphicsView::updateTopLevel()
{
    if(playerVehicle && hitpointsBar_->value() != playerVehicle->hitpoints())
        hitpointsBar_->setValue(playerVehicle->hitpoints());

    if(highScoreCounter_ &&  highScoreCounter_->intValue() != graphicsEngine->destroyedSubmarineCount())
        highScoreCounter_->display(graphicsEngine->destroyedSubmarineCount());

    else if(playerVehicle  == Q_NULLPTR)
	{
		graphicsEngine->showText(tr("GAME OVER"));
        hitpointsBar_->setValue(0);
	}
}



// read window geometry
void GraphicsView::readSettings()
{
    QSettings settings;
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(800, 600)).toSize();
    pixmapCaching_ = settings.value("pixmapCaching",false).toBool();
	gameState->setPhase(settings.value("phase",0).toInt());
	resize(size);
	move(pos);
}

// save window geometry
void GraphicsView::writeSettings()
{
    QSettings settings;
	settings.setValue("pos", pos());
	settings.setValue("size", size());
	settings.setValue("pixmapCaching",mainMenu_ ? mainMenu_->cacheCheckBox->isChecked() : false);
	if(!settings.contains("phase"))
		settings.setValue("phase",0);
}

void GraphicsView::createMainMenu()
{
	mainMenu_ = new MainMenu;
	mainMenu_->setWindowOpacity(0.8);

	mainMenu_->cacheCheckBox->setChecked(pixmapCaching_);

	connect(mainMenu_->quitButton,SIGNAL(clicked()),this,SLOT(close()));
	connect(mainMenu_->continueButton,SIGNAL(clicked()),mainMenu_,SLOT(hide()));

	connect(mainMenu_->cacheCheckBox,SIGNAL(toggled(bool)),graphicsEngine,SLOT(setPixmapCaching(bool)));

    mainMenuProxy_ = scene()->addWidget(mainMenu_);
	mainMenuProxy_->setZValue(1000);
    mainMenuProxy_->setPos(centerRectInRect(scene()->sceneRect(),mainMenuProxy_->geometry()));
	mainMenuProxy_->setFlag(QGraphicsItem::ItemIsMovable);
	mainMenuProxy_->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	mainMenuProxy_->hide();
}

void GraphicsView::adjustSoftButtonPositions(qreal borderSceneRectDist)
{

    qreal yDistanceFromTop = 336;

    for(int i = 0; i< leftSoftButtons_.size(); ++i)
    {
        GraphicsSoftButton *item = leftSoftButtons_[i];
        item->setPos(QPointF(0.0 - borderSceneRectDist + i*256,yDistanceFromTop));
    }

    for(int i = 0; i< rightSoftButtons_.size(); ++i)
    {
        GraphicsSoftButton *item = rightSoftButtons_[i];
        item->setPos(QPointF(800.0 + borderSceneRectDist -256-i*128,yDistanceFromTop));
    }
}

void GraphicsView::createSoftButtons()
{
    GraphicsSoftButton * moveRightButton = new GraphicsSoftButton(":images2/ic_arrow_forward_black_24px.svg");
    GraphicsSoftButton * moveLeftButton = new GraphicsSoftButton(":images2/ic_arrow_back_black_24px.svg");
    GraphicsSoftButton * shootButton = new GraphicsSoftButton(":images2/crosshairs.svg");

    leftSoftButtons_ = QList<GraphicsSoftButton*>()  << moveLeftButton << moveRightButton;
    rightSoftButtons_ = QList<GraphicsSoftButton*>()  <<  shootButton;
    QList<GraphicsSoftButton*> buttons = QList<GraphicsSoftButton*>() << leftSoftButtons_ << rightSoftButtons_;

    for(GraphicsSoftButton * item :  buttons)
    {
        item->setZValue(100.0);
        item->scaleToWidth(196.0);
        item->setOpacity(0.5);
        scene()->addItem(item);
    }

    connect(moveRightButton,&GraphicsSoftButton::pressed,[this](){
        emit signalKeyPress(Qt::Key_Right);
    });
    connect(moveRightButton,&GraphicsSoftButton::released, [this](){
        emit signalKeyRelease(Qt::Key_Right);
    });

    connect(moveLeftButton,&GraphicsSoftButton::pressed,[this](){
        emit signalKeyPress(Qt::Key_Left);
    });
    connect(moveLeftButton,&GraphicsSoftButton::released, [this](){
        emit signalKeyRelease(Qt::Key_Left);
    });

    connect(shootButton,&GraphicsSoftButton::pressed,[this](){
        emit signalKeyPress(Qt::Key_Space);
    });
    connect(shootButton,&GraphicsSoftButton::released, [this](){
        emit signalKeyRelease(Qt::Key_Space);
    });
}

void GraphicsView::onApplicationStateChanged(Qt::ApplicationState state)
{
    switch (state) {
    case Qt::ApplicationActive:
        setPaused(false);
        break;
    case Qt::ApplicationHidden:
    case Qt::ApplicationInactive:
    case Qt::ApplicationSuspended:
        setPaused(true);
        break;
    }
}

