#ifndef VIEW_FLY_ARM_H
#define VIEW_FLY_ARM_H

#include <QMainWindow>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QList>
#include <QRandomGenerator>
#include <QtMath>
#include <QRect>
#include <QPointF>
#include <QPixmap>
#include <QBasicTimer>
#include <QThread>
#include <QTime>
#include <QDateTime>
#include <QSerialPort>
#include <QSerialPortInfo>
//#include <QtConcurrent>

#include "arm_prop_simulator.h"
#include "arm_prop_controller.h"
#include "hilsModeSerialCommunicator.h"
#include "threadSimulatorController.h"

#include "model_draw.h"
#include "model_file_setting.h"
#include "model_time.h"
#include "model_setting_PC_controller.h"
#include "model_setting_sample_time.h"
#include "model_formule.h"

#include "view_graph.h"
#include "view_about.h"
#include "view_setting_sample_time.h"
#include "view_setting_PC_controller.h"
#include "view_formule.h"

#define HAUTEUR_A_ENLEVER 27

#define ANALYSE_INDEX  0
#define TEST_INDEX     1
#define VARIABLE_INDEX 2

QT_BEGIN_NAMESPACE
namespace Ui { class view_fly_arm; }
QT_END_NAMESPACE

class view_fly_arm : public QMainWindow
{
    Q_OBJECT

    public:
        view_fly_arm(QWidget *parent = nullptr);
        ~view_fly_arm() override;

    public slots:
        void slot_formule_a_tester(void); // FORMULE

    protected:
      void resizeEvent(QResizeEvent *event) override;
      void timerEvent(QTimerEvent *event) override;
      void closeEvent(QCloseEvent *event) override;

    private slots:
      void on_actionSave_triggered();

      void on_actionSave_Data_triggered();

      void on_actionExit_triggered();

      void on_actionPC_Controleur_triggered();

      void on_actionHils_mode_1_triggered();

      void on_actionPC_Display_Real_angle_triggered();

      void on_actionHils_mode_3_triggered();

      void on_actionDemo_Manuel_Thrust_Command_triggered();

      void on_actionFormule_Calcul_Thrust_triggered();

      void on_actionSample_time_Configuration_triggered();

      void on_actionPC_controller_Configuration_triggered();

      void on_actionAbout_triggered();

      void on_actionRepair_file_setting_txt_triggered();

      void on_play_button_clicked();

      void on_pause_button_clicked();

      void on_stop_button_clicked();

      void on_usb_port_radioButton_clicked();

      void on_com_port_radioButton_clicked();

      void on_com_port_comboBox_currentIndexChanged(int index);

      void on_baud_rate_comboBox_currentIndexChanged(int index);

      void on_connect_button_clicked();

      void on_disconnect_button_clicked();

      void on_theta_OR_thrust_desired_trackBar_valueChanged(int value);

      void on_time_textBox_textChanged(const QString &arg1);

      void update_graphs_sample_time_timer(void); //SLOT - NE PAS DEPLACER DANS LA PARTIE FONCTIONS PRIVATE

      void update_controller_type(void); //SLOT - NE PAS DEPLACER DANS LA PARTIE FONCTIONS PRIVATE

      // FORMULE
      void view_formule_closed(void);

      void view_formule_modification(void);

  private:
        Ui::view_fly_arm *ui;

        model_draw *myDraw;
        model_file_setting* myModel_file_setting;
        model_time* myTime;

        // CLASSES VUES
        view_graph graph_theta;
        view_graph graph_thrust;
        view_graph graph_theta_dot;
        view_graph graph_theta_dotdot;

        view_about myView_about;
        view_setting_PC_controller myView_setting_PC_controller;
        view_setting_sample_time myView_setting_sample_time;

        ArmPropSimulator* myArmPropSimulator;
        ArmPropController* myArmPropController;
        hilsModeSerialCommunicator* myHilsModeSerialCommunicator;
        threadSimulatorController* myThreadSimulatorController;
        model_setting_PC_controller* myModel_setting_PC_controller;
        model_setting_sample_time* my_model_setting_sample_time;

