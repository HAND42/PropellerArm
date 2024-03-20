#ifndef VIEW_SETTING_SAMPLE_TIME_H
#define VIEW_SETTING_SAMPLE_TIME_H

#include <QDialog>

#include "model_setting_sample_time.h"

namespace Ui {
    class view_setting_sample_time;
}

class view_setting_sample_time : public QDialog
{
        Q_OBJECT

    public:
        explicit view_setting_sample_time(QWidget *parent = nullptr);
        ~view_setting_sample_time();

    signals:
        void modification_valeurs();

    private slots:
        void on_apply_button_clicked();

        void on_cancel_button_clicked();

        void on_sample_time_step_value_editingFinished();

        void on_sample_time_simulator_step_value_editingFinished();

        void on_sample_time_controller_step_value_editingFinished();

        void on_sample_time_graph_step_value_editingFinished();

    private:
        Ui::view_setting_sample_time *ui;

        model_setting_sample_time* my_model_setting_sample_time;

        void data_load(void);
        void step_update(void);
        void simulator_step_update(void);
        void controller_step_update(void);
        void graph_step_update(void);
};

#endif // VIEW_SETTING_SAMPLE_TIME_H
