// all script functions that must be called regulary must be in here

ScriptProxy.signalTimerEvent.connect(mainLoop);
gameState.signalPhaseChanged.connect(phaseChange);


var mainLoopCounter = 1;

function mainLoop()
{
	if(mainLoopCounter %gameState.phaseChangeCycles != 0 || gameState.finalPhase)
	{
		++mainLoopCounter;
		switch(gameState.phase)
		{
			case 0:
			{
				AIRoutine0();
				emitter0();
				backgroundEmitter();
				break;
			}
			case 1:
			{
				AIRoutine1();
				emitter1();
				backgroundEmitter();
				break;
			}
			case 2:
			{
				AIRoutine2();
				emitter2();
				backgroundEmitter();
				break;
			}
			case 3:
			{
				gameState.finalPhase = true; // disable timed phase change
				AIRoutine3();
				emitter3();
				backgroundEmitter();
				if(graphicsEngine.submarines().length == 0)
					gameState.phase = gameState.phase +1;

				break;
			}
		}
	}
	else
	{
		AIRoutineDefault();
		backgroundEmitter();
        if(graphicsEngine.submarines().length === 0 && graphicsEngine.projectiles().length === 0)
		{
			gameState.phase = gameState.phase +1;
			mainLoopCounter = 1;
			PlayerVehicle.hitpoints = 10;
		}
	}
}

function phaseChange(newPhase) // argument not used
{
	if(newPhase == 4)
		graphicsEngine.showText(qsTr('Victory'), 5000);
	else if(newPhase <4)
		graphicsEngine.showText(qsTr('Phase ') + newPhase,3000);
}
