#ifndef MODEL_FILE_SETTING_H
#define MODEL_FILE_SETTING_H

#include <QFile>
#include <QString>
#include <QStringList>
#include <QList>
#include <QTextStream>
//#include <iostream>
//#include <fstream>

#include "include_define.h"

#include "model_setting_PC_controller.h"
#include "model_setting_sample_time.h"
#include "model_time.h"

#include "view_messages.h"

class model_file_setting
{
    public:
        static model_file_setting* getInstance();

        void init(void);
        void file_read(void);
        void file_write(void);

    private:
        model_file_setting(void);
        ~model_file_setting(void);

        model_setting_PC_controller* myModel_setting_PC_controller;
        model_setting_sample_time* my_model_setting_sample_time;
        model_time* myTime;
        view_messages myMessage;

//        char myFile[17];
        void file_analyse(QString*);
        char* comparaison(char**, char*);

        QString texte;
        QString texte_erreur_reinitialisation;

        QString test = "/// bonjour les commentaires";
};

#endif

