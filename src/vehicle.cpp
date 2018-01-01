#include "vehicle.h"
#include "globalvariables.h"

Vehicle::Vehicle(QGraphicsItem * parent)
	:
	MovingItem(parent),
    AdvanceTimer(40),
    hitpoints_(1),
	indestructible_(false),
    diplomacy_(0),
    subSea_(false)
{
}

Vehicle::~Vehicle()
{
}

bool Vehicle::subSea() const
{
    return subSea_;
}

void Vehicle::setSubSea(bool subSea)
{
    subSea_ = subSea;
}


