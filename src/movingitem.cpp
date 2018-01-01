#include "movingitem.h"

MovingItem::MovingItem(QGraphicsItem * parent)
	:
	AnimatedSvgItem(parent),
	velocity_(0.0,0.0),
	maximumVelocity_(0.0,0.0),
	acceleration_(0.0,0.0),
	decceleration_(0.0,0.0)
{
}

/*static*/ void MovingItem::flipHorizontal(QGraphicsItem *s)
{
    s->setTransform(QTransform::fromScale(-1, 1));
    //s->translate(-s->boundingRect().width(),0);

}

/*static*/ void MovingItem::rotateMovingDirection(MovingItem * item)
{
			   qreal angle = 0.0;
			   if(!item->velocity_.isNull()) // atan2 mustn't be called with x==y==0.0
			   angle = -atan2(item->velocity_.x(),item->velocity_.y()) + global::PI;

			   // rotate in moving direction
			   item->setTransform(QTransform(cos(angle), sin(angle), -sin(angle), cos(angle), 0, 0));
}

void MovingItem::advance(int phase)
{
	if(isAdvancing() && phase == 1)
	{
		// slow down over timer
		if(qAbs(velocity().x()) > qAbs(decceleration().x()) || qAbs(velocity().y()) > qAbs(decceleration().y()))
		setVelocity(velocity() + QPointF(  (velocity().x() > 0 ? -decceleration().x() : +decceleration().x()),
			(velocity().y() > 0 ? -decceleration().y() : +decceleration().y())  ));

		setVelocity(velocity() + acceleration());
		setPos(pos() + velocity()); // move by velocity()

		AnimatedSvgItem::advance(phase); // advance animation
	}
}
