#ifndef MOVINGITEM_H
#define MOVINGITEM_H
//
#include "animatedsvgitem.h"
#include "globalvariables.h"
#include <QGraphicsScene>
#include "math.h"

//  a MovingItem object moves on its own if you set the velocity properties

// A maximumVelocity_ of 0.0 means the velocity is unbounded
class MovingItem : public AnimatedSvgItem
{
	Q_OBJECT

	Q_PROPERTY(qreal xVelocity READ xVelocity WRITE setXVelocity STORED false)
	Q_PROPERTY(qreal yVelocity READ yVelocity WRITE setYVelocity STORED false)

	Q_PROPERTY(qreal xAcceleration READ xAcceleration WRITE setXAcceleration STORED false)
	Q_PROPERTY(qreal yAcceleration READ yAcceleration WRITE setYAcceleration STORED false)

public:
	MovingItem(QGraphicsItem * parent = 0);

	inline void setVelocity(const QPointF& velocity);
	inline const QPointF& velocity() const {return velocity_;}

	inline void setMaximumVelocity(const QPointF& velocity){ maximumVelocity_ = velocity;}

	inline void setXVelocity(qreal x);
	inline void setYVelocity(qreal y);

	inline qreal xVelocity() const { return velocity_.x();}
	inline qreal yVelocity() const { return velocity_.y();}

	inline void setAcceleration(QPointF acceleration){acceleration_= acceleration;}
	inline const QPointF& acceleration() const {return acceleration_;}

	inline void setXAcceleration(qreal x) { acceleration_.setX(x);}
	inline void setYAcceleration(qreal y) { acceleration_.setY(y);}

	inline qreal xAcceleration() const { return acceleration_.x();}
	inline qreal yAcceleration() const { return acceleration_.y();}

	inline void setDecceleration(QPointF decceleration){decceleration_ = decceleration;}
	inline const QPointF& decceleration() const {return decceleration_;}

	static void rotateMovingDirection(MovingItem * item);
	Q_INVOKABLE void rotateMovingDirection() { rotateMovingDirection(this);}

	static void flipHorizontal(QGraphicsItem *s);
	Q_INVOKABLE void flipHorizontal(){flipHorizontal(this);}

	// compute the angle between x and y speed
	qreal radiansAngleFromVelocity() const {return -atan2(velocity_.x(),velocity_.y());}

	enum { Type = UserType+TYPE_MOVINGITEM};
	int type() const {return Type;} // Enable the use of qgraphicsitem_cast with this item.

protected:
	void advance(int phase);
	static inline qreal nonZeroBound(qreal value, qreal max);

private:
	QPointF velocity_;
	QPointF  maximumVelocity_;
	QPointF acceleration_;
	QPointF decceleration_;
};


// returns value bounded by absolute value of max or returns value if max == 0.0
/*static*/ inline qreal MovingItem::nonZeroBound(qreal value, qreal max)
{
	return qFuzzyCompare(max, 0.0) ? value : qBound(-max, value, max);
}

void MovingItem::setVelocity(const QPointF& velocity)
{
	if( maximumVelocity_.isNull())
	{
		velocity_ = velocity;
	}
	else
	{
		velocity_.setX(nonZeroBound(velocity.x(),maximumVelocity_.x()));
		velocity_.setY(nonZeroBound(velocity.y(),maximumVelocity_.y()));
	}
}

void MovingItem::setXVelocity(qreal x)
{
	velocity_.setX(nonZeroBound(x,maximumVelocity_.x()));
}

void MovingItem::setYVelocity(qreal y)
{
	velocity_.setY(nonZeroBound(y,maximumVelocity_.y()));
}
#endif

