#ifndef MODEL_FORMULE_H
#define MODEL_FORMULE_H

#define FORMULE_MIN 0
#define FORMULE_MAX 1

#define OPERANDE            0
#define OPERATEUR            1
#define PARENTHESE_OUVERTE    2
#define PARENTHESE_FERMEE    3
#define FONCTIONS_DE_CALCUL 4
#define VARIABLES_POSSIBLES 5
#define RIEN                6

#define PRIORITE_OPERANDE_OU_VARIABLES_POSSIBLES    0
#define PRIORITE_PLUS_MOINS                            1
#define PRIORITE_MULTIPLIER_DIVISER_MODULO            2
#define PRIORITE_EXPONENTIEL                        3
#define PRIORITE_FONCTIONS_DE_CALCUL                4

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QList>
#include <QRegularExpression>
#include <QtMath>

struct structure
{
    QString element;
    int type_element;
    QList<structure*> liste;
    bool variable_dans_liste = false;
};

struct operation
{
    QString operateur_ou_fonction;
    int element_gauche; // index pour operandes_liste
    int element_droit = -1; // index pour operandes_liste
    int resutat; // index pour operandes_liste
};

struct parentheses
{
    int parenthese_ouverte; // index_structure
    int parenthese_fermee; // index_structure
};

class model_formule
{
    public:
        static model_formule* getInstance(void);

        void init(void);

        bool recevoir_les_variables_possibles(const QStringList variables_possibles);
        void preparer_la_formule_pour_le_calcul(const QString formule);

        bool est_elle_sans_erreur(void);
        QString donne_l_erreur(void);

        QList<int> donne_les_variables_possibles_utilisees_index(void);

        double calculer_le_resultat(void); // calcul SANS variables
        double calculer_le_resultat(const QList<double*> valeurs); // calcul AVEC variables

    private:
        model_formule();
        ~model_formule();

        QStringList elements_base;
        QStringList elements_base_sans_variables_possibles;
        QMap<QString, int> elements_base_map;

        QStringList operandes;
        QStringList operateurs;
        QStringList parenthese_ouverte;
        QStringList parenthese_fermee;
        QStringList fonctions_de_calcul;
        QStringList variables_possibles;

        QList<QList<int>> types_elements_limites;
        QList<int> operandes_limites;
        QList<int> operateurs_limites;
        QList<int> parenthese_ouverte_limites;
        QList<int> parenthese_fermee_limites;
        QList<int> fonctions_de_calcul_limites;
        QList<int> variables_possibles_limites;

        bool formule_sans_erreur;
        bool variables_possibles_valides;
        structure liste_structures_par_niveau;

        bool erreur_dans_operation;

        QList<int> variables_possibles_utilisees_index; // info dont a besoin la classe qui utilise la classe 'model_formule'
        bool variables_possibles_utilisees_donnees; // variable pour savoir si la classe qui utilise la classe 'model_formule' a bien appelé la fonction 'donne_les_variables_possibles_utilisees_index'

        QList<QList<int>> variables_possibles_index_sauvegarde;
        QList<QList<int>> variables_possibles_index;    // position des variables possibles dans "operandes_liste"
        QList<operation*> operations_liste;        // liste des opérations pour le calcul final
        QList<double> operandes_liste;            // liste utilisée par "operations_liste" pour accéder aux variables_possibles, aux opérandes et aux résultats intermédiaires
        bool calculer_le_resultat_pret;

        QString erreur;
        int verifier_la_validite_de_la_formule(const QString formule);
        int trouver_toutes_les_structures_et_verifier_la_validite(const QString ligne);
        void regrouper_les_structures_par_niveaux(structure* structure_parent, int niveau, int index);
        void supprimer_les_structures_de_tous_les_niveaux(structure* structure_parent);
        void mettre_les_variables_a_la_fin(structure* structure_parent);
        void transformer_les_operations_en_notation_polonaise_inverse(structure* structure_parent);
        int definir_operateur_priorite(const QString operateur);
        bool faire_les_calculs_sans_variable(structure* structure_parent);
        double faire_l_operation(const QString operateur, const double element_gauche, const double element_droit);
        double faire_l_operation(const QString fonction_de_calcul, const double element);
        bool preparer_le_calcul_final(structure* structure_parent);

        void afficher_toutes_les_structures(const structure* structure_parent);
        void afficher_les_variables_a_la_fin(const structure* structure_parent, int groupe_avec_variable_debut, int groupe_avec_variable_fin);
        void afficher_toutes_les_operations(const QList<operation*>* operations_liste);
};

#endif // MODEL_FORMULE_H
