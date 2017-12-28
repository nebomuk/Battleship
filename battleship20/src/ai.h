#ifndef AI_H
#define AI_H

#include <QPointer>
#include "graphicsengine.h"
#include "vehicle.h"
#include "math.h"

class AI
{
public:
	AI() : shootCooldown(0) {}

	void setPlayerVehicle(Vehicle* vehicle){ PlayerVehicle = vehicle; }

	void setGraphicsEngine(GraphicsEngine* engine){ graphicsEngine = engine; }

	void AIRoutineDefault()
	{

        qDebug("method not supported");
//		QList<Vehicle*> subs = graphicsEngine->submarines();
//		for (int i = 0; i< subs.size(); ++i)
//		{
//			subs[i]->setXVelocity( 3.5);
//			subs[i]->setYVelocity(0.0);
//		}
	}

	void AIRoutine3()
	{
        qDebug("method not supported");

//		if(shootCooldown >0)
//			--shootCooldown;

//		QList<Vehicle*> subs = graphicsEngine->submarines();

//		if(subs.size() == 0)
//			return;

//		if(subs[0]->x() > 800)
//		for (int i = 0; i< subs.size(); ++i)
//			subs[i]->setXVelocity( -1.5);

//		if(subs[0]->x() < -100)
//		for (int i = 0; i< subs.size(); ++i)
//			subs[i]->setXVelocity( 1.5);

//		if(subs[0]->y() > 550)
//		for (int i = 0; i< subs.size(); ++i)
//			subs[i]->setYVelocity(-1.0);

//		if(subs[0]->y() < 350)
//		for (int i = 0; i< subs.size(); ++i)
//			subs[i]->setYVelocity(1.0);

//		if(shootCooldown == 0)
//			for (int i = 0; i< subs.size(); ++i)
//			{
//				shootCooldown = 150;
//				Vehicle *  blueBomb = graphicsEngine->createBallisticProjectileAt(subs[i]->x(), subs[i]->y());
//				blueBomb->setFile(":images2/blueBomb.svg");
//				blueBomb->setDiplomacy(2);
//				blueBomb->setYVelocity(3.0);
//				blueBomb->setXVelocity( subs[i]->xVelocity());
//			}
	}

	void AIRoutine2()
	{
        qDebug("method not supported");

//		if(!PlayerVehicle)
//			return;

//		if(shootCooldown >0)
//			--shootCooldown;

//		QList<Vehicle*> subs = graphicsEngine->submarines();
//		for (int i = 0; i< subs.size(); ++i)
//		{
//			if(shootCooldown == 0 && subs[i]->x() > PlayerVehicle->x() -50 && subs[i]->x() < PlayerVehicle->x() +200)
//			{
//				Vehicle * torpedo = graphicsEngine->createVehicleAt(subs[i]->x(),subs[i]->y());
//				torpedo->setYVelocity(-1.0);
//				torpedo->setXVelocity( 1.0);
//				torpedo->setDiplomacy(2);
//				torpedo->rotateMovingDirection();
//				torpedo->setFile(":images2/greenTorpedo.svg");
//				torpedo->setProperty("subSea",true);
//				graphicsEngine->appendProjectile(torpedo);
//				shootCooldown = 60;
//			}
//		}
	}


	void AIRoutine1()
	{
        qDebug("method not supported");

		if(shootCooldown >0)
			--shootCooldown;

//		//int args = Array.prototype.slice.call(arguments);
//		QList<Vehicle*> subs = graphicsEngine->submarines();
//		for (int i = 0; i< subs.size(); ++i)
//		{

//			if(subs[i]->y() <380) // dont go near sea level
//			{
//				subs[i]->setYVelocity(0.2);
//				continue;
//			}
//			else if(subs[i]->y() > 550) // dont go too deep
//			{
//				subs[i]->setYVelocity(-0.2);
//				continue;
//			}

//			if(!PlayerVehicle)
//				return;

//			if(shootCooldown == 0 && subs[i]->x() > PlayerVehicle->x() -300 && subs[i]->x() < PlayerVehicle->x() +100)
//			{
//				Vehicle * torpedo = graphicsEngine->createVehicleAt(subs[i]->x(),subs[i]->y());
//				torpedo->setDiplomacy(2);
//				torpedo->setVelocity(QPointF(0.0,-2.0));
//				torpedo->rotateMovingDirection();
//				torpedo->setFile(":images2/greenTorpedo.svg");
//				graphicsEngine->appendProjectile(torpedo);
//				torpedo->setProperty("subSea",true);
//				shootCooldown = 100;
//			}

//			if(i&1)
//			updown(subs[i]);
//			else
//			sine(subs[i]);
//		}
	}


	void AIRoutine0()
	{
        qDebug("method not supported");

//		//int args = Array.prototype.slice.call(arguments);
//		QList<Vehicle*> subs = graphicsEngine->submarines();
//		for (int i = 0; i< subs.size(); ++i)
//		{
//			if(subs[i]->y() <380) // dont go near sea level
//			{
//				subs[i]->setYVelocity(0.2);
//				continue;
//			}
//			else if(subs[i]->y() > 550) // dont go too deep
//			{
//				subs[i]->setYVelocity(-0.2);
//				continue;
//			}

//			if(!PlayerVehicle)
//				return;

//			// FIXME
//			/*
//			if(Math.random() < 0.002)
//			{
//				Vehicle * torpedo = graphicsEngine->createUpriseTorpedoAt(subs[i]->x,subs[i]->y());
//				torpedo->setDiplomacy(2);
//			}
//			//*/
//			if(i&1)
//			updown(subs[i]);
//			else
//			sine(subs[i]);
//		}
	}

	void updown(Vehicle *  submarine)
	{
			if(submarine->x() > 0 && submarine->x() < 350)
				submarine->setYVelocity(0.5);
			if(submarine->x() > 351 && submarine->x() < 450)
				submarine->setYVelocity(submarine->yVelocity() - 0.05);
			if(submarine->x() > 451)
				submarine->setYVelocity(-0.2);
	}

	void sine(Vehicle *  submarine)
	{
		submarine->setYVelocity( sin(2*global::PI/800.0*submarine->x()));
	}

private:
	QPointer<Vehicle> PlayerVehicle;
	GraphicsEngine * graphicsEngine;
	int shootCooldown;
};

#endif // AI_H
