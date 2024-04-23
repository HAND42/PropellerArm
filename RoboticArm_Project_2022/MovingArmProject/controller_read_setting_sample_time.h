#ifndef CONTROLLER_READ_SETTING_SAMPLE_TIME_H
#define CONTROLLER_READ_SETTING_SAMPLE_TIME_H

#include <QString>

#include "model_singleton.h"
#include "model_setting_sample_time.h"

class controller_read_setting_sample_time : public Singleton<controller_read_setting_sample_time>
{
    friend class Singleton<controller_read_setting_sample_time>;

    public:
        void init(void);

        QString step_string_get(void) const;
        int step_get(void) const;
        QString simulator_step_string_get(void) const;
        int simulator_step_get(void) const;
        QString controller_step_string_get(void) const;
        int controller_step_get(void) const;
        QString graph_step_string_get(void) const;
        int graph_step_get(void) const;

        double sample_time_simulator_get(void) const;
        double sample_time_controller_get(void) const;

    private:
        controller_read_setting_sample_time();

        model_setting_sample_time* my_model_setting_sample_time;
};
#endif // controller_read_setting_sample_time_H
