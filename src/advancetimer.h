#ifndef ADVANCETIMER_H
#define ADVANCETIMER_H

// "timer" that measures the elapsed time in a QGraphicsItem::advance
// via calls to its advance function
// the timer is initially active unless you stop it explicitly
class AdvanceTimer
{

public:
	AdvanceTimer(int Interval = 0)
	: interval_(Interval), elapsedTime_(0),
	 timeout_(0),timingOut_(false),active_(false){}
	virtual ~AdvanceTimer(){}

	void setTimeout(int timeout){timeout_ = timeout;	}
	int timeout() const {return timeout_; }

	void setInterval ( int msec ) {interval_ = msec;}
	int interval () const { return interval_;}
	void setElapsedTime ( int msec ) {elapsedTime_ = msec;}
	int elapsedTime () const { return elapsedTime_;}

	void setTimingOut(bool isTimingOut){timingOut_ = isTimingOut;}
	bool isTimingOut() const {return timingOut_;}

	void start() {active_ = true;}
	void stop()  {active_ = false;}

	bool isActive() const {return active_;}

	void advanceTime() // always increases elapsedTime_ for compatibility with BallisticProjectile class
	{
		if(active_)
		elapsedTime_+= interval_;

		if(timingOut_ && elapsedTime_ >= timeout_)
		{
			active_ = false;
			elapsedTime_ = 0;
		}
	}

private:
	int interval_;
	int elapsedTime_;
	int timeout_;
	bool timingOut_;
	bool active_;

};
#endif
