#include "model_setting_PC_controller.h"

// -------- PUBLIC --------
model_setting_PC_controller *model_setting_PC_controller::getInstance()
{
    static model_setting_PC_controller* instance_classe = nullptr;

    if (instance_classe == nullptr)
        instance_classe = new model_setting_PC_controller();

    return  instance_classe;
}

void model_setting_PC_controller::init(void)
{
    this->my_verification_saisie_utilisateur = verification_saisie_utilisateur::getInstance();

    this->controllers_types.append("cascade");
    this->controllers_types.append("leadlag");

    this->init_values();
}

void model_setting_PC_controller::init_values()
{
    // cascade controller
    this->k1 = sqrt(2.0) * MY_PI;
    this->k2 = this->k1 * 2;

    // leadlag controller
    this->a1 = 1.0;
    this->a2 = 0.0;
    this->b1 = 5.83;
    this->b2 = -5.33;
    this->b3 = 0.0;

    this->controller_type_set(CASCADE_CONTROLLER);
}

// ++++++++++++++++ K1_GET
double model_setting_PC_controller::k1_get(void) const
{
    return this->k1;
}

// ++++++++++++++++ K1_SET
bool model_setting_PC_controller::k1_set(const QString k1_new)
{
    if(this->my_verification_saisie_utilisateur->saisie_nombre_valide_setting_PC_controller(k1_new))
    {
        this->k1 = k1_new.toDouble();

        return true;
    }

    this->my_view_messages.show_warning("K1 value error");

    return false;
}

// ++++++++++++++++ K2_GET
double model_setting_PC_controller::k2_get(void) const
{
    return this->k2;
}

// ++++++++++++++++ K2_SET
bool model_setting_PC_controller::k2_set(const QString k2_new)
{
    if(this->my_verification_saisie_utilisateur->saisie_nombre_valide_setting_PC_controller(k2_new))
    {
        this->k2 = k2_new.toDouble();

        return true;
    }

    this->my_view_messages.show_warning("K2 value error");

    return false;
}

// ++++++++++++++++ A1_GET
double model_setting_PC_controller::a1_get(void) const
{
    return this->a1;
}

// ++++++++++++++++ A1_SET
bool model_setting_PC_controller::a1_set(const QString a1_new)
{
    if(this->my_verification_saisie_utilisateur->saisie_nombre_valide_setting_PC_controller(a1_new))
    {
        this->a1 = a1_new.toDouble();

        return true;
    }

    this->my_view_messages.show_warning("A1 value error");

    return false;
}

// ++++++++++++++++ A2_GET
double model_setting_PC_controller::a2_get(void) const
{
    return this->a2;
}

// ++++++++++++++++ A2_SET
bool model_setting_PC_controller::a2_set(const QString a2_new)
{
    if(this->my_verification_saisie_utilisateur->saisie_nombre_valide_setting_PC_controller(a2_new))
    {
        this->a2 = a2_new.toDouble();

        return true;
    }

    this->my_view_messages.show_warning("A2 value error");

    return false;
}

// ++++++++++++++++ B1_GET
double model_setting_PC_controller::b1_get(void) const
{
    return this->b1;
}

// ++++++++++++++++ B1_SET
bool model_setting_PC_controller::b1_set(const QString b1_new)
{
    if(this->my_verification_saisie_utilisateur->saisie_nombre_valide_setting_PC_controller(b1_new))
    {
        this->b1 = b1_new.toDouble();

        return true;
    }

    this->my_view_messages.show_warning("B1 value error");

    return false;
}

// ++++++++++++++++ B2_GET
double model_setting_PC_controller::b2_get(void) const
{
    return this->b2;
}

// ++++++++++++++++ B2_SET
bool model_setting_PC_controller::b2_set(const QString b2_new)
{
    if(this->my_verification_saisie_utilisateur->saisie_nombre_valide_setting_PC_controller(b2_new))
    {
        this->b2 = b2_new.toDouble();

        return true;
    }

    this->my_view_messages.show_warning("B2 value error");

    return false;
}

// ++++++++++++++++ B3_GET
double model_setting_PC_controller::b3_get(void) const
{
    return this->b3;
}

// ++++++++++++++++ B3_SET
bool model_setting_PC_controller::b3_set(const QString b3_new)
{
    if(this->my_verification_saisie_utilisateur->saisie_nombre_valide_setting_PC_controller(b3_new))
    {
        this->b3 = b3_new.toDouble();

        return true;
    }

    this->my_view_messages.show_warning("B3 value error");

    return false;
}

// ++++++++++++++++ TYPE_GET
QString model_setting_PC_controller::controller_type_string_get(void) const
{
    return this->controller_type_name;
}

bool model_setting_PC_controller::controller_type_get(void) const
{
    return this->controller_type;
}

// ++++++++++++++++ TYPE_SET
void model_setting_PC_controller::controller_type_set(const bool controller_type_new)
{
    this->controller_type = controller_type_new;

    if(controller_type_new == CASCADE_CONTROLLER)
        this->controller_type_name = "cascade";
    else
        this->controller_type_name = "leadlag";
}

model_setting_PC_controller::model_setting_PC_controller(void)
{
}

model_setting_PC_controller::~model_setting_PC_controller(void)
{
}
