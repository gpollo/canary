#include <cstdlib>
#include <vector>

#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtWidgets/QApplication>

#include "CanBus.hpp"
#include "UserConfiguration.hpp"
#include "Component/CanSniffer.hpp"

int main(int argc, char* argv[]) {
    std::vector<char*> arguments;
    for (int i = 0; i < argc; i++) {
        arguments.push_back(argv[i]);
    }

    putenv("QT_QUICK_CONTROLS_STYLE=Material");

    argc = arguments.size();
    argv = arguments.data();

    Canary::UserConfiguration::load();
    Canary::CanBus::getInstance().loadConfig();

    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterType<Canary::CanDevice>("Canary", 1, 0, "CanDevice");
    qmlRegisterType<Canary::CanDatabase>("Canary", 1, 0, "CanDatabase");
    qmlRegisterType<Canary::Component::CanSniffer>("Canary", 1, 0, "CanSnifferModel");
    qRegisterMetaType<QList<Canary::CanDevice*>>("QList<Canary::CanDevice*>");
    qRegisterMetaType<QList<QPair<QString, QString>>>("QList<QPair<QString,QString>>");

    engine.rootContext()->setContextProperty("canBus", &Canary::CanBus::getInstance());
    engine.load(QUrl(QStringLiteral("qrc:/view/Main.qml")));

    return app.exec();
}
