/********************************************************************************
** Form generated from reading UI file 'view_formule_info.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEW_FORMULE_INFO_H
#define UI_VIEW_FORMULE_INFO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_view_formule_info
{
public:
    QLabel *info_label;

    void setupUi(QDialog *view_formule_info)
    {
        if (view_formule_info->objectName().isEmpty())
            view_formule_info->setObjectName("view_formule_info");
        view_formule_info->setWindowModality(Qt::NonModal);
        view_formule_info->resize(210, 210);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(view_formule_info->sizePolicy().hasHeightForWidth());
        view_formule_info->setSizePolicy(sizePolicy);
        view_formule_info->setMinimumSize(QSize(210, 210));
        view_formule_info->setMaximumSize(QSize(210, 210));
        view_formule_info->setFocusPolicy(Qt::NoFocus);
        view_formule_info->setModal(false);
        info_label = new QLabel(view_formule_info);
        info_label->setObjectName("info_label");
        info_label->setGeometry(QRect(10, 0, 201, 211));

        retranslateUi(view_formule_info);

        QMetaObject::connectSlotsByName(view_formule_info);
    } // setupUi

    void retranslateUi(QDialog *view_formule_info)
    {
        view_formule_info->setWindowTitle(QCoreApplication::translate("view_formule_info", "Formule Information", nullptr));
        info_label->setText(QCoreApplication::translate("view_formule_info", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:12px; margin-bottom:2px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2'; font-weight:600;\">Variables possibles:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:12px;\"><span style=\" font-family:'MS Shell Dlg 2';\">- Theta</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:12px;\"><span style=\" font-family:'MS Shell Dlg 2';\">- Thetadotdot</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:2px; margin-left:0px; margin-"
                        "right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2'; font-weight:600;\">Op\303\251rations:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:12px;\"><span style=\" font-family:'MS Shell Dlg 2';\">'+'        addition</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:12px;\"><span style=\" font-family:'MS Shell Dlg 2';\">'-'         soustraction</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:12px;\"><span style=\" font-family:'MS Shell Dlg 2';\">'*'         mutiplication</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:12px;\"><span style=\" font-family:'MS Shell Dlg 2';\">'/'         division</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; ma"
                        "rgin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:12px;\"><span style=\" font-family:'MS Shell Dlg 2';\">'^'        exponentiel</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:12px;\"><span style=\" font-family:'MS Shell Dlg 2';\">'%'       modulo</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:12px;\"><span style=\" font-family:'MS Shell Dlg 2';\">'cos'     cosinus (valeur en radian)</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:12px;\"><span style=\" font-family:'MS Shell Dlg 2';\">'sin'      sinus (valeur en radian)</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:12px;\"><span style=\" font-family:'MS Shell Dlg 2';\">'tan'     tangeante (valeur en radian)</span></p>\n"
"<p"
                        " style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:12px;\"><span style=\" font-family:'MS Shell Dlg 2';\">'sqrt'    racine carr\303\251e (valeur positive)</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class view_formule_info: public Ui_view_formule_info {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEW_FORMULE_INFO_H
