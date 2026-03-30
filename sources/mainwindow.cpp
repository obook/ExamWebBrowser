/*
 * Nom : mainwindow.cpp
 * Description : Fenêtre principale — constructeur, destructeur et gestion des événements.
 * Auteur : O. Booklage
 * Date : 2024
 */

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QLabel>
#include <QMouseEvent>
#include <QLayout>
#include <QWebEngineProfile>
#include <QWebEngineUrlRequestInterceptor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowTitle("ExamWebBrowser");

    /* Barre d'outils */
    setupToolBar();

    /* Moteur web pour Moodle */
    m_moodleWebview = new QWebEngineView(this);
    m_moodleWebview->setContextMenuPolicy(Qt::NoContextMenu);

    connect(m_moodleWebview, &QWebEngineView::loadStarted, [] {
        QApplication::setOverrideCursor(Qt::BusyCursor);
    });

    connect(m_moodleWebview, &QWebEngineView::loadFinished, [] {
        QApplication::restoreOverrideCursor();
    });

    /* Moteur web pour Basthon */
    m_basthonWebview = new QWebEngineView(this);
    m_basthonWebview->setContextMenuPolicy(Qt::NoContextMenu);

    /* Intercepteurs de requêtes et profils isolés */
    m_moodleProfile = new QWebEngineProfile(m_moodleWebview);
    m_moodleInterceptor = new RequestInterceptor(m_moodleWebview);
    m_moodleProfile->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
    m_moodleProfile->setHttpCacheType(QWebEngineProfile::NoCache);
    m_moodleProfile->setHttpUserAgent(m_moodleProfile->httpUserAgent() + " ExamWebBrowser/1.0");
    m_moodleProfile->setUrlRequestInterceptor(m_moodleInterceptor);
    m_moodlePage = new QWebEnginePage(m_moodleProfile, m_moodleWebview);
    m_moodleWebview->setPage(m_moodlePage);

    m_basthonProfile = new QWebEngineProfile(m_basthonWebview);
    m_basthonInterceptor = new RequestInterceptor(m_basthonWebview);
    m_basthonProfile->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
    m_basthonProfile->setHttpCacheType(QWebEngineProfile::NoCache);
    m_basthonProfile->setUrlRequestInterceptor(m_basthonInterceptor);
    m_basthonPage = new QWebEnginePage(m_basthonProfile, m_basthonWebview);
    m_basthonWebview->setPage(m_basthonPage);
    m_basthonWebview->setUrl(QUrl("https://basthon.fr/"));
    m_basthonWebview->reload();

    /* URL par défaut depuis ExamWebBrowser.ini */
    QString url = m_settings.getUrl();
    if (url == "notice.html") {
        m_moodleWebview->load(QUrl::fromLocalFile(QApplication::applicationDirPath() + "/notice.html"));
    } else {
        m_moodleWebview->setUrl(QUrl(url));
    }

    /* Texte de blocage affiché lors de la perte de focus */
    m_blockedLabel = new QLabel(this);
    m_blockedLabel->setAlignment(Qt::AlignCenter);
    QFont font("Arial", BLOCKED_FONT_SIZE, QFont::Bold);
    m_blockedLabel->setFont(font);
    m_blockedLabel->setText(QString("MERCI D'APPELER UN SURVEILLANT"));

    /* Pile de widgets : texte de blocage / Moodle / Basthon */
    m_stackedWidget = new QStackedWidget(this);
    m_stackedWidget->addWidget(m_blockedLabel);
    m_stackedWidget->addWidget(m_moodleWebview);
    m_stackedWidget->addWidget(m_basthonWebview);

    setCentralWidget(m_stackedWidget);
    unlockWebView();

    /* Timer de surveillance du focus */
    m_focusTimer = new QTimer(this);
    connect(m_focusTimer, SIGNAL(timeout()), this, SLOT(onFocusTimer()));
    m_focusTimer->start(FOCUS_TIMER_INTERVAL_MS);
    updateToolBar();

    installEventFilter(this);
}

void MainWindow::unlockWebView() {
    m_focusLost = false;
    m_toolbar->setFocusOnStyle();
    m_webViewHidden = false;
    m_stackedWidget->setCurrentIndex(TAB_WEB);
    m_homeButton->setEnabled(true);
    m_webButton->setStyleSheet("QPushButton { background-color : blue }");
    m_basthonButton->setStyleSheet("QPushButton { background-color : normal }");
}

void MainWindow::lockWebView() {
    m_focusLost = true;
    m_toolbar->setFocusOffStyle();
    m_webViewHidden = true;
    m_stackedWidget->setCurrentIndex(TAB_BLOCKED);
    m_homeButton->setEnabled(false);
}

/* Détection de la perte de focus */
bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == this) {
        switch (event->type()) {
        case QEvent::WindowActivate:
            break;
        case QEvent::WindowDeactivate:
            if (!m_dialogRunning) {
                m_toolbar->setFocusOffStyle();
                lockWebView();
                m_focusLostCounter++;
            }
            break;
        default:
            break;
        }
    }

    return QWidget::eventFilter(obj, event);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    event->ignore();
}

MainWindow::~MainWindow() {
    delete m_moodlePage;
    delete m_moodleProfile;
    delete m_basthonPage;
    delete m_basthonProfile;
    delete ui;
}
