

var counter = 0;
var backgroundCounter = 0;

function emitter0()
{
    if(++counter == 500)
		counter = 0;
    if(counter%50 == 0)
	{
		submarineEmitter0();
	}
}

function emitter1()
{
    if(++counter == 500)
		counter = 0;
    if(counter%100 == 0)
	{
		submarineEmitter1();
	}
}

function emitter2()
{
    if(++counter == 500)
		counter = 0;
    if(counter%100 == 0)
	{
		submarineEmitter2();
	}
}

function emitter3()
{
    if(counter <5000) // TODO set this to 900
	{
		submarineEmitter3();
	}
    counter = 5000;
}

function backgroundEmitter()
{
	++backgroundCounter;
    if(backgroundCounter%50 == 0)
	{
		grassEmitter();
	}
    if(backgroundCounter% 300 == 0)
	{
		cloudEmitter();
	}
}


function submarineEmitter0()
{
	var submarine = graphicsEngine.createVehicleAt(-800/2-128,480,':images2/orangeSubmarine.svg');
    submarine.xVelocity = 2 + Math.random();
	submarine.diplomacy = 2;
	graphicsEngine.appendSubmarine(submarine);
}

function submarineEmitter1()
{
	var submarine = graphicsEngine.createVehicleAt(-800/2-128,430,':images2/redSubmarine.svg');
    submarine.xVelocity = 2.0 + Math.random();
	submarine.diplomacy = 2;
	graphicsEngine.appendSubmarine(submarine);
}

function submarineEmitter2()
{
	var submarine = graphicsEngine.createVehicleAt(-800/2-128,400 + 180*Math.random(),':images2/blueSubmarine.svg');
    submarine.xVelocity = 3 + Math.random();
	submarine.diplomacy = 2;
	submarine.hitpoints = 2;
	graphicsEngine.appendSubmarine(submarine);
	var mine = graphicsEngine.createVehicleAt(800+800/2 + 128,300,':images2/seaMine.svg');
	mine.diplomacy = 2;
    mine.xVelocity = -1;
	graphicsEngine.appendProjectile(mine);
}

function submarineEmitter3()
{
	var boss = [4];
	boss[0]	= graphicsEngine.createVehicleAt(-800/2-128,400,':images2/bossSubmarine0.svg'); // upper left
	boss[1] = graphicsEngine.createVehicleAt(-800/2 -128 + 80, 400, ':images2/bossSubmarine1.svg'); // upper right
	boss[2] = graphicsEngine.createVehicleAt(-800/2-128, 435, ':images2/bossSubmarine2.svg'); // lower left
	boss[3] = graphicsEngine.createVehicleAt(-800/2-128 + 80, 435, ':images2/bossSubmarine3.svg'); // lower right

	for(var i = 0; i< boss.length; ++i)
	{
		boss[i].hitpoints = 10;
		boss[i].diplomacy = 2;
        boss[i].xVelocity = 3;
        boss[i].yVelocity = 3;
		graphicsEngine.appendSubmarine(boss[i]);
	}
}

function grassEmitter()
{
	var grass = graphicsEngine.createMovingItemAt(800.0+800/2.0,590,':weed.svg');
	grass.pixmapCaching = true;
	graphicsEngine.appendBackgroundItem(grass);
	grass.zValue = 50.0;
    grass.xVelocity = -1.6;
}

function cloudEmitter()
{
	var randomNumber = Math.random();
	var cloudfile = ':cloud_images/cloud' + Math.floor(randomNumber*3) + '.svg';
	var cloud = graphicsEngine.createMovingItemAt(800.0+800/2.0,50 + 200*randomNumber,cloudfile);
    cloud.xVelocity = -1;
	cloud.zValue = 0.0;
	graphicsEngine.appendBackgroundItem(cloud);
}
