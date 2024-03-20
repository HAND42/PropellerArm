#include "model_setting_sample_time.h"

model_setting_sample_time *model_setting_sample_time::getInstance()
{
    static model_setting_sample_time *instance_classe = nullptr;

    if (instance_classe == nullptr)
        instance_classe = new model_setting_sample_time();

    return instance_classe;
}

void model_setting_sample_time::init(void)
{
    this->my_verification_saisie_utilisateur = verification_saisie_utilisateur::getInstance();

    this->step = 50; // 25;
    this->simulator_step = 2;
    this->controller_step = 1; // 2;
    this->graph_step = 2;      // 4;

    this->sample_time_simulator_update();
    this->sample_time_controller_update();
}

int model_setting_sample_time::step_get(void) const
{
    return this->step;
}

bool model_setting_sample_time::step_set(const QString new_step)
{
    if (new_step != "0" && this->my_verification_saisie_utilisateur->saisie_nombre_valide_setting_sample_time(new_step))
    {
        this->step = new_step.toInt();

        return true;
    }

    this->my_view_messages.show_warning("step value error");

    return false;
}

int model_setting_sample_time::simulator_step_get(void) const
{
    return this->simulator_step;
}

bool model_setting_sample_time::simulator_step_set(const QString new_simulator_step)
{
    if (new_simulator_step != "0" && this->my_verification_saisie_utilisateur->saisie_nombre_valide_setting_sample_time(new_simulator_step))
    {
        this->simulator_step = new_simulator_step.toInt();

        return true;
    }

    this->my_view_messages.show_warning("simulator step value error");

    this->sample_time_simulator_update();

    return false;
}

int model_setting_sample_time::controller_step_get(void) const
{
    return this->controller_step;
}

bool model_setting_sample_time::controller_step_set(const QString new_controller_step)
{
    if (new_controller_step != "0" && this->my_verification_saisie_utilisateur->saisie_nombre_valide_setting_sample_time(new_controller_step))
    {
        this->controller_step = new_controller_step.toInt();

        return true;
    }

    this->my_view_messages.show_warning("controller step value error");

    this->sample_time_controller_update();

    return false;
}

int model_setting_sample_time::graph_step_get(void) const
{
    return this->graph_step;
}

bool model_setting_sample_time::graph_step_set(const QString new_graph_step)
{
    if (new_graph_step != "0" && this->my_verification_saisie_utilisateur->saisie_nombre_valide_setting_sample_time(new_graph_step))
    {
        this->graph_step = new_graph_step.toInt();

        return true;
    }

    this->my_view_messages.show_warning("graph step value error");

    return false;
}

double model_setting_sample_time::sample_time_simulator_get(void) const
{
    return this->sample_time_simulator;
}

double model_setting_sample_time::sample_time_controller_get(void) const
{
    return this->sample_time_controller;
}

void model_setting_sample_time::sample_time_simulator_update(void)
{
    this->sample_time_simulator = double(this->step) * double(this->simulator_step) / double(1000);
}

void model_setting_sample_time::sample_time_controller_update(void)
{
    this->sample_time_controller = double(this->step) * double(this->controller_step) / double(1000);
}

model_setting_sample_time::model_setting_sample_time(void)
{
}

model_setting_sample_time::~model_setting_sample_time(void)
{
}
