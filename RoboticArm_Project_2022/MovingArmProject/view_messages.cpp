#include "view_messages.h"

view_messages::view_messages()
{
}

void view_messages::show_information(const QString text)
{
    this->mon_message.setIcon(QMessageBox::Information);
    this->mon_message.setText(text);
    this->mon_message.exec();
}

void view_messages::show_warning(const QString text)
{
    this->mon_message.setIcon(QMessageBox::Warning);
    this->mon_message.setText(text);
    this->mon_message.exec();
}
