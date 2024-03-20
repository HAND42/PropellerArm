#ifndef MODEL_SETTING_SAMPLE_TIME_H
#define MODEL_SETTING_SAMPLE_TIME_H

#include "verification_saisie_utilisateur.h"
#include "view_messages.h"

class model_setting_sample_time
{
    public:
        static model_setting_sample_time* getInstance();

        void init(void);

        int step_get(void) const;
        bool step_set(const QString);

        int simulator_step_get(void) const;
        bool simulator_step_set(const QString);

        int controller_step_get(void) const;
        bool controller_step_set(const QString);

        int graph_step_get(void) const;
        bool graph_step_set(const QString);

        double sample_time_simulator_get(void) const;
        double sample_time_controller_get(void) const;

    private:
        model_setting_sample_time(void);
        ~model_setting_sample_time(void);

        verification_saisie_utilisateur* my_verification_saisie_utilisateur;
        view_messages my_view_messages;

        int step;
        int simulator_step;
        int controller_step;
        int graph_step;

        double sample_time_simulator;
        double sample_time_controller;

        void sample_time_simulator_update(void);
        void sample_time_controller_update(void);
};

#endif
