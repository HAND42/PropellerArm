#ifndef HILSMODESERIALCOMMUNICATOR_H
#define HILSMODESERIALCOMMUNICATOR_H

//#include "include_define.h"

#include "arm_prop_simulator.h"
#include "serialCommunicator2.h"

class hilsModeSerialCommunicator
{
    public:
        static hilsModeSerialCommunicator* getInstance();

        void init(void);
        void run(void);
        void stop(void);

        SerialCommunicator2* myCommunicator;

        // Some Get functions
        int getHilsMode(void);
        int getSignal(void);
        int getMessage(void);
        char* getMessage_serial_communicator(void);
        SerialCommunicator2* GetCommunicator(void);

        // Some Set functions
        void setHilsMode(int);
        void SetCommunicator(SerialCommunicator2*);
        void setSignal(int);
        char* update_angle();

    private:
        hilsModeSerialCommunicator(void);
        ~hilsModeSerialCommunicator(void);

        int hils_mode;
        int    signal;
        int message;

        ArmPropSimulator* myArmPropSimulator;
};

#endif
