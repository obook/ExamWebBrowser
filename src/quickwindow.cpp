#include "quickwindow.h"

#include "util.h"

#include <QFileInfo>
#include <QObject>
#include <QQmlContext>
#include <QQmlEngine>
#include <QUrl>

class Utils : public QObject {
    Q_OBJECT
public:
    Utils(QObject* parent = 0) : QObject(parent) { }
    Q_INVOKABLE static QUrl fromUserInput(const QString& userInput) { return urlFromUserInput(userInput); }
};

#include "quickwindow.moc"

ApplicationEngine::ApplicationEngine()
{
    rootContext()->setContextProperty("utils", new Utils(this));
    load(QUrl("qrc:/quickwindow.qml"));
    QMetaObject::invokeMethod(rootObjects().first(), "load", Q_ARG(QVariant, startupUrl()));
}
