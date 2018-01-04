#include "graphicsengine.h"
#include <QFont>
#include <QGraphicsProxyWidget>
#include <QProgressBar>

GraphicsEngine::GraphicsEngine(QObject *parent) :
	QObject(parent)
{
	{
		QList<QSvgRenderer*> renderers;
		for(int i = 0; i <14; ++i)
			renderers.push_back(new QSvgRenderer(":explosion_images1/explosion" + QString::number(i)+".svg"));

		explosionCache = new SvgCache;
		explosionCache->addSvgRenderers(renderers);
	}

	textSprite = new QGraphicsSimpleTextItem;
	QFont font( "Courier", 32);
	textSprite->setFont( font );
	textSprite->setZValue(20.0);

	qreal adjust = 128.0;
	levelRect_ =QRectF(-800.0/2.0,0.0,800.0+800.0,600.0).adjusted(-adjust,-adjust,adjust,adjust);

	pixmapCaching_ = false;

	hideTextTimer_.setSingleShot(true);
	connect(&hideTextTimer_,SIGNAL(timeout()),this,SLOT(hideText()));

	destroyedSubmarineCount_ = 0;
}

void GraphicsEngine::processExplosions()
{
	for(int i = 0; i < explosions.size(); ++i)
	{
		AnimatedSvgItem * explosion = explosions[i];

		if(explosion->loopsRemaining() == 0) // animation finished
		{ // remove
			explosions.removeAt(i);
			scene_->removeItem(explosion);
			delete explosion;
		}
	}
}

QVariantList GraphicsEngine::submarines() {

    QList<QVariant> variantList;
    for(auto obj : submarines_)
    {
        variantList << QVariant::fromValue(obj);
    }
    return variantList;

}

QVariantList GraphicsEngine::projectiles() {
    QList<QVariant> variantList;
    for(auto obj : projectiles_)
    {
        variantList << QVariant::fromValue(obj);
    }
    return variantList;

}

QVariantList GraphicsEngine::backgroundItems() {
    QList<QVariant> variantList;
    for(auto obj : backgroundItems_)
    {
        variantList << QVariant::fromValue(obj);
    }
    return variantList;


}

void GraphicsEngine::processProjectiles()
{
    for(int i = 0; i < projectiles_.size(); ++i)
	{
		Vehicle * projectile = projectiles_[i];

		if(!projectile)
		{
			projectiles_.removeAt(i);
			continue;
		}

        auto collided = processCollision(projectile);

        if(collided || !levelRect_.contains(projectile->pos())
           || (projectile->subSea() == true && projectile->y() < 0.52*600))
		{
			// projectile might has already been removed by the call to processCollision(projectile)
			// this happens if projectile hits another projectile
			if(projectiles_.removeOne(projectile))
			{
				scene_->removeItem(projectile);
				delete projectile;
			}
			continue;
		}
	}
}

bool GraphicsEngine::processCollision(Vehicle * projectile)
{
	// remove if collides with a vehicle
	if(!projectile)
		return false;

	bool collides = false;

	QList<QGraphicsItem*> collisions = projectile->collidingItems(Qt::IntersectsItemBoundingRect);
    for(QGraphicsItem* item : collisions)
		{
			Vehicle * vehicle = qgraphicsitem_cast<Vehicle*>(item);
			if(vehicle != Q_NULLPTR
			   && vehicle->diplomacy() != 0 // neutral is 0
			   && vehicle->diplomacy() != projectile->diplomacy()) // compare diplomacy
			{
				vehicle->setHitpoints(vehicle->hitpoints()-1); // decrease hitpoints
				if(!vehicle->indestructible() && vehicle->hitpoints() == 0) // if destroyed remove it
				{
					if(vehicle->diplomacy() == 2) // 2 is diplomacy of submarines
						++destroyedSubmarineCount_;

						if(!submarines_.removeOne(vehicle))
							projectiles_.removeOne(vehicle);

						scene_->removeItem(vehicle);
						delete vehicle;
				}
				collides = true;
				createExplosionAt(projectile->pos());
				break;
			}
		}
	return collides;
}

void GraphicsEngine::processLevelRect()
{
	for(int i = 0; i < submarines_.size(); ++i)
	{
		Vehicle * submarine = submarines_[i];

		if(submarine && !levelRect_.contains(submarine->pos()))
		{
			submarines_.removeAt(i);
			scene_->removeItem(submarine);
			delete submarine;
		}
	}
	for(int j = 0; j< backgroundItems_.size(); ++j)
	{
		MovingItem * backgroundItem = backgroundItems_[j];

		if(backgroundItem && !levelRect_.contains(backgroundItem->pos()))
		{
			backgroundItems_.removeAt(j);
			scene_->removeItem(backgroundItem);
			delete backgroundItem;
		}
	}
}

