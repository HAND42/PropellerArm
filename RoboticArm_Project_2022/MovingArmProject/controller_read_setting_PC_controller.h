#ifndef CONTROLLER_READ_SETTING_PC_CONTROLLER_H
#define CONTROLLER_READ_SETTING_PC_CONTROLLER_H

#include <QString>

#include "model_singleton.h"
#include "model_setting_PC_controller.h"

class controller_read_setting_PC_controller : public Singleton<controller_read_setting_PC_controller>
{
    friend class Singleton<controller_read_setting_PC_controller>;

    public:
        void init(void);

        QString k1_string_get(void);
        double k1_get(void);
        QString k2_string_get(void);
        double k2_get(void);
        QString a1_string_get(void);
        double a1_get(void);
        QString a2_string_get(void);
        double a2_get(void);
        QString b1_string_get(void);
        double b1_get(void);
        QString b2_string_get(void);
        double b2_get(void);
        QString b3_string_get(void);
        double b3_get(void);
        QString controller_type_string_get(void);
        bool controller_type_get(void);
    private:
        controller_read_setting_PC_controller();

        model_setting_PC_controller* myModel_setting_PC_controller;
};
#endif // controller_read_setting_PC_controller_H
