#ifndef QUICKWINDOW_H
#define QUICKWINDOW_H

#include <QQmlApplicationEngine>

class QWebEngineView;

class ApplicationEngine : public QQmlApplicationEngine {
    Q_OBJECT
public:
    ApplicationEngine();
};

#endif // QUICKWINDOW_H
