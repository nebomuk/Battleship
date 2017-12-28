#include <QApplication>
#include <QTime>

#include "graphicsview.h"

int main(int argc, char ** argv)
{
	QApplication app( argc, argv );
	app.setApplicationName("battleship");
	app.setOrganizationName("Taiko");
	QString locale = QLocale::system().name();
	QTranslator translator;
	translator.load(':' + locale);
	app.installTranslator(&translator);

	qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));  // seed the random number generator
	GraphicsView win;
	win.setWindowTitle("Battleship");
	win.show();

	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	return app.exec();
}
