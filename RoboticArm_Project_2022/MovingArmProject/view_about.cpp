#include "view_about.h"
#include "ui_view_about.h"

view_about::view_about(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::view_about)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

view_about::~view_about()
{
    delete ui;
}
