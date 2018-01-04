#include <QApplication>
#include <QTime>
#ifdef Q_OS_ANDROID
#include "androidhelper.h"
#endif
#include "graphicsview.h"

int main(int argc, char ** argv)
{
	QApplication app( argc, argv );
    app.setQuitOnLastWindowClosed(true);
	app.setApplicationName("battleship");
	app.setOrganizationName("Taiko");
	QString locale = QLocale::system().name();
	QTranslator translator;
	translator.load(':' + locale);
	app.installTranslator(&translator);

    // background image and tap gesture only work in landscape
//#ifdef Q_OS_ANDROID
//    AndroidHelper helper;
//    const int SCREEN_ORIENTATION_SENSOR_LANDSCAPE = 6;
//    helper.setScreenOrientation(SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
//#endif



	qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));  // seed the random number generator
	GraphicsView win;

#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
    QObject::connect(&app,&QGuiApplication::applicationStateChanged,&win,&GraphicsView::onApplicationStateChanged);
#endif

	win.setWindowTitle("Battleship");
	win.show();

	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	return app.exec();
}
