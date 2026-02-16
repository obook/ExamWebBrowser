/*
 * mainwindows.cpp
 *
 * */

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QSettings>
#include "clickablelabel.h"
#include <QMouseEvent>
#include <QLayout>
#include <QWebEngineProfile>
#include <QWebEngineUrlRequestInterceptor>

#define OngletSurveillant 0
#define OngletWeb 1
#define OngletBasthon 2

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    setWindowTitle("ExamWebBrowser");

    /* Toobar */
    setupToolBar();

    /* WebEngine for Moodle */
    MoodleWebview = new QWebEngineView(this);
    MoodleWebview->setContextMenuPolicy(Qt::NoContextMenu);
/*
    QWebEngineProfile *Mprofile = MoodleWebview->page()->profile();
    Mprofile->clearHttpCache(); // Load = blocked ?
    Mprofile->clearAllVisitedLinks();
    Mprofile->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
    Mprofile->setHttpCacheType(QWebEngineProfile::NoCache);

    QWebEngineCookieStore * pCookie = Mprofile->cookieStore();
    pCookie->deleteAllCookies();
    pCookie->deleteSessionCookies();
*/
    connect(MoodleWebview, &QWebEngineView::loadStarted, [] {
        QApplication::setOverrideCursor(Qt::BusyCursor);
    });

    connect(MoodleWebview, &QWebEngineView::loadFinished, [] {
        QApplication::restoreOverrideCursor();
    });

    /* WebEngine for Basthon */
    BasthonWebview = new QWebEngineView(this);
    BasthonWebview->setContextMenuPolicy(Qt::NoContextMenu);



    /* Interceptors */
    MoodleProfile = new QWebEngineProfile(MoodleWebview);
    MoodleInterceptor = new RequestInterceptor(MoodleWebview);
    MoodleProfile->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
    MoodleProfile->setHttpCacheType(QWebEngineProfile::NoCache);
    MoodleProfile->setHttpUserAgent(MoodleProfile->httpUserAgent() + " ExamWebBrowser/1.0");
    MoodleProfile->setUrlRequestInterceptor(MoodleInterceptor);
    MoodlePage = new QWebEnginePage(MoodleProfile, MoodleWebview);
    MoodleWebview->setPage(MoodlePage);

    BasthonProfile = new QWebEngineProfile(BasthonWebview);
    BasthonInterceptor = new RequestInterceptor(BasthonWebview);
    BasthonProfile->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
    BasthonProfile->setHttpCacheType(QWebEngineProfile::NoCache);
    BasthonProfile->setUrlRequestInterceptor(MoodleInterceptor);
    BasthonPage = new QWebEnginePage(BasthonProfile, BasthonWebview);
    BasthonWebview->setPage(BasthonPage);
    BasthonWebview->setUrl(QUrl("https://basthon.fr/"));
    BasthonWebview->reload();

    /* Default URL founded in ExamWebBrowser.ini */
    QString URL = pSettings.GetUrl();
    if(URL == "notice.html") {
        MoodleWebview->load(QUrl::fromLocalFile(QApplication::applicationDirPath() + "/notice.html"));
    }
    else {
        MoodleWebview->setUrl(QUrl(URL));
    }

    /* Blocked Text (Label)  */
    TextBlockedlabel = new QLabel(this);
    TextBlockedlabel->setAlignment( Qt::AlignCenter );
    QFont font( "Arial", 24, QFont::Bold);
    TextBlockedlabel->setFont(font);
    TextBlockedlabel->setText(QString("MERCI D'APPELER UN SURVEILLANT"));

    /* QStackedWidget */
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(TextBlockedlabel); /* 0 */
    stackedWidget->addWidget(MoodleWebview); /* 1 */
    stackedWidget->addWidget(BasthonWebview); /* 2 */
    stackedWidget->setCurrentIndex(0); /* ne sert à rien ici ... */

    setCentralWidget(stackedWidget);

    UnlockWebView();

    /* Network - not used */
    // pNetclient = new network_client();

    /* Timers */
    FocusTimer = new QTimer(this);
    connect(FocusTimer, SIGNAL(timeout()), this,SLOT(onFocusTimer()));
    FocusTimer->start(1000);
    updateToolBar();

    NetworkTimer = new QTimer(this);
    connect(NetworkTimer, SIGNAL(timeout()), this,SLOT(onNetworkTimer()));
    NetworkTimer->start(15000);
    updateNetwork();

    /* Variables */
    bFocusLostCounter = 0;
    DialogRun = false;

    installEventFilter(this);
}

