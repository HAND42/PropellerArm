#ifndef ARM_PROP_SIMULATOR_H
#define ARM_PROP_SIMULATOR_H

#include "include_define.h"

struct SimSensorPacket
{
    unsigned int theta, theta_dot, ADC_value;

};

class ArmPropSimulator
{
    public:
        static ArmPropSimulator* getInstance();

        void init(void);
        void Run(double, double);

        // Some Get functions
        double GetTheta(void);
        double GetThetaDot(void);
        double GetThetaDotDot(void);
        double GetPropThrust(void);

        // Some Set functions
        void SetTheta(double);
        SimSensorPacket GetSimSensorPacket(void);

    private:
        ArmPropSimulator();
        ~ArmPropSimulator();

        double theta_;
        double theta_dot_;
        double theta_dotdot_;
        double prop_thrust_;
};

#endif
