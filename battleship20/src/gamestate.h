#include <QObject>


// this classed is used by the scripts to save its state if the script crashes
class GameState : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int phase WRITE setPhase READ phase)
	Q_PROPERTY(bool finalPhase READ finalPhase WRITE setFinalPhase)
	Q_PROPERTY(int phaseChangeCycles READ phaseChangeCycles)
public:
	GameState() : phase_(2), finalPhase_(false) {}
	void setPhase(int phase){phase_ = phase; emit signalPhaseChanged(phase);}
	int phase() const { return phase_;}
	bool finalPhase() const{ return finalPhase_;}
	void setFinalPhase(bool b){finalPhase_ = b;}
	int phaseChangeCycles() const { return 4000;}

signals:
	void signalPhaseChanged(int);
private:
	int phase_;
	bool finalPhase_;
};