void MainWindow::setupToolBar() {
    toolbar = (ToolBar*)addToolBar(tr(""));
    toolbar->setFloatable(false);
    toolbar->setMovable(false);
    toolbar->setFixedHeight(40);
    toolbar->toggleViewAction()->setEnabled(false); // Disable QToolbar right menu
    toolbar->setContextMenuPolicy(Qt::NoContextMenu);
    toolbar->setContextMenuPolicy(Qt::PreventContextMenu);

    SetupToolBarStyleFocusOn();
    // not used : connect(toolbar , SIGNAL(clicked()), this, SLOT(onToolbarClicked()));

    /* Toolbar Left home button */
    PushButtonLeft = new QPushButton(this);
    QPixmap pixmap("images/home.svg");
    QIcon ButtonIcon(pixmap);
    PushButtonLeft->setIcon(ButtonIcon);
    toolbar->addWidget(PushButtonLeft);
    connect(PushButtonLeft, &QPushButton::released, this, &MainWindow::handleButtonLeft);

    /* Two buttons : Web and Python online */
    PushButtonWeb = new QPushButton(this);
    PushButtonWeb->setText("MOODLE");
    toolbar->addWidget(PushButtonWeb);
    connect(PushButtonWeb, &QPushButton::released, this, &MainWindow::onPushButtonWeb);

    PushButtonBasthon = new QPushButton(this);
    PushButtonBasthon->setText("BASTHON");
    toolbar->addWidget(PushButtonBasthon);
    connect(PushButtonBasthon, &QPushButton::released, this, &MainWindow::onPushButtonBasthon);
    /* For instance ... */
    PushButtonBasthon->setEnabled(false);

    /* Toolbar Separator */
    QWidget *spacerWidget1 = new QWidget(this);
    spacerWidget1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget1->setVisible(true);
    toolbar->addWidget(spacerWidget1);

    /* Toolbar title */
    QLabel *Label = new ClickableLabel(this);
    Label->setText(pSettings.GetAppName());
    Label->setStyleSheet("* { background-color: rgba(0,125,0,0) }");
    toolbar->addWidget(Label);
    connect(Label , SIGNAL(clicked()), this, SLOT(onLabelClicked()));

    /* Toolbar Separator */
    QWidget *spacerWidget2 = new QWidget(this);
    spacerWidget2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget2->setVisible(true);
    toolbar->addWidget(spacerWidget2);

    /* Toolbar Right clock button */
    PushButtonRight = new ClickableButton(this);
    PushButtonRight->setFlat(true);
    PushButtonRight->setStyleSheet("* { background-color: rgba(0,125,0,0) }");
    toolbar->addWidget(PushButtonRight);
    connect(PushButtonRight, SIGNAL(rightClicked()), this, SLOT(onButtonRightClicked()));
    connect(PushButtonRight, SIGNAL(leftClicked()), this, SLOT(onButtonLeftClicked()));
    PushButtonRight->installEventFilter(this);
}

void MainWindow::onButtonRightClicked(){

    if (!bFocusLost) {
        ExitDialog();
    }
}

void MainWindow::onButtonLeftClicked(){

    if (bFocusLost==true) {
        CodeInputDialog();
    }
}

void MainWindow::CodeInputDialog() {
    DialogRun = true;
    bFocusLostCounter--;

    bool ok;
    QInputDialog InputDialog = new QInputDialog;
    InputDialog.setWindowTitle("EWB");
    InputDialog.setCancelButtonText(QString("Abandon")); // Do not work
    InputDialog.setOkButtonText(QString("Valider")); // Do not work

    QString text = InputDialog.getText(this, "EWB", "Code de déverrouillage:", QLineEdit::Password, "", &ok);
    if ( ok && !text.isEmpty() ) {
        QString code_secret = QDateTime::currentDateTime().toString("ddMM"); /* Eg 1605 pour le 16 mai */
        /* Si la chaine saisie contient le code secret */
        if(text.contains(code_secret)) {
            bFocusLostCounter=0;
            UnlockWebView();
        }
    }

    DialogRun = false;
}

void MainWindow::ExitDialog() {
    DialogRun = true;
    bFocusLostCounter--;

    QMessageBox msgBox;
    msgBox.setWindowTitle("EWB");
    msgBox.setText("Quitter l'application?");
    msgBox.setInformativeText("Tout travail non enregistré sera perdu.");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.button(QMessageBox::Ok)->setText("Quitter");
    msgBox.button(QMessageBox::Cancel)->setText("Rester");
    msgBox.setDefaultButton(QMessageBox::Cancel);
    msgBox.setIcon(QMessageBox::Critical);
    if( msgBox.exec() == QMessageBox::Ok ) {
        QCoreApplication::exit();
    }
    else {
        UnlockWebView();
    }
    DialogRun = false;
}

void MainWindow::onToolbarClicked() {
    //qDebug() <<  QDateTime::currentMSecsSinceEpoch(), " onToolbarClicked";
}

void MainWindow::onLabelClicked() {
    DialogRun = true;
    bFocusLostCounter--;

    QMessageBox msgBox;
    msgBox.setWindowTitle("EWB");
    msgBox.setText("Navigateur Mode Examen");
    msgBox.setInformativeText("(C) obooklage 2024");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();

    DialogRun = false;
}

