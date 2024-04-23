#ifndef VIEW_MESSAGES_H
#define VIEW_MESSAGES_H

#include <QMessageBox>
#include <QString>

class view_messages
{
    public:
        view_messages();

        void show_information(const QString text);
        void show_warning(const QString text);

    private:
        QMessageBox mon_message;
};

#endif // VIEW_MESSAGES_H
