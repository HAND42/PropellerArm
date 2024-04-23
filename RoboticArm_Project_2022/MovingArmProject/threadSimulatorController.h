#ifndef THREADSIMULATORCONTROLLER_H
#define THREADSIMULATORCONTROLLER_H

#define START true
#define STOP false

class hilsModeSerialCommunicator;

#include "include_define.h"

#include "model_setting_PC_controller.h"
#include "model_setting_sample_time.h"

#include "arm_prop_simulator.h"
#include "arm_prop_controller.h"
#include "hilsModeSerialCommunicator.h"

class threadSimulatorController
{
    public:
        static threadSimulatorController* getInstance();

        void init(void);
        void runSimulator(void);
        void runController(void);

        void hils_mode_update(int);
        void hils_mode_execute(void);

    private:
        threadSimulatorController(void);
        ~threadSimulatorController(void);

        ArmPropSimulator* myArmPropSimulator;
        ArmPropController* myArmPropController;
        hilsModeSerialCommunicator* myHilsModeSerialCommunicator;
        model_setting_PC_controller* myModel_setting_PC_controller;
        model_setting_sample_time* my_model_setting_sample_time;

        double e_k;
        double e_k_1;
        double e_k_2;
        double theta_dotdot_desired_k_1;
        double theta_dotdot_desired_k_2;

        void (threadSimulatorController::*ptr_hils_modes_function)(void);
        // hils_modes functions
        void hils_mode_pc_controller_function(void);
        void hils_mode_1_function(void);
        void hils_mode_real_angle_function(void);
        void hils_mode_3_function(void);
        void hils_mode_manual_thrust_command_function(void);

        //GD HILS addon: 26.02.2015
        int convertAngleDegtoADCvalue(double);

        void temp(void); // A SUPPRIMER
};

#endif
