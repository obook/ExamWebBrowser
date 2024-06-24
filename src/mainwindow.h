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
#include <QStackedWidget>

#include "toolbar.h"
#include "settings.h"
#include "network_client.h"
#include "clickablebutton.h"
#include "requestinterceptor.h"

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
    ClickableButton* PushButtonRight;
    QPushButton* PushButtonLeft;

    QTimer *FocusTimer;
    QTimer *NetworkTimer;

    Settings pSettings; /* static */
    network_client pNetclient; /* static */

    void CodeInputDialog();
    void ExitDialog();

    void setupToolBar();
    void SetupToolBarStyleFocusOn();
    void SetupToolBarStyleFocusOff();
    void updateToolBar();
    void handleButtonLeft();
    void UnlockWebView();
    void LockWebView();

    void updateNetwork();

    bool bFocusLost;
    bool bToogleColors;
    bool bWebViewHidden;
    bool DialogRun;
    int bFocusLostCounter;
    QLabel *TextBlockedlabel;
    QWebEnginePage * page;
    RequestInterceptor *interceptor;
    QWebEngineProfile * profile;
    QStackedWidget *stackedWidget;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

protected:
    void closeEvent(QCloseEvent *event) override;

public slots :
    void onFocusTimer();
    void onNetworkTimer();
    void onToolbarClicked();
    void onLabelClicked();
    void onButtonRightClicked();
    void onButtonLeftClicked();
};

#endif // MAINWINDOW_H
