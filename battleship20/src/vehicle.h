#ifndef VEHICLE_H
#define VEHICLE_H

#include "movingitem.h"
#include "advancetimer.h"

// if you set the velocity properties inherited from MovingItem, a Vehicle object will move automatically

// if you want to use timers, put start() in your constructor and
// put advanceTimer() in your advance reimplementation

// if you want to compare types always use e.g.  myItem->type() == QGraphicsItem::UserType +TYPE_ITEMTYPE

class Vehicle : public MovingItem, protected AdvanceTimer
{
	Q_OBJECT

	Q_PROPERTY(int hitpoints READ hitpoints WRITE setHitpoints)
	Q_PROPERTY(bool indestructible READ indestructible WRITE setIndestructible)
	Q_PROPERTY(int diplomacy READ diplomacy WRITE setDiplomacy)
    Q_PROPERTY(int subSea READ subSea WRITE setSubSea) // must be set externally, helper for projectile
public:
	Vehicle(QGraphicsItem * parent = 0);
	~ Vehicle();

	void resetTimer() {setElapsedTime(0);}

	void setHitpoints(int hitpoints){hitpoints_ = hitpoints;}
	int hitpoints() const { return hitpoints_;}

	void setIndestructible(bool b){indestructible_ = b;}
	bool indestructible() const { return indestructible_;}

	// important for collision detection. by default, diplomacy is neutral == 0
	void setDiplomacy(int diplomacy){	diplomacy_ = diplomacy;}
	int diplomacy() const { return diplomacy_;}

	enum { Type = UserType+TYPE_VEHICLE};
	int type() const {return Type;} // Enable the use of qgraphicsitem_cast with this item.

    bool subSea() const;
    void setSubSea(bool subSea);

private:
    int hitpoints_;
    bool indestructible_;
    int diplomacy_;
    bool subSea_;
};
#endif
