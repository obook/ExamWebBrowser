#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QToolBar>
#include <QTimer>
#include <QEvent>
#include <QPushButton>
#include <QKeyEvent>
#include <QDateTime>
#include <QInputDialog>
#include <QWebEngineProfile>
#include <QWebEngineCookieStore>
#include <QMessageBox>
#include <QWebEnginePage>

#include "toolbar.h"
#include "settings.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void FocusLost();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QWebEngineView *webview;
    ToolBar* toolbar;
    QPushButton* PushButtonRight;
    QPushButton* PushButtonLeft;

    QTimer *monTimer;

    Settings *pSettings;
    QString MainUrl;
    QString ServerIp;

    void setupToolBar();
    void SetupToolBarStyleFocusOn();
    void SetupToolBarStyleFocusOff();
    void handleButtonRight();
    void handleButtonLeft();
    void UnlockWebView();
    void LockWebView();
    bool bFocusLost;
    bool bToogleColors;
    bool bWebViewHidden;
    bool DialogRun;
    int bFocusLostCounter;

protected:
    bool event(QEvent *event) override;

protected:
    void closeEvent(QCloseEvent *event) override;

public slots :
    void EndTimer();
    void slotLabelClicked();
    void slotToolbarClicked();
};
#endif // MAINWINDOW_H
