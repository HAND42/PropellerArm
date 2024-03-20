/********************************************************************************
** Form generated from reading UI file 'view_setting_PC_controller.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEW_SETTING_PC_CONTROLLER_H
#define UI_VIEW_SETTING_PC_CONTROLLER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_view_setting_PC_controller
{
public:
    QLabel *info_label;
    QGroupBox *type_1_groupBox;
    QCheckBox *cascade_controller_checkBox;
    QLabel *k1_label;
    QLineEdit *k1_textBox;
    QLineEdit *k2_textBox;
    QLabel *k2_label;
    QGroupBox *type_2_groupBox;
    QCheckBox *leadlag_controller_checkBox;
    QLineEdit *a1_textBox;
    QLabel *a1_label;
    QLabel *a2_label;
    QLineEdit *a2_textBox;
    QLineEdit *b1_textBox;
    QLabel *b1_label;
    QLabel *b2_label;
    QLineEdit *b2_textBox;
    QLineEdit *b3_textBox;
    QLabel *b3_label;
    QPushButton *apply_button;
    QPushButton *cancel_button;
    QPushButton *reset_button;

    void setupUi(QDialog *view_setting_PC_controller)
    {
        if (view_setting_PC_controller->objectName().isEmpty())
            view_setting_PC_controller->setObjectName("view_setting_PC_controller");
        view_setting_PC_controller->setWindowModality(Qt::ApplicationModal);
        view_setting_PC_controller->resize(290, 297);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(view_setting_PC_controller->sizePolicy().hasHeightForWidth());
        view_setting_PC_controller->setSizePolicy(sizePolicy);
        view_setting_PC_controller->setMinimumSize(QSize(290, 297));
        view_setting_PC_controller->setMaximumSize(QSize(290, 297));
        view_setting_PC_controller->setModal(false);
        info_label = new QLabel(view_setting_PC_controller);
        info_label->setObjectName("info_label");
        info_label->setGeometry(QRect(14, 12, 241, 16));
        type_1_groupBox = new QGroupBox(view_setting_PC_controller);
        type_1_groupBox->setObjectName("type_1_groupBox");
        type_1_groupBox->setGeometry(QRect(13, 42, 259, 100));
        cascade_controller_checkBox = new QCheckBox(type_1_groupBox);
        cascade_controller_checkBox->setObjectName("cascade_controller_checkBox");
        cascade_controller_checkBox->setGeometry(QRect(6, 0, 16, 16));
        cascade_controller_checkBox->setChecked(true);
        cascade_controller_checkBox->setTristate(false);
        k1_label = new QLabel(type_1_groupBox);
        k1_label->setObjectName("k1_label");
        k1_label->setGeometry(QRect(6, 42, 21, 16));
        k1_textBox = new QLineEdit(type_1_groupBox);
        k1_textBox->setObjectName("k1_textBox");
        k1_textBox->setGeometry(QRect(32, 39, 50, 20));
        k1_textBox->setAlignment(Qt::AlignCenter);
        k2_textBox = new QLineEdit(type_1_groupBox);
        k2_textBox->setObjectName("k2_textBox");
        k2_textBox->setGeometry(QRect(116, 39, 50, 20));
        k2_label = new QLabel(type_1_groupBox);
        k2_label->setObjectName("k2_label");
        k2_label->setGeometry(QRect(90, 42, 21, 13));
        type_2_groupBox = new QGroupBox(view_setting_PC_controller);
        type_2_groupBox->setObjectName("type_2_groupBox");
        type_2_groupBox->setGeometry(QRect(13, 148, 259, 100));
        leadlag_controller_checkBox = new QCheckBox(type_2_groupBox);
        leadlag_controller_checkBox->setObjectName("leadlag_controller_checkBox");
        leadlag_controller_checkBox->setGeometry(QRect(6, 0, 16, 16));
        leadlag_controller_checkBox->setChecked(false);
        leadlag_controller_checkBox->setTristate(false);
        a1_textBox = new QLineEdit(type_2_groupBox);
        a1_textBox->setObjectName("a1_textBox");
        a1_textBox->setGeometry(QRect(32, 25, 50, 20));
        a1_label = new QLabel(type_2_groupBox);
        a1_label->setObjectName("a1_label");
        a1_label->setGeometry(QRect(6, 28, 21, 13));
        a2_label = new QLabel(type_2_groupBox);
        a2_label->setObjectName("a2_label");
        a2_label->setGeometry(QRect(90, 28, 21, 13));
        a2_textBox = new QLineEdit(type_2_groupBox);
        a2_textBox->setObjectName("a2_textBox");
        a2_textBox->setGeometry(QRect(116, 25, 50, 20));
        b1_textBox = new QLineEdit(type_2_groupBox);
        b1_textBox->setObjectName("b1_textBox");
        b1_textBox->setGeometry(QRect(33, 65, 50, 20));
        b1_label = new QLabel(type_2_groupBox);
        b1_label->setObjectName("b1_label");
        b1_label->setGeometry(QRect(6, 68, 21, 13));
        b2_label = new QLabel(type_2_groupBox);
        b2_label->setObjectName("b2_label");
        b2_label->setGeometry(QRect(90, 68, 21, 13));
        b2_textBox = new QLineEdit(type_2_groupBox);
        b2_textBox->setObjectName("b2_textBox");
        b2_textBox->setGeometry(QRect(116, 65, 50, 20));
        b3_textBox = new QLineEdit(type_2_groupBox);
        b3_textBox->setObjectName("b3_textBox");
        b3_textBox->setGeometry(QRect(200, 65, 50, 20));
        b3_label = new QLabel(type_2_groupBox);
        b3_label->setObjectName("b3_label");
        b3_label->setGeometry(QRect(174, 68, 21, 13));
        apply_button = new QPushButton(view_setting_PC_controller);
        apply_button->setObjectName("apply_button");
        apply_button->setGeometry(QRect(13, 260, 75, 23));
        cancel_button = new QPushButton(view_setting_PC_controller);
        cancel_button->setObjectName("cancel_button");
        cancel_button->setGeometry(QRect(105, 260, 75, 23));
        reset_button = new QPushButton(view_setting_PC_controller);
        reset_button->setObjectName("reset_button");
        reset_button->setGeometry(QRect(197, 260, 75, 23));

        retranslateUi(view_setting_PC_controller);

        QMetaObject::connectSlotsByName(view_setting_PC_controller);
    } // setupUi

    void retranslateUi(QDialog *view_setting_PC_controller)
    {
        view_setting_PC_controller->setWindowTitle(QCoreApplication::translate("view_setting_PC_controller", "PC control desired theta --> Configuration", nullptr));
        info_label->setText(QCoreApplication::translate("view_setting_PC_controller", "Select your controller type and fill the arguments", nullptr));
        type_1_groupBox->setTitle(QCoreApplication::translate("view_setting_PC_controller", "     Cascade Controller", nullptr));
        cascade_controller_checkBox->setText(QString());
        k1_label->setText(QCoreApplication::translate("view_setting_PC_controller", "K1 : ", nullptr));
        k2_label->setText(QCoreApplication::translate("view_setting_PC_controller", "K2 : ", nullptr));
        type_2_groupBox->setTitle(QCoreApplication::translate("view_setting_PC_controller", "     Leadlag Controller", nullptr));
        leadlag_controller_checkBox->setText(QString());
        a1_label->setText(QCoreApplication::translate("view_setting_PC_controller", "A1 : ", nullptr));
        a2_label->setText(QCoreApplication::translate("view_setting_PC_controller", "A2 : ", nullptr));
        b1_label->setText(QCoreApplication::translate("view_setting_PC_controller", "B1 : ", nullptr));
        b2_label->setText(QCoreApplication::translate("view_setting_PC_controller", "B2 : ", nullptr));
        b3_label->setText(QCoreApplication::translate("view_setting_PC_controller", "B3 : ", nullptr));
        apply_button->setText(QCoreApplication::translate("view_setting_PC_controller", "Apply", nullptr));
        cancel_button->setText(QCoreApplication::translate("view_setting_PC_controller", "Cancel", nullptr));
        reset_button->setText(QCoreApplication::translate("view_setting_PC_controller", "Reset", nullptr));
    } // retranslateUi

};

namespace Ui {
    class view_setting_PC_controller: public Ui_view_setting_PC_controller {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEW_SETTING_PC_CONTROLLER_H
