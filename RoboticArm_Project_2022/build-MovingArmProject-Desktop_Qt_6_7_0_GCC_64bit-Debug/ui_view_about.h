/********************************************************************************
** Form generated from reading UI file 'view_about.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEW_ABOUT_H
#define UI_VIEW_ABOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_view_about
{
public:
    QLabel *label;

    void setupUi(QDialog *view_about)
    {
        if (view_about->objectName().isEmpty())
            view_about->setObjectName("view_about");
        view_about->setWindowModality(Qt::ApplicationModal);
        view_about->resize(171, 230);
        view_about->setMinimumSize(QSize(171, 230));
        view_about->setMaximumSize(QSize(171, 230));
        label = new QLabel(view_about);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 10, 151, 201));
        QFont font;
        font.setPointSize(9);
        label->setFont(font);

        retranslateUi(view_about);

        QMetaObject::connectSlotsByName(view_about);
    } // setupUi

    void retranslateUi(QDialog *view_about)
    {
        view_about->setWindowTitle(QCoreApplication::translate("view_about", "About", nullptr));
        label->setText(QCoreApplication::translate("view_about", "Fly Arm System\n"
"\n"
"\n"
"Initial contributers:\n"
"\n"
"      Nima Soroush Haddadi\n"
"      Kiran C Roy\n"
"      Philippe Pernet\n"
"\n"
"\n"
"Assistant Professor :\n"
"\n"
"      Dr. Guillaume Ducard ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class view_about: public Ui_view_about {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEW_ABOUT_H
