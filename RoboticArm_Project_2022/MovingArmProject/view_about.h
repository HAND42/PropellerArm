#ifndef VIEW_ABOUT_H
#define VIEW_ABOUT_H

#include <QDialog>

namespace Ui {
    class view_about;
}

class view_about : public QDialog
{
        Q_OBJECT

    public:
        explicit view_about(QWidget *parent = nullptr);
        ~view_about();

    private:
        Ui::view_about *ui;
};

#endif // VIEW_ABOUT_H
