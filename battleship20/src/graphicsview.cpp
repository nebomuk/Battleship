#include <QDesktopServices>
#include <QApplication>
#include <QSettings>
#include <QDir>
#include "graphicsview.h"
#include "globalvariables.h"
#include "svgcache.h"

#include "control.h"

#include <QtDebug>



// note internal resolution is 800*600 for positions of items

GraphicsView::GraphicsView( QWidget * parent)
		: QGraphicsView(parent)
{
	playerVehicle = NULL;
	hitpointsBar_ = NULL;
	highscoreCounter_ = NULL;
	mainMenu_ = NULL;
	mainMenuProxy_ = NULL;
	renderer_ = "Software";
	pixmapCaching_ = false;
	ai_ = NULL;
	emitter_ = NULL;
	mainLoopCounter_ = 1;

	// dataLocation is the location for this application's data
#ifdef Q_OS_LINUX
	 dataLocation = QString(QDir::homePath() + "/.battleship/");
#else
	 dataLocation = QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/battleship/";
#endif

	gameState = new GameState;
	readSettings();

	// disable scroll bars
	this->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	this->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

	// activates OpenGL
	// if you uncomment this, add opengl to the QT variable in the project file
	this->setViewport(renderer_ == "OpenGL" ? new QGLWidget(QGLFormat(QGL::SampleBuffers)) : new QWidget);
	// hide cursor
	//setCursor(Qt::BlankCursor);

	// optimizations
	this->setCacheMode(QGraphicsView::CacheBackground);

	this->setOptimizationFlags(QGraphicsView::DontClipPainter
										|QGraphicsView::DontSavePainterState
										|QGraphicsView::DontAdjustForAntialiasing);

	scene = new QGraphicsScene(0.0,0.0,800.0,600.0,this);
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

	createPlayerVehicle();

	createScriptProxy();

	highscoreCounter_ = new CounterItem;
	highscoreCounter_->setDigitsFile(":train_digits.svg");
	highscoreCounter_->setZValue(100.0);
	scene->addItem(highscoreCounter_);

	createMainMenu();

	this->setScene(scene);

	timer = new QBasicTimer;
	timer->start(global::TimeoutInterval,this); // starting global timer here

	// initial phase, upcoming "enemy phases" are automatically shown by the script
	graphicsEngine->showText(tr("Phase ") + QString::number(gameState->phase()),3000);
}

void GraphicsView::resizeEvent(QResizeEvent *event)
{
	QGraphicsView::resizeEvent(event);
	fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

	int width = event->size().width();
	int height= event->size().height();

	this->setMaximumHeight(width*600/800); // mustn't be higher than sceneRect

	qreal widthSceneCoords = width* (600.0/height);
	qreal borderSceneRectDist = (widthSceneCoords -800)/2.0;

	if(hitpointsBar_)
	{
		hitpointsBar_->setPos(800.0 + borderSceneRectDist -128,32.0);
	}
	if(highscoreCounter_)
	{
		highscoreCounter_->setPos(QPointF(0 - borderSceneRectDist +64.0, 32.0));
	}

}

void GraphicsView::timerEvent(QTimerEvent* event)
{
	// pause
	if((mainMenuProxy_ && mainMenuProxy_->isVisible()))
		return;

	scene->advance();

	if(scriptProxy)
		scriptProxy->timerEvent(event);

	graphicsEngine->processExplosions();
	graphicsEngine->processProjectiles();
	graphicsEngine->processLevelRect();
	updateTopLevel();

	//mainLoop();
}

void GraphicsView::togglePause()
{
	if(timer != NULL)
    {
		if(!timer->isActive())
			timer->start(global::TimeoutInterval,this);
		else if(timer->isActive())
			timer->stop();
    }

	graphicsEngine->showText(tr("Pause"));
}

void GraphicsView::keyPressEvent(QKeyEvent *event)
{
	// pause game
	switch(event->key())
	{
	case Qt::Key_Pause:
	{
		togglePause();
		break;
	}
	case Qt::Key_K:
	{
		qDebug() << scene->items().size();

		if(scriptProxy != NULL )
		{
			scriptProxy->disconnect();
			scriptProxy->deleteLater();
		}
		else
			createScriptProxy();

		break;
	}
	default:
		{
				emit signalKeyPress(event->key());
		}
	}
}

void GraphicsView::keyReleaseEvent(QKeyEvent *event)
{
	// quits the application
	if(event->key() == Qt::Key_Escape)
	{
		if(!mainMenuProxy_)
			return;

		mainMenu_->setCurrentIndex(0); // mainMenu
			mainMenuProxy_->show();

		return;
	}
		emit signalKeyRelease(event->key());
}

