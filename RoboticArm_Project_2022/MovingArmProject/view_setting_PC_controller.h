#ifndef VIEW_SETTING_PC_CONTROLLER_H
#define VIEW_SETTING_PC_CONTROLLER_H

#include <QDialog>
#include <QString>
#include <QDebug>

#include "model_setting_PC_controller.h"

namespace Ui {
    class view_setting_PC_controller;
}

class view_setting_PC_controller : public QDialog
{
        Q_OBJECT

    public:
        explicit view_setting_PC_controller(QWidget *parent = nullptr);
        ~view_setting_PC_controller();

    signals:
        void modification_valeurs();

    private slots:
        void on_apply_button_clicked();

        void on_cancel_button_clicked();

        void on_reset_button_clicked();

        void on_cascade_controller_checkBox_stateChanged(int arg1);

        void on_leadlag_controller_checkBox_stateChanged(int arg1);

    private:
        Ui::view_setting_PC_controller *ui;

        model_setting_PC_controller* myModel_setting_PC_controller;

        bool controller_type;

        void data_load(void);
        void data_load_cascade_controller(void);
        void data_load_leadlag_controller(void);
};

#endif // VIEW_SETTING_PC_CONTROLLER_H
