//*************************************************************
// Last Update: May 2013
// Guillaume DUCARD
// Balancing Arm Simulator
//*************************************************************
#include "arm_prop_simulator.h"
//#include <fstream>

//#define DEBUG_LOG_DOUBLE(__X) (*fsim<< (__X) << " ") ; fsim->flush();
//#define DEBUG_LOG_STRING(__X) (*fsim<< (__X)) ; fsim->flush();

ArmPropSimulator *ArmPropSimulator::getInstance()
{
    static ArmPropSimulator* instance_classe = nullptr;

    if (instance_classe == nullptr)
        instance_classe = new ArmPropSimulator();

    return  instance_classe;
}

void ArmPropSimulator::init(void)
{
    this->theta_ = 0.0;
    this->theta_dot_ = 0.0;
    this->theta_dotdot_ = 0.0;
    this->prop_thrust_ = 0.0;

    //fsim = NULL; // log file

}

//********************************************************
//
//********************************************************
void ArmPropSimulator::Run(double sampleTime, double propThrustcmd)
{
    assert((sampleTime > 0) && (sampleTime<=1));

    /*
    if (sampleTime <= 0 || sampleTime>1)// || _isnan(sampleTime))
    {
        printf("Something is wrong with sampleTime: %l", sampleTime);
    }
    */
    //==============================
    // 1. Create file to log data
    //==============================
    /*if(fsim==NULL)// TODO: temporary
    {
        char buf[100];
        sprintf_s(buf,100,"gsimdata.txt");
        fsim = new fstream(buf, ios::out);
    }*/
    //====================================
    // 2. get the propeller thrust command
    //====================================
    this->prop_thrust_ = propThrustcmd;

    //Equation to program: theta_dotdot = (1/Jz)(L*Ft - d*m*g sin(theta))
    this->theta_dotdot_ = (1/Inertia_Jz)*(L_ARM * propThrustcmd - distOG * mass_arm_prop * gravity * sin(this->theta_));
    this->theta_dot_ += this->theta_dotdot_ * sampleTime;
    this->theta_ += this->theta_dot_ * sampleTime;

    if (this->theta_ > MAX_THETA)
    {
        this->theta_ = MAX_THETA;
        this->theta_dot_ = 0.0;
    }
    else if (this->theta_ < MIN_THETA)
    {
        this->theta_ = MIN_THETA;
        this->theta_dot_ = 0.0;
    }
}

//
// GET FUNCTIONS
//
double ArmPropSimulator::GetTheta(void)
{
    return this->theta_;
}

double ArmPropSimulator::GetThetaDot(void)
{
    return this->theta_dot_;
}

double ArmPropSimulator::GetThetaDotDot(void)
{
    return this->theta_dotdot_;
}

double ArmPropSimulator::GetPropThrust(void)
{
    
    return this->prop_thrust_;
}

//
// SET FUNCTIONS
//
// This function is used in HILS_MODE = 2,
// to set the simulator angle that is received from the hardware
void ArmPropSimulator::SetTheta(double realAngle)
{
    this->theta_ = realAngle;
}

//**********************************************************************************
// Below are all the Auxiliary functions
//**********************************************************************************
// This function is used inside the Run() function of the simulator here above
// in order to populate the structure which will sent to setHILSSensorData (...).
SimSensorPacket ArmPropSimulator::GetSimSensorPacket() //GD: 03/06/2013
{
    SimSensorPacket local_ssp;
    local_ssp.theta = static_cast<unsigned int>(this->theta_);
    local_ssp.theta_dot = static_cast<unsigned int>(this->theta_dot_);
    return local_ssp;

}

ArmPropSimulator::ArmPropSimulator()
{
}

ArmPropSimulator::~ArmPropSimulator()
{
}
