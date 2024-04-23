#include "view_formule_info.h"
#include "ui_view_formule_info.h"

view_formule_info::view_formule_info(QWidget *parent) :
      QDialog(parent),
      ui(new Ui::view_formule_info)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

view_formule_info::~view_formule_info()
{
    delete ui;
}
