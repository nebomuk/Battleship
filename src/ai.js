
var shootCooldown = 0;

function AIRoutineDefault()
{
	var subs = graphicsEngine.submarines();
	for (var i = 0; i< subs.length; ++i)
	{
        subs[i].xVelocity = 6.5;
		subs[i].yVelocity = 0.0;
	}
}

function AIRoutine3()
{
	if(shootCooldown >0)
		--shootCooldown;

	var subs = graphicsEngine.submarines();

    if(subs.length !== 0 && subs[0].x > 800)
    for (var i = 0; i< subs.length; ++i)
        subs[i].xVelocity = -3;

    if(subs.length !== 0 && subs[0].x < -100)
	for (var i = 0; i< subs.length; ++i)
        subs[i].xVelocity = 3;

    if(subs.length !== 0 && subs[0].y > 550)
	for (var i = 0; i< subs.length; ++i)
        subs[i].yVelocity = -2;

    if(subs.length !== 0 && subs[0].y < 350)
	for (var i = 0; i< subs.length; ++i)
        subs[i].yVelocity = 2;

    if(shootCooldown === 0)
		for (var i = 0; i< subs.length; ++i)
		{
            shootCooldown = 50;
			var blueBomb = graphicsEngine.createBallisticProjectileAt(subs[i].x, subs[i].y);
			blueBomb.file = ':images2/blueBomb.svg'
			blueBomb.diplomacy = 2;
            blueBomb.yVelocity = 6.0;
			blueBomb.xVelocity = subs[i].xVelocity;
		}
}

function AIRoutine2()
{
	if(shootCooldown >0)
		--shootCooldown;

	var subs = graphicsEngine.submarines();
	for (var i = 0; i< subs.length; ++i)
	{
		if(shootCooldown == 0 && subs[i].x > PlayerVehicle.x -50 && subs[i].x < PlayerVehicle.x +200)
		{
			var blueBomb = graphicsEngine.createUpriseTorpedoAt(subs[i].x,subs[i].y);
			blueBomb.autoRotate = false;
			blueBomb.file = ':images2/blueBomb.svg';
			blueBomb.diplomacy = 2;
			blueBomb.subSea = true;
            shootCooldown = 20;
		}
	}
}


function AIRoutine1()
{
	if(shootCooldown >0)
		--shootCooldown;

	//var args = Array.prototype.slice.call(arguments);
	var subs = graphicsEngine.submarines();
	for (var i = 0; i< subs.length; ++i)
	{

		if(subs[i].y <380) // dont go near sea level
		{
            subs[i].yVelocity = 0.4;
			continue;
		}
		else if(subs[i].y > 550) // dont go too deep
		{
            subs[i].yVelocity = -0.4;
			continue;
		}

		if(shootCooldown == 0 && subs[i].x > PlayerVehicle.x -300 && subs[i].x < PlayerVehicle.x +100)
		{
			var torpedo = graphicsEngine.createVehicleAt(subs[i].x,subs[i].y);
			torpedo.diplomacy = 2;
            torpedo.yVelocity = -3;
			torpedo.rotateMovingDirection();
			torpedo.file = ':images2/greenTorpedo.svg';
			torpedo.subSea = true;
			graphicsEngine.appendProjectile(torpedo);
			shootCooldown = 100;
		}

		AIArray[i%AIArray.length](subs[i]);
	}
}


function AIRoutine0()
{
	//var args = Array.prototype.slice.call(arguments);
	var subs = graphicsEngine.submarines();
	for (var i = 0; i< subs.length; ++i)
	{
		if(subs[i].y <380) // dont go near sea level
		{
            subs[i].yVelocity = 0.4;
			continue;
		}
		else if(subs[i].y > 550) // dont go too deep
		{
            subs[i].yVelocity = -0.4;
			continue;
		}

		if(Math.random() < 0.002)
		{
			var torpedo = graphicsEngine.createVehicleAt(subs[i].x,subs[i].y);
			torpedo.diplomacy = 2;
            torpedo.yVelocity = -3;
			torpedo.rotateMovingDirection();
			torpedo.file = ':images2/greenTorpedo.svg';
			torpedo.subSea = true;
			graphicsEngine.appendProjectile(torpedo);
		}

		AIArray[i%AIArray.length](subs[i]);
	}
}

function updown(/* Vehicle* */ submarine)
{
	//*
		if(submarine.x > 0 && submarine.x < 350)
            submarine.yVelocity = 1;
		if(submarine.x > 351 && submarine.x < 450)
            submarine.yVelocity -=0.1;
		if(submarine.x > 451)
            submarine.yVelocity = -0.4;
			//*/
}

function sine(/* Vehicle* */ submarine)
{
			submarine.yVelocity = Math.sin(2*Math.PI/800.0*submarine.x);
}

var AIArray = [];

AIArray.push(updown);
AIArray.push(sine);



