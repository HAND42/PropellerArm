#include "hilsModeSerialCommunicator.h"

// -------- PUBLIC --------
hilsModeSerialCommunicator *hilsModeSerialCommunicator::getInstance()
{
    static hilsModeSerialCommunicator* instance_classe = nullptr;

    if (instance_classe == nullptr)
        instance_classe = new hilsModeSerialCommunicator();

    return  instance_classe;
}

//
// init
//
void hilsModeSerialCommunicator::init(void)
{
    this->hils_mode = HILS_MODE_PC_CONTROLLER;
    this->signal = 0;
    this->message = 0;

    this->myCommunicator = new SerialCommunicator2;
}
//
// run: gets executed when the com port is open
//
void hilsModeSerialCommunicator::run(void)
{
    // hils_mode = 0 OR 4
    if (this->hils_mode == HILS_MODE_PC_CONTROLLER || this->hils_mode == HILS_MODE_MANUAL_THRUST_COMMAND)
    {
        // Create a simulator of the arm-propeller system without serial communication
        this->myArmPropSimulator->init();
        //this->myHilsModeSerialCommunicator->SetCommunicator(NULL);
    }

    // hils_mode = 1 OR 2 OR 3
    else
    {
        if(!this->myCommunicator->portSuccessfullyOpened())
        {
            this->message = 1;
        }
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// DEBUT
/*        else
        {
            //create the necessary threads:
            CreateThread(0,0,(LPTHREAD_START_ROUTINE)communicationThread::receiver, this->myCommunicator,0,0); //create the receiver thread
            CreateThread(0,0,(LPTHREAD_START_ROUTINE)communicationThread::generalTransmitter, this->myCommunicator,0,0); //create the transmitter thread

            //GD: 26/02/2015
            if (this->hils_mode == HILS_MODE_1_ || this->hils_mode == HILS_MODE_3_)
            {
                CreateThread(0,0,communicationThread::HILSSensorDataTransmitter, this->myCommunicator,0,0); //create the HILS transmitter thread
            }

            this->myCommunicator->setConfirmRequestTimeout_ms(1000);
            this->myCommunicator->setTransmitterThreadSleepTime_ms(100);

            if (this->hils_mode == HILS_MODE_1_)
            {
                this->myCommunicator->setHILSTransmitSleepTime_ms(100);
            }

            if(this->myCommunicator->switchIntoHILSMode((TypeHILS)this->hils_mode,1))
            {
                this->message = 2;
            }
            else
            {
                this->message = 3;
            }

            Sleep(1000);

            this->SetCommunicator(this->myCommunicator);

            // Request debug data: gets executed just once at beginning when initializing communication: when we click on connect
            if (hils_mode == HILS_MODE_REAL_ANGLE || hils_mode == HILS_MODE_1_ || hils_mode == HILS_MODE_3_)
            {
                Sleep(500);
                this->myCommunicator->sendDebugDataRequestNow(100); // Should be multiple of 20 [ms]
                // 100 ms: ==> 10Hz
            }
        }// end else: no communicator

        */
// FIN
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    }// end else: hils_mode = 1 OR 2 OR 3

    //------------------------------------------------------------
    // Create the Thread in which the controller + Guidance System + Simulator get updated
    //------------------------------------------------------------
    // this->myThreadSimulatorController->create();
    //CreateThread(NULL,NULL,this->myThreadSimulatorController->run(),NULL,NULL,NULL); // this is in this thread that the simulator gets updated, HILS or not.

}

// ++++++++++++++++ STOP
void hilsModeSerialCommunicator::stop(void)
{
    if(this->myCommunicator->portSuccessfullyOpened())
    {
        this->myCommunicator->sendStopRequestNow();
        this->myCommunicator->close_port();
        //this->myCommunicator->~SerialCommunicator();
    }
}

//
// GET FUNCTIONS
//
int hilsModeSerialCommunicator::getHilsMode(void)
{
    return this->hils_mode;
}

int hilsModeSerialCommunicator::getSignal(void)
{
    return this->signal;
}

int hilsModeSerialCommunicator::getMessage(void)
{
    return this->message;
}

SerialCommunicator2* hilsModeSerialCommunicator::GetCommunicator(void)
{
    return this->myCommunicator;
}

//
// SET FUNCTIONS
//
void hilsModeSerialCommunicator::setHilsMode(int new_hils_mode)
{
    this->hils_mode = new_hils_mode;
}

void hilsModeSerialCommunicator::SetCommunicator(SerialCommunicator2* newCommunicator)
{
    this->myCommunicator = newCommunicator;
}

void hilsModeSerialCommunicator::setSignal(int new_signal)
{
    this->signal = new_signal;
}

//
// GETMESSAGE_SERIAL_COMMUNICATOR
//
char* hilsModeSerialCommunicator::getMessage_serial_communicator(void)
{
    return this->myCommunicator->getLastMessage();
}

char* hilsModeSerialCommunicator::update_angle()
{
    this->myArmPropSimulator->SetTheta(this->myCommunicator->readLastAngle());
    return getMessage_serial_communicator();
}




//************************************************************
// -------- PRIVATE --------
hilsModeSerialCommunicator::hilsModeSerialCommunicator(void)
{
    this->myArmPropSimulator = ArmPropSimulator::getInstance();
}

hilsModeSerialCommunicator::~hilsModeSerialCommunicator(void)
{
}
