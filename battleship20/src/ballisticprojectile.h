#ifndef BALLISTICPROJECTILE_H
#define BALLISTICPROJECTILE_H
//
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include "vehicle.h"
//

// a projectile with a ballistic trajectory

class BallisticProjectile : public Vehicle
{
	Q_OBJECT

	Q_PROPERTY(qreal gravity READ gravity WRITE setGravity)

public:
	BallisticProjectile(QGraphicsItem * parent = 0);
	~BallisticProjectile();

	// if set, projectile slows down when entering water
	void setWaterFriction(bool b){waterFriction_ = b;}
	bool waterFriction() const {return waterFriction_;}

	void setGravity(qreal gravity){gravity_ = gravity;}
	qreal gravity() const {return gravity_;}

	enum { Type = UserType+TYPE_BALLISTICPROJECTILE};
	int type() const {return Type;} // Enable the use of qgraphicsitem_cast with this item.

protected:
	void advance(int phase);

private:
	qreal waterFriction_;
	qreal gravity_;
	qreal launchVelocityY_;
	qreal time;
};
#endif
