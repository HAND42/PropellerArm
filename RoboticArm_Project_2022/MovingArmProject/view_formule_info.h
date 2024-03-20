#ifndef VIEW_FORMULE_INFO_H
#define VIEW_FORMULE_INFO_H

#include <QDialog>

namespace Ui {
class view_formule_info;
}

class view_formule_info : public QDialog
{
    Q_OBJECT

public:
    explicit view_formule_info(QWidget *parent = nullptr);
    ~view_formule_info();

private:
    Ui::view_formule_info *ui;
};

#endif // VIEW_FORMULE_INFO_H
