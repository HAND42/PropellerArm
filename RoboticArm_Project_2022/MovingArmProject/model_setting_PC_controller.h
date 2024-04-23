#ifndef MODEL_SETTING_PC_CONTROLLER_H
#define MODEL_SETTING_PC_CONTROLLER_H

#include <QString>
#include <QStringList>

#include "include_define.h"

#include "verification_saisie_utilisateur.h"
#include "view_messages.h"

class model_setting_PC_controller
{
    public:
        static model_setting_PC_controller* getInstance();

        void init(void);
        void init_values(void);

        double k1_get(void) const;
        bool k1_set(const QString);

        double k2_get(void) const;
        bool k2_set(const QString);

        double a1_get(void) const;
        bool a1_set(const QString);

        double a2_get(void) const;
        bool a2_set(const QString);

        double b1_get(void) const;
        bool b1_set(const QString);

        double b2_get(void) const;
        bool b2_set(const QString);

        double b3_get(void) const;
        bool b3_set(const QString);

        QString controller_type_string_get(void) const;
        bool controller_type_get(void) const;
        void controller_type_set(const bool);

//            void show(const System::String*);

    private:
        model_setting_PC_controller(void);
        ~model_setting_PC_controller(void);

        verification_saisie_utilisateur* my_verification_saisie_utilisateur;
        view_messages my_view_messages;

        double k1;
        double k2;

        double a1;
        double a2;
        double b1;
        double b2;
        double b3;

        QString controller_type_name;
        bool controller_type;
        QStringList controllers_types;
};
#endif