void GraphicsEngine::showText( const QString &text, int msec /* = 0 */)
{
	if(hideTextTimer_.isActive())
		hideTextTimer_.stop();

	if(msec > 0)
	{
		hideTextTimer_.setInterval(msec);
		hideTextTimer_.start();
	}

	if(!textSprite->scene())
		scene_->addItem(textSprite);

	textSprite->setText(text);

	// center
	textSprite->setPos( (scene_->width()-textSprite->boundingRect().width()) / 2,
							(scene_->height()-textSprite->boundingRect().height()) / 2 );

	textSprite->show();
}

AnimatedSvgItem* GraphicsEngine::createExplosionAt(const QPointF& position)
{
	AnimatedSvgItem * explosion = new AnimatedSvgItem;
	explosion->setPixmapCaching(pixmapCaching_);
	explosion->setSvgCache(explosionCache);
	explosion->setLoopCount(1);
    explosion->setFrameRateDivisor(2);
	explosion->setZValue(5.0);
	explosion->setOffset(AnimatedSvgItem::center(explosion));
	explosion->setPos(position);
	explosions.push_back(explosion);
	scene_->addItem(explosion);
	return explosion;
}

QProgressBar* GraphicsEngine::createHitpointsBarAt(const QPointF& position)
{

    QProgressBar * progress = new QProgressBar();
    progress->resize(80,30);
    progress->setRange(0,10);
    progress->setValue(5);
    progress->setTextVisible(false);
    progress->setStyleSheet("QProgressBar{"
                               "border: 1px solid transparent;"
                               "text-align: center;"
                               "color:rgba(0,0,0,100);"
                               "border-radius: 5px;"
                               "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(182, 182, 80, 100), stop:1 rgba(209, 209, 209, 100));"
                                   "}"
                               "QProgressBar::chunk{"
                               "background-color: rgba(0,255,0,100);"
                               "}");
    QGraphicsProxyWidget * proxy =  scene_->addWidget(progress);
    proxy->setZValue(100.0);
    proxy->setPos(position);
    return progress;
}

Vehicle * GraphicsEngine::createUpriseTorpedoAt(const QPointF& position /*=QPointF()*/,
										 const QString& svg /* = QString() */)
{
	UpriseTorpedo * torpedo = new UpriseTorpedo;
	torpedo->setZValue(4.0);
	torpedo->setDiplomacy(-1);
	torpedo->setFile(svg.isEmpty() ? ":images2/greenTorpedo.svg" : svg);
	torpedo->setOffset(Vehicle::center(torpedo)); // origin to center
    torpedo->setVelocity(QPointF(2.0,0.0));
    torpedo->setAcceleration(QPointF(0.0,8.0)); // only y part of acceleration is used
	torpedo->setPos(position);
	projectiles_.push_back(torpedo);
	scene_->addItem(torpedo);
	return torpedo;
}

Vehicle * GraphicsEngine::createBallisticProjectileAt(const QPointF& position /*=QPointF()*/,
											   const QString& svg /* = QString() */)
{
		BallisticProjectile * bomb = new BallisticProjectile;
		bomb->setZValue(4.0);
		bomb->setDiplomacy(-1);
		bomb->setFile(svg.isEmpty() ? ":images2/bomb.svg" : svg);
        bomb->setVelocity(QPointF(2.0,2.0));
		bomb->setOffset(Vehicle::center(bomb)); // origin to center
		bomb->setPos(position);
		projectiles_.push_back(bomb);
		scene_->addItem(bomb);
		return bomb;
}

Vehicle * GraphicsEngine::createVehicleAt(const QPointF& position /*=QPointF()*/,
									 const QString& svg /* = QString() */)
{
		Vehicle * vehicle = new Vehicle;
		vehicle->setZValue(3.0);
		vehicle->setFile(svg.isEmpty() ? ":images2/redSubmarine.svg" : svg);
		vehicle->setOffset(Vehicle::center(vehicle)); // origin to center
		vehicle->setPos(position);
		scene_->addItem(vehicle);
		return vehicle;
}

MovingItem * GraphicsEngine::createMovingItemAt(const QPointF& position,
								   const QString& svg)
{
		MovingItem * movingItem = new MovingItem;
		movingItem->setZValue(10.0);
		movingItem->setFile(svg.isEmpty() ? ":images2/weed.svg" : svg);
		movingItem->setOffset(MovingItem::center(movingItem)); // origin to center
		movingItem->setPos(position);
		scene_->addItem(movingItem);
		return movingItem;
}
