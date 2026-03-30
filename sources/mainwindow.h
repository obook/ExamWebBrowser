/*
 * Nom : mainwindow.h
 * Description : Fenêtre principale du navigateur en mode examen.
 *               Gère le verrouillage, la détection de perte de focus et la navigation web.
 * Auteur : O. Booklage
 * Date : 2024
 */

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
#include "clickablebutton.h"
#include "requestinterceptor.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/* Indices des onglets dans le QStackedWidget */
static const int TAB_BLOCKED = 0;
static const int TAB_WEB     = 1;
static const int TAB_BASTHON = 2;

/* Intervalles des timers en millisecondes */
static const int FOCUS_TIMER_INTERVAL_MS   = 1000;
static const int NETWORK_TIMER_INTERVAL_MS = 15000;

/* Taille de police du message de blocage */
static const int BLOCKED_FONT_SIZE = 24;

/* Hauteur de la barre d'outils */
static const int TOOLBAR_HEIGHT = 40;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    /* Vues web et profils associés */
    QWebEngineView    *m_moodleWebview;
    QWebEnginePage    *m_moodlePage;
    RequestInterceptor *m_moodleInterceptor;
    QWebEngineProfile *m_moodleProfile;

    QWebEngineView    *m_basthonWebview;
    QWebEnginePage    *m_basthonPage;
    RequestInterceptor *m_basthonInterceptor;
    QWebEngineProfile *m_basthonProfile;

    /* Barre d'outils et boutons */
    ToolBar         *m_toolbar;
    ClickableButton *m_clockButton;
    QPushButton     *m_homeButton;
    QPushButton     *m_webButton;
    QPushButton     *m_basthonButton;

    /* Timers */
    QTimer *m_focusTimer;

    /* Configuration */
    Settings m_settings;

    /* État du verrouillage */
    bool m_focusLost       = false;
    bool m_toggleColors    = false;
    bool m_webViewHidden   = false;
    bool m_dialogRunning   = false;
    int  m_focusLostCounter = 0;

    /* Widgets */
    QLabel          *m_blockedLabel;
    QStackedWidget  *m_stackedWidget;

    /* Méthodes privées */
    void setupToolBar();
    void showCodeInputDialog();
    void showExitDialog();
    void updateToolBar();
    void handleHomeButton();
    void unlockWebView();
    void lockWebView();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

public slots:
    void onFocusTimer();
    void onLabelClicked();
    void onClockRightClicked();
    void onClockLeftClicked();
    void onWebButtonClicked();
    void onBasthonButtonClicked();
};

#endif /* MAINWINDOW_H */
