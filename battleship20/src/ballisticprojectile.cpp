#include "ballisticprojectile.h"
//
BallisticProjectile::BallisticProjectile(QGraphicsItem * parent )
		:
		Vehicle(parent),
		waterFriction_(false),
		gravity_(6.0),
		launchVelocityY_(0.0)
{
	start(); // start Timer
}

BallisticProjectile::~BallisticProjectile()
{
}

void BallisticProjectile::advance(int phase)
{
	if (phase == 1)
	{
		if(qFuzzyCompare(elapsedTime(),0.0))
		{
			launchVelocityY_ = velocity().y();
		}
		advanceTime();

		if(waterFriction_ ) // under water
		{
			setXVelocity(velocity().x() * time/elapsedTime()); // x speed ---> 0
		}
		else // trajectory is a parabolic arc above sea level
		{
			time = elapsedTime();        // like physics formula if multiplied by globalTimer
			setYVelocity(- (launchVelocityY_ - gravity_ * time/4096.0) );  // change overall speed here
		}
		setPos(pos() + velocity()*scaleFactor()); // move by velocity()
	}
}


