#ifndef EMITTER_H
#define EMITTER_H

#include "graphicsengine.h"
#include "vehicle.h"

class Emitter
{

public:
	Emitter() : counter(0),backgroundCounter(0){}

void setGraphicsEngine(GraphicsEngine* engine){ graphicsEngine = engine; }

inline void emitter0();
inline void emitter1();
inline void emitter2();
inline void emitter3();
inline void backgroundEmitter();
inline void submarineEmitter0();
inline void submarineEmitter1();
inline void submarineEmitter2();
inline void submarineEmitter3();
inline void grassEmitter();
inline void cloudEmitter();

private:

int counter;
int backgroundCounter;

GraphicsEngine * graphicsEngine;
};

void Emitter::emitter0()
{
	if(++counter == 1000)
		counter = 0;
	if(counter%100 == 0)
	{
		submarineEmitter0();
	}
}

void Emitter::emitter1()
{
	if(++counter == 1000)
		counter = 0;
	if(counter%200 == 0)
	{
		submarineEmitter1();
	}
}

void Emitter::emitter2()
{
	if(++counter == 1000)
		counter = 0;
	if(counter%200 == 0)
	{
		submarineEmitter2();
	}
}

void Emitter::emitter3()
{
	if(counter <10000) // TODO set this to 900
	{
		submarineEmitter3();
	}
	counter = 10000;
}

void Emitter::backgroundEmitter()
{
	++backgroundCounter;
	if(backgroundCounter%100 == 0)
	{
		grassEmitter();
	}
	if(backgroundCounter% 600 == 0)
	{
		cloudEmitter();
	}
}


void Emitter::submarineEmitter0()
{
	Vehicle *  submarine = graphicsEngine->createVehicleAt(-800/2-128,480,":images2/orangeSubmarine.svg");
	submarine->setXVelocity( 1.0 + 0.55);
	submarine->setDiplomacy(2);
	graphicsEngine->appendSubmarine(submarine);
}

void Emitter::submarineEmitter1()
{
	Vehicle *  submarine = graphicsEngine->createVehicleAt(-800/2-128,430,":images2/redSubmarine.svg");
	submarine->setXVelocity( 1.0 + 0.55);
	submarine->setDiplomacy(2);
	graphicsEngine->appendSubmarine(submarine);
}

void Emitter::submarineEmitter2()
{
	Vehicle *  submarine = graphicsEngine->createVehicleAt(-800/2-128,400 + 180*0.55,":images2/blueSubmarine.svg");
	submarine->setXVelocity( 1.4 + 0.55);
	submarine->setDiplomacy(2);
	submarine->setHitpoints(2);
	graphicsEngine->appendSubmarine(submarine);
	Vehicle * mine = graphicsEngine->createVehicleAt(800+800/2 + 128,300,":images2/seaMine.svg");
	mine->setDiplomacy(2);
	mine->setXVelocity(-0.5);
	graphicsEngine->appendProjectile(mine);
}

void Emitter::submarineEmitter3()
{
	const int bossSize = 4;
	Vehicle *  boss[bossSize];
	boss[0]	= graphicsEngine->createVehicleAt(-800/2-128,400,":images2/bossSubmarine0.svg"); // upper left
	boss[1] = graphicsEngine->createVehicleAt(-800/2 -128 + 80, 400, ":images2/bossSubmarine1.svg"); // upper right
	boss[2] = graphicsEngine->createVehicleAt(-800/2-128, 435, ":images2/bossSubmarine2.svg"); // lower left
	boss[3] = graphicsEngine->createVehicleAt(-800/2-128 + 80, 435, ":images2/bossSubmarine3.svg"); // lower right

	for(int i = 0; i<  bossSize; ++i)
	{
		boss[i]->setHitpoints(3);
		boss[i]->setDiplomacy(2);
		boss[i]->setXVelocity(1.5);
		boss[i]->setYVelocity(1.5);
		graphicsEngine->appendSubmarine(boss[i]);
	}
}

void Emitter::grassEmitter()
{
	MovingItem*  grass = graphicsEngine->createMovingItemAt(800.0+800/2.0,590,":weed.svg");
	grass->setPixmapCaching(true);
	graphicsEngine->appendBackgroundItem(grass);
	grass->setZValue(50.0);
	grass->setXVelocity(-0.8);
}

void Emitter::cloudEmitter()
{
	int randomNumber = 0.5;
	QString cloudfile = ":cloud_images/cloud" + QString::number(1) + ".svg";
	MovingItem*   cloud = graphicsEngine->createMovingItemAt(800.0+800/2.0,50 + 200*randomNumber,cloudfile);
	cloud->setXVelocity(-0.5);
	cloud->setZValue(0.0);
	graphicsEngine->appendBackgroundItem(cloud);
}




#endif // EMITTER_H
