#include "mainwindow.h"
#include "./ui_mainwindow.h"
//#include <QCursor>
#include <QKeyEvent>

/*

Todo :

Voir 
QWebEngineView::loadStarted() // This signal is emitted when a new load of the page is started.
QWebEngineView::loadProgress(int progress) // This signal is emitted every time an element in the web view completes loading
void QWebEngineView::loadFinished(bool ok) // This signal is emitted when a load of the page has finished. ok will indicate whether the load was successful or an error occurred.

*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("ExamWebBrowser");
    setupToolBar();
    webview = new QWebEngineView(this);
    webview->load(QUrl("https://smb33.keosystems.com"));
    setCentralWidget(webview);
    bFocusLost = false;
    bToogleColors = false;
    bWebViewHidden = false;
    QObject::connect(monTimer, SIGNAL(timeout()), this,SLOT(finTempo()));
}

void MainWindow::setupToolBar()
{
    toolbar = addToolBar(tr("File"));
    toolbar->setFloatable(false);
    toolbar->setMovable(false);
    toolbar->setFixedHeight(36);
    toolbar->setStyleSheet("QToolBar {background-color: green;}");
    monTimer = new QTimer(this);
    connect(monTimer, SIGNAL(timeout()), this, SLOT(update()));
    monTimer->start(1000);
}

void MainWindow::finTempo()
{
    if (bFocusLost==true)
    {
        if(bToogleColors==false)
        {
            toolbar->setStyleSheet("QToolBar {background-color: black;}");
            bToogleColors = true;
        }
        else
        {
            toolbar->setStyleSheet("QToolBar {background-color: red;}");
            bToogleColors = false;
        }
    }
}

/* Détection de la perte de focus */
bool MainWindow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::WindowActivate:
         qDebug() << "Window has focus!!!";
         webview->setVisible(true);
         bWebViewHidden = false;
        break;
    case QEvent::WindowDeactivate:
        qDebug() << "Window lost focus!!!";
        toolbar->setStyleSheet("QToolBar {background-color: red;}");
        //QCursor cursor(Qt::BlankCursor);
        //QApplication::setOverrideCursor(cursor);
        //QApplication::changeOverrideCursor(cursor);
        webview->setVisible(false);
        bWebViewHidden = true;
        bFocusLost = true;
        break;
/*
 *
 * Voir
 * https://stackoverflow.com/questions/6647970/how-can-i-capture-qkeysequence-from-qkeyevent-depending-on-current-keyboard-layo
 * */

    case QEvent::KeyPress: /* On n'a plus de KeyPress car la webview est cachée !!!!!!!!!!!!!! */
        if(static_cast<QKeyEvent*>(event)->key() == Qt::Key_F1)
        {
            qDebug() << "TOUCHE F1";
            toolbar->setStyleSheet("QToolBar {background-color: green;}");
            bFocusLost = false;
        }
        break;
    case QEvent::KeyRelease:
        qDebug() << "Key Release Event for" << static_cast<QKeyEvent*>(event)->text();
        if( bWebViewHidden == true )
        {
            webview->setVisible(true);
            bWebViewHidden = false;
        }
        break;
    default:
        break;
    }

    return QWidget::event(event);
}

MainWindow::~MainWindow()
{
    delete ui;
}
