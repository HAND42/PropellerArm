#ifndef CONTROLLER_WRITE_SETTING_SAMPLE_TIME_H
#define CONTROLLER_WRITE_SETTING_SAMPLE_TIME_H

#include <QString>

#include "model_singleton.h"
#include "view_messages.h"
#include "model_setting_sample_time.h"
#include "verification_saisie_utilisateur.h"

class controller_write_setting_sample_time : public Singleton<controller_write_setting_sample_time>
{
    friend class Singleton<controller_write_setting_sample_time>;

    public:
        void init(void);
        void init_setting_sample_time(void);

        bool step_set(const QString);
        bool simulator_step_set(const QString);
        bool controller_step_set(const QString);
        bool graph_step_set(const QString);

    private:
        controller_write_setting_sample_time();

        model_setting_sample_time* my_model_setting_sample_time;
        verification_saisie_utilisateur* my_verification_saisie_utilisateur;
        view_messages my_view_messages;
};
#endif // controller_write_setting_sample_time_H
