#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QPointer>
#include <QProgressBar>
#include "vehicle.h"
#include "uprisetorpedo.h"
#include "ballisticprojectile.h"

/*
this class provides a convennient interface to the script environment to place enemies in the scene

The createItemAt functions create items at the specified position
BUT drawing and bounding Rect is set to the center ( except for hitpointsBar)
 __________
|		   |
|		   |
|   (xy)   |
|		   |
|__________|

(xy) is the position that is returned by QGraphicsItem::pos()

the processT() functions except for processCollision must be called in e.g. timerEvent
if items should be removed if they move out of the area that can be visible, add them to (max) one of the
lists via one of the appendT functions

use setProperty("subSea",true) to automatically remove projectiles that go beyond sea level

*/


class GraphicsEngine : public QObject
{
Q_OBJECT


public:
	GraphicsEngine(QObject *parent = 0);

	/*Q_INVOKABLE*/ AnimatedSvgItem* createExplosionAt(const QPointF& position);

    /*Q_INVOKABLE*/ QProgressBar *createHitpointsBarAt(const QPointF& position);

	void setScene(QGraphicsScene * scene){scene_ = scene;}

	// returns true if the projectile collides, and removes vehicles whose hitpoints reach 0
	//playerVehicle pointer is automatically set to NULL
	bool processCollision(Vehicle * projectile);

	// must be called in timerEvent
	void processProjectiles(); // check for collisions and create an explosion if necessary
	void processLevelRect(); // remove submarines and backgroundItems that went went out of the level
	void processExplosions(); // remove explosions that finished its animation

	// returns a list of all submarines tha are automatically destroyed if they leave the levelRect
    Q_INVOKABLE QVariantList submarines();
    Q_INVOKABLE QVariantList projectiles();
    Q_INVOKABLE QVariantList backgroundItems();

	// add a submarine to submarines_. The list can be used to run AI scripts on the submarines_
	Q_INVOKABLE inline void appendSubmarine(Vehicle* submarine);

	// add a vehicle to projectiles_. processProjectiles() automatically checks for collision for each vehicle in
	// projectiles_
	Q_INVOKABLE inline void appendProjectile(Vehicle* projectile);

	// background items are automatically destroyed if they leave the levelRect
	Q_INVOKABLE void appendBackgroundItem(MovingItem * bgItem){backgroundItems_.push_back(bgItem);}

	// number of submarines that have been destroyed by projectiles, used for displaying highscore
	Q_INVOKABLE int destroyedSubmarineCount() const { return destroyedSubmarineCount_;}

	// items in backgroundItems_, submarines_ and projectiles_ that go beyond those bounds are automatically destroyed
	void setLevelRect(const QRectF& rect){levelRect_ = rect;}
	const QRectF & levelRect() const { return levelRect_;}

	bool pixmapCaching() const { return pixmapCaching_; }

public slots:
	// returns true if pixmapCaching for movingItems and explosions is enabled, this is false by default
	void setPixmapCaching(bool pixmapCaching){ pixmapCaching_ = pixmapCaching; }

	// these slots create an item at the specified position with the vector graphic svg
	Vehicle * createUpriseTorpedoAt(  const QPointF& position = QPointF(),
										   const QString& svg = QString());
	Vehicle * createUpriseTorpedoAt(  qreal xPos, qreal yPos,
										   const QString& svg = QString())
	{ return createUpriseTorpedoAt(QPointF(xPos,yPos),svg);}

	Vehicle * createBallisticProjectileAt(const QPointF& position  = QPointF(),
													  const QString& svg = QString());
	Vehicle * createBallisticProjectileAt(  qreal xPos, qreal yPos,
										   const QString& svg = QString())
	{ return createBallisticProjectileAt(QPointF(xPos,yPos),svg);}

	Vehicle * createVehicleAt(const QPointF& position = QPointF(),
									   const QString& svg = QString());
	Vehicle *  createVehicleAt(  qreal xPos, qreal yPos,
										   const QString& svg = QString())
	{ return  createVehicleAt(QPointF(xPos,yPos),svg);}

	MovingItem * createMovingItemAt(const QPointF& position = QPointF(),
									   const QString& svg = QString());
	MovingItem *  createMovingItemAt(  qreal xPos, qreal yPos,
										   const QString& svg = QString())
	{ return  createMovingItemAt(QPointF(xPos,yPos),svg);}

	// show text for msec miliseconds, by default, text is shown untill hideText is called
	void showText(const QString& text, int msec = 0);

	void hideText() { textSprite->hide();}


private:
	SvgCache * explosionCache;
	QList<AnimatedSvgItem*> explosions;
	QList<Vehicle*> projectiles_;
	QList<Vehicle*> submarines_;
	QList<MovingItem*> backgroundItems_;

	QRectF levelRect_;

	bool pixmapCaching_;

	QGraphicsSimpleTextItem * textSprite;

	QPointer<QGraphicsScene> scene_;
	QTimer hideTextTimer_;
	int destroyedSubmarineCount_;

};

void GraphicsEngine::appendSubmarine(Vehicle* submarine)
{
	if(!submarines_.contains(submarine))
	submarines_.push_back(submarine);
}

void GraphicsEngine::appendProjectile(Vehicle* projectile)
{
	if(!projectiles_.contains(projectile))
	projectiles_.push_back(projectile);
}

#endif // GRAPHICSENGINE_H
