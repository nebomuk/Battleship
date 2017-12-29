#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H


#include <QKeyEvent>
#include <QGraphicsView>
#include <QBasicTimer>
#include <QtOpenGL/QtOpenGL>
#include <QPointer>
#include "centerrectinrect.h"
#include "jsproxy.h"
#include "graphicsengine.h"
#include "gamestate.h"
#include "mainmenu.h"


class GraphicsView : public QGraphicsView
{
	Q_OBJECT
public:
	GraphicsView( QWidget * parent = 0);

public slots:
	// set this to "Software" or "OpenGL", changes will be visible next start
	void setRenderer(const QString& renderer);

	// pauses the game
	void togglePause();

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

private:
	QGraphicsScene *  scene;
	GraphicsEngine * graphicsEngine;
	QBasicTimer *timer;
	QString applicationFolder;
	QPointer<Vehicle> playerVehicle; // automatically set to NULL if destroyed
	AnimatedSvgItem * hitpointsBar_;
    QPointer<QLCDNumber> highScoreCounter_;

    QPointer<JSProxy> scriptProxy;
	GameState * gameState;
	QString renderer_;
	bool pixmapCaching_;

	MainMenu * mainMenu_;
	QGraphicsProxyWidget * mainMenuProxy_;

	int mainLoopCounter_;
};
#endif




