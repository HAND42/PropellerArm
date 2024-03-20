/********************************************************************************
** Form generated from reading UI file 'view_fly_arm.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEW_FLY_ARM_H
#define UI_VIEW_FLY_ARM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_view_fly_arm
{
public:
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionExit;
    QAction *actionPC_Controleur;
    QAction *actionHils_mode_1;
    QAction *actionPC_Display_Real_angle;
    QAction *actionHils_mode_3;
    QAction *actionDemo_Manuel_Thrust_Command;
    QAction *actionSample_time_Configuration;
    QAction *actionPC_controller_Configuration;
    QAction *actionHow_to_do;
    QAction *actionOnline;
    QAction *actionAbout;
    QAction *actionRepair_file_setting_txt;
    QAction *actionFormule_Calcul_Thrust;
    QAction *actiontests;
    QAction *actionSave_Data;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QGroupBox *groupBox_6;
    QGroupBox *modes_panel;
    QLabel *mode_label;
    QPushButton *play_button;
    QPushButton *pause_button;
    QPushButton *stop_button;
    QLabel *time_label;
    QLabel *theta_label;
    QLabel *thrust_label;
    QLineEdit *time_textBox;
    QLabel *theta_value_label;
    QLabel *thrust_value_label;
    QLabel *time_unite_label;
    QLabel *theta_unite_label;
    QLabel *thrust_unite_label;
    QLabel *theta_OR_thrust_desired_label;
    QLabel *theta_OR_thrust_desired_value_label;
    QLabel *theta_desired_unite_label;
    QSlider *theta_OR_thrust_desired_trackBar;
    QFrame *view_panel;
    QGroupBox *terminal_panel;
    QGroupBox *consol_panel;
    QWidget *consol_panel_panel;
    QLabel *consol_panel_label;
    QRadioButton *usb_port_radioButton;
    QRadioButton *com_port_radioButton;
    QLabel *baud_rate_label;
    QComboBox *com_port_comboBox;
    QComboBox *baud_rate_comboBox;
    QPushButton *connect_button;
    QPushButton *disconnect_button;
    QGroupBox *status_groupBox;
    QWidget *status_panel;
    QLabel *status_label;
    QGroupBox *terminal_groupBox;
    QLabel *terminal_info_label;
    QProgressBar *signal_value_progressBar;
    QLabel *terminal_signal_value_label;
    QSplitter *splitter_2;
    QSplitter *splitter_3;
    QWidget *graph_theta_legend;
    QLabel *graph_theta_value_label;
    QLabel *graph_theta_time_label;
    QLabel *graph_theta_temps_legend_label;
    QComboBox *angle_unite_listBox;
    QWidget *graph_thrust_legend;
    QLabel *graph_thrust_value_label;
    QLabel *graph_thrust_time_label;
    QLabel *graph_thrust_temps_legend_label;
    QWidget *graph_theta_dot_legend;
    QLabel *graph_theta_dot_time_label;
    QLabel *graph_theta_dot_value_label;
    QLabel *graph_theta_dot_temps_legend_label;
    QWidget *graph_theta_dotdot_legend;
    QLabel *graph_theta_dotdot_value_label;
    QLabel *graph_theta_dotdot_time_label;
    QLabel *graph_theta_dotdot_temps_legend_label;
    QLabel *graph_theta_dotdot_controlleur_time_label;
    QLabel *graph_rouge_label;
    QLabel *graph_bleu_label;
    QLabel *graph_theta_dotdot_controlleur_value_label;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuModes;
    QMenu *menuSetting;
    QMenu *menuHelp;

    void setupUi(QMainWindow *view_fly_arm)
    {
        if (view_fly_arm->objectName().isEmpty())
            view_fly_arm->setObjectName("view_fly_arm");
        view_fly_arm->resize(1520, 783);
        view_fly_arm->setMinimumSize(QSize(1520, 783));
        QIcon icon;
        icon.addFile(QString::fromUtf8("app.ico"), QSize(), QIcon::Normal, QIcon::Off);
        view_fly_arm->setWindowIcon(icon);
        view_fly_arm->setStyleSheet(QString::fromUtf8(""));
        actionOpen = new QAction(view_fly_arm);
        actionOpen->setObjectName("actionOpen");
        actionSave = new QAction(view_fly_arm);
        actionSave->setObjectName("actionSave");
        actionExit = new QAction(view_fly_arm);
        actionExit->setObjectName("actionExit");
        actionPC_Controleur = new QAction(view_fly_arm);
        actionPC_Controleur->setObjectName("actionPC_Controleur");
        actionHils_mode_1 = new QAction(view_fly_arm);
        actionHils_mode_1->setObjectName("actionHils_mode_1");
        actionPC_Display_Real_angle = new QAction(view_fly_arm);
        actionPC_Display_Real_angle->setObjectName("actionPC_Display_Real_angle");
        actionHils_mode_3 = new QAction(view_fly_arm);
        actionHils_mode_3->setObjectName("actionHils_mode_3");
        actionDemo_Manuel_Thrust_Command = new QAction(view_fly_arm);
        actionDemo_Manuel_Thrust_Command->setObjectName("actionDemo_Manuel_Thrust_Command");
        actionSample_time_Configuration = new QAction(view_fly_arm);
        actionSample_time_Configuration->setObjectName("actionSample_time_Configuration");
        actionPC_controller_Configuration = new QAction(view_fly_arm);
        actionPC_controller_Configuration->setObjectName("actionPC_controller_Configuration");
        actionHow_to_do = new QAction(view_fly_arm);
        actionHow_to_do->setObjectName("actionHow_to_do");
        actionOnline = new QAction(view_fly_arm);
        actionOnline->setObjectName("actionOnline");
        actionAbout = new QAction(view_fly_arm);
        actionAbout->setObjectName("actionAbout");
        actionRepair_file_setting_txt = new QAction(view_fly_arm);
        actionRepair_file_setting_txt->setObjectName("actionRepair_file_setting_txt");
        actionFormule_Calcul_Thrust = new QAction(view_fly_arm);
        actionFormule_Calcul_Thrust->setObjectName("actionFormule_Calcul_Thrust");
        actiontests = new QAction(view_fly_arm);
        actiontests->setObjectName("actiontests");
        actionSave_Data = new QAction(view_fly_arm);
        actionSave_Data->setObjectName("actionSave_Data");
        centralwidget = new QWidget(view_fly_arm);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        splitter = new QSplitter(centralwidget);
        splitter->setObjectName("splitter");
        splitter->setOrientation(Qt::Vertical);
        groupBox_6 = new QGroupBox(splitter);
        groupBox_6->setObjectName("groupBox_6");
        groupBox_6->setMinimumSize(QSize(748, 640));
        groupBox_6->setFlat(true);
        modes_panel = new QGroupBox(groupBox_6);
        modes_panel->setObjectName("modes_panel");
        modes_panel->setGeometry(QRect(10, 6, 198, 128));
        mode_label = new QLabel(modes_panel);
        mode_label->setObjectName("mode_label");
        mode_label->setGeometry(QRect(4, 6, 190, 23));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        mode_label->setFont(font);
        mode_label->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 0);"));
        mode_label->setAlignment(Qt::AlignCenter);
        play_button = new QPushButton(modes_panel);
        play_button->setObjectName("play_button");
        play_button->setEnabled(true);
        play_button->setGeometry(QRect(19, 32, 52, 28));
        play_button->setAutoFillBackground(false);
        play_button->setStyleSheet(QString::fromUtf8("border-image: url(:/images/play_gris.jpg);"));
        play_button->setAutoDefault(false);
        play_button->setFlat(false);
        pause_button = new QPushButton(modes_panel);
        pause_button->setObjectName("pause_button");
        pause_button->setGeometry(QRect(73, 32, 52, 28));
        pause_button->setStyleSheet(QString::fromUtf8("border-image: url(:/images/pause_gris.jpg);"));
        stop_button = new QPushButton(modes_panel);
        stop_button->setObjectName("stop_button");
        stop_button->setGeometry(QRect(127, 32, 52, 28));
        stop_button->setStyleSheet(QString::fromUtf8("border-image: url(:/images/stop_gris.jpg);"));
        time_label = new QLabel(modes_panel);
        time_label->setObjectName("time_label");
        time_label->setGeometry(QRect(30, 66, 47, 13));
        QFont font1;
        font1.setPointSize(10);
        time_label->setFont(font1);
        time_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        theta_label = new QLabel(modes_panel);
        theta_label->setObjectName("theta_label");
        theta_label->setGeometry(QRect(30, 86, 47, 13));
        theta_label->setFont(font1);
        theta_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        thrust_label = new QLabel(modes_panel);
        thrust_label->setObjectName("thrust_label");
        thrust_label->setGeometry(QRect(30, 106, 47, 13));
        thrust_label->setFont(font1);
        thrust_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        time_textBox = new QLineEdit(modes_panel);
        time_textBox->setObjectName("time_textBox");
        time_textBox->setGeometry(QRect(80, 66, 59, 20));
        time_textBox->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        theta_value_label = new QLabel(modes_panel);
        theta_value_label->setObjectName("theta_value_label");
        theta_value_label->setGeometry(QRect(84, 86, 47, 13));
        theta_value_label->setFont(font1);
        theta_value_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        thrust_value_label = new QLabel(modes_panel);
        thrust_value_label->setObjectName("thrust_value_label");
        thrust_value_label->setGeometry(QRect(84, 106, 47, 13));
        thrust_value_label->setFont(font1);
        thrust_value_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        time_unite_label = new QLabel(modes_panel);
        time_unite_label->setObjectName("time_unite_label");
        time_unite_label->setGeometry(QRect(148, 66, 30, 13));
        time_unite_label->setFont(font1);
        time_unite_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        theta_unite_label = new QLabel(modes_panel);
        theta_unite_label->setObjectName("theta_unite_label");
        theta_unite_label->setGeometry(QRect(148, 86, 30, 21));
        theta_unite_label->setFont(font1);
        theta_unite_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        theta_unite_label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        thrust_unite_label = new QLabel(modes_panel);
        thrust_unite_label->setObjectName("thrust_unite_label");
        thrust_unite_label->setGeometry(QRect(148, 106, 30, 13));
        thrust_unite_label->setFont(font1);
        thrust_unite_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        theta_OR_thrust_desired_label = new QLabel(groupBox_6);
        theta_OR_thrust_desired_label->setObjectName("theta_OR_thrust_desired_label");
        theta_OR_thrust_desired_label->setGeometry(QRect(40, 136, 91, 16));
        theta_OR_thrust_desired_label->setFont(font1);
        theta_OR_thrust_desired_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        theta_OR_thrust_desired_value_label = new QLabel(groupBox_6);
        theta_OR_thrust_desired_value_label->setObjectName("theta_OR_thrust_desired_value_label");
        theta_OR_thrust_desired_value_label->setGeometry(QRect(130, 136, 21, 16));
        theta_OR_thrust_desired_value_label->setFont(font1);
        theta_OR_thrust_desired_value_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        theta_desired_unite_label = new QLabel(groupBox_6);
        theta_desired_unite_label->setObjectName("theta_desired_unite_label");
        theta_desired_unite_label->setGeometry(QRect(160, 136, 30, 16));
        theta_desired_unite_label->setFont(font1);
        theta_desired_unite_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        theta_desired_unite_label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        theta_OR_thrust_desired_trackBar = new QSlider(groupBox_6);
        theta_OR_thrust_desired_trackBar->setObjectName("theta_OR_thrust_desired_trackBar");
        theta_OR_thrust_desired_trackBar->setGeometry(QRect(40, 160, 160, 22));
        theta_OR_thrust_desired_trackBar->setMaximum(179);
        theta_OR_thrust_desired_trackBar->setSingleStep(1);
        theta_OR_thrust_desired_trackBar->setOrientation(Qt::Horizontal);
        view_panel = new QFrame(groupBox_6);
        view_panel->setObjectName("view_panel");
        view_panel->setGeometry(QRect(0, 0, 748, 661));
        view_panel->setMinimumSize(QSize(748, 661));
        view_panel->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        view_panel->setFrameShape(QFrame::StyledPanel);
        view_panel->setFrameShadow(QFrame::Raised);
        splitter->addWidget(groupBox_6);
        view_panel->raise();
        modes_panel->raise();
        theta_OR_thrust_desired_label->raise();
        theta_OR_thrust_desired_value_label->raise();
        theta_desired_unite_label->raise();
        theta_OR_thrust_desired_trackBar->raise();
        terminal_panel = new QGroupBox(splitter);
        terminal_panel->setObjectName("terminal_panel");
        terminal_panel->setMinimumSize(QSize(748, 96));
        terminal_panel->setMaximumSize(QSize(16777215, 96));
        terminal_panel->setStyleSheet(QString::fromUtf8(""));
        terminal_panel->setFlat(false);
        terminal_panel->setCheckable(false);
        consol_panel = new QGroupBox(terminal_panel);
        consol_panel->setObjectName("consol_panel");
        consol_panel->setGeometry(QRect(5, 6, 77, 24));
        consol_panel->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        consol_panel->setFlat(true);
        consol_panel_panel = new QWidget(consol_panel);
        consol_panel_panel->setObjectName("consol_panel_panel");
        consol_panel_panel->setGeometry(QRect(6, 12, 25, 2));
        consol_panel_panel->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));
        consol_panel_label = new QLabel(consol_panel);
        consol_panel_label->setObjectName("consol_panel_label");
        consol_panel_label->setGeometry(QRect(38, 5, 39, 13));
        consol_panel_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        usb_port_radioButton = new QRadioButton(terminal_panel);
        usb_port_radioButton->setObjectName("usb_port_radioButton");
        usb_port_radioButton->setGeometry(QRect(5, 34, 81, 17));
        usb_port_radioButton->setFont(font1);
        usb_port_radioButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        com_port_radioButton = new QRadioButton(terminal_panel);
        com_port_radioButton->setObjectName("com_port_radioButton");
        com_port_radioButton->setGeometry(QRect(5, 54, 81, 17));
        com_port_radioButton->setFont(font1);
        com_port_radioButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        com_port_radioButton->setChecked(true);
        baud_rate_label = new QLabel(terminal_panel);
        baud_rate_label->setObjectName("baud_rate_label");
        baud_rate_label->setGeometry(QRect(9, 75, 71, 16));
        baud_rate_label->setFont(font1);
        baud_rate_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        com_port_comboBox = new QComboBox(terminal_panel);
        com_port_comboBox->setObjectName("com_port_comboBox");
        com_port_comboBox->setGeometry(QRect(90, 48, 72, 21));
        com_port_comboBox->setFont(font1);
        com_port_comboBox->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"background-color: rgb(220, 220, 220);"));
        com_port_comboBox->setEditable(true);
        com_port_comboBox->setMaxVisibleItems(2);
        com_port_comboBox->setMaxCount(2);
        com_port_comboBox->setInsertPolicy(QComboBox::NoInsert);
        baud_rate_comboBox = new QComboBox(terminal_panel);
        baud_rate_comboBox->setObjectName("baud_rate_comboBox");
        baud_rate_comboBox->setGeometry(QRect(90, 72, 72, 21));
        baud_rate_comboBox->setFont(font1);
        baud_rate_comboBox->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"background-color: rgb(220, 220, 220);"));
        baud_rate_comboBox->setEditable(true);
        baud_rate_comboBox->setMaxVisibleItems(2);
        baud_rate_comboBox->setMaxCount(2);
        baud_rate_comboBox->setInsertPolicy(QComboBox::NoInsert);
        connect_button = new QPushButton(terminal_panel);
        connect_button->setObjectName("connect_button");
        connect_button->setGeometry(QRect(170, 48, 75, 23));
        connect_button->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"background-color: rgb(220, 220, 220);"));
        disconnect_button = new QPushButton(terminal_panel);
        disconnect_button->setObjectName("disconnect_button");
        disconnect_button->setEnabled(true);
        disconnect_button->setGeometry(QRect(170, 72, 75, 23));
        disconnect_button->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"background-color: rgb(220, 220, 220);"));
        disconnect_button->setCheckable(false);
        disconnect_button->setChecked(false);
        disconnect_button->setAutoDefault(false);
        disconnect_button->setFlat(false);
        status_groupBox = new QGroupBox(terminal_panel);
        status_groupBox->setObjectName("status_groupBox");
        status_groupBox->setGeometry(QRect(250, 38, 73, 57));
        status_groupBox->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        status_panel = new QWidget(status_groupBox);
        status_panel->setObjectName("status_panel");
        status_panel->setGeometry(QRect(14, 14, 45, 26));
        status_panel->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));
        status_label = new QLabel(status_groupBox);
        status_label->setObjectName("status_label");
        status_label->setGeometry(QRect(4, 40, 68, 13));
        status_label->setFont(font1);
        terminal_groupBox = new QGroupBox(terminal_panel);
        terminal_groupBox->setObjectName("terminal_groupBox");
        terminal_groupBox->setGeometry(QRect(330, 9, 412, 81));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(terminal_groupBox->sizePolicy().hasHeightForWidth());
        terminal_groupBox->setSizePolicy(sizePolicy);
        terminal_groupBox->setContextMenuPolicy(Qt::DefaultContextMenu);
        terminal_groupBox->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
"color: rgb(255, 255, 255);"));
        terminal_info_label = new QLabel(terminal_groupBox);
        terminal_info_label->setObjectName("terminal_info_label");
        terminal_info_label->setGeometry(QRect(10, 10, 401, 61));
        terminal_info_label->setFont(font1);
        signal_value_progressBar = new QProgressBar(terminal_panel);
        signal_value_progressBar->setObjectName("signal_value_progressBar");
        signal_value_progressBar->setGeometry(QRect(170, 10, 151, 23));
        signal_value_progressBar->setMaximum(1023);
        signal_value_progressBar->setValue(0);
        signal_value_progressBar->setAlignment(Qt::AlignCenter);
        terminal_signal_value_label = new QLabel(terminal_panel);
        terminal_signal_value_label->setObjectName("terminal_signal_value_label");
        terminal_signal_value_label->setGeometry(QRect(85, 12, 82, 13));
        terminal_signal_value_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        splitter->addWidget(terminal_panel);

        horizontalLayout->addWidget(splitter);

        splitter_2 = new QSplitter(centralwidget);
        splitter_2->setObjectName("splitter_2");
        splitter_2->setOrientation(Qt::Vertical);
        splitter_2->setHandleWidth(2);

        horizontalLayout->addWidget(splitter_2);

        splitter_3 = new QSplitter(centralwidget);
        splitter_3->setObjectName("splitter_3");
        splitter_3->setMaximumSize(QSize(96, 16777215));
        splitter_3->setOrientation(Qt::Vertical);
        splitter_3->setHandleWidth(5);
        graph_theta_legend = new QWidget(splitter_3);
        graph_theta_legend->setObjectName("graph_theta_legend");
        graph_theta_legend->setMinimumSize(QSize(0, 175));
        graph_theta_legend->setMaximumSize(QSize(96, 16777215));
        graph_theta_legend->setFont(font);
        graph_theta_legend->setStyleSheet(QString::fromUtf8("color: rgb(0, 127, 0);"));
        graph_theta_value_label = new QLabel(graph_theta_legend);
        graph_theta_value_label->setObjectName("graph_theta_value_label");
        graph_theta_value_label->setGeometry(QRect(0, 52, 91, 20));
        sizePolicy.setHeightForWidth(graph_theta_value_label->sizePolicy().hasHeightForWidth());
        graph_theta_value_label->setSizePolicy(sizePolicy);
        graph_theta_value_label->setFont(font1);
        graph_theta_value_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 0, 0);"));
        graph_theta_value_label->setAlignment(Qt::AlignCenter);
        graph_theta_time_label = new QLabel(graph_theta_legend);
        graph_theta_time_label->setObjectName("graph_theta_time_label");
        graph_theta_time_label->setGeometry(QRect(-2, 10, 96, 38));
        graph_theta_time_label->setFont(font1);
        graph_theta_time_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 0, 0);"));
        graph_theta_time_label->setAlignment(Qt::AlignCenter);
        graph_theta_temps_legend_label = new QLabel(graph_theta_legend);
        graph_theta_temps_legend_label->setObjectName("graph_theta_temps_legend_label");
        graph_theta_temps_legend_label->setGeometry(QRect(0, 160, 91, 20));
        sizePolicy.setHeightForWidth(graph_theta_temps_legend_label->sizePolicy().hasHeightForWidth());
        graph_theta_temps_legend_label->setSizePolicy(sizePolicy);
        graph_theta_temps_legend_label->setMinimumSize(QSize(91, 20));
        graph_theta_temps_legend_label->setMaximumSize(QSize(91, 20));
        graph_theta_temps_legend_label->setFont(font);
        graph_theta_temps_legend_label->setStyleSheet(QString::fromUtf8("color: rgb(0, 170, 93);"));
        graph_theta_temps_legend_label->setAlignment(Qt::AlignCenter);
        angle_unite_listBox = new QComboBox(graph_theta_legend);
        angle_unite_listBox->setObjectName("angle_unite_listBox");
        angle_unite_listBox->setGeometry(QRect(14, 80, 69, 22));
        angle_unite_listBox->setFont(font1);
        angle_unite_listBox->setStyleSheet(QString::fromUtf8("color: rgb(0, 170, 93);\n"
"background-color: rgb(0, 0, 0);"));
        angle_unite_listBox->setEditable(false);
        angle_unite_listBox->setMaxVisibleItems(2);
        angle_unite_listBox->setMaxCount(2);
        angle_unite_listBox->setInsertPolicy(QComboBox::NoInsert);
        splitter_3->addWidget(graph_theta_legend);
        graph_thrust_legend = new QWidget(splitter_3);
        graph_thrust_legend->setObjectName("graph_thrust_legend");
        graph_thrust_legend->setMinimumSize(QSize(0, 175));
        graph_thrust_legend->setMaximumSize(QSize(96, 16777215));
        graph_thrust_legend->setFont(font);
        graph_thrust_legend->setStyleSheet(QString::fromUtf8("color: rgb(0, 127, 0);"));
        graph_thrust_value_label = new QLabel(graph_thrust_legend);
        graph_thrust_value_label->setObjectName("graph_thrust_value_label");
        graph_thrust_value_label->setGeometry(QRect(0, 52, 91, 20));
        sizePolicy.setHeightForWidth(graph_thrust_value_label->sizePolicy().hasHeightForWidth());
        graph_thrust_value_label->setSizePolicy(sizePolicy);
        graph_thrust_value_label->setFont(font1);
        graph_thrust_value_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 0, 0);"));
        graph_thrust_value_label->setAlignment(Qt::AlignCenter);
        graph_thrust_time_label = new QLabel(graph_thrust_legend);
        graph_thrust_time_label->setObjectName("graph_thrust_time_label");
        graph_thrust_time_label->setGeometry(QRect(-2, 10, 96, 38));
        graph_thrust_time_label->setFont(font1);
        graph_thrust_time_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 0, 0);"));
        graph_thrust_time_label->setAlignment(Qt::AlignCenter);
        graph_thrust_temps_legend_label = new QLabel(graph_thrust_legend);
        graph_thrust_temps_legend_label->setObjectName("graph_thrust_temps_legend_label");
        graph_thrust_temps_legend_label->setGeometry(QRect(0, 160, 91, 20));
        sizePolicy.setHeightForWidth(graph_thrust_temps_legend_label->sizePolicy().hasHeightForWidth());
        graph_thrust_temps_legend_label->setSizePolicy(sizePolicy);
        graph_thrust_temps_legend_label->setMinimumSize(QSize(91, 20));
        graph_thrust_temps_legend_label->setMaximumSize(QSize(91, 20));
        graph_thrust_temps_legend_label->setFont(font);
        graph_thrust_temps_legend_label->setStyleSheet(QString::fromUtf8("color: rgb(0, 170, 93);"));
        graph_thrust_temps_legend_label->setAlignment(Qt::AlignCenter);
        splitter_3->addWidget(graph_thrust_legend);
        graph_theta_dot_legend = new QWidget(splitter_3);
        graph_theta_dot_legend->setObjectName("graph_theta_dot_legend");
        graph_theta_dot_legend->setMinimumSize(QSize(0, 175));
        graph_theta_dot_legend->setMaximumSize(QSize(96, 16777215));
        graph_theta_dot_legend->setFont(font);
        graph_theta_dot_legend->setAutoFillBackground(false);
        graph_theta_dot_legend->setStyleSheet(QString::fromUtf8("color: rgb(0, 127, 0);"));
        graph_theta_dot_time_label = new QLabel(graph_theta_dot_legend);
        graph_theta_dot_time_label->setObjectName("graph_theta_dot_time_label");
        graph_theta_dot_time_label->setGeometry(QRect(-2, 10, 96, 31));
        graph_theta_dot_time_label->setFont(font1);
        graph_theta_dot_time_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 0, 0);"));
        graph_theta_dot_time_label->setAlignment(Qt::AlignCenter);
        graph_theta_dot_value_label = new QLabel(graph_theta_dot_legend);
        graph_theta_dot_value_label->setObjectName("graph_theta_dot_value_label");
        graph_theta_dot_value_label->setGeometry(QRect(0, 54, 91, 20));
        sizePolicy.setHeightForWidth(graph_theta_dot_value_label->sizePolicy().hasHeightForWidth());
        graph_theta_dot_value_label->setSizePolicy(sizePolicy);
        graph_theta_dot_value_label->setFont(font1);
        graph_theta_dot_value_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 0, 0);"));
        graph_theta_dot_value_label->setAlignment(Qt::AlignCenter);
        graph_theta_dot_temps_legend_label = new QLabel(graph_theta_dot_legend);
        graph_theta_dot_temps_legend_label->setObjectName("graph_theta_dot_temps_legend_label");
        graph_theta_dot_temps_legend_label->setGeometry(QRect(0, 160, 91, 20));
        sizePolicy.setHeightForWidth(graph_theta_dot_temps_legend_label->sizePolicy().hasHeightForWidth());
        graph_theta_dot_temps_legend_label->setSizePolicy(sizePolicy);
        graph_theta_dot_temps_legend_label->setMinimumSize(QSize(91, 20));
        graph_theta_dot_temps_legend_label->setMaximumSize(QSize(91, 20));
        graph_theta_dot_temps_legend_label->setFont(font);
        graph_theta_dot_temps_legend_label->setStyleSheet(QString::fromUtf8("color: rgb(0, 170, 93);"));
        graph_theta_dot_temps_legend_label->setAlignment(Qt::AlignCenter);
        splitter_3->addWidget(graph_theta_dot_legend);
        graph_theta_dotdot_legend = new QWidget(splitter_3);
        graph_theta_dotdot_legend->setObjectName("graph_theta_dotdot_legend");
        graph_theta_dotdot_legend->setMinimumSize(QSize(0, 175));
        graph_theta_dotdot_legend->setMaximumSize(QSize(96, 16777215));
        graph_theta_dotdot_legend->setFont(font);
        graph_theta_dotdot_legend->setStyleSheet(QString::fromUtf8("color: rgb(0, 127, 0);"));
        graph_theta_dotdot_value_label = new QLabel(graph_theta_dotdot_legend);
        graph_theta_dotdot_value_label->setObjectName("graph_theta_dotdot_value_label");
        graph_theta_dotdot_value_label->setGeometry(QRect(0, 50, 91, 20));
        sizePolicy.setHeightForWidth(graph_theta_dotdot_value_label->sizePolicy().hasHeightForWidth());
        graph_theta_dotdot_value_label->setSizePolicy(sizePolicy);
        graph_theta_dotdot_value_label->setFont(font1);
        graph_theta_dotdot_value_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 0, 0);"));
        graph_theta_dotdot_value_label->setAlignment(Qt::AlignCenter);
        graph_theta_dotdot_time_label = new QLabel(graph_theta_dotdot_legend);
        graph_theta_dotdot_time_label->setObjectName("graph_theta_dotdot_time_label");
        graph_theta_dotdot_time_label->setGeometry(QRect(0, 8, 96, 38));
        graph_theta_dotdot_time_label->setFont(font1);
        graph_theta_dotdot_time_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 0, 0);"));
        graph_theta_dotdot_time_label->setAlignment(Qt::AlignCenter);
        graph_theta_dotdot_temps_legend_label = new QLabel(graph_theta_dotdot_legend);
        graph_theta_dotdot_temps_legend_label->setObjectName("graph_theta_dotdot_temps_legend_label");
        graph_theta_dotdot_temps_legend_label->setGeometry(QRect(0, 160, 91, 20));
        sizePolicy.setHeightForWidth(graph_theta_dotdot_temps_legend_label->sizePolicy().hasHeightForWidth());
        graph_theta_dotdot_temps_legend_label->setSizePolicy(sizePolicy);
        graph_theta_dotdot_temps_legend_label->setMinimumSize(QSize(91, 20));
        graph_theta_dotdot_temps_legend_label->setMaximumSize(QSize(91, 20));
        graph_theta_dotdot_temps_legend_label->setFont(font);
        graph_theta_dotdot_temps_legend_label->setStyleSheet(QString::fromUtf8("color: rgb(0, 170, 93);"));
        graph_theta_dotdot_temps_legend_label->setAlignment(Qt::AlignCenter);
        graph_theta_dotdot_controlleur_time_label = new QLabel(graph_theta_dotdot_legend);
        graph_theta_dotdot_controlleur_time_label->setObjectName("graph_theta_dotdot_controlleur_time_label");
        graph_theta_dotdot_controlleur_time_label->setGeometry(QRect(-2, 78, 96, 38));
        graph_theta_dotdot_controlleur_time_label->setFont(font1);
        graph_theta_dotdot_controlleur_time_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 0, 0);"));
        graph_theta_dotdot_controlleur_time_label->setAlignment(Qt::AlignCenter);
        graph_rouge_label = new QLabel(graph_theta_dotdot_legend);
        graph_rouge_label->setObjectName("graph_rouge_label");
        graph_rouge_label->setGeometry(QRect(-2, 44, 96, 6));
        graph_rouge_label->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));
        graph_bleu_label = new QLabel(graph_theta_dotdot_legend);
        graph_bleu_label->setObjectName("graph_bleu_label");
        graph_bleu_label->setGeometry(QRect(-2, 120, 96, 6));
        graph_bleu_label->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 0, 255);"));
        graph_theta_dotdot_controlleur_value_label = new QLabel(graph_theta_dotdot_legend);
        graph_theta_dotdot_controlleur_value_label->setObjectName("graph_theta_dotdot_controlleur_value_label");
        graph_theta_dotdot_controlleur_value_label->setGeometry(QRect(0, 130, 91, 20));
        sizePolicy.setHeightForWidth(graph_theta_dotdot_controlleur_value_label->sizePolicy().hasHeightForWidth());
        graph_theta_dotdot_controlleur_value_label->setSizePolicy(sizePolicy);
        graph_theta_dotdot_controlleur_value_label->setFont(font1);
        graph_theta_dotdot_controlleur_value_label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 0, 0);"));
        graph_theta_dotdot_controlleur_value_label->setAlignment(Qt::AlignCenter);
        splitter_3->addWidget(graph_theta_dotdot_legend);

        horizontalLayout->addWidget(splitter_3);

        view_fly_arm->setCentralWidget(centralwidget);
        menubar = new QMenuBar(view_fly_arm);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1520, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuModes = new QMenu(menubar);
        menuModes->setObjectName("menuModes");
        menuSetting = new QMenu(menubar);
        menuSetting->setObjectName("menuSetting");
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName("menuHelp");
        view_fly_arm->setMenuBar(menubar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuModes->menuAction());
        menubar->addAction(menuSetting->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_Data);
        menuFile->addAction(actionExit);
        menuModes->addAction(actionPC_Controleur);
        menuModes->addAction(actionHils_mode_1);
        menuModes->addAction(actionPC_Display_Real_angle);
        menuModes->addAction(actionHils_mode_3);
        menuModes->addAction(actionDemo_Manuel_Thrust_Command);
        menuModes->addAction(actionFormule_Calcul_Thrust);
        menuSetting->addAction(actionSample_time_Configuration);
        menuSetting->addAction(actionPC_controller_Configuration);
        menuHelp->addAction(actionHow_to_do);
        menuHelp->addAction(actionOnline);
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionRepair_file_setting_txt);

        retranslateUi(view_fly_arm);

        play_button->setDefault(false);
        com_port_comboBox->setCurrentIndex(-1);
        baud_rate_comboBox->setCurrentIndex(-1);
        disconnect_button->setDefault(false);
        angle_unite_listBox->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(view_fly_arm);
    } // setupUi

    void retranslateUi(QMainWindow *view_fly_arm)
    {
        view_fly_arm->setWindowTitle(QCoreApplication::translate("view_fly_arm", "Balancing Arm System", nullptr));
        actionOpen->setText(QCoreApplication::translate("view_fly_arm", "Open", nullptr));
        actionSave->setText(QCoreApplication::translate("view_fly_arm", "Save", nullptr));
        actionExit->setText(QCoreApplication::translate("view_fly_arm", "Exit", nullptr));
        actionPC_Controleur->setText(QCoreApplication::translate("view_fly_arm", "PC Controller", nullptr));
        actionHils_mode_1->setText(QCoreApplication::translate("view_fly_arm", "Hils_mode 1", nullptr));
        actionPC_Display_Real_angle->setText(QCoreApplication::translate("view_fly_arm", "PC display - real angle", nullptr));
        actionHils_mode_3->setText(QCoreApplication::translate("view_fly_arm", "Hils_mode 3", nullptr));
        actionDemo_Manuel_Thrust_Command->setText(QCoreApplication::translate("view_fly_arm", "Demo: Manuel Thrust Command", nullptr));
        actionSample_time_Configuration->setText(QCoreApplication::translate("view_fly_arm", "Sample time  --> Configuration", nullptr));
        actionPC_controller_Configuration->setText(QCoreApplication::translate("view_fly_arm", "PC controller --> Configuration", nullptr));
        actionHow_to_do->setText(QCoreApplication::translate("view_fly_arm", "How to do", nullptr));
        actionOnline->setText(QCoreApplication::translate("view_fly_arm", "Online", nullptr));
        actionAbout->setText(QCoreApplication::translate("view_fly_arm", "About", nullptr));
        actionRepair_file_setting_txt->setText(QCoreApplication::translate("view_fly_arm", "Repair 'file.setting.txt'", nullptr));
        actionFormule_Calcul_Thrust->setText(QCoreApplication::translate("view_fly_arm", "Formule Calcul Thrust", nullptr));
        actiontests->setText(QCoreApplication::translate("view_fly_arm", "Tests", nullptr));
        actionSave_Data->setText(QCoreApplication::translate("view_fly_arm", "Save Data", nullptr));
        groupBox_6->setTitle(QString());
        modes_panel->setTitle(QString());
        mode_label->setText(QCoreApplication::translate("view_fly_arm", "Mode:", nullptr));
        play_button->setText(QString());
        pause_button->setText(QString());
        stop_button->setText(QString());
        time_label->setText(QCoreApplication::translate("view_fly_arm", "Time   :", nullptr));
        theta_label->setText(QCoreApplication::translate("view_fly_arm", "Theta  :", nullptr));
        thrust_label->setText(QCoreApplication::translate("view_fly_arm", "Thrust :", nullptr));
        theta_value_label->setText(QCoreApplication::translate("view_fly_arm", "0.0", nullptr));
        thrust_value_label->setText(QCoreApplication::translate("view_fly_arm", "0.0", nullptr));
        time_unite_label->setText(QCoreApplication::translate("view_fly_arm", "s", nullptr));
        theta_unite_label->setText(QCoreApplication::translate("view_fly_arm", "deg", nullptr));
        thrust_unite_label->setText(QCoreApplication::translate("view_fly_arm", "N", nullptr));
        theta_OR_thrust_desired_label->setText(QCoreApplication::translate("view_fly_arm", "Desired Theta :", nullptr));
        theta_OR_thrust_desired_value_label->setText(QCoreApplication::translate("view_fly_arm", "180", nullptr));
        theta_desired_unite_label->setText(QCoreApplication::translate("view_fly_arm", "deg", nullptr));
        terminal_panel->setTitle(QString());
        consol_panel->setTitle(QString());
        consol_panel_label->setText(QCoreApplication::translate("view_fly_arm", "Consol", nullptr));
        usb_port_radioButton->setText(QCoreApplication::translate("view_fly_arm", "USB Port", nullptr));
        com_port_radioButton->setText(QCoreApplication::translate("view_fly_arm", "COM Port", nullptr));
        baud_rate_label->setText(QCoreApplication::translate("view_fly_arm", "Baud Rate", nullptr));
        com_port_comboBox->setCurrentText(QString());
        baud_rate_comboBox->setCurrentText(QString());
        connect_button->setText(QCoreApplication::translate("view_fly_arm", "Connect", nullptr));
        disconnect_button->setText(QCoreApplication::translate("view_fly_arm", "Disconnect", nullptr));
        status_groupBox->setTitle(QCoreApplication::translate("view_fly_arm", "Status", nullptr));
        status_label->setText(QCoreApplication::translate("view_fly_arm", "Disconnect", nullptr));
        terminal_groupBox->setTitle(QCoreApplication::translate("view_fly_arm", "Terminal", nullptr));
        terminal_info_label->setText(QCoreApplication::translate("view_fly_arm", "Press connect ...", nullptr));
        signal_value_progressBar->setFormat(QString());
        terminal_signal_value_label->setText(QString());
        graph_theta_value_label->setText(QCoreApplication::translate("view_fly_arm", "0.0", nullptr));
        graph_theta_time_label->setText(QCoreApplication::translate("view_fly_arm", "Theta\n"
"Time", nullptr));
        graph_theta_temps_legend_label->setText(QCoreApplication::translate("view_fly_arm", "Time ( s )", nullptr));
        angle_unite_listBox->setCurrentText(QString());
        graph_thrust_value_label->setText(QCoreApplication::translate("view_fly_arm", "0.0", nullptr));
        graph_thrust_time_label->setText(QCoreApplication::translate("view_fly_arm", "Thrust\n"
"Time", nullptr));
        graph_thrust_temps_legend_label->setText(QCoreApplication::translate("view_fly_arm", "Time ( s )", nullptr));
        graph_theta_dot_time_label->setText(QCoreApplication::translate("view_fly_arm", "Theta Dot\n"
"Time", nullptr));
        graph_theta_dot_value_label->setText(QCoreApplication::translate("view_fly_arm", "0.0", nullptr));
        graph_theta_dot_temps_legend_label->setText(QCoreApplication::translate("view_fly_arm", "Time ( s )", nullptr));
        graph_theta_dotdot_value_label->setText(QCoreApplication::translate("view_fly_arm", "0.0", nullptr));
        graph_theta_dotdot_time_label->setText(QCoreApplication::translate("view_fly_arm", "Theta DotDot\n"
"Time", nullptr));
        graph_theta_dotdot_temps_legend_label->setText(QCoreApplication::translate("view_fly_arm", "Time ( s )", nullptr));
        graph_theta_dotdot_controlleur_time_label->setText(QCoreApplication::translate("view_fly_arm", "Theta DotDot\n"
"Controlleur", nullptr));
        graph_rouge_label->setText(QString());
        graph_bleu_label->setText(QString());
        graph_theta_dotdot_controlleur_value_label->setText(QCoreApplication::translate("view_fly_arm", "0.0", nullptr));
        menuFile->setTitle(QCoreApplication::translate("view_fly_arm", "File", nullptr));
        menuModes->setTitle(QCoreApplication::translate("view_fly_arm", "Mode", nullptr));
        menuSetting->setTitle(QCoreApplication::translate("view_fly_arm", "Setting", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("view_fly_arm", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class view_fly_arm: public Ui_view_fly_arm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEW_FLY_ARM_H
