/*
    Guillaume Ducard, May 2013
    Controller for the balancing arm
    This fonction/class needs to know the current arm angle and angle speed

*/

#ifndef ARM_PROP_CONTROLLER_H
#define ARM_PROP_CONTROLLER_H


class ArmPropSimulator;

#include "model_setting_PC_controller.h"
#include "arm_prop_simulator.h"

class ArmPropController
{
    public:
        static ArmPropController* getInstance();

        void init(void);
        void RunArmController(void);

        // Some Get functions
        double GetThetaCmd(void);
        double GetThetaDotdotCmd(void);
        double GetThrustCmd(void);

        // Some Set functions
        void SetThetaCmd(double);
        void SetThetaDotdotCmd(double);
        void SetThrustCmd(double);

        // FORMULE
        double* formule_GetThetaCmd(void);
        double* formule_GetThetaDotdotCmd(void);

    private:
        ArmPropController(); // constructor
        ~ArmPropController(); //destructor

        double thetaCmd_;
        double theta_dot_cmd;
        double theta_dotdot_cmd;
        double propThrustcmd;

        // for theta and theta_dot
        ArmPropSimulator* myArmPropSimulator;
        // for k1 (K_theta) and k2 (K_theta_dot)
        model_setting_PC_controller* myModel_setting_PC_controller;
};

#endif
