#ifndef CONTROLLER_WRITE_SETTING_PC_CONTROLLER_H
#define CONTROLLER_WRITE_SETTING_PC_CONTROLLER_H

#include <QString>

#include "model_singleton.h"
#include "view_messages.h"
#include "model_setting_PC_controller.h"
#include "verification_saisie_utilisateur.h"

class controller_write_setting_PC_controller : public Singleton<controller_write_setting_PC_controller>
{
    friend class Singleton<controller_write_setting_PC_controller>;

    public:
        void init(void);
        void init_setting_PC_controller(void);

        bool k1_set(const QString);
        bool k2_set(const QString);
        bool a1_set(const QString);
        bool a2_set(const QString);
        bool b1_set(const QString);
        bool b2_set(const QString);
        bool b3_set(const QString);
        void controller_type_set(const bool);

    private:
        controller_write_setting_PC_controller();

        model_setting_PC_controller* myModel_setting_PC_controller;
        verification_saisie_utilisateur* my_verification_saisie_utilisateur;
        view_messages my_view_messages;
};
#endif // controller_write_setting_PC_controller_H
