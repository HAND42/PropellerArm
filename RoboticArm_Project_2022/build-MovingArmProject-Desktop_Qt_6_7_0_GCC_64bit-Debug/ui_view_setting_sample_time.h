/********************************************************************************
** Form generated from reading UI file 'view_setting_sample_time.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEW_SETTING_SAMPLE_TIME_H
#define UI_VIEW_SETTING_SAMPLE_TIME_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_view_setting_sample_time
{
public:
    QLabel *sample_time_step_label;
    QLabel *sample_time_simulator_label;
    QLabel *sample_time_controller_label;
    QLabel *sample_time_graph_label;
    QLabel *sample_time_simulator_step_label;
    QLabel *sample_time_controller_step_label;
    QLabel *sample_time_graph_step_label;
    QLabel *label_1;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *sample_time_step_unite_mesure;
    QLabel *sample_time_simulator_unite_mesure;
    QLabel *sample_time_controller_unite_mesure;
    QLabel *sample_time_graph_unite_mesure;
    QLineEdit *sample_time_step_value;
    QLineEdit *sample_time_simulator_step_value;
    QLineEdit *sample_time_controller_step_value;
    QLineEdit *sample_time_graph_step_value;
    QLabel *sample_time_simulator_value;
    QLabel *sample_time_controller_value;
    QLabel *sample_time_graph_value;
    QPushButton *apply_button;
    QPushButton *cancel_button;

    void setupUi(QDialog *view_setting_sample_time)
    {
        if (view_setting_sample_time->objectName().isEmpty())
            view_setting_sample_time->setObjectName("view_setting_sample_time");
        view_setting_sample_time->setWindowModality(Qt::ApplicationModal);
        view_setting_sample_time->resize(240, 248);
        view_setting_sample_time->setMinimumSize(QSize(240, 248));
        view_setting_sample_time->setMaximumSize(QSize(240, 248));
        sample_time_step_label = new QLabel(view_setting_sample_time);
        sample_time_step_label->setObjectName("sample_time_step_label");
        sample_time_step_label->setGeometry(QRect(30, 20, 90, 13));
        sample_time_simulator_label = new QLabel(view_setting_sample_time);
        sample_time_simulator_label->setObjectName("sample_time_simulator_label");
        sample_time_simulator_label->setGeometry(QRect(30, 50, 111, 13));
        sample_time_controller_label = new QLabel(view_setting_sample_time);
        sample_time_controller_label->setObjectName("sample_time_controller_label");
        sample_time_controller_label->setGeometry(QRect(30, 100, 113, 13));
        sample_time_graph_label = new QLabel(view_setting_sample_time);
        sample_time_graph_label->setObjectName("sample_time_graph_label");
        sample_time_graph_label->setGeometry(QRect(30, 150, 97, 13));
        sample_time_simulator_step_label = new QLabel(view_setting_sample_time);
        sample_time_simulator_step_label->setObjectName("sample_time_simulator_step_label");
        sample_time_simulator_step_label->setGeometry(QRect(52, 70, 40, 13));
        sample_time_controller_step_label = new QLabel(view_setting_sample_time);
        sample_time_controller_step_label->setObjectName("sample_time_controller_step_label");
        sample_time_controller_step_label->setGeometry(QRect(52, 120, 40, 13));
        sample_time_graph_step_label = new QLabel(view_setting_sample_time);
        sample_time_graph_step_label->setObjectName("sample_time_graph_step_label");
        sample_time_graph_step_label->setGeometry(QRect(52, 170, 40, 13));
        label_1 = new QLabel(view_setting_sample_time);
        label_1->setObjectName("label_1");
        label_1->setGeometry(QRect(127, 70, 13, 13));
        label_2 = new QLabel(view_setting_sample_time);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(127, 120, 13, 13));
        label_3 = new QLabel(view_setting_sample_time);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(127, 170, 13, 13));
        sample_time_step_unite_mesure = new QLabel(view_setting_sample_time);
        sample_time_step_unite_mesure->setObjectName("sample_time_step_unite_mesure");
        sample_time_step_unite_mesure->setGeometry(QRect(193, 20, 20, 13));
        sample_time_simulator_unite_mesure = new QLabel(view_setting_sample_time);
        sample_time_simulator_unite_mesure->setObjectName("sample_time_simulator_unite_mesure");
        sample_time_simulator_unite_mesure->setGeometry(QRect(193, 70, 20, 13));
        sample_time_controller_unite_mesure = new QLabel(view_setting_sample_time);
        sample_time_controller_unite_mesure->setObjectName("sample_time_controller_unite_mesure");
        sample_time_controller_unite_mesure->setGeometry(QRect(193, 120, 20, 13));
        sample_time_graph_unite_mesure = new QLabel(view_setting_sample_time);
        sample_time_graph_unite_mesure->setObjectName("sample_time_graph_unite_mesure");
        sample_time_graph_unite_mesure->setGeometry(QRect(193, 170, 20, 13));
        sample_time_step_value = new QLineEdit(view_setting_sample_time);
        sample_time_step_value->setObjectName("sample_time_step_value");
        sample_time_step_value->setGeometry(QRect(145, 17, 41, 20));
        sample_time_simulator_step_value = new QLineEdit(view_setting_sample_time);
        sample_time_simulator_step_value->setObjectName("sample_time_simulator_step_value");
        sample_time_simulator_step_value->setGeometry(QRect(90, 67, 30, 20));
        sample_time_simulator_step_value->setAlignment(Qt::AlignCenter);
        sample_time_controller_step_value = new QLineEdit(view_setting_sample_time);
        sample_time_controller_step_value->setObjectName("sample_time_controller_step_value");
        sample_time_controller_step_value->setGeometry(QRect(90, 117, 30, 20));
        sample_time_controller_step_value->setAlignment(Qt::AlignCenter);
        sample_time_graph_step_value = new QLineEdit(view_setting_sample_time);
        sample_time_graph_step_value->setObjectName("sample_time_graph_step_value");
        sample_time_graph_step_value->setGeometry(QRect(90, 167, 30, 20));
        sample_time_graph_step_value->setAlignment(Qt::AlignCenter);
        sample_time_simulator_value = new QLabel(view_setting_sample_time);
        sample_time_simulator_value->setObjectName("sample_time_simulator_value");
        sample_time_simulator_value->setGeometry(QRect(150, 70, 38, 13));
        sample_time_controller_value = new QLabel(view_setting_sample_time);
        sample_time_controller_value->setObjectName("sample_time_controller_value");
        sample_time_controller_value->setGeometry(QRect(150, 120, 38, 13));
        sample_time_graph_value = new QLabel(view_setting_sample_time);
        sample_time_graph_value->setObjectName("sample_time_graph_value");
        sample_time_graph_value->setGeometry(QRect(150, 170, 38, 13));
        apply_button = new QPushButton(view_setting_sample_time);
        apply_button->setObjectName("apply_button");
        apply_button->setGeometry(QRect(33, 208, 75, 23));
        cancel_button = new QPushButton(view_setting_sample_time);
        cancel_button->setObjectName("cancel_button");
        cancel_button->setGeometry(QRect(138, 208, 75, 23));

        retranslateUi(view_setting_sample_time);

        QMetaObject::connectSlotsByName(view_setting_sample_time);
    } // setupUi

    void retranslateUi(QDialog *view_setting_sample_time)
    {
        view_setting_sample_time->setWindowTitle(QCoreApplication::translate("view_setting_sample_time", "Setting Sample Time", nullptr));
        sample_time_step_label->setText(QCoreApplication::translate("view_setting_sample_time", "Sample time step:", nullptr));
        sample_time_simulator_label->setText(QCoreApplication::translate("view_setting_sample_time", "Sample time simulator:", nullptr));
        sample_time_controller_label->setText(QCoreApplication::translate("view_setting_sample_time", "Sample time controller:", nullptr));
        sample_time_graph_label->setText(QCoreApplication::translate("view_setting_sample_time", "Sample time graph:", nullptr));
        sample_time_simulator_step_label->setText(QCoreApplication::translate("view_setting_sample_time", "Step  x", nullptr));
        sample_time_controller_step_label->setText(QCoreApplication::translate("view_setting_sample_time", "Step  x", nullptr));
        sample_time_graph_step_label->setText(QCoreApplication::translate("view_setting_sample_time", "Step  x", nullptr));
        label_1->setText(QCoreApplication::translate("view_setting_sample_time", "=", nullptr));
        label_2->setText(QCoreApplication::translate("view_setting_sample_time", "=", nullptr));
        label_3->setText(QCoreApplication::translate("view_setting_sample_time", "=", nullptr));
        sample_time_step_unite_mesure->setText(QCoreApplication::translate("view_setting_sample_time", "ms", nullptr));
        sample_time_simulator_unite_mesure->setText(QCoreApplication::translate("view_setting_sample_time", "ms", nullptr));
        sample_time_controller_unite_mesure->setText(QCoreApplication::translate("view_setting_sample_time", "ms", nullptr));
        sample_time_graph_unite_mesure->setText(QCoreApplication::translate("view_setting_sample_time", "ms", nullptr));
        sample_time_step_value->setText(QCoreApplication::translate("view_setting_sample_time", "100", nullptr));
        sample_time_simulator_step_value->setText(QCoreApplication::translate("view_setting_sample_time", "1", nullptr));
        sample_time_controller_step_value->setText(QCoreApplication::translate("view_setting_sample_time", "1", nullptr));
        sample_time_graph_step_value->setText(QCoreApplication::translate("view_setting_sample_time", "1", nullptr));
        sample_time_simulator_value->setText(QCoreApplication::translate("view_setting_sample_time", "100", nullptr));
        sample_time_controller_value->setText(QCoreApplication::translate("view_setting_sample_time", "100", nullptr));
        sample_time_graph_value->setText(QCoreApplication::translate("view_setting_sample_time", "100", nullptr));
        apply_button->setText(QCoreApplication::translate("view_setting_sample_time", "Apply", nullptr));
        cancel_button->setText(QCoreApplication::translate("view_setting_sample_time", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class view_setting_sample_time: public Ui_view_setting_sample_time {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEW_SETTING_SAMPLE_TIME_H
