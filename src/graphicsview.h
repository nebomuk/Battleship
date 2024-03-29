#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H


#include <QKeyEvent>
#include <QGraphicsView>
#include <QBasicTimer>
#include <QtOpenGL/QtOpenGL>
#include <QLCDNumber>
#include <QPointer>
#include "centerrectinrect.h"
#include "jsproxy.h"
#include "graphicsengine.h"
#include "gamestate.h"
#include "mainmenu.h"
#include "graphicssoftbutton.h"


class GraphicsView : public QGraphicsView
{
	Q_OBJECT
public:
	GraphicsView( QWidget * parent = 0);

public slots:

    void onApplicationStateChanged(Qt::ApplicationState state);

protected:

	// events
	virtual void timerEvent(QTimerEvent* event);
	void resizeEvent(QResizeEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

	// save window geometry
	void closeEvent(QCloseEvent *event) {writeSettings(); QGraphicsView::closeEvent(event);}

	// updates hitpoints and score
	void updateTopLevel();

	// initialization functions
	void createPlayerVehicle();
	void createScriptProxy();
	QGradient createBackgroundGradient() const;
	void createMainMenu();

	// read and save window position & size
	void readSettings();
	void writeSettings();

signals:
	void signalKeyPress(int);
	void signalKeyRelease(int);

private slots:
#ifdef Q_OS_ANDROID
    void hideDoublePressToExit();
#endif


private:
    void setPaused(bool b);
    bool isPaused();

	GraphicsEngine * graphicsEngine;
	QBasicTimer *timer;
    int timerTimeoutInterval_;
	QString applicationFolder;
	QPointer<Vehicle> playerVehicle; // automatically set to NULL if destroyed
    QProgressBar * hitpointsBar_;
    QPointer<QLCDNumber> highScoreCounter_;

    QPointer<JSProxy> scriptProxy;
	GameState * gameState;
	bool pixmapCaching_;

	MainMenu * mainMenu_;
	QGraphicsProxyWidget * mainMenuProxy_;

	int mainLoopCounter_;

    QList<GraphicsSoftButton* > leftSoftButtons_;
    QList<GraphicsSoftButton* > rightSoftButtons_;

    bool doubleBackToExitPressedOnce_;


    void adjustSoftButtonPositions(qreal borderSceneRectDist);
    void createHighScoreCounter();
    void createSoftButtons();
};
#endif




