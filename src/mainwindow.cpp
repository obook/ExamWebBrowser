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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    setWindowTitle("ExamWebBrowser");

    /* Toobar */
    setupToolBar();

    /* WebEngine */
    webview = new QWebEngineView(this);
    webview->setContextMenuPolicy(Qt::NoContextMenu);

    QWebEngineProfile *engineProfile = webview->page()->profile();
    engineProfile->clearHttpCache(); // Load = blocked ?
    engineProfile->clearAllVisitedLinks();
    engineProfile->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
    engineProfile->setHttpCacheType(QWebEngineProfile::NoCache);

    QWebEngineCookieStore * pCookie = engineProfile->cookieStore();
    pCookie->deleteAllCookies();
    pCookie->deleteSessionCookies();

    connect(webview, &QWebEngineView::loadStarted, [] {
        QApplication::setOverrideCursor(Qt::BusyCursor);
    });

    connect(webview, &QWebEngineView::loadFinished, [] {
        QApplication::restoreOverrideCursor();
    });

    /* Interceptor */
    interceptor = new RequestInterceptor(webview);
    profile = new QWebEngineProfile(webview);
    profile->setUrlRequestInterceptor(interceptor);
    page = new QWebEnginePage(profile, webview);
    webview->setPage(page);

    /* Default URL founded in ExamWebBrowser.ini */
    QString URL = pSettings.GetUrl();
    if(URL == "notice.html") {
        webview->load(QUrl::fromLocalFile(QApplication::applicationDirPath() + "/notice.html"));
    }
    else {
        webview->setUrl(QUrl(URL));
    }

    /* Blocked Text (Label)  */
    TextBlockedlabel = new QLabel(this);
    TextBlockedlabel->setAlignment( Qt::AlignCenter );
    QFont font( "Arial", 24, QFont::Bold);
    TextBlockedlabel->setFont(font);
    TextBlockedlabel->setText(QString("MERCI D'APPELER UN SURVEILLANT"));

    /* QStackedWidget */
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(TextBlockedlabel);
    stackedWidget->addWidget(webview);
    stackedWidget->setCurrentIndex(1);

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
    msgBox.setInformativeText("Tout travail non enregistré sera perdu.");
     msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.button(QMessageBox::Ok)->setText("Oui");
    msgBox.button(QMessageBox::Cancel)->setText("Non");
    msgBox.setDefaultButton(QMessageBox::Cancel);
    msgBox.setIcon(QMessageBox::Warning);
    if( msgBox.exec() == QMessageBox::Ok ) {
        webview->load(QUrl(pSettings.GetUrl()));
        if(!bFocusLostCounter) {
            UnlockWebView();
        }
    }
    else {
        if(!bFocusLostCounter) {
            UnlockWebView();
        }
    }
    DialogRun = false;
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

void MainWindow::onNetworkTimer() {
    //qDebug() << "EndNetworkTimer";
    updateNetwork();
}

void MainWindow::updateNetwork() {
    // qDebug() << "updateNetwork";
}

void MainWindow::UnlockWebView() {
    bFocusLost = false;
    SetupToolBarStyleFocusOn();
    bWebViewHidden = false;
    stackedWidget->setCurrentIndex(1);
    PushButtonLeft->setEnabled(true);
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
                // CodeInputDialog();
            }
            break;
        default:
            break;
        }
    }

    /* A terminer ...
    else if (obj == PushButtonRight) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->buttons() & Qt::LeftButton && mouseEvent->modifiers() == Qt::ShiftModifier) {

            QDateTime date = QDateTime::currentDateTime();
            QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");
            QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();

            //qDebug() << "Date:"+formattedTime +  "eventFilter PushButtonRight";

            // handleButtonRight(); Loop

            return QWidget::eventFilter(obj, event);
        }
    } */
    return QWidget::eventFilter(obj, event);
}

void MainWindow::closeEvent(QCloseEvent * event) {
    event->ignore();
}

MainWindow::~MainWindow() {
    delete page;
    delete profile;
    delete ui;
}
