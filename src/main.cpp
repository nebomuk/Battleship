#include <QApplication>
#include <QQmlApplicationEngine>
#include <QTime>
#include "graphicsview.h"
#include <QQmlContext>


int main(int argc, char ** argv)
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication app( argc, argv );
    app.setApplicationName("Battleship");
    app.setOrganizationName("Battleship");
	QString locale = QLocale::system().name();
	QTranslator translator;
	translator.load(':' + locale);
	app.installTranslator(&translator);



    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/src/main.qml")));

    GraphicsView graphicsView;

#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
    QObject::connect(&app,&QGuiApplication::applicationStateChanged,&graphicsView,&GraphicsView::onApplicationStateChanged);
#endif

    graphicsView.setWindowTitle("Battleship");
    graphicsView.setWindowIcon(QIcon(":/images2/ic_launcher.svg"));

    engine.rootContext()->setContextProperty("GraphicsView",&graphicsView);

	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	return app.exec();
}
