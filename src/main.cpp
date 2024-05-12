#include "quickwindow.h"
#ifndef QT_NO_WIDGETS
#include <QtWidgets/QApplication>
typedef QApplication Application;
#else
#include <QtGui/QGuiApplication>
typedef QGuiApplication Application;
#endif
#include <QtWebEngine/qtwebengineglobal.h>

int main(int argc, char **argv)
{
    Application app(argc, argv);

    QtWebEngine::initialize();

    ApplicationEngine appEngine;

    return app.exec();
}
