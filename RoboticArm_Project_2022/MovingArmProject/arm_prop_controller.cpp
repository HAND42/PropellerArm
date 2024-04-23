/****************************************************************************

    Guillaume Ducard, May 2013
    Controller for the balancing arm
    This fonction/class needs to know the current arm angle and angle speed

*****************************************************************************/
#include "arm_prop_controller.h"
#include <algorithm>
#include "include_define.h"


ArmPropController *ArmPropController::getInstance()
{
    static ArmPropController* instance_classe = nullptr;

    if (instance_classe == nullptr)
        instance_classe = new ArmPropController();

    return  instance_classe;
}

void ArmPropController::init(void)
{
    this->thetaCmd_ = 0.0;
    this->theta_dot_cmd = 0.0;
    this->theta_dotdot_cmd = 0.0;
    this->propThrustcmd = 0.0;
}

// The function that effectively runs the controller code
void ArmPropController::RunArmController(void)
{
    //    this->theta_dot_cmd = this->myModel_setting_PC_controller->k1_get() * (this->thetaCmd_ - theta);
//    this->theta_dotdot_cmd = this->myModel_setting_PC_controller->k2_get() * (this->theta_dot_cmd - theta_dot);
    // fonction de base de Guillaume
    //double thrust = (Inertia_Jz/L_ARM)* ( this->theta_dotdot_cmd + distOG * mass_arm_prop * gravity * sin(this->thetaCmd_)/Inertia_Jz);
    this->propThrustcmd = std::clamp((Inertia_Jz * this->theta_dotdot_cmd) + (distOG * mass_arm_prop * gravity * sin(this->thetaCmd_)) / L_ARM, MIN_THRUST, MAX_THRUST);

}

//
// GET FUNCTIONS
//
double ArmPropController::GetThetaCmd(void)
{
    return this->thetaCmd_;
}

double ArmPropController::GetThetaDotdotCmd(void)
{
    return this->theta_dotdot_cmd;
}

double ArmPropController::GetThrustCmd(void)
{
    return this->propThrustcmd;
}

//
// SET FUNCTIONS
//
void ArmPropController::SetThetaDotdotCmd(double NewThetaDotdotCmd)
{
    this->theta_dotdot_cmd = NewThetaDotdotCmd;
}

void ArmPropController::SetThetaCmd(double NewThetaCmd)
{
    this->thetaCmd_ = NewThetaCmd;
}

void ArmPropController::SetThrustCmd(double NewThrustCmd)
{
    this->propThrustcmd = NewThrustCmd;
}

double *ArmPropController::formule_GetThetaCmd()
{
    return &this->thetaCmd_;
}

double *ArmPropController::formule_GetThetaDotdotCmd()
{
    return &this->theta_dotdot_cmd;
}

//
// constructor
//
ArmPropController::ArmPropController()
{
    this->myArmPropSimulator = ArmPropSimulator::getInstance();
    this->myModel_setting_PC_controller = model_setting_PC_controller::getInstance();
}

//
// destructor
//
ArmPropController::~ArmPropController()
{

}