        QBasicTimer* timer1;
        QSerialPort* serialPort;

        QString controller_type_name;
        QString bouton_gris;
        QString bouton_vert;
        QString bouton_racine;

        bool myThreadSimulatorController_is_create;
        int time_desired;

        double DesiredTheta_Rad;
        double DesiredTheta_Deg;

        // attributs pour 'setting_sample_time
        int tick_compteur;
        int tick_max; // = nombre de tick pour la durée déterminée
        QTime tick_duree_totale;
        int step;
        int simulator_step;
        int controller_step;
        int graph_step;
        int simulator_step_count;
        int controller_step_count;
        int graph_step_count;
        double timer_graph;
        double timer_graph_step;

        bool hils_mode_choisi;

        int signalValue;
        bool theta_desired;             // choice: theta desired OR thrust desired
        double thrust_desired;

        double rad_arm;
        double degree_arm;
        double thrust;
        double theta_dot;
        double theta_dotdot;
        double theta_dotdot_controlleur;

        bool consol_is_show;
        bool theta_desired_is_show;

        // FORMULE
        model_formule* myModel_formule;
        view_formule* myView_formule;

        bool formule_active;
        QStringList formule_variables_possibles;
        QList<int> formule_variables_possibles_index;
        QList<double*> formule_valeurs;
        double formule_propThrustcmd;
        // FORMULE: ((0.0576 * thetadotdot) + (0.48 * 0.220 * 9.81 * sin(theta))) / 0.65

        void donne_a_la_formule_les_variables_possibles(void);

        // VIEW_FLY_ARM
        void console_info_update(void);

        void disconnect_method(void);

        void graphs_draw(void);

        void objects_init(void);
        void sample_time_init(void);
        void graph_init(void);
        void graph_after_change_sample_time(void);
        void attributs_init(void);
        void widgets_hide(void);

        void modes_infos_communes(bool consol_is_show_bool, bool consol_show_OR_hide_bool, bool theta_OR_thrust_desired_is_show_bool, bool theta_OR_thrust_desired_show_OR_hide_bool, QString texte, int hils_mode_int);

        void buttons_enabled(bool play_state,bool pause_state, bool stop_state);
        void theta_desired_show_OR_hide(bool true_or_false);
        void theta_OR_thrust_desired_save(int new_value);

        void consol_show_OR_hide(bool true_or_false);
        void connect_disconnect_true_or_false(bool true_or_false);

        void graphs_clear(void);

        void find_ports(void);
        void connect_button_state(void);
        void disconnect_serial_port(void);

        // ajuster le temps d'exécution de QBasicTimer
        int temps_execution_theorique;
        int temps_execution_reel;
        int timer_en_avance;
        int temps_a_rattraper;

        void timer_ajuster_init(void);

        // AFFICHAGE
        qreal graph_temps_label_top;
        QRect nouvelles_dimensions;

        // VALEURS DES VARIABLES ENREGISTREES LORS DE L'EXECUTION DE QTIMER
//        QStringList variables_valeurs_enregistrees;
        QString variables_valeurs_enregistrees;
        QString variables_valeurs_timer; // partie = 1
        QString variables_valeurs_ArmPropController; // partie = 2
        QString variables_valeurs_ArmPropSimulator; // partie = 3
        QString variables_valeurs_graph_1; // partie = 4
        QString variables_valeurs_graph_2; // partie = 5
        void variables_valeurs_a_enregistrer_initialisation(void);
        void variables_valeurs_a_enregistrer(const int partie);
// ======================================================
// PARTIE TEMPORAIRE
        void remplir_valeurs(void);

        int nombre_d_elements;
        QPointF valeurs_temp;
        QPointF valeur2;

        QRandomGenerator generateur_nombre_aleatoire;
};
#endif // VIEW_FLY_ARM_H

// TEST DE MERCURIAL
// AJOUT DE COMMENTAIRES POUR VERIFIER QUE MERCURIAL REPERE LES CHANGEMENTS DANS LES FICHIERS
