#ifndef VERIFICATION_SAISIE_UTILISATEUR_H
#define VERIFICATION_SAISIE_UTILISATEUR_H

#include <QString>
#include <QRegularExpression>
#include <QDebug>

class verification_saisie_utilisateur
{
    public:
        verification_saisie_utilisateur();

        static verification_saisie_utilisateur* getInstance();

        bool saisie_nombre_valide_setting_PC_controller(const QString texte_nombre);
        bool saisie_nombre_valide_setting_sample_time(const QString texte_nombre);

    private:
        QRegularExpression rx;
};

#endif // VERIFICATION_SAISIE_UTILISATEUR_H
