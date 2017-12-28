#ifndef CONTROL_H
#define CONTROL_H

#include <Qt>
#include "vehicle.h"
#include "graphicsengine.h"

// This class is a c++ port of the control.js script.
// the port was necessary because user input didn't respond under high cpu loads

class Control : public QObject
{
	Q_OBJECT
public:
	//coordinate system 0,0 of PlayerVehicle is in topleft corner
	Control() : keyLeftActive(false),keyRightActive(false){}

	void setPlayerVehicle(Vehicle * v) { PlayerVehicle = v;}
	void setGraphicsEngine(GraphicsEngine * eng){ graphicsEngine = eng;}

public slots:
	void onKeyPress(int key)
		{
		if(PlayerVehicle)
		switch(key)
		{
			case Qt::Key_Left:
			case Qt::Key_A:
			{
				keyLeftActive = true;
					PlayerVehicle->setXAcceleration(-0.18);
				break;
			}
			case Qt::Key_Right:
			case Qt::Key_D:
			{
				keyRightActive = true;
					PlayerVehicle->setXAcceleration(0.18);
				break;
			}
			case Qt::Key_Space:
			{
				Vehicle* projectile = graphicsEngine->createBallisticProjectileAt(PlayerVehicle->x() + PlayerVehicle->width(),PlayerVehicle->y()-10);
				projectile->setXVelocity(projectile->xVelocity() + PlayerVehicle->xVelocity());
				projectile->setDiplomacy(1); // diplomacy of player
				break;
			}
			default:;
		}
	}

	void onKeyRelease(int key)
	{
	if(PlayerVehicle)
	switch (key)
		{
			case Qt::Key_Left:
			case Qt::Key_A:
			{
				keyLeftActive = false;
					if(keyRightActive)
						PlayerVehicle->setXAcceleration(0.18);
					else
						PlayerVehicle->setXAcceleration(0.0);
				break;
			}
			case Qt::Key_Right:
			case Qt::Key_D:
			{
				keyRightActive = false;
					if(keyLeftActive)
						PlayerVehicle->setXAcceleration(0.18);
					else
						PlayerVehicle->setXAcceleration(0.0);
				break;
			}
			default:;
		}
	}
	private:

	GraphicsEngine * graphicsEngine;
	QPointer<Vehicle> PlayerVehicle;

	bool keyLeftActive;
	bool keyRightActive;
};

#endif // CONTROL_H
