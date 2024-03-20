//#include "controller_write_setting_sample_time.h"

void controller_write_setting_sample_time::init(void)
{
    this->my_model_setting_sample_time = model_setting_sample_time::getInstance();
    this->my_verification_saisie_utilisateur = verification_saisie_utilisateur::getInstance();
}

void controller_write_setting_sample_time::init_setting_sample_time()
{
    this->my_model_setting_sample_time->init();
}

bool controller_write_setting_sample_time::step_set(const QString new_step)
{
    if(new_step != "0" && this->my_verification_saisie_utilisateur->saisie_nombre_valide_setting_sample_time(new_step))
    {
        this->my_model_setting_sample_time->step_set(new_step.toInt());

        return true;
    }

    this->my_view_messages.show_warning("step value error");

    return false;
}

bool controller_write_setting_sample_time::simulator_step_set(const QString new_simulator_step)
{
    if(new_simulator_step != "0" && this->my_verification_saisie_utilisateur->saisie_nombre_valide_setting_sample_time(new_simulator_step))
    {
        this->my_model_setting_sample_time->simulator_step_set(new_simulator_step.toInt());

        return true;
    }

    this->my_view_messages.show_warning("simulator step value error");

    return false;
}

bool controller_write_setting_sample_time::controller_step_set(const QString new_controller_step)
{
    if(new_controller_step != "0" && this->my_verification_saisie_utilisateur->saisie_nombre_valide_setting_sample_time(new_controller_step))
    {
        this->my_model_setting_sample_time->controller_step_set(new_controller_step.toInt());

        return true;
    }

    this->my_view_messages.show_warning("controller step value error");

    return false;
}

bool controller_write_setting_sample_time::graph_step_set(const QString new_graph_step)
{
    if(new_graph_step != "0" && this->my_verification_saisie_utilisateur->saisie_nombre_valide_setting_sample_time(new_graph_step))
    {
        this->my_model_setting_sample_time->graph_step_set(new_graph_step.toInt());

        return true;
    }

    this->my_view_messages.show_warning("graph step value error");

    return false;
}
// ++++++++++++++++ K1_SET
controller_write_setting_sample_time::controller_write_setting_sample_time()
{
}
