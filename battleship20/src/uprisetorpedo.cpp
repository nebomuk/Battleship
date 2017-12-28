#include "uprisetorpedo.h"
//
UpriseTorpedo::UpriseTorpedo(  )
	: Vehicle(),
	launchVelocityY_(0.0),
	autoRotate_(true)

{
	start(); // start Timer
}

UpriseTorpedo::~UpriseTorpedo()
{
}

void UpriseTorpedo::advance(int phase)
{
	if(isAdvancing() && phase == 1)
	{
		// rotate to fit launch angle
		if(qFuzzyCompare(elapsedTime(),0.0))
		{
			rotateMovingDirection(this);
			launchVelocityY_ = velocity().y();
		}

		advanceTime();

		time = elapsedTime();
		setYVelocity(launchVelocityY_ - acceleration().y()  * time/4096.0);  // change overall speed here

		if(autoRotate_)
		rotateMovingDirection(this);

		setPos(pos() + velocity()*scaleFactor()); // move by velocity()
	}

}
