#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <Qt3DQuickExtras/qt3dquickwindow.h>


int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
#endif

    QApplication app(argc, argv); // QWidget/QApplication required to include svg libs in android to avoid the "svg image format not supported" bug

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
//    Qt3DExtras::Quick::Qt3DQuickWindow view;
//        view.setSource(QUrl("qrc:/main.qml"));
//        view.show();


    return app.exec();
}
