/*
 * Nom : mainwindow_dialogs.cpp
 * Description : Boîtes de dialogue : déverrouillage, quitter, accueil et à propos.
 * Auteur : O. Booklage
 * Date : 2024
 */

#include "mainwindow.h"

void MainWindow::showCodeInputDialog() {
    m_dialogRunning = true;
    m_focusLostCounter--;

    bool ok;
    QString text = QInputDialog::getText(
        this, "EWB", "Code de déverrouillage :",
        QLineEdit::Password, "", &ok);

    if (ok && !text.isEmpty()) {
        /* Le code secret est la date du jour au format JJMM (ex. : 1605 pour le 16 mai) */
        QString secretCode = QDateTime::currentDateTime().toString("ddMM");
        if (text.contains(secretCode)) {
            m_focusLostCounter = 0;
            unlockWebView();
        }
    }

    m_dialogRunning = false;
}

void MainWindow::showExitDialog() {
    m_dialogRunning = true;
    m_focusLostCounter--;

    QMessageBox msgBox;
    msgBox.setWindowTitle("EWB");
    msgBox.setText("Quitter l'application ?");
    msgBox.setInformativeText("Tout travail non enregistré sera perdu.");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.button(QMessageBox::Ok)->setText("Quitter");
    msgBox.button(QMessageBox::Cancel)->setText("Rester");
    msgBox.setDefaultButton(QMessageBox::Cancel);
    msgBox.setIcon(QMessageBox::Critical);

    if (msgBox.exec() == QMessageBox::Ok) {
        QCoreApplication::exit();
    } else {
        unlockWebView();
    }

    m_dialogRunning = false;
}

void MainWindow::onLabelClicked() {
    m_dialogRunning = true;
    m_focusLostCounter--;

    QMessageBox msgBox;
    msgBox.setWindowTitle("EWB");
    msgBox.setText("Navigateur Mode Examen");
    msgBox.setInformativeText("(C) obooklage 2024");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();

    m_dialogRunning = false;
}

/* Retour à la page d'accueil */
void MainWindow::handleHomeButton() {
    m_dialogRunning = true;
    m_focusLostCounter--;

    QMessageBox msgBox;
    msgBox.setWindowTitle("EWB");
    msgBox.setText("Revenir à l'accueil ?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.button(QMessageBox::Ok)->setText("Oui");
    msgBox.button(QMessageBox::Cancel)->setText("Non");
    msgBox.setDefaultButton(QMessageBox::Cancel);
    msgBox.setIcon(QMessageBox::Warning);

    if (msgBox.exec() == QMessageBox::Ok) {
        if (m_stackedWidget->currentIndex() == TAB_WEB) {
            m_moodleWebview->load(QUrl(m_settings.getUrl()));
        } else {
            m_basthonWebview->load(QUrl("https://basthon.fr/"));
        }
    }

    if (!m_focusLostCounter) {
        unlockWebView();
    }

    m_dialogRunning = false;
}
