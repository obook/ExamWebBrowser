#include "mainwindow.h"
#include "./ui_mainwindow.h"

/*

Todo :

Voir 
QWebEngineView::loadStarted() // This signal is emitted when a new load of the page is started.
QWebEngineView::loadProgress(int progress) // This signal is emitted every time an element in the web view completes loading
void QWebEngineView::loadFinished(bool ok) // This signal is emitted when a load of the page has finished. ok will indicate whether the load was successful or an error occurred.

Voir
https://blog.csdn.net/CHAMSDONCON/article/details/124403358
https://cpp.hotexamples.com/examples/-/QWebEnginePage/-/cpp-qwebenginepage-class-examples.html

Il est possible de toute refaire avec webenginewidgets et être compatible Windows/Linux

*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("ExamWebBrowser");
    setupToolBar();
    webview = new QWebEngineView(this);
    webview->setContextMenuPolicy(Qt::NoContextMenu);
    //QWebEnginePage *page =  webview->page();
    //page->
    //connect(page, &QWebEnginePage::)
    //webview->setUrl(QUrl("https://smb33.keosystems.com")); /* ne fait rien, Ok en reload */
    webview->load(QUrl("https://smb33.keosystems.com"));
    webview->reload();
/* Pose problème ...
    QWebEngineProfile * engineProfile = webview->page()->profile();
    engineProfile->clearHttpCache();
    engineProfile->clearAllVisitedLinks();
    engineProfile->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
    engineProfile->setHttpCacheType(QWebEngineProfile::NoCache);

    QWebEngineCookieStore * pCookie = engineProfile->cookieStore();
    pCookie->deleteAllCookies();
    pCookie->deleteSessionCookies();
*/

    connect(webview, &QWebEngineView::loadStarted, [] {
        QApplication::setOverrideCursor(Qt::WaitCursor);
    });

    connect(webview, &QWebEngineView::loadFinished, [] {
        QApplication::restoreOverrideCursor();
    });

    setCentralWidget(webview);

    UnlockWebView();

    QObject::connect(monTimer, SIGNAL(timeout()), this,SLOT(finTempo()));

    bFocusLostCounter = 0;
}

void MainWindow::SetupToolBarStyleFocusOn() {
    int radius = 1;

    QString styleSheet(QString(
                           "QToolBar "
                           "{background-color: rgba(0,204,0,128); "
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

void MainWindow::setupToolBar()
{
    toolbar = addToolBar(tr("File"));
    toolbar->setFloatable(false);
    toolbar->setMovable(false);
    toolbar->setFixedHeight(48);
    //toolbar->setStyleSheet("QToolBar {background-color: green;}");
    //toolbar->setStyleSheet(styleSheet);
    SetupToolBarStyleFocusOn();

    PushButtonLeft = new QPushButton(this);
    PushButtonLeft->setText("Accueil");
    toolbar->addWidget(PushButtonLeft);
    connect(PushButtonLeft, &QPushButton::released, this, &MainWindow::handleButtonLeft);

    QWidget *spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget->setVisible(true);
    toolbar->addWidget(spacerWidget);

    PushButtonRight = new QPushButton(this);
    toolbar->addWidget(PushButtonRight);
    connect(PushButtonRight, &QPushButton::released, this, &MainWindow::handleButtonRight);

    monTimer = new QTimer(this);
    connect(monTimer, SIGNAL(timeout()), this, SLOT(update()));
    monTimer->start(1000);
}

void MainWindow::handleButtonRight() {
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
        msgBox.setDefaultButton(QMessageBox::Cancel);
        msgBox.setButtonText(QMessageBox::Ok, "Quitter");
        msgBox.setButtonText(QMessageBox::Cancel, "Rester");
        msgBox.setIcon(QMessageBox::Warning);
        if( msgBox.exec() == QMessageBox::Ok ) {
            QCoreApplication::exit();
        }
        else {
            UnlockWebView();
        }
    }
}

void MainWindow::handleButtonLeft() {
    bFocusLostCounter--;
    QMessageBox msgBox;
    msgBox.setWindowTitle("EWB");
    msgBox.setText("Revenir à l'accueil?");
    msgBox.setInformativeText("Tout travail non enregistré sera perdu.");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    msgBox.setButtonText(QMessageBox::Ok, "Oui");
    msgBox.setButtonText(QMessageBox::Cancel, "Non");
    msgBox.setIcon(QMessageBox::Warning);
    if( msgBox.exec() == QMessageBox::Ok ) {
        webview->load(QUrl("https://smb33.keosystems.com"));
        if(!bFocusLostCounter) {
            UnlockWebView();
        }
    }
    else {
        if(!bFocusLostCounter) {
            UnlockWebView();
        }
    }
}

void MainWindow::finTempo()
{
    if (bFocusLost==true)
    {
        if(bToogleColors==false)
        {
            PushButtonRight->setText("Appel");
            toolbar->setStyleSheet("QToolBar {background-color: black;}");
            bToogleColors = true;
        }
        else
        {
            PushButtonRight->setText("Surveillant");
            //toolbar->setStyleSheet("QToolBar {background-color: red;}");
            SetupToolBarStyleFocusOff();
            bToogleColors = false;
        }
    } else {
        /* Clock */

        PushButtonRight->setText(QDateTime::currentDateTime().toString("hh:mm"));
    }
}


void MainWindow::UnlockWebView() {
    bFocusLost = false;
    //toolbar->setStyleSheet("QToolBar {background-color: green;}");
    SetupToolBarStyleFocusOn();
    bWebViewHidden = false;
    webview->setVisible(true);
    PushButtonLeft->setEnabled(true);
}

void MainWindow::LockWebView() {
    bFocusLost = true;
    //toolbar->setStyleSheet("QToolBar {background-color: red;}");
    SetupToolBarStyleFocusOff();
    bWebViewHidden = true;
    webview->setVisible(false);
    PushButtonLeft->setEnabled(false);
}

/* Détection de la perte de focus */
bool MainWindow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::WindowActivate:
        break;
    case QEvent::WindowDeactivate:
        //toolbar->setStyleSheet("QToolBar {background-color: red;}");
        SetupToolBarStyleFocusOff();
        LockWebView();
        bFocusLostCounter++;
        break;
    default:
        break;
    }

    return QWidget::event(event);
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    event->ignore();
}

MainWindow::~MainWindow()
{
    delete ui;
}
