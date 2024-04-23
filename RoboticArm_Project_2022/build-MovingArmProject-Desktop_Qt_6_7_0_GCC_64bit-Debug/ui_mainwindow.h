/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *ouvrir_view_formule_pushButton;
    QPushButton *affichage_debuter_pushButton;
    QPushButton *affichage_finir_pushButton;
    QPushButton *plus_pushButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(407, 121);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        ouvrir_view_formule_pushButton = new QPushButton(centralwidget);
        ouvrir_view_formule_pushButton->setObjectName("ouvrir_view_formule_pushButton");
        ouvrir_view_formule_pushButton->setGeometry(QRect(80, 10, 181, 25));
        affichage_debuter_pushButton = new QPushButton(centralwidget);
        affichage_debuter_pushButton->setObjectName("affichage_debuter_pushButton");
        affichage_debuter_pushButton->setEnabled(false);
        affichage_debuter_pushButton->setGeometry(QRect(10, 50, 141, 25));
        affichage_finir_pushButton = new QPushButton(centralwidget);
        affichage_finir_pushButton->setObjectName("affichage_finir_pushButton");
        affichage_finir_pushButton->setEnabled(false);
        affichage_finir_pushButton->setGeometry(QRect(210, 50, 121, 25));
        plus_pushButton = new QPushButton(centralwidget);
        plus_pushButton->setObjectName("plus_pushButton");
        plus_pushButton->setEnabled(false);
        plus_pushButton->setGeometry(QRect(165, 50, 31, 25));
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        ouvrir_view_formule_pushButton->setText(QCoreApplication::translate("MainWindow", "Entrer une formule", nullptr));
        affichage_debuter_pushButton->setText(QCoreApplication::translate("MainWindow", "D\303\251buter l'affichage", nullptr));
        affichage_finir_pushButton->setText(QCoreApplication::translate("MainWindow", "Finir l'affichage", nullptr));
        plus_pushButton->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
