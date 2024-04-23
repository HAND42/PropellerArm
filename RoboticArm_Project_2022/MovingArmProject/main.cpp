#include "view_fly_arm.h"

#include <QApplication>

#include "arm_prop_simulator.h"
#include "arm_prop_controller.h"
#include "threadSimulatorController.h"
#include "hilsModeSerialCommunicator.h"

#include "model_time.h"
#include "model_setting_PC_controller.h"
#include "model_setting_sample_time.h"
#include "model_file_setting.h"
#include "model_formule.h"

#include "verification_saisie_utilisateur.h"

void test_verification_saisie_utilisateur(void);
void afficher_les_settings(void);

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    // pointeurs sur l'unique instance de la classe UniqueObject
    ArmPropSimulator* myArmPropSimulator = ArmPropSimulator::getInstance();
    myArmPropSimulator->init();

    ArmPropController* myArmPropController = ArmPropController::getInstance();
    myArmPropController->init();

    threadSimulatorController* myThreadSimulatorController = threadSimulatorController::getInstance();
    myThreadSimulatorController->init();

    hilsModeSerialCommunicator* myHilsModeSerialCommunicator = hilsModeSerialCommunicator::getInstance();
    myHilsModeSerialCommunicator->init();

    model_time* myTime = model_time::getInstance();
    myTime->init();

    model_setting_PC_controller* myModel_setting_PC_controller = model_setting_PC_controller::getInstance();
    myModel_setting_PC_controller->init();

    model_setting_sample_time* my_model_setting_sample_time = model_setting_sample_time::getInstance();
    my_model_setting_sample_time->init();

    model_file_setting* myModel_file_setting = model_file_setting::getInstance();
    myModel_file_setting->init();
    myModel_file_setting->file_read();

    model_formule* myModel_formule = model_formule::getInstance();
    myModel_formule->init();

    view_fly_arm w;
    w.show();

//    myModel_file_setting->file_write();

//    **************************************************
//    TESTS DEBUT
//    test_classes();
//    afficher_les_settings();

//    myModel_setting_PC_controller->init();
//    my_model_setting_sample_time->init();
//    myTime->init();
//    TESTS FIN
//    **************************************************

//    test_verification_saisie_utilisateur();

    return a.exec();
}


void test_verification_saisie_utilisateur(void)
{
    verification_saisie_utilisateur* ma_verification_saisie_utilisateur = verification_saisie_utilisateur::getInstance();

    qDebug() << "3 :" << ma_verification_saisie_utilisateur->saisie_nombre_valide_setting_PC_controller("3");
    qDebug() << "-2 :" << ma_verification_saisie_utilisateur->saisie_nombre_valide_setting_PC_controller("-2");
    qDebug() << "1.0 :" << ma_verification_saisie_utilisateur->saisie_nombre_valide_setting_PC_controller("1.0");
    qDebug() << "-10 :" << ma_verification_saisie_utilisateur->saisie_nombre_valide_setting_PC_controller("-10");
    qDebug() << "-14.0 :" << ma_verification_saisie_utilisateur->saisie_nombre_valide_setting_PC_controller("-14.0");
    qDebug() << "AAAA0 :" << ma_verification_saisie_utilisateur->saisie_nombre_valide_setting_PC_controller("AAAA0");
    qDebug() << "-0.47AAAA0 :" << ma_verification_saisie_utilisateur->saisie_nombre_valide_setting_PC_controller("-0.47AAAA0");
    qDebug() << "3 :" << ma_verification_saisie_utilisateur->saisie_nombre_valide_setting_sample_time("3");
    qDebug() << "-2 :" << ma_verification_saisie_utilisateur->saisie_nombre_valide_setting_sample_time("-2");
    qDebug() << "1.0 :" << ma_verification_saisie_utilisateur->saisie_nombre_valide_setting_sample_time("1.0");
    qDebug() << "-10 :" << ma_verification_saisie_utilisateur->saisie_nombre_valide_setting_sample_time("-10");
    qDebug() << "140 :" << ma_verification_saisie_utilisateur->saisie_nombre_valide_setting_sample_time("140");
    qDebug() << "AAAA0 :" << ma_verification_saisie_utilisateur->saisie_nombre_valide_setting_sample_time("AAAA0");
    qDebug() << "-0.47AAAA0 :" << ma_verification_saisie_utilisateur->saisie_nombre_valide_setting_sample_time("-0.47AAAA0");
}

void afficher_les_settings(void)
{
    model_setting_PC_controller* myModel_setting_PC_controller = model_setting_PC_controller::getInstance();
    model_setting_sample_time* my_model_setting_sample_time = model_setting_sample_time::getInstance();
    model_time* myTime = model_time::getInstance();

    qDebug() << "controller_type_string_get :" << myModel_setting_PC_controller->controller_type_string_get();
    qDebug() << "k1_get :" << myModel_setting_PC_controller->k1_get();
    qDebug() << "k2_get :" << myModel_setting_PC_controller->k2_get();
    qDebug() << "a1_get :" << myModel_setting_PC_controller->a1_get();
    qDebug() << "a2_get :" << myModel_setting_PC_controller->a2_get();
    qDebug() << "b1_get :" << myModel_setting_PC_controller->b1_get();
    qDebug() << "b2_get :" << myModel_setting_PC_controller->b2_get();
    qDebug() << "b3_get :" << myModel_setting_PC_controller->b3_get();
    qDebug() << "time_step :" << my_model_setting_sample_time->step_get();
    qDebug() << "simulation_step :" << my_model_setting_sample_time->simulator_step_get();
    qDebug() << "controller_step :" << my_model_setting_sample_time->controller_step_get();
    qDebug() << "graph_step :" << my_model_setting_sample_time->graph_step_get();
    qDebug() << "time_desired :" << myTime->time_desired_get();
}