void GraphicsView::createScriptProxy()
{
//*
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
//*/

	//*replacement for control.js. To let the script handle user input, uncomment this and enable the above line
//	Control * control = new Control;
//	control->setPlayerVehicle(playerVehicle);
//	control->setGraphicsEngine(graphicsEngine);
//	connect(this,SIGNAL(signalKeyPress(int)),control,SLOT(onKeyPress(int)));
//	connect(this,SIGNAL(signalKeyRelease(int)),control,SLOT(onKeyRelease(int)));

	/*
	emitter_ = new Emitter;
	emitter_->setGraphicsEngine(graphicsEngine);
	ai_ = new AI;
	ai_->setGraphicsEngine(graphicsEngine);
	ai_->setPlayerVehicle(playerVehicle);
//*/
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

	connect(playerVehicle,SIGNAL(destroyed()),this,SLOT(togglePause()));
	scene->addItem(playerVehicle);
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
	if(playerVehicle && hitpointsBar_->frame() != playerVehicle->hitpoints())
		hitpointsBar_->setFrame(playerVehicle->hitpoints());

	if(highscoreCounter_ && highscoreCounter_->value() != graphicsEngine->destroyedSubmarineCount())
		highscoreCounter_->setValue(graphicsEngine->destroyedSubmarineCount());

	else if(!playerVehicle)
	{
		graphicsEngine->showText(tr("GAME OVER"));
		hitpointsBar_->setFrame(0);
	}
}

// read window geometry
void GraphicsView::readSettings()
{
	QSettings settings(dataLocation + "battleship.ini", QSettings::IniFormat,this);
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(800, 600)).toSize();
	renderer_ = settings.value("renderer",QString("Software")).toString();
	pixmapCaching_ = settings.value("pixmapCaching",false).toBool();
	gameState->setPhase(settings.value("phase",0).toInt());
	resize(size);
	move(pos);
}

// save window geometry
void GraphicsView::writeSettings()
{
	QSettings settings(dataLocation + "battleship.ini", QSettings::IniFormat,this);
	settings.setValue("pos", pos());
	settings.setValue("size", size());
	settings.setValue("renderer",renderer_);
	settings.setValue("pixmapCaching",mainMenu_ ? mainMenu_->cacheCheckBox->isChecked() : false);
	if(!settings.contains("phase"))
		settings.setValue("phase",0);
}

void GraphicsView::createMainMenu()
{
	mainMenu_ = new MainMenu;
	mainMenu_->setWindowOpacity(0.8);
	QComboBox * comboBox = mainMenu_->rendererComboBox;
	comboBox->setCurrentIndex(comboBox->findText(renderer_,Qt::MatchFixedString));

	mainMenu_->cacheCheckBox->setChecked(pixmapCaching_);

	connect(mainMenu_->rendererComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(setRenderer(QString)));
	connect(mainMenu_->quitButton,SIGNAL(clicked()),this,SLOT(close()));
	connect(mainMenu_->continueButton,SIGNAL(clicked()),mainMenu_,SLOT(hide()));

	connect(mainMenu_->cacheCheckBox,SIGNAL(toggled(bool)),graphicsEngine,SLOT(setPixmapCaching(bool)));

	mainMenuProxy_ = scene->addWidget(mainMenu_);
	mainMenuProxy_->setZValue(1000);
	mainMenuProxy_->setPos(centerRectInRect(scene->sceneRect(),mainMenuProxy_->geometry()));
	mainMenuProxy_->setFlag(QGraphicsItem::ItemIsMovable);
	mainMenuProxy_->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
	mainMenuProxy_->hide();
}

void GraphicsView::setRenderer(const QString& renderer)
{
	renderer_ = renderer;
	if(!mainMenu_)
		return;

	QMessageBox warning (QMessageBox::Warning, tr("Restart required"),
	tr("In order to take place for the changes, the application must be restarted."),
	QMessageBox::Ok,mainMenu_);

	warning.setWindowOpacity(mainMenu_->windowOpacity());
	warning.exec();
}

void GraphicsView::mainLoop()
{
	if(!ai_ || !emitter_)
	return;

	if(mainLoopCounter_ %gameState->phaseChangeCycles() != 0 || gameState->finalPhase())
	{
		++mainLoopCounter_;
		switch(gameState->phase())
		{
			case 0:
			{
				ai_->AIRoutine0();
				emitter_->emitter0();
				emitter_->backgroundEmitter();
				break;
			}
			case 1:
			{
				ai_->AIRoutine1();
				emitter_->emitter1();
				emitter_->backgroundEmitter();
				break;
			}
			case 2:
			{
				ai_->AIRoutine2();
				emitter_->emitter2();
				emitter_->backgroundEmitter();
				break;
			}
			case 3:
			{
				gameState->setFinalPhase(true); // disable timed phase change
				ai_->AIRoutine3();
				emitter_->emitter3();
				emitter_->backgroundEmitter();
				if(graphicsEngine->submarines().size() == 0)
					gameState->setPhase(gameState->phase() +1);
				break;
			}
		}
	}
	else
	{
		ai_->AIRoutineDefault();
		emitter_->backgroundEmitter();
		if(graphicsEngine->submarines().size() == 0 && graphicsEngine->projectiles().size() == 0)
		{
			gameState->setPhase(gameState->phase() +1);
			mainLoopCounter_ = 1;
			playerVehicle->setHitpoints(10);

			if(gameState->phase() == 4)
				graphicsEngine->showText("Victory", 5000);
			else if(gameState->phase() <4)
				graphicsEngine->showText("Phase " + QString::number(gameState->phase()),1000);
		}
	}

}
