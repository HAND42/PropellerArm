#include "view_setting_sample_time.h"
#include "ui_view_setting_sample_time.h"

view_setting_sample_time::view_setting_sample_time(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::view_setting_sample_time)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint & ~Qt::WindowCloseButtonHint);

    this->my_model_setting_sample_time = model_setting_sample_time::getInstance();

    this->data_load();
}

view_setting_sample_time::~view_setting_sample_time()
{
    delete ui;
}

void view_setting_sample_time::on_apply_button_clicked()
{
    bool error = false;

    if(!this->my_model_setting_sample_time->step_set(this->ui->sample_time_step_value->text()))
    {
        this->ui->sample_time_step_value->setText(QString::number(this->my_model_setting_sample_time->step_get()));
        this->step_update();
        error = true;
    }

    if(!this->my_model_setting_sample_time->simulator_step_set(this->ui->sample_time_simulator_step_value->text()))
    {
        this->ui->sample_time_simulator_step_value->setText(QString::number(this->my_model_setting_sample_time->simulator_step_get()));
        this->simulator_step_update();
        error = true;
    }

    if(!this->my_model_setting_sample_time->controller_step_set(this->ui->sample_time_controller_step_value->text()))
    {
        this->ui->sample_time_controller_step_value->setText(QString::number(this->my_model_setting_sample_time->controller_step_get()));
        this->controller_step_update();
        error = true;
    }

    if(!this->my_model_setting_sample_time->graph_step_set(this->ui->sample_time_graph_step_value->text()))
    {
        this->ui->sample_time_graph_step_value->setText(QString::number(this->my_model_setting_sample_time->graph_step_get()));
        this->graph_step_update();
        error = true;
    }

    if(!error)
    {
        emit modification_valeurs();

        this->close();
    }
}

void view_setting_sample_time::on_cancel_button_clicked()
{
    this->data_load();

    this->close();
}

void view_setting_sample_time::on_sample_time_step_value_editingFinished()
{
    this->step_update();
}

void view_setting_sample_time::on_sample_time_simulator_step_value_editingFinished()
{
    this->simulator_step_update();
}

void view_setting_sample_time::on_sample_time_controller_step_value_editingFinished()
{
    this->controller_step_update();
}

void view_setting_sample_time::on_sample_time_graph_step_value_editingFinished()
{
    this->graph_step_update();
}

void view_setting_sample_time::data_load(void)
{
    // step
    this->ui->sample_time_step_value->setText(QString::number(this->my_model_setting_sample_time->step_get()));
    this->step_update();

    // simulator
    this->ui->sample_time_simulator_step_value->setText(QString::number(this->my_model_setting_sample_time->simulator_step_get()));
    this->simulator_step_update();

    // controller
    this->ui->sample_time_controller_step_value->setText(QString::number(this->my_model_setting_sample_time->controller_step_get()));
    this->controller_step_update();

    // graph
    this->ui->sample_time_graph_step_value->setText(QString::number(this->my_model_setting_sample_time->graph_step_get()));
    this->graph_step_update();
}

void view_setting_sample_time::step_update(void)
{
    this->simulator_step_update();
    this->controller_step_update();
    this->graph_step_update();
}

void view_setting_sample_time::simulator_step_update(void)
{
    this->ui->sample_time_simulator_value->setNum(this->ui->sample_time_simulator_step_value->text().toInt() * this->ui->sample_time_step_value->text().toInt());
}

void view_setting_sample_time::controller_step_update(void)
{
    this->ui->sample_time_controller_value->setNum(this->ui->sample_time_controller_step_value->text().toInt() * this->ui->sample_time_step_value->text().toInt());
}

void view_setting_sample_time::graph_step_update(void)
{
    this->ui->sample_time_graph_value->setNum(this->ui->sample_time_graph_step_value->text().toInt() * this->ui->sample_time_step_value->text().toInt());
}