void MainWindow::SetupToolBarStyleFocusOn() {
    int radius = 1;

    QString styleSheet(QString(
                           "QToolBar "
                           "{background-color: rgba(50,255,50,128); "
                           "border-radius: %1px;} "
                           "QToolButton "
                           "{max-width: 48px; "
                           "max-height: 49px; "
                           "border: 0px;}").
                       arg(radius));
    toolbar->setStyleSheet(styleSheet);
}

void MainWindow::SetupToolBarStyleFocusOff() {
    int radius = 1;

    QString styleSheet(QString(
                           "QToolBar "
                           "{background-color: rgba(255,50,50,128); "
                           "border-radius: %1px;} "
                           "QToolButton "
                           "{max-width: 48px; "
                           "max-height: 49px; "
                           "border: 0px;}").
                       arg(radius));
    toolbar->setStyleSheet(styleSheet);
}

/* Go to home */
void MainWindow::handleButtonLeft() {
    DialogRun = true;
    bFocusLostCounter--;

    QMessageBox msgBox;
    msgBox.setWindowTitle("EWB");
    msgBox.setText("Revenir à l'accueil?");
    //msgBox.setInformativeText("Tout travail non enregistré sera perdu.");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.button(QMessageBox::Ok)->setText("Oui");
    msgBox.button(QMessageBox::Cancel)->setText("Non");
    msgBox.setDefaultButton(QMessageBox::Cancel);
    msgBox.setIcon(QMessageBox::Warning);
    if( msgBox.exec() == QMessageBox::Ok ) {

        if ( stackedWidget->currentIndex() == OngletWeb ) {
            MoodleWebview->load(QUrl(pSettings.GetUrl()));
            if(!bFocusLostCounter) {
                UnlockWebView();
            }
        }
        else {
            qDebug() << "On recharge Basthon";
            BasthonWebview->load(QUrl("https://basthon.fr/"));
        }
    }
    else {
        if(!bFocusLostCounter) {
            UnlockWebView();
        }
    }
    DialogRun = false;
}

void MainWindow::onPushButtonWeb() {
    stackedWidget->setCurrentIndex(1);
    PushButtonWeb->setStyleSheet("QPushButton { background-color : blue }");
    PushButtonBasthon->setStyleSheet("QPushButton { background-color : normal }");
    // PushButtonLeft->setEnabled(true);
}

void MainWindow::onPushButtonBasthon() {
    stackedWidget->setCurrentIndex(2);
    PushButtonWeb->setStyleSheet("QPushButton { background-color : normal }");
    PushButtonBasthon->setStyleSheet("QPushButton { background-color : blue }");
    // PushButtonLeft->setEnabled(false);
}

void MainWindow::onFocusTimer() {
    // qDebug() << "EndFocusTimer";
    updateToolBar();
}

void MainWindow::updateToolBar() {
    if (bFocusLost==true) {
        if(bToogleColors==false) {
            PushButtonRight->setText("APPEL");
            toolbar->setStyleSheet("QToolBar {background-color: yellow;}");
            bToogleColors = true;
        } else {
            PushButtonRight->setText("SURVEILLANT");
            SetupToolBarStyleFocusOff();
            bToogleColors = false;
        }
    } else {
        /* Clock */
        PushButtonRight->setText(QDateTime::currentDateTime().toString("hh:mm"));
    }
}

/* Not used */
void MainWindow::onNetworkTimer() {
    //qDebug() << "EndNetworkTimer";
    updateNetwork();
}

/* Not used */
void MainWindow::updateNetwork() {
    // qDebug() << "updateNetwork";
}

void MainWindow::UnlockWebView() {
    bFocusLost = false;
    SetupToolBarStyleFocusOn();
    bWebViewHidden = false;
    stackedWidget->setCurrentIndex(1);
    PushButtonLeft->setEnabled(true);
    PushButtonWeb->setStyleSheet("QPushButton { background-color : blue }");
    PushButtonBasthon->setStyleSheet("QPushButton { background-color : normal }");
}

void MainWindow::LockWebView() {
    bFocusLost = true;
    SetupToolBarStyleFocusOff();
    bWebViewHidden = true;
    stackedWidget->setCurrentIndex(0);
    PushButtonLeft->setEnabled(false);
}

/* Détection de la perte de focus */
bool MainWindow::eventFilter(QObject *obj, QEvent *event) {

    if (obj == this ){
        switch (event->type()) {
        case QEvent::WindowActivate:
            break;
        case QEvent::WindowDeactivate:
            if(!DialogRun) {
                SetupToolBarStyleFocusOff();
                LockWebView();
                bFocusLostCounter++;
            }
            break;
        default:
            break;
        }
    }

    return QWidget::eventFilter(obj, event);
}

void MainWindow::closeEvent(QCloseEvent * event) {
    event->ignore();
}

MainWindow::~MainWindow() {
    delete MoodlePage;
    delete MoodleProfile;
    delete BasthonPage;
    delete BasthonProfile;
    delete ui;
}
