
ScriptProxy.signalKeyPress.connect(onKeyPress);
ScriptProxy.signalKeyRelease.connect(onKeyRelease);

var keyLeftActive = false;
var keyRightActive = false;

//coordinate system 0,0 of PlayerVehicle is in topleft corner

function onKeyPress(key)
	{
	switch(key)
	{
		case Qt.Key_Left:
		case Qt.Key_A:
		{
			keyLeftActive = true;
				PlayerVehicle.xAcceleration = -0.18;
			break;
		}
		case Qt.Key_Right:
		case Qt.Key_D:
		{
			keyRightActive = true;
				PlayerVehicle.xAcceleration = 0.18;
			break;
		}
		case Qt.Key_S:
		{
			PlayerVehicle.file = ':images2/redSubmarine.svg';
			break;
		}
		case Qt.Key_Space:
		{
			var projectile = graphicsEngine.createBallisticProjectileAt(PlayerVehicle.x + PlayerVehicle.width,PlayerVehicle.y-10);
			/*var projectile = graphicsEngine.createUpriseTorpedoAt(PlayerVehicle.x + PlayerVehicle.width/2,
																		 PlayerVehicle.y + PlayerVehicle.height + 30);//*/
			projectile.xVelocity += PlayerVehicle.xVelocity;
			projectile.diplomacy = 1; // diplomacy of player
			break;
		}
		case Qt.Key_T:
		{
			var sub = graphicsEngine.createVehicleAt(512,512);
			//graphicsEngine.appendSubmarine(sub);
			graphicsEngine.showText("TestText",3000);
			break;
		}
		default:;
	}
}

function onKeyRelease(key)
{
switch (key)
	{
		case Qt.Key_Left:
		case Qt.Key_A:
		{
			keyLeftActive = false;
				if(keyRightActive)
					PlayerVehicle.xAcceleration = 0.18;
				else
					PlayerVehicle.xAcceleration = 0.0;
			break;
		}
		case Qt.Key_Right:
		case Qt.Key_D:
		{
			keyRightActive = false;
				if(keyLeftActive)
					PlayerVehicle.xAcceleration = 0.18;
				else
					PlayerVehicle.xAcceleration = 0.0;
			break;
		}
		default:;
	}
}

