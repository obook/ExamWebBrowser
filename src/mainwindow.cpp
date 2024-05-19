#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QSettings>
#include "clickablelabel.h"
#include <QMouseEvent>
#include <QLayout>

/*
 * mainwindows.cpp
 *
 * */

/*

Todo :

Voir
https://blog.csdn.net/CHAMSDONCON/article/details/124403358
https://cpp.hotexamples.com/examples/-/QWebEnginePage/-/cpp-qwebenginepage-class-examples.html

*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("ExamWebBrowser");

    /* Toobar */

    setupToolBar();

    /* Settings */

    Settings *pSettings = new Settings();
    MainUrl = pSettings->GetUrl();

    /* WebEngine */
    webview = new QWebEngineView(this);
    webview->setContextMenuPolicy(Qt::NoContextMenu);

    QWebEngineProfile *engineProfile = webview->page()->profile();
    // engineProfile->clearHttpCache(); // Load blocked
    engineProfile->clearAllVisitedLinks();
    engineProfile->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
    engineProfile->setHttpCacheType(QWebEngineProfile::NoCache);

    QWebEngineCookieStore * pCookie = engineProfile->cookieStore();
    pCookie->deleteAllCookies();
    pCookie->deleteSessionCookies();

    connect(webview, &QWebEngineView::loadStarted, [] {
        QApplication::setOverrideCursor(Qt::WaitCursor);
    });

    connect(webview, &QWebEngineView::loadFinished, [] {
        QApplication::restoreOverrideCursor();
    });

    setCentralWidget(webview);

    webview->setUrl(QUrl(MainUrl));
    //webview->load(QUrl(MainUrl));
    //webview->reload();

    UnlockWebView();

    /* Network */

    pNetclient = new network_client();

    /* Timers */

    FocusTimer = new QTimer(this);
    connect(FocusTimer, SIGNAL(timeout()), this,SLOT(EndFocusTimer()));
    FocusTimer->start(1000);
    updateToolBar();

    NetworkTimer = new QTimer(this);
    connect(NetworkTimer, SIGNAL(timeout()), this,SLOT(EndNetworkTimer()));
    NetworkTimer->start(15000);
    updateNetwork();

    /* Variables */

    bFocusLostCounter = 0;
    DialogRun = false;

    installEventFilter(this);
}

void MainWindow::setupToolBar()
{
    toolbar = (ToolBar*)addToolBar(tr("File"));
    toolbar->setFloatable(false);
    toolbar->setMovable(false);
    toolbar->setFixedHeight(40);
    toolbar->toggleViewAction()->setEnabled(false); // Disable QToolbar right menu
    SetupToolBarStyleFocusOn();
    connect(toolbar , SIGNAL(clicked()), this, SLOT(slotToolbarClicked()));

    PushButtonLeft = new QPushButton(this);
    PushButtonLeft->setText("Accueil");
    toolbar->addWidget(PushButtonLeft);
    connect(PushButtonLeft, &QPushButton::released, this, &MainWindow::handleButtonLeft);

    QWidget *spacerWidget1 = new QWidget(this);
    spacerWidget1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget1->setVisible(true);
    toolbar->addWidget(spacerWidget1);

    QLabel *Label = new ClickableLabel(this);
    Label->setText("   NAVIGATEUR MODE EXAMEN   ");
    Label->setStyleSheet("QLabel { background-color : black; color : white; }");
    toolbar->addWidget(Label);
    connect(Label , SIGNAL(clicked()), this, SLOT(slotLabelClicked()));

    QWidget *spacerWidget2 = new QWidget(this);
    spacerWidget2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget2->setVisible(true);
    toolbar->addWidget(spacerWidget2);

    PushButtonRight = new PushButton(this);
    toolbar->addWidget(PushButtonRight);
    connect(PushButtonRight, &QPushButton::released, this, &MainWindow::handleButtonRight);
    PushButtonRight->installEventFilter(this);
}

void MainWindow::slotToolbarClicked() {
    qDebug() <<  QDateTime::currentMSecsSinceEpoch(), " slotToolbarClicked";
}

void MainWindow::slotLabelClicked() {
    DialogRun = true;
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
                           "{background-color: rgba(255,0,0,128); "
                           "border-radius: %1px;} "
                           "QToolButton "
                           "{max-width: 48px; "
                           "max-height: 49px; "
                           "border: 0px;}").
                       arg(radius));
    toolbar->setStyleSheet(styleSheet);
}

void MainWindow::handleButtonRight() {
    DialogRun = true;
    bFocusLostCounter--;
    if (bFocusLost==true) {
        bool ok;
        QInputDialog Dialog = new QInputDialog;
        Dialog.setWindowTitle("EWB");
        Dialog.setCancelButtonText(QString("Abandon"));
        Dialog.setOkButtonText(QString("Valider"));
        QString text = Dialog.getText(this, "EWB", "Code de déverouillage:", QLineEdit::Password, "", &ok);
        if ( ok && !text.isEmpty() ) {
            QString code_secret = QDateTime::currentDateTime().toString("ddMM"); /* Eg 1605 pour le 16 mai */
            /* Si la chaine saisie contient le code secret */
            if(text.contains(code_secret)) {
                bFocusLostCounter=0;
                UnlockWebView();
            }
        }
    }
    else {
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
    }

    DialogRun = false;
}

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
        webview->load(QUrl(MainUrl));
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

void MainWindow::EndFocusTimer() {
    // qDebug() << "EndFocusTimer";
    updateToolBar();
}

void MainWindow::updateToolBar() {
    if (bFocusLost==true) {
        if(bToogleColors==false) {
            PushButtonRight->setText("Appel");
            toolbar->setStyleSheet("QToolBar {background-color: black;}");
            bToogleColors = true;
        } else {
            PushButtonRight->setText("Surveillant");
            SetupToolBarStyleFocusOff();
            bToogleColors = false;
        }
    } else {
        /* Clock */
        PushButtonRight->setText(QDateTime::currentDateTime().toString("hh:mm"));
    }
}

void MainWindow::EndNetworkTimer() {
    //qDebug() << "EndNetworkTimer";
    updateNetwork();
}

void MainWindow::updateNetwork() {
    qDebug() << "updateNetwork";
}

void MainWindow::UnlockWebView() {
    bFocusLost = false;
    SetupToolBarStyleFocusOn();
    bWebViewHidden = false;
    webview->setVisible(true);
    PushButtonLeft->setEnabled(true);
}

void MainWindow::LockWebView() {
    bFocusLost = true;
    SetupToolBarStyleFocusOff();
    bWebViewHidden = true;
    webview->setVisible(false);
    PushButtonLeft->setEnabled(false);
}

/* Détection de la perte de focus */
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
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

    /* A terminer ... */
    else if (obj == PushButtonRight) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->buttons() & Qt::LeftButton && mouseEvent->modifiers() == Qt::ShiftModifier) {

            QDateTime date = QDateTime::currentDateTime();
            QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");
            QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();

            qDebug() << "Date:"+formattedTime +  "eventFilter PushButtonRight";

            // handleButtonRight();

            return QWidget::eventFilter(obj, event);
        }
    }
    return QWidget::eventFilter(obj, event);
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    event->ignore();
}

MainWindow::~MainWindow()
{
    delete ui;
}
