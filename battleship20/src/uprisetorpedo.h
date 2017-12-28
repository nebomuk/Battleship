#ifndef UPRISETORPEDO_H
#define UPRISETORPEDO_H

#include "vehicle.h"

// you can set acceleration for UpriseTorpedo objects
// only y part of acceleration is used

class UpriseTorpedo : public Vehicle
{
	Q_OBJECT

	Q_PROPERTY(bool autoRotate READ autoRotate WRITE setAutoRotate STORED true)
public:
	UpriseTorpedo();
	~UpriseTorpedo();

	// rotate in moving direction when advance() is called
	void setAutoRotate(bool autoRotate){ autoRotate_ = autoRotate; }
	bool autoRotate() const { return autoRotate_; }

	enum { Type = UserType+TYPE_UPRISETORPEDO};
	int type() const {return Type;} // Enable the use of qgraphicsitem_cast with this item.

protected:
	void advance(int phase);

private:
	qreal time;
	qreal launchVelocityY_;
	bool autoRotate_;

};
#endif
