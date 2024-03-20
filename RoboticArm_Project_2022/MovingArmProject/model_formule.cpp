#include "model_formule.h"

model_formule* model_formule::getInstance()
{
    static model_formule* instance = nullptr;

    if(!instance)
        instance = new model_formule();

    return instance;
}

model_formule::model_formule()
{
}

model_formule::~model_formule()
{
}

void model_formule::init(void)
{
    // qDebug() << "model_formule::init";

    // CHARGER LES ELEMENTS DANS LES QStringList
    this->operandes << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "." << "-";
    this->operateurs << "+" << "-" << "*" << "/" << "%" << "^";
    this->parenthese_ouverte << "(";
    this->parenthese_fermee << ")";
    this->fonctions_de_calcul << "cos" << "sin" << "tan" << "sqrt";

    // CHARGER LES elements_base DANS LA MAP
    // CHARGER LES INDEX MINIMUM ET MAXIMUM POUR LES this->operandes_liste_limites, LES operateurs_limites ET LES parenthese_ouverte_limites
    QList<int> limites_temp;

    this->elements_base << this->operandes;
    limites_temp.append(0);
    limites_temp.append(this->elements_base.count() - 1);
    this->types_elements_limites.append(limites_temp);

    limites_temp.clear();
    limites_temp.append(this->elements_base.count());
    this->elements_base << this->operateurs;
    limites_temp.append(this->elements_base.count() - 1);
    this->types_elements_limites.append(limites_temp);

    limites_temp.clear();
    this->elements_base << this->parenthese_ouverte;
    limites_temp.append(this->elements_base.count() - 1);
    limites_temp.append(this->elements_base.count() - 1);
    this->types_elements_limites.append(limites_temp);

    limites_temp.clear();
    this->elements_base << this->parenthese_fermee;
    limites_temp.append(this->elements_base.count() - 1);
    limites_temp.append(this->elements_base.count() - 1);
    this->types_elements_limites.append(limites_temp);

    limites_temp.clear();
    limites_temp.append(this->elements_base.count());
    this->elements_base << this->fonctions_de_calcul;
    limites_temp.append(this->elements_base.count() - 1);
    this->types_elements_limites.append(limites_temp);

//    qDebug() << "types_elements_limites = " << this->types_elements_limites;

    // REMPLIR ELEMENTS_BASE_MAP
    for(int index = 0; index < this->elements_base.count(); index++)
    {
        this->elements_base_map.insert(this->elements_base.at(index), index);

//        qDebug() << "elements_base_map(" << index << ") = " << this->elements_base_map.keys(index);
    }

    this->formule_sans_erreur = false;
    this->erreur = "";
}

bool model_formule::recevoir_les_variables_possibles(const QStringList variables_possibles)
{
    // qDebug() << "model_formule::recevoir_les_variables_possibles";

    /*
     * CONDITIONS D'UNE VARIABLE BONNE:
     *    - contient au moins 1 lettre
     *    - peut contenir aussi des chiffres et '_'
     *    - longueur > 2
     *
    */
    QRegularExpression expression_reguliere_variable("[^\\w]");
    QRegularExpression expression_reguliere_1_lettre("[a-zA-Z]");

    QList<int> liste_index;
    QString variable_temp;

    QList<int> limites_temp;

    liste_index.append(-1);

    for(int index = 0; index < variables_possibles.length(); index++)
    {
        variable_temp = variables_possibles.at(index);
//        qDebug() << "\n";
//        qDebug() << "rx_1.indexIn(" + variable_temp + ") = " << expression_reguliere_variable.indexIn(variable_temp);
//        qDebug() << "rx_2.indexIn(" + variable_temp + ") = " << expression_reguliere_1_lettre.indexIn(variable_temp);

        if(expression_reguliere_variable.match(variable_temp).hasMatch() || variable_temp.length() < 3 || fonctions_de_calcul.contains(variable_temp, Qt::CaseInsensitive))
        {
            this->erreur = "ERREUR LOGICIEL: '" + variable_temp + "' EST UNE MAUVAISE VARIABLE.";
            this->formule_sans_erreur = false;
            // qDebug() << "CONDITIONS D'UNE BONNE VARIABLE:";
            // qDebug() << "\t- contient au moins 1 lettre";
            // qDebug() << "\t- peut contenir aussi des chiffres et '_'";
            // qDebug() << "\t- longueur > 2";

            return false;
        }
        this->variables_possibles.append(variable_temp.toLower()); // mettre les noms de variables en minuscules

        // initialiser 'variables_possibles_index' à -1 pour chaque variable possible
        this->variables_possibles_index.append(liste_index);

        this->formule_sans_erreur = true;
    }
    // sauvegarder 'variables_possibles_index'
    this->variables_possibles_index_sauvegarde = this->variables_possibles_index;

    limites_temp.append(this->elements_base.count());
    this->elements_base << this->variables_possibles;
    limites_temp.append(this->elements_base.count() - 1);
    this->types_elements_limites.append(limites_temp);

//    qDebug() << "types_elements_limites = " << this->types_elements_limites;
    return true;
}

bool model_formule::est_elle_sans_erreur(void)
{
    // qDebug() << "model_formule::est_elle_sans_erreur";

    return this->formule_sans_erreur;
}

QString model_formule::donne_l_erreur()
{
    // qDebug() << "model_formule::donne_l_erreur";

    return this->erreur;
}

QList<int> model_formule::donne_les_variables_possibles_utilisees_index()
{
    // qDebug() << "model_formule::donne_les_variables_possibles_utilisees_index";

    this->variables_possibles_utilisees_index.clear();

    for(int index = 0; index < this->variables_possibles_index.length(); index++)
    {
        if(this->variables_possibles_index.at(index).at(0) != -1)
            this->variables_possibles_utilisees_index.append(index);
    }
//    qDebug() << "\nvariables_possibles_utilisees_index = " << this->variables_possibles_utilisees_index;

    this->variables_possibles_utilisees_donnees = true;

    return this->variables_possibles_utilisees_index;
}

void model_formule::preparer_la_formule_pour_le_calcul(const QString formule)
{
    // qDebug() << "model_formule::preparer_la_formule_pour_le_calcul";

    this->formule_sans_erreur = true;
    this->erreur = "";

    if(this->formule_sans_erreur)
    {
        this->variables_possibles_utilisees_donnees = false;
        this->erreur_dans_operation = false;
        this->calculer_le_resultat_pret = true;

        if(this->verifier_la_validite_de_la_formule(formule) == 0)
        {
//            qDebug() << "\nnombre de structures = " << liste_structures_par_niveau.liste.length();
    //        this->afficher_toutes_les_structures(&(this->liste_structures_par_niveau));

            this->regrouper_les_structures_par_niveaux(&(this->liste_structures_par_niveau), -1, 0);
//            qDebug() << "\n'regrouper_les_structures_par_niveaux' est termine";
//            qDebug() << "\n'afficher_toutes_les_structures'";
//            this->afficher_toutes_les_structures(&(this->liste_structures_par_niveau));

            // METTRE LES VARIABLES EN FIN D'OPERATIONS
            if(this->liste_structures_par_niveau.variable_dans_liste)
            {
//                qDebug() << "\nMETTRE LES VARIABLES A LA FIN";
                this->mettre_les_variables_a_la_fin(&(this->liste_structures_par_niveau));

//                qDebug() << "\n'afficher_toutes_les_structures'";
//                this->afficher_toutes_les_structures(&(this->liste_structures_par_niveau));
            }

//            qDebug() << "\nTRANSFORMER LES OPERATIONS EN NOTATION POLONAISE INVERSE";
            this->transformer_les_operations_en_notation_polonaise_inverse(&(this->liste_structures_par_niveau));

//            qDebug() << "\n'afficher_toutes_les_structures'";
//            this->afficher_toutes_les_structures(&(this->liste_structures_par_niveau));

//            qDebug() << "\nFAIRE LES CALCULS SANS VARIABLES";
            if(!this->faire_les_calculs_sans_variable(&(this->liste_structures_par_niveau)))
            {
                this->erreur = "ERREUR LOGICIEL dans les calculs sans variables";
                this->formule_sans_erreur = false;
//                qDebug() << "\nUNE ERREUR S'EST PRODUITE DANS LES CALCULS";
                this->calculer_le_resultat_pret = false;
            }

//            qDebug() << "\n'afficher_toutes_les_structures'";
//            this->afficher_toutes_les_structures(&(this->liste_structures_par_niveau));

//            qDebug() << "\nPREPARER LE CALCUL FINAL";
            if(!this->preparer_le_calcul_final(&(this->liste_structures_par_niveau)))
            {
                this->erreur = "ERREUR LOGICIEL dans le calcul final";
                this->formule_sans_erreur = false;
//                qDebug() << "\nUNE ERREUR S'EST PRODUITE DANS LES CALCULS";
                this->calculer_le_resultat_pret = false;
            }
//            else
//            {
//                qDebug() << "\n'afficher_toutes_les_operations'";
//                this->afficher_toutes_les_operations(&(this->operations_liste));
//            }
        }
        else
            this->calculer_le_resultat_pret = false;
    }
}

double model_formule::calculer_le_resultat(void)
{
    // qDebug() << "model_formule::calculer_le_resultat";

    return this->liste_structures_par_niveau.liste.last()->element.toDouble();
}

double model_formule::calculer_le_resultat(const QList<double*> valeurs)
{
    // qDebug() << "model_formule::calculer_le_resultat2";

//    for(int index = 0; index < valeurs.length(); index++)
//        qDebug() << "index =" << index << " - valeur =" << *valeurs.at(index);

    for(int index_1 = 0; index_1 < this->variables_possibles_utilisees_index.length(); index_1++)
    {
        for(int index_2 = 0; index_2 < this->variables_possibles_index.at(this->variables_possibles_utilisees_index.at(index_1)).length(); index_2++)
        {
            if(this->variables_possibles_index.at(this->variables_possibles_utilisees_index.at(index_1)).at(index_2) != -1)
            {
                this->operandes_liste[this->variables_possibles_index.at(this->variables_possibles_utilisees_index.at(index_1)).at(index_2)] = *valeurs.at(index_1);
            }
        }
    }

//    qDebug() << "\nvariables_possibles_index = " << this->variables_possibles_index;
//    qDebug() << "operandes_liste = " << this->operandes_liste;

    for(int index = 0; index < this->operations_liste.length(); index++)
    {
//    qDebug() << "\nindex = " << index;
//    qDebug() << "operateur_ou_fonction = " << this->operations_liste.at(index)->operateur_ou_fonction;
//    qDebug() << "element_gauche = " << this->operandes_liste.at(this->operations_liste.at(index)->element_gauche);
//    if(this->operations_liste.at(index)->element_droit != -1)
//        qDebug() << "element_droit = " << this->operandes_liste.at(this->operations_liste.at(index)->element_droit);

    if(this->operations_liste.at(index)->element_droit != -1) // opérateur
        this->operandes_liste[this->operations_liste.at(index)->resutat] = this->faire_l_operation(this->operations_liste.at(index)->operateur_ou_fonction, this->operandes_liste.at(this->operations_liste.at(index)->element_gauche), this->operandes_liste.at(this->operations_liste.at(index)->element_droit));
    else // fonction
        this->operandes_liste[this->operations_liste.at(index)->resutat] = this->faire_l_operation(this->operations_liste.at(index)->operateur_ou_fonction.toLower(), this->operandes_liste.at(this->operations_liste.at(index)->element_gauche));
    }
//    qDebug() << "operandes_liste = " << this->operandes_liste;

    return this->operandes_liste.last();
}

int model_formule::verifier_la_validite_de_la_formule(const QString formule)
{
    // qDebug() << "model_formule::verifier_la_validite_de_la_formule";

    /* VALEURS DE RETOUR
     * 0 = TOUT S'EST BIEN PASSÉ
     * 1 = LA LONGUEUR DE LA FORMULE < 3
     * 2 = LA FORMULE CONTIENT AUTRE CHOSE QUE DES CHIFFRES, DES LETTRES ET LES SIGNES D'OPÉRATIONS
     * 3 = IL N'Y A PAS LE MEME NOMBRE DE PARENTHESES OUVRANTES ET FERMANTES
     * 4 = Il MANQUE UNE PARENTHESE OUVRANTE AVANT UNE PARENTHESE FERMANTE
     */
    /* **************** AVERTISSEMENTS AUX UTILISATEURS ****************
     * PAS DE VARIABLE CONTENANT LE SIGNE - POUR LES VARIABLES POSSIBLES
     */
    // VERIFIER QU'IL Y A AU MOINS 3 ÉLÉMENTS
    if(formule.length() < 3)
    {
        this->erreur = "La formule doit contenir au moins 2 opérandes et 1 opérateur";
        this->formule_sans_erreur = false;

        return 1;
    }

    QString ligne;
    QString texte_erreur;

    int ligne_erreur_index;

    QRegularExpression expression_reguliere_formule("[^a-zA-Z0-9 \\+\\-\\*/%\\^\\(\\)\\.]");

    ligne.append(formule);

    // VÉRIFIER QUE LA FORMULE NE CONTIENT AUCUN ÉLÉMENT INTERDIT
    QRegularExpressionMatch res = expression_reguliere_formule.match(ligne);
    if(res.hasMatch())
    {
        texte_erreur = res.captured();
//        qDebug() << "element non valide = " << texte_erreur;
        this->erreur = "La formule contient un caractère interdit: '" + texte_erreur + "'";
        this->formule_sans_erreur = false;

        return 2;
    }

    // SUPPRIMER LES ESPACES + REMPLACER LA VIRGULE PAR UN POINT
    ligne.remove(" ");
    ligne.replace(",", ".");
//    qDebug() << "\nLa formule est: " << ligne;
//    qDebug() << "La longueur de la formule est: " << ligne.count() << "\n";

    // VERIFIER QU'IL Y A UN NOMBRE EQUIVALENT DE PARENTHESES OUVRANTES ET FERMANTES
    if(ligne.count("(") != ligne.count(")"))
    {
        this->erreur = "Il n'y a pas le même nombre de '(' et de ')'";
        this->formule_sans_erreur = false;

        return 3;
    }

    // VERIFIER LES DUO DE PARENTHESES
    int parenthese = 0;
    for(int index = 0; index < ligne.size(); index++)
    {
        if(ligne.at(index) == QString("("))
            parenthese++;
        else if(ligne.at(index) == QString(")"))
            parenthese--;

        if(parenthese < 0)
        {
            this->erreur = "Il manque '(' AVANT ')'";
            this->formule_sans_erreur = false;

            return 4;
        }
    }

    // TROUVER TOUTES LES STRUCTURES
    return this->trouver_toutes_les_structures_et_verifier_la_validite(ligne);
}

int model_formule::trouver_toutes_les_structures_et_verifier_la_validite(const QString ligne)
{
    // qDebug() << "model_formule::trouver_toutes_les_structures_et_verifier_la_validite";

    /* VALEURS DE RETOUR
     * 0 = TOUT S'EST BIEN PASSÉ
     * 5 = '-' EN DEBUT DE LIGNE DOIT ETRE SUIVI D'UN CHIFFRE
     * 6 = L'OPERANDE CONTIENT DES LETTRES OU BIEN EST MAL FORMÉE
     * 7 = UNE OPÉRANDE OU UNE FONCTION DE CALCUL OU UNE VARIABLE POSSIBLE DOIVENT
     *       ÊTRE PRÉCÉDÉES PAR RIEN OU ‘(‘ OU UN OPÉRATEUR
     * 8 = UN OPÉRATEUR OU ')' DOIVENT ÊTRE PRÉCÉDÉES PAR UNE OPÉRANDE OU UNE VARIABLE POSSIBLE OU ')'
     * 9 = '(' DOIT ÊTRE PRÉCÉDÉE PAR RIEN OU ‘(‘ OU UN OPÉRATEUR OU UNE FONCTION DE
     *        CALCUL
     * 10 = UN OPÉRATEUR NE DOIT PAS ÊTRE EN DÉBUT DE LIGNE
     * 11 = UN OPÉRATEUR ET UNE FONCTION DE CALCUL NE DOIVENT PAS ÊTRE EN FIN DE LIGNE
     */

    QString temp_element;
    int type_element_base;
    int index_element;

    // pour les erreurs
    int index_structure;
    int type_element;
    QString element_avant;
    QString element_apres;

    // POUR VÉRIFIER QUE LE NOMBRE EST BIEN FORMÉ:
    //    - PEUT CONTENIR: 0 1 2 3 4 5 6 7 8 9 - .
    //    - NE CONTIENT PAS DE LETTRES
    QRegularExpression expression_reguliere_operande("^-?\\d{1,}(\\.\\d{1,})?$");

    index_element = 0;
    type_element = 0;

    this->supprimer_les_structures_de_tous_les_niveaux(&(this->liste_structures_par_niveau));

    // TROUVER TOUTES LES STRUCTURES

    // DEFINIR LE TYPE D'ELEMENT POUR CHAQUE ELEMENT DE LIGNE
    while (index_element < ligne.length())
    {
        // CAS HABITUEL
        if(ligne.at(index_element) != QString("-"))
        {
            type_element_base = this->elements_base_map.value(ligne.at(index_element));
//            qDebug() << "element(" << index_element << ") = " << ligne.at(index_element) << " ||||| map(" << ligne.at(index_element) << ") = " << type_element_base;
        }
        else // CAS PARTICULIER: "-" POUR UN NOMBRE NÉGATIF OU SIGNE D'OPERATION
        {
//            qDebug() << "'-' TROUVE";
            if(index_element == 0)
            {
//                qDebug() << "index_element == 0";
                if(ligne.at(index_element + 1).isDigit())
                {
//                    qDebug() << "ligne.at(index_element + 1).isDigit()";
                    type_element_base = this->elements_base.indexOf("-"); // type_element_base = 12;
//                    qDebug() << "type_element_base = " << type_element_base;
                }
                else
                {
                    this->erreur = "'-' en début de ligne doit être suivi d'un chiffre";
                    this->formule_sans_erreur = false;

                    return 5;
                }
            }
            else
            {
//                qDebug() << "index_element != 0";

                // DEFINIR LE TYPE D'ÉLÉMENT DEVANT '-'
                type_element_base = this->elements_base_map.value(ligne.at(index_element - 1));
//                qDebug() << "type_element_base DEVANT = " << type_element_base << this->elements_base.at(type_element_base);
                if(type_element_base > this->types_elements_limites.at(OPERANDE).at(FORMULE_MAX) && type_element_base < this->types_elements_limites.at(PARENTHESE_FERMEE).at(FORMULE_MAX)) // TOUTES LES this->operandes_liste ET LA PARENTHESE OUVERTE
                {
//                    qDebug() << "type_element_base > this->this->operandes_liste_limites.at(FORMULE_MAX)";
                    type_element_base = this->elements_base.indexOf("-"); // type_element_base = 12;
                }
                else
                    type_element_base = this->elements_base.lastIndexOf("-"); //type_element_base = 14;

//                qDebug() << "type_element_base = " << type_element_base;
            }
        }
        structure* nouvelle_structure = new structure;

        // SI OPERATEUR OU PARENTHESE OUVERTE OU PARENTHESE FERMEE
        if(type_element_base > this->types_elements_limites.at(OPERANDE).at(FORMULE_MAX) && type_element_base < this->types_elements_limites.at(FONCTIONS_DE_CALCUL).at(FORMULE_MIN))
        {
            // OPERATEUR
            if(type_element_base >= this->types_elements_limites.at(OPERATEUR).at(FORMULE_MIN) && type_element_base <= this->types_elements_limites.at(OPERATEUR).at(FORMULE_MAX))
                nouvelle_structure->type_element = OPERATEUR;

            // PARENTHESE OUVERTE
            else if(type_element_base >= this->types_elements_limites.at(PARENTHESE_OUVERTE).at(FORMULE_MIN) && type_element_base <= this->types_elements_limites.at(PARENTHESE_OUVERTE).at(FORMULE_MAX))
                nouvelle_structure->type_element = PARENTHESE_OUVERTE;

            // PARENTHESE FERMEE
            else
                nouvelle_structure->type_element = PARENTHESE_FERMEE;

            nouvelle_structure->element.append(ligne.at(index_element));

            this->liste_structures_par_niveau.liste.append(nouvelle_structure);

            // POINTER L'ELEMENT SUIVANT
            index_element++;
        }

        // OPERANDE, VARIABLES POSSIBLES OU FONCTIONS_DE_CALCUL
        else
        {
//            temp_element.append(ligne.at(index_element));

            while(type_element_base <= this->types_elements_limites.at(OPERANDE).at(FORMULE_MAX))
            {
                temp_element.append(ligne.at(index_element));
                index_element++;
                if(index_element != ligne.length())
                {
                    type_element_base = this->elements_base_map.value(ligne.at(index_element));
//                    qDebug() << "OPERANDE->index_element = " << index_element << " ||||| temp_element = " << temp_element;
                }
                else
                    break;
            }


            if(this->variables_possibles.contains(temp_element, Qt::CaseInsensitive)) // VARIABLES_POSSIBLES
                nouvelle_structure->type_element = VARIABLES_POSSIBLES;

            else if(this->fonctions_de_calcul.contains(temp_element, Qt::CaseInsensitive)) // FONCTIONS_DE_CALCUL
                nouvelle_structure->type_element = FONCTIONS_DE_CALCUL;

            else // OPERANDE
            {
// A FAIRE: UTILISER QRegExp POUR VÉRIFIER QUE temp_element CONTIENT BIEN LES CHIFFRES DE 0-9 . et -
                if(expression_reguliere_operande.match(temp_element).capturedStart() == 0)
                    nouvelle_structure->type_element = OPERANDE;
                else // L'OPERANDE CONTIENT DES LETTRES OU BIEN EST MAL FORMÉE
                {
                    // qDebug() << "temp_element = " << temp_element;
                    this->erreur = "'" + temp_element + "' contient des lettres ou bien est mal formée";
                    this->formule_sans_erreur = false;

                    return 6;
                }
            }
            if(nouvelle_structure->type_element != VARIABLES_POSSIBLES)
                nouvelle_structure->element.append(temp_element);
            else
                nouvelle_structure->element.append(temp_element.toLower());

            this->liste_structures_par_niveau.liste.append(nouvelle_structure);

            temp_element.clear();
        }

        nouvelle_structure = nullptr;

        // CONTINUER DE VÉRIFIER LA VALIDITE DE LA FORMULE

        index_structure = this->liste_structures_par_niveau.liste.length() - 2; // AVANT-DERNIER ÉLÉMENT
        type_element = this->liste_structures_par_niveau.liste.last()->type_element; // DERNIER ÉLÉMENT
        // SI il y a plus d'une structure
        if(index_structure > -1)
        {
    //        opérandes            précédée par rien ou ‘(‘ ou opérateur
    //        fonctions de calcul    précédée par rien ou ‘(‘ ou opérateur
    //        variables_possibles    précédée par rien ou ‘(‘ ou opérateur
            if(type_element == OPERANDE || type_element == FONCTIONS_DE_CALCUL || type_element == VARIABLES_POSSIBLES)
            {
                if(this->liste_structures_par_niveau.liste.at(index_structure)->type_element != OPERATEUR && this->liste_structures_par_niveau.liste.at(index_structure)->type_element != PARENTHESE_OUVERTE)
                {
                    element_avant = this->liste_structures_par_niveau.liste.at(index_structure)->element;
                    element_apres = this->liste_structures_par_niveau.liste.last()->element;
                    this->erreur = "'" + element_avant + "' ne doit pas être suivie par '" + element_apres + "'";
                    this->formule_sans_erreur = false;

                    return 7;
                }
            }
    //        opérateurs    précédé par opérande ou ‘)‘ ou variable possible
    //        ')'            précédée par opérande ou ‘)‘ ou variable possible
            else if(type_element == OPERATEUR || type_element == PARENTHESE_FERMEE)
            {
                if(this->liste_structures_par_niveau.liste.at(index_structure)->type_element != OPERANDE && this->liste_structures_par_niveau.liste.at(index_structure)->type_element != PARENTHESE_FERMEE && this->liste_structures_par_niveau.liste.at(index_structure)->type_element != VARIABLES_POSSIBLES)
                {
                    element_avant = this->liste_structures_par_niveau.liste.at(index_structure)->element;
                    element_apres = this->liste_structures_par_niveau.liste.last()->element;
                    this->erreur = "'" + element_avant + "' ne doit pas être suivie par '" + element_apres + "'";
                    this->formule_sans_erreur = false;

                    return 8;
                }
            }
    //        '(' précédée par rien ou ‘(‘ ou opérateur ou par fonction de calcul
            else if(type_element == PARENTHESE_OUVERTE)
            {
                if(this->liste_structures_par_niveau.liste.at(index_structure)->type_element != OPERATEUR && this->liste_structures_par_niveau.liste.at(index_structure)->type_element != PARENTHESE_OUVERTE && this->liste_structures_par_niveau.liste.at(index_structure)->type_element != FONCTIONS_DE_CALCUL)
                {
                    element_avant = this->liste_structures_par_niveau.liste.at(index_structure)->element;
                    element_apres = this->liste_structures_par_niveau.liste.last()->element;
                    this->erreur = "'" + element_avant + "' ne doit pas être suivie par '" + element_apres + "'";
                    this->formule_sans_erreur = false;

                    return 9;
                }
            }
        }
        else // première structure
        {
            //    opérateurs
            //        pas en début de ligne
            if(this->liste_structures_par_niveau.liste.first()->type_element == OPERATEUR)
            {
                element_avant = this->liste_structures_par_niveau.liste.first()->element;
                this->erreur = "Une formule ne doit pas se débuter par '" + element_avant + "'";
                this->formule_sans_erreur = false;

                return 10;
            }
        }
    }
//    type_element = this->liste_structures_par_niveau.liste.last()->type_element; // DERNIER ÉLÉMENT
    //    opérateurs ET fonction de calcul
    //        pas en fin de ligne
    if(type_element == OPERATEUR || type_element == FONCTIONS_DE_CALCUL)
    {
        element_avant = this->liste_structures_par_niveau.liste.last()->element;
        this->erreur = "Une formule ne doit pas se terminer par '" + element_avant + "'";
        this->formule_sans_erreur = false;

        return 11;
    }
//    qDebug() << "nombre de structures = " << this->liste_structures_par_niveau.liste.length();

//    return this->verifier_la_validite_des_structures();
    return 0;
}

void model_formule::regrouper_les_structures_par_niveaux(structure* structure_parent, int niveau, int index)
{
    // qDebug() << "model_formule::regrouper_les_structures_par_niveaux";

    bool variable_dans_liste;

    niveau++;
    variable_dans_liste = false;

    while(index < this->liste_structures_par_niveau.liste.length())
    {
        if(this->liste_structures_par_niveau.liste.at(index)->type_element == PARENTHESE_OUVERTE || this->liste_structures_par_niveau.liste.at(index)->type_element == FONCTIONS_DE_CALCUL)
        {
            if(this->liste_structures_par_niveau.liste.at(index)->type_element == PARENTHESE_OUVERTE)
            {
                // changer l'élément de la structure
                this->liste_structures_par_niveau.liste[index]->element = "";

                // changer le type d'élément de la structure
                this->liste_structures_par_niveau.liste[index]->type_element = RIEN;

                // SI NIVEAU > 0
                if(niveau > 0)
                {
                    // déplacer la structure dans la liste de la structure_parent
                    structure_parent->liste.append(this->liste_structures_par_niveau.liste.takeAt(index));

                    this->regrouper_les_structures_par_niveaux(structure_parent->liste.last(), niveau, index); // structure_parent = FONCTIONS_DE_CALCUL
                }

//                CONDITIONS POUR INCRÉMENTER 'index':
//                    niveau = 0
                else // niveau == 0
                {
                    index++;

                    this->regrouper_les_structures_par_niveaux(structure_parent->liste.at(index - 1), niveau, index);
                }
            }
            // SINON C'EST UNE FONCTION DE CALCUL ALORS AJOUTER CE QUI EST ENTRE PARENTHÈSES DANS LA LISTE DE LA STRUCTURE PARENT
            else // FONCTIONS_DE_CALCUL
            {
                // mettre en minuscule le texte de la fonction de calcul
                this->liste_structures_par_niveau.liste[index]->element = this->liste_structures_par_niveau.liste.at(index)->element.toLower();
                // supprimer la parenthèse ouverte qui est après FONCTIONS_DE_CALCUL
                this->liste_structures_par_niveau.liste.removeAt(index + 1);

                if(niveau == 0)
                {
                    index++;

                    this->regrouper_les_structures_par_niveaux(this->liste_structures_par_niveau.liste.at(index - 1), niveau, index); // structure_parent = FONCTIONS_DE_CALCUL
                }
                else
                {
                    // déplacer la structure FONCTIONS_DE_CALCUL dans la liste de la structure_parent
                    structure_parent->liste.append(this->liste_structures_par_niveau.liste.takeAt(index));

                    this->regrouper_les_structures_par_niveaux(structure_parent->liste.last(), niveau, index); // structure_parent = FONCTIONS_DE_CALCUL
                }
            }
            if(variable_dans_liste)
                structure_parent->variable_dans_liste = true;
        }
        else if(this->liste_structures_par_niveau.liste.at(index)->type_element == PARENTHESE_FERMEE)// PARENTHESE_FERMEE trouvée
        {
            // supprimer la parenthèse fermée
            this->liste_structures_par_niveau.liste.removeAt(index);
            niveau--;

            break;
        }
        else if(niveau > 0)
        {
            // déplacer la structure dans la liste_structures_par_niveau de la structure_parent
            structure_parent->liste.append(this->liste_structures_par_niveau.liste.takeAt(index));

            if(structure_parent->liste.last()->type_element == VARIABLES_POSSIBLES)
            {
//                qDebug() << "\tVARIABLES_POSSIBLES trouvée";
                variable_dans_liste = true;
                structure_parent->variable_dans_liste = true;
            }
        }
        else if(niveau == 0)
        {
            if(this->liste_structures_par_niveau.liste.at(index)->type_element == VARIABLES_POSSIBLES)
            {
//                qDebug() << "\tVARIABLES_POSSIBLES trouvée";
                variable_dans_liste = true;
                structure_parent->variable_dans_liste = true;
            }

            index++;
        }
    }
}

void model_formule::supprimer_les_structures_de_tous_les_niveaux(structure* structure_parent)
{
    // qDebug() << "model_formule::supprimer_les_structures_de_tous_les_niveaux";

    for(int index = structure_parent->liste.length() - 1; index > -1; index--)
    {
        if(structure_parent->liste.at(index)->liste.length() > 0) // SI CET STRUCTURE CONTIENT DES STRUCTURES DANS LA LISTE
            this->supprimer_les_structures_de_tous_les_niveaux(structure_parent->liste.at(index));
        if(structure_parent->liste.at(index)->liste.isEmpty()) // SI CET STRUCTURE CONTIENT DES STRUCTURES DANS LA LISTE
        {
            structure_parent->liste.removeAt(index);
//            qDebug() << "\n=====================================================";
//            this->afficher_toutes_les_structures((&(this->liste_structures_par_niveau)));
        }
    }
}

void model_formule::mettre_les_variables_a_la_fin(structure* structure_parent)
{
    // qDebug() << "model_formule::mettre_les_variables_a_la_fin";

    /* EXPLICATIONS POUR LA SUITE
     *
     * LIGNE = GROUPE_SANS_VARIABLE et GROUPE_AVEC_VARIABLE
     *
     * GROUPE_AVEC_VARIABLE = VARIABLE_AVEC_OPERATIONS_PRIORITAIRES
     *
     * VARIABLE_AVEC_OPERATIONS_PRIORITAIRES = VARIABLE_AVEC_OPERATIONS_HAUTE_PRIORITE et VARIABLE_AVEC_OPERATIONS_BASSE_PRIORITE
     *
     * */
    int nombre_de_variables = 0;

    QString groupe_avec_variable; // VARIABLE UTILISÉE DANS qDebug() EN COMMENTAIRE
    int groupe_avec_variable_debut;
    int groupe_avec_variable_fin;
    int groupe_avec_variable_difference_fin_moins_debut;
    int groupe_avec_variable_nombre_de_signes;

    int variable_index;
    int variable_position;
    int variable_avec_operations_haute_priorite_position;

    QString operations_basse_priorite = "+-";
    QString operations_haute_priorite = "^%";

    bool groupe_avec_variable_signe_avant_existe;
    bool groupe_avec_variable_signe_apres_existe;
    QString groupe_avec_variable_signe_avant;
    QString groupe_avec_variable_signe_apres;

    QString variable_avec_operations_haute_priorite; // VARIABLE UTILISÉE DANS qDebug() EN COMMENTAIRE
    int variable_avec_operations_haute_priorite_debut;
    int variable_avec_operations_haute_priorite_fin;
    int variable_avec_operations_haute_priorite_difference_fin_moins_debut;
    int variable_avec_operations_haute_priorite_nombre_de_signes;

    QString    variable_avec_operations_basse_priorite_signe_avant = "";

    int nombre_de_signes_restants;

    // ==>> RECHERCHER TOUTES LES VARIABLES DANS LA LIGNE
//    formule1 = formule->split(" ");
//    nombre_de_variables = formule1.count(variable);
//    qDebug() << "\tnombre_de_variables = " << nombre_de_variables;
    // SI IL Y A UNE VARIABLE DANS LA LISTE
    if(structure_parent->variable_dans_liste)
    {
//        qDebug() << "IL Y A UNE VARIABLE DANS LA LISTE";

//        qDebug() << "\tstructure_parent->liste.length() = " << structure_parent->liste.length();
        // RECHERCHER LE NOMBRE DE VARIABLES DANS LA LISTE
        for(int index = 0; index < structure_parent->liste.length(); index++)
        {
//            qDebug() << "\tindex: " << index;

            if(structure_parent->liste.at(index)->variable_dans_liste)
                nombre_de_variables += 1;

            else if(structure_parent->liste.at(index)->element != "")
                if(structure_parent->liste.at(index)->type_element == VARIABLES_POSSIBLES)
                    nombre_de_variables += 1;
        }
//        qDebug() << "\tnombre de variable = " << nombre_de_variables;

        // DÉPLACER TOUTES LES VARIABLES À LA FIN DE LA LISTE
        for(int index = 0; index < nombre_de_variables; index++)
        {
//            qDebug() << "\tformule de depart: " << *formule << "\n";

            // TROUVER LA POSITION DE LA PREMIÈRE VARIABLE
            for(int index = 0; index < structure_parent->liste.length(); index++)
            {
    //            qDebug() << "\tformule de depart: " << *formule << "\n";

                if(structure_parent->liste.at(index)->variable_dans_liste)
                {
                    variable_index = index;
                    break;
                }

                else if(structure_parent->liste.at(index)->element != "")
                {
                    if(structure_parent->liste.at(index)->type_element == VARIABLES_POSSIBLES)
                    {
                        variable_index = index;
                        break;
                    }
                }
            }
//            qDebug() << "\tvariable_index = " << variable_index;
            // ==>> DÉFINIR LE GROUPE CONTENANT LA VARIABLE
            // ==>> RECHERCHER LES OPERATIONS */^% AUTOUR DE LA VARIABLE
            variable_position = 0;
            groupe_avec_variable_debut = variable_index;
            groupe_avec_variable_fin = variable_index;

            // ==>> RECHERCHER LES OPERATIONS */^% AVANT LA VARIABLE
            for(int index_1 = variable_index - 1; index_1 > -1;    index_1 -= 2)
            {
                if(!operations_basse_priorite.contains(structure_parent->liste.at(index_1)->element))
                {
                    groupe_avec_variable_debut -= 2;
                    variable_position += 2;
                }
                else
                    break;
            }
    //    qDebug() << "\tgroupe_avec_variable_debut = " << groupe_avec_variable_debut;

            // ==>> RECHERCHER LES OPERATIONS */^% APRÈS LA VARIABLE
            for(int index_2 = variable_index + 1; index_2 < structure_parent->liste.length(); index_2 += 2)
            {
                if(!operations_basse_priorite.contains(structure_parent->liste.at(index_2)->element))
                {
                    groupe_avec_variable_fin += 2;
                }
                else
                    break;
            }
    //        qDebug() << "\tgroupe_avec_variable_fin = " << groupe_avec_variable_fin;

//            afficher_les_variables_a_la_fin(structure_parent,  groupe_avec_variable_debut, groupe_avec_variable_fin);

            // ==>> DÉFINIR LES CARACTÉRISTIQUES DU GROUPE CONTENANT LA VARIABLE
            // ==>>    - AU DÉBUT
            // ==>> - À LA FIN
            // ==>> - SIGNE AVANT
            // ==>> - SIGNE APRÈS
    //    - SI pas de signe avant le groupe_avec_variable
            if(groupe_avec_variable_debut == 0)
    //        - groupe_avec_variable_signe_avant_existe = false
                groupe_avec_variable_signe_avant_existe = false;
    //    - SINON
            else
            {
    //        - groupe_avec_variable_signe_avant_existe = true
    //            - groupe_avec_variable_signe_avant =
                groupe_avec_variable_signe_avant_existe = true;
                groupe_avec_variable_signe_avant = structure_parent->liste.at(groupe_avec_variable_debut - 1)->element;
            }
//            qDebug() << "\tgroupe_avec_variable_signe_avant_existe = " << groupe_avec_variable_signe_avant_existe;
//            qDebug() << "\tgroupe_avec_variable_signe_avant = " << groupe_avec_variable_signe_avant;

    //    - SI pas de signe après le groupe_avec_variable
            if(groupe_avec_variable_fin == structure_parent->liste.length() - 1)
    //        - groupe_avec_variable_signe_apres_existe = false
                groupe_avec_variable_signe_apres_existe = false;
    //    - SINON
            else
            {
    //        - groupe_avec_variable_signe_apres_existe = true
    //            - signe_après_groupe =
                groupe_avec_variable_signe_apres_existe = true;
                groupe_avec_variable_signe_apres = structure_parent->liste.at(groupe_avec_variable_fin + 1)->element;
            }
    //        qDebug() << "\tgroupe_avec_variable_signe_apres_existe = " << groupe_avec_variable_signe_apres_existe;
    //        qDebug() << "\tgroupe_avec_variable_signe_apres = " << groupe_avec_variable_signe_apres;

            // ==>> CONDITION POUR DÉPLACER LE GROUPE AVEC LA VARIABLE À LA FIN DE LA LIGNE
    //        - SI groupe_avec_variable_signe_apres_existe = true
            if(groupe_avec_variable_signe_apres_existe == true)
            {
    //            - SI groupe_avec_variable_signe_avant_existe = false
                if(groupe_avec_variable_signe_avant_existe == false)
                {
    //                - SI groupe_avec_variable_signe_apres = +
                    if(groupe_avec_variable_signe_apres == "+")
                    {
    //                    - supprimer +
                        structure_parent->liste.removeAt(groupe_avec_variable_fin + 1);
                    }
    //                - SINON // groupe_avec_variable_signe_apres = -
                    else
                    {
    //                    - créer une structure pour l'élément 0 dans 'nouvelle_structure'
                        structure* nouvelle_structure = new structure;
                        nouvelle_structure->element = "0";
                        nouvelle_structure->type_element = OPERANDE;

    //                    - ajouter 0 devant le signe -
                        structure_parent->liste.insert(groupe_avec_variable_fin + 1, nouvelle_structure);
                        nouvelle_structure = nullptr;
                    }
//                    - créer une structure pour l'élément + dans 'nouvelle_structure'
                    structure* nouvelle_structure = new structure;
                    nouvelle_structure->element = "+";
                    nouvelle_structure->type_element = OPERATEUR;

    //                - ajouter + à la fin de la ligne
                    structure_parent->liste.append(nouvelle_structure);
                    nouvelle_structure = nullptr;
                }
    //            - SINON // groupe_avec_variable_signe_avant_existe = true
                else
                {
    //                - déplacer groupe_avec_variable_signe_avant à la fin de la ligne
                    structure_parent->liste.append(structure_parent->liste.takeAt(groupe_avec_variable_debut - 1));
                    groupe_avec_variable_debut--;
                    groupe_avec_variable_fin--;
                }
                // ==>> DÉPLACER LE GROUPE AVEC LA VARIABLE À LA FIN DE LA LIGNE
    //            - déplacer le groupe à la fin de la ligne
                for(int index = 0; index < groupe_avec_variable_fin - groupe_avec_variable_debut + 1; index++)
                    structure_parent->liste.append(structure_parent->liste.takeAt(groupe_avec_variable_debut));
            }
            // ==>> DÉFINIR LES NOUVELLES VALEURS DE DÉBUT ET DE FIN DU GROUPE
            groupe_avec_variable_difference_fin_moins_debut = groupe_avec_variable_fin - groupe_avec_variable_debut;
            groupe_avec_variable_fin = structure_parent->liste.length() - 1;
            groupe_avec_variable_debut = groupe_avec_variable_fin - groupe_avec_variable_difference_fin_moins_debut;

//            afficher_les_variables_a_la_fin(&formule1,  groupe_avec_variable_debut, groupe_avec_variable_fin);
//            qDebug() << "\tGROUPE AVEC VARIABLE APRES DEPLACEMENT = " << groupe_avec_variable;

//            afficher_les_variables_a_la_fin(&formule1,  0, structure_parent->liste.length() - 1);
//            qDebug() << "\tFORMULE INTERMEDIAIRE = " << *formule;
    //        qDebug() << "\tvariable_position = " << variable_position;

    // ====>>>> GROUPE_AVEC_VARIABLE = VARIABLE_AVEC_OPERATIONS_PRIORITAIRES et OPERATIONS_SANS_VARIABLE <<<<====
        // ==>> TRAVAIL DANS LE GROUPE AVEC LA VARIABLE
    //    variables :
    //        groupe_nombre_de_signes
    //        groupe_nombre_de_signes_exp_et_mod
    //        variable_et_plus;
    //        variable_et_plus_nombre_de_signes
    //        variable_et_plus_signe_avant
    //        variable_et_plus_signe_apres

            // ==>> DÉFINIR LE SOUS-GROUPE CONTENANT LA VARIABLE ET LES OPERATIONS HAUTE PRIORITE
            // ==>> RECHERCHER LES OPERATIONS HAUTE PRIORITE ^ % AUTOUR DE LA VARIABLE
    //    //  variable_et_plus = VARIABLE PLUS LES SIGNES (^ ou %) AVANT OU APRÈS VARIABLE
    //    variable_et_plus = chercher tous les signes ^ % tout de suite avant ou après VARIABLE
            // définir le début de la recherche
            variable_avec_operations_haute_priorite_debut = structure_parent->liste.length() - 1 - groupe_avec_variable_fin + groupe_avec_variable_debut + variable_position;
            variable_avec_operations_haute_priorite_fin = variable_avec_operations_haute_priorite_debut;
//            qDebug() << "\tvariable_avec_operations_haute_priorite_debut = " << variable_avec_operations_haute_priorite_debut;
    //        qDebug() << "\tvariable apres deplacement = " << elements.at(structure_parent->liste.at(variable_avec_operations_haute_priorite_debut);

            // ==>> RECHERCHER LES OPERATIONS HAUTE PRIORITE ^ % AVANT LA VARIABLE
            variable_avec_operations_haute_priorite_position = 0;
            for(int index_1 = variable_avec_operations_haute_priorite_debut - 1; index_1 > -1;    index_1 -= 2)
            {
                if(operations_haute_priorite.contains(structure_parent->liste.at(index_1)->element))
                {
                    variable_avec_operations_haute_priorite_debut -= 2;
                    variable_avec_operations_haute_priorite_position += 2;
                }
                else
                    break;
            }
    //    qDebug() << "\tvariable_avec_operations_haute_priorite_debut = " << variable_avec_operations_haute_priorite_debut;

            // ==>> RECHERCHER LES OPERATIONS HAUTE PRIORITE ^ % APRÈS LA VARIABLE
            for(int index_2 = variable_avec_operations_haute_priorite_fin + 1; index_2 < structure_parent->liste.length();    index_2 += 2)
            {
                if(operations_haute_priorite.contains(structure_parent->liste.at(index_2)->element))
                {
                    variable_avec_operations_haute_priorite_fin += 2;
                }
                else
                    break;
            }
    //        qDebug() << "\tvariable_avec_operations_haute_priorite_fin = " << variable_avec_operations_haute_priorite_fin;

//            afficher_les_variables_a_la_fin(&formule1,  variable_avec_operations_haute_priorite_debut, variable_avec_operations_haute_priorite_fin);
//            qDebug() << "\tvariable_avec_operations_haute_priorite = " << variable_avec_operations_haute_priorite;

            // ==>> CONDITION POUR DÉPLACER LA VARIABLE À LA FIN DU SOUS-GROUPE
    //        - SI variable_avec_operations_haute_priorite_fin n'est pas à la fin du groupe
            if( variable_avec_operations_haute_priorite_fin < groupe_avec_variable_fin)
            {
                // ==>> CONDITION POUR DÉPLACER LA VARIABLE "AVEC" LES OPERATIONS HAUTE PRIORITE À LA FIN DU SOUS-GROUPE
    //            - SI variable_avec_operations_haute_priorite contient des opérations haute priorité
                if(variable_avec_operations_haute_priorite_fin - variable_avec_operations_haute_priorite_debut > 0)
                {
//                    qDebug() << "\tvariable_avec_operations_haute_priorite_fin < groupe_avec_variable_fin\n\t" << variable_avec_operations_haute_priorite_fin << " < " << groupe_avec_variable_fin;

        //        groupe_avec_variable_nombre_de_signes
                    groupe_avec_variable_nombre_de_signes = (groupe_avec_variable_fin - groupe_avec_variable_debut) / 2;
//                    qDebug() << "\tgroupe_avec_variable_nombre_de_signes = " << groupe_avec_variable_nombre_de_signes;

                    //        variable_avec_operations_haute_priorite_nombre_de_signes
                    variable_avec_operations_haute_priorite_nombre_de_signes = (variable_avec_operations_haute_priorite_fin - variable_avec_operations_haute_priorite_debut) / 2;
//                    qDebug() << "\tvariable_avec_operations_haute_priorite_nombre_de_signes = " << variable_avec_operations_haute_priorite_nombre_de_signes;

        //        nombre_de_signes_restants = groupe_avec_variable_nombre_de_signes - variable_avec_operations_haute_priorite_nombre_de_signes
                    nombre_de_signes_restants = groupe_avec_variable_nombre_de_signes - variable_avec_operations_haute_priorite_nombre_de_signes;

        //        // CONDITION POUR DÉPLACER variable_et_plus À LA FIN DU GROUPE
        //            - SI nombre_de_signes_restants > 1
                    if(nombre_de_signes_restants > 1)
                    {
//                        qDebug() << "\tnombre_de_signes_restants = " << nombre_de_signes_restants;

        //                - variable_avec_operations_basse_priorite_signe_avant
                        variable_avec_operations_basse_priorite_signe_avant = structure_parent->liste.at(variable_avec_operations_haute_priorite_debut - 1)->element;
//                        qDebug() << "\tvariable_avec_operations_basse_priorite_signe_avant = " << variable_avec_operations_basse_priorite_signe_avant;

                        // ==>> DÉPLACER LA VARIABLE ET LES OPERATIONS HAUTE PRIORITE À LA FIN DE LA LIGNE
        //                - déplacer variable_avec_operations_basse_priorite_signe_avant ET variable_avec_operations_haute_priorite à la fin du groupe_avec_variable
                        for(int index = 0; index < variable_avec_operations_haute_priorite_fin - variable_avec_operations_haute_priorite_debut + 2; index++)
                            structure_parent->liste.append(structure_parent->liste.takeAt(variable_avec_operations_haute_priorite_debut - 1));

//                        afficher_les_variables_a_la_fin(&formule1,  groupe_avec_variable_debut, groupe_avec_variable_fin);
//                        qDebug() << "\tSOUS-GROUPE AVEC VARIABLE = " << groupe_avec_variable;

                        // DÉFINIR LES NOUVELLES VALEURS DE DÉBUT ET DE FIN DE VARIABLE ET OPERATIONS HAUTE PRIORITE
                        variable_avec_operations_haute_priorite_difference_fin_moins_debut = variable_avec_operations_haute_priorite_fin - variable_avec_operations_haute_priorite_debut;
                        variable_avec_operations_haute_priorite_fin = structure_parent->liste.length() - 1;
                        variable_avec_operations_haute_priorite_debut = variable_avec_operations_haute_priorite_fin - variable_avec_operations_haute_priorite_difference_fin_moins_debut;

//                        afficher_les_variables_a_la_fin(&formule1,  variable_avec_operations_haute_priorite_debut, variable_avec_operations_haute_priorite_fin);
//                        qDebug() << "\tVARIABLE ET OPERATIONS HAUTE PRIORITE A LA FIN = " << variable_avec_operations_haute_priorite;
                    }
                }
                // ==>> CONDITION POUR DÉPLACER LA VARIABLE "SANS" LES OPERATIONS HAUTE PRIORITE À LA FIN DU SOUS-GROUPE
                else
                {
//                    qDebug() << "\n\tgroupe_avec_variable_debut = " << groupe_avec_variable_debut;
//                    qDebug() << "\n\tgroupe_avec_variable_fin = " << groupe_avec_variable_fin;
//                    qDebug() << "\tvariable_position = " << variable_position;

    //                - SI le nombre d'opérations dans le groupe > 1
                    if(groupe_avec_variable_fin - groupe_avec_variable_debut > 2)
                    {
                        // ==>> DÉPLACER LA VARIABLE À LA FIN DE LA LIGNE
    //                    - déplacer VARIABLE à la fin du groupe
    //                    - SI VARIABLE n'est pas au début du groupe
                        if(variable_position > 0)
                        {
                            structure_parent->liste.append(structure_parent->liste.takeAt(groupe_avec_variable_debut + variable_position - 1));
                            structure_parent->liste.append(structure_parent->liste.takeAt(groupe_avec_variable_debut + variable_position - 1));
                        }
                        else // SINON VARIABLE est au début du groupe
                        {
        //                    - créer une structure pour l'élément + dans 'nouvelle_structure'
                            structure* nouvelle_structure = new structure;
                            nouvelle_structure->element = "*";
                            nouvelle_structure->type_element = OPERATEUR;

            //                - ajouter + à la fin de la ligne
                            structure_parent->liste.append(nouvelle_structure);
                            nouvelle_structure = nullptr;

                            structure_parent->liste.append(structure_parent->liste.takeAt(groupe_avec_variable_debut));
    //                        - SI le SIGNE en début de GROUPE EST /
                            if(structure_parent->liste.at(groupe_avec_variable_debut)->element == "/")
                            {
            //                    - créer une structure pour l'élément 1 dans 'nouvelle_structure'
                                structure* nouvelle_structure = new structure;
                                nouvelle_structure->element = "1";
                                nouvelle_structure->type_element = OPERANDE;

                                structure_parent->liste.insert(groupe_avec_variable_debut, nouvelle_structure);
                                nouvelle_structure = nullptr;

                                groupe_avec_variable_fin++;
                            }
                            else // SINON le SIGNE en début de GROUPE EST *
                                structure_parent->liste.removeAt(groupe_avec_variable_debut);
                        }
                        variable_position = structure_parent->liste.length() - 1;
                    }
                }
            }

    //    AFFICHER LE RESULTAT
//            afficher_les_variables_a_la_fin(&formule1,  0, structure_parent->liste.length() - 1);
//            qDebug() << "\n\tFORMULE NOUVELLE = " << *formule << "\n";
        } // FIN BOUCLE FOR
        // METTRE LES VARIABLES À LA FIN POUR CHAQUE LISTE CONTENANT UNE VARIABLE
        for(int index = 0; index < structure_parent->liste.length(); index++)
        {
//            qDebug() << "\tindex: " << index;

            if(structure_parent->liste.at(index)->variable_dans_liste)
                this->mettre_les_variables_a_la_fin(structure_parent->liste.at(index));

            else if(structure_parent->liste.at(index)->element != "")
                if(structure_parent->liste.at(index)->type_element == VARIABLES_POSSIBLES)
                    this->mettre_les_variables_a_la_fin(structure_parent->liste.at(index));
        }
    } // FIN IF
}

void model_formule::transformer_les_operations_en_notation_polonaise_inverse(structure* structure_parent)
{
    // qDebug() << "model_formule::transformer_les_operations_en_notation_polonaise_inverse";

    // UTILISER LA FORMULE DONNEE EN PARAMÈTRE POUR LA TRANSFORMER EN NOTATION POLONAISE INVERSE
    // 1 - mettre tous les opérateurs de la formule dans liste_operateurs
    // 2 - utilisation de la variable decalage_index pour insérer dans la formule les opérateurs

    structure* operateur;
    structure* operateur_suivant;
    int operateur_priorite; // 0 = + - ; 1 = * / ; 2 = ^ %
    int operateur_suivant_priorite; // 0 = + - ; 1 = * / ; 2 = ^ %

    QList<structure*> liste_operateurs;
    QList<structure*> operateurs_en_attente;
    QList<int> operateurs_en_attente_priorite;
    int decalage_index;
    int fonction_de_calcul_decalage_index;

    if(structure_parent->liste.length() > 2)
    {
        decalage_index = 0;

    //    Mettre les signes dans une liste
        for(int index = structure_parent->liste.length() - 2; index > 0; index -= 2)
            liste_operateurs.prepend(structure_parent->liste.takeAt(index));

    //    SI il a un signe
        while (!liste_operateurs.isEmpty())
        {
    //        Incrémenter le décalage d’index
            decalage_index++;

    //        Prendre le premier signe
            operateur = liste_operateurs.takeFirst();
            operateur_priorite = definir_operateur_priorite(operateur->element);

    //        SI il a un signe après
            if(!liste_operateurs.isEmpty())
            {
    //            Lire le signe suivant
                operateur_suivant = liste_operateurs.at(0);
                operateur_suivant_priorite = definir_operateur_priorite(operateur_suivant->element);

    //            SI Signe > signe suivant
                if(operateur_priorite > operateur_suivant_priorite)
                {
    //                Incrémenter le décalage d’index
                    decalage_index++;

    //                Ajouter le signe dans les opérations au décalage d’index
                    structure_parent->liste.insert(decalage_index, operateur);

    //                TANT QU’ il a un signe en attente
                    while (!operateurs_en_attente.isEmpty())
                    {
    //                    SI le dernier signe en attente >= signe suivant
                        if(operateurs_en_attente_priorite.last() >= operateur_suivant_priorite)
                        {
    //                        Incrémenter le décalage d’index
                            decalage_index++;

    //                        Ajouter le dernier signe dans les opérations au décalage d’index
                            structure_parent->liste.insert(decalage_index, operateurs_en_attente.takeLast());
                            operateurs_en_attente_priorite.removeLast();
                        }
    //                    SINON
                        else
    //                        sortir de TANT QUE
                            break;
                    }
                }
    //            SINON SI Signe = signe suivant
                else if(operateur_priorite == operateur_suivant_priorite)
                {
    //                Incrémenter le décalage d’index
                    decalage_index++;

    //                Ajouter le signe dans les opérations au décalage d’index
                    structure_parent->liste.insert(decalage_index, operateur);
                }

    //            SINON
                else
                {
    //                Ajouter le signe en attente
                    operateurs_en_attente.append(operateur);
                    operateurs_en_attente_priorite.append(operateur_priorite);
                }
            }
    //        SINON
            else
            {
    //            Incrémenter le décalage d’index
                decalage_index++;

    //            Ajouter le signe dans les opérations au décalage d’index
                structure_parent->liste.insert(decalage_index, operateur);

    //            operateurs_en_attente_index = operateurs_en_attente.length() - 1;
    //            TANT QU’ il a un signe en attente
                while (!operateurs_en_attente.isEmpty())
                {
    //                Incrémenter le décalage d’index
                    decalage_index++;

    //                Ajouter le dernier signe dans les opérations au décalage d’index
                    structure_parent->liste.insert(decalage_index, operateurs_en_attente.takeLast());
                    operateurs_en_attente_priorite.removeLast();
                }
            }
        } // FIN WHILE
        // TRANSFORMER LES LISTES DES STRUCTURES EN NOTATION POLONAISE INVERSE POUR CHAQUE STRUCTURE DONT index_element = -1
        for(int index = structure_parent->liste.length() - 1; index > -1 ; index--)
        {
//            qDebug() << "\tindex: " << index;

            if(structure_parent->liste.at(index)->element == "")
            {
                // transformer la liste en notation polonaise inverse
                this->transformer_les_operations_en_notation_polonaise_inverse(structure_parent->liste.at(index));

                // mettre le contenu de la liste à la place de la structure parent
                while(!structure_parent->liste.at(index)->liste.isEmpty())
                    structure_parent->liste.insert(index + 1, structure_parent->liste.at(index)->liste.takeLast());

                // supprimer la strucutre parent dont index_element = -1
                structure_parent->liste.removeAt(index);
            }
            else if(structure_parent->liste.at(index)->type_element == FONCTIONS_DE_CALCUL)
            {
//                qDebug() << "\tFONCTIONS_DE_CALCUL trouvee ";
                this->transformer_les_operations_en_notation_polonaise_inverse(structure_parent->liste.at(index));

                fonction_de_calcul_decalage_index = 0;
                // mettre le contenu de la liste de la structure dans la structure_parent
                while(!structure_parent->liste.at(index + fonction_de_calcul_decalage_index)->liste.isEmpty())
                {
                    structure_parent->liste.insert(index, structure_parent->liste.at(index + fonction_de_calcul_decalage_index)->liste.takeLast());
                    fonction_de_calcul_decalage_index++;
                }
            }
        }

    } // FIN FOR
}

int model_formule::definir_operateur_priorite(const QString operateur)
{
    // qDebug() << "model_formule::definir_operateur_priorite";

    QString operateurs_priorite_basse;
    QString operateurs_priorite_moyenne;

    operateurs_priorite_basse = "+-";
    operateurs_priorite_moyenne = "*/";

    if(operateurs_priorite_basse.contains(operateur)) // + -
        return 0;
    else if(operateurs_priorite_moyenne.contains(operateur)) // * /
        return 1;
    else // ^ %
        return 2;
}

bool model_formule::faire_les_calculs_sans_variable(structure* structure_parent)
{
    // qDebug() << "model_formule::faire_les_calculs_sans_variable";

    static bool pas_d_erreur;
    double resultat;
    double element_gauche;
    double element_droit;
    int structure_index;
    structure* structure_1;
    int type_element;
    structure* structure_operande;

    pas_d_erreur = true;
    structure_index = 0;

    while(structure_index < structure_parent->liste.length())
    {
//        qDebug() << "\tstructure_parent->liste = " << structure_parent->liste.length() << " elements";

        structure_1 = structure_parent->liste.at(structure_index);
        type_element = structure_1->type_element;

//        SI c’est un opérateur ALORS
        if(type_element == OPERATEUR)
        {
            // SI les 2 éléments avant l’opérateur ne sont pas des fonctions de calcul NI des variables
            if(structure_parent->liste.at(structure_index - 1)->type_element == OPERANDE && structure_parent->liste.at(structure_index - 2)->type_element == OPERANDE)
            {
//                opérande droit = prendre l’élément avant l’opérateur
                structure_operande = structure_parent->liste.takeAt(structure_index - 1);
                element_droit = structure_operande->element.toDouble();
//                qDebug() << "\n\telement_droit  = " << element_droit;

//                opérande gauche = prendre l’élément avant l’opérateur
                structure_operande = structure_parent->liste.takeAt(structure_index - 2);
                element_gauche = structure_operande->element.toDouble();
//                qDebug() << "\telement_gauche = " << element_gauche;

//                mettre dans le signe le résultat de l’opération
//                qDebug() << "\toperateur      = " << structure_1->element;
                resultat = this->faire_l_operation(structure_1->element, element_gauche, element_droit);
                if(!this->erreur_dans_operation)
                {
//                    qDebug() << "\tresultat       = " << resultat;
                    structure_parent->liste[structure_index - 2]->element = QString::number(resultat);
                    structure_parent->liste[structure_index - 2]->type_element = OPERANDE;
                }
                else // erreur dans l'opération
                {
                    pas_d_erreur = false;
                    break;
                }

//                Index (pour la suite de l’algorithme) = index_actuel – 1
                structure_index -= 2; // -2 en fin de while: structure_index++
            }
        }

//        SINON SI c’est une fonction de calcul ALORS
        else if(type_element == FONCTIONS_DE_CALCUL)
        {
//            qDebug() << "\n\ttype_element == FONCTIONS_DE_CALCUL";

//            faire les calculs sans variable pour le contenu de la liste de la structure
            this->faire_les_calculs_sans_variable(structure_1);
            this->afficher_toutes_les_structures(structure_1);

            // SI la liste de structure_1 contient 1 élément ALORS
            if(structure_1->liste.length() == 1)
            {
                QString fonction_de_calcul = structure_1->element;
                QString element = structure_1->liste.first()->element;
//                qDebug() << "\n\tfonction_de_calcul = " << fonction_de_calcul;
//                qDebug() << "\telement            = " << element;
//                resultat = this->faire_l_operation_d_une_fonction_de_calcul(fonction_de_calcul, element.toDouble());
                resultat = this->faire_l_operation(fonction_de_calcul, element.toDouble());

                if(!this->erreur_dans_operation)
                {
//                    qDebug() << "\tresultat           = " << resultat;
                    structure_parent->liste[structure_index]->element = QString::number(resultat);
                    structure_parent->liste[structure_index]->type_element = OPERANDE;
                    structure_parent->liste.at(structure_index)->liste.clear();
                }
                else // erreur dans l'opération
                {
                    pas_d_erreur = false;
                    break;
                }
            }
        }

        structure_index++;
    }
    return pas_d_erreur;
}

double model_formule::faire_l_operation(const QString operateur, const double element_gauche, const double element_droit)
{
    // qDebug() << "model_formule::faire_l_operation";

    if(operateur == "+")
        return element_gauche + element_droit;

    else if(operateur == "-")
        return element_gauche - element_droit;

    else if(operateur == "*")
        return element_gauche * element_droit;

    else if(operateur == "/")
        return element_gauche / element_droit;

    else if(operateur == "%")
    {
        int element_droit_temp = static_cast<int>(element_droit);

        if(element_droit_temp == 0)
        {
            qDebug() << "\nERREUR DANS L'OPERATION %: LA PARTIE DROITE = 0";
            this->erreur_dans_operation = true;

            return 0.0;
        }
        int element_gauche_temp = static_cast<int>(element_gauche);
        return element_gauche_temp % element_droit_temp;
    }

    else if(operateur == "^")
        return qPow(element_gauche,element_droit);

    else
    {
            qDebug() << "\nERREUR DANS LA FONCTION 'faire_l_operation'";
            this->erreur_dans_operation = true;

            return 0.0;
    }
}

double model_formule::faire_l_operation(const QString fonction_de_calcul, const double element)
{
    // qDebug() << "model_formule::faire_l_operation2";

    if(fonction_de_calcul == "cos")
        return cos(element);

    else if(fonction_de_calcul == "sin")
        return sin(element);

    else if(fonction_de_calcul == "tan")
        return tan(element);

    else if(fonction_de_calcul == "sqrt")
    {
        if(element >= 0)
            return sqrt(element);
        else
        {
            qDebug() << "\nERREUR DANS L'OPERATION %: LA PARTIE DROITE = 0";
            this->erreur_dans_operation = true;

            return 0.0;
        }
    }
    else
    {
            qDebug() << "\nERREUR DANS LA FONCTION 'faire_l_operation'";
            this->erreur_dans_operation = true;

            return 0.0;
    }
}

bool model_formule::preparer_le_calcul_final(structure* structure_parent)
{
    // qDebug() << "model_formule::preparer_le_calcul_final";

    QStringList elements_liste;
    QString variable_possible;

    // remplir elements_liste avec les éléments suivant
    // valeur     = OPERANDE
    // valeur     = VARIABLES_POSSIBLES
    // opérateur = OPERATEUR
    // fonction     = FONCTIONS_DE_CALCUL
    // résultat     = résultat d'un calcul
    // nul         = élément utilisé

    this->operandes_liste.clear();
    this->operations_liste.clear();

    // restaurer la sauvegarde de 'variables_possibles_index'
    this->variables_possibles_index = this->variables_possibles_index_sauvegarde;

    for(int index = 0; index < structure_parent->liste.length(); index++)
    {
        if(structure_parent->liste.at(index)->type_element == OPERANDE || structure_parent->liste.at(index)->type_element == VARIABLES_POSSIBLES)
            elements_liste.append("valeur");
        else if(structure_parent->liste.at(index)->type_element == OPERATEUR)
            elements_liste.append("operateur");
        else
            elements_liste.append("fonction");
    }
//    qDebug() << "\nelements_liste = " << elements_liste;


    for(int index = 0; index < structure_parent->liste.length(); index++)
    {
        // ajouter une nouvelle place pour le résultat
        this->operandes_liste.append(0.0);

        if(structure_parent->liste.at(index)->type_element == OPERANDE)
            this->operandes_liste.last() = structure_parent->liste.at(index)->element.toDouble();

//        rechercher une fonction OU un opérateur dans la liste d’éléments
//        SI c’est un opérateur ALORS
        else if(structure_parent->liste.at(index)->type_element == OPERATEUR)
        {
//            créer une nouvelle opération
            operation* nouvelle_operation = new operation;
//            résultat = index de l’opérateur
            nouvelle_operation->resutat = index;
//            opérateur_ou_fonction = contenu de la structure
            nouvelle_operation->operateur_ou_fonction = structure_parent->liste.at(index)->element;
//            Element_droit = index de l’opérateur – 1
            nouvelle_operation->element_droit = index - 1;
//            Element_gauche = index non ‘nul’ avant Element_droit
            for(int index_element_gauche = index - 2; index_element_gauche > -1; index_element_gauche--)
            {
                if(elements_liste.at(index_element_gauche) != "nul")
                {
                    nouvelle_operation->element_gauche = index_element_gauche;
                    break;
                }
            }
//            ajouter cette opération à la liste des opérations
            this->operations_liste.append(nouvelle_operation);

//            mettre dans la liste d’éléments à élément_gauche ‘nul’
            elements_liste[nouvelle_operation->element_gauche] = "nul";
//            mettre dans la liste d’éléments à élément_droit ‘nul’
            elements_liste[index - 1] = "nul";
//            mettre dans la liste d’éléments à résultat ‘resultat’
            elements_liste[index] = "resultat";

            nouvelle_operation = nullptr;
        }

//        SINON SI c’est une fonction ALORS
        else if(structure_parent->liste.at(index)->type_element == FONCTIONS_DE_CALCUL)
        {
//            créer une nouvelle opération
            operation* nouvelle_operation = new operation;
//            résultat = index de l’opérateur
            nouvelle_operation->resutat = index;
//            opérateur_ou_fonction = contenu de la structure
            nouvelle_operation->operateur_ou_fonction = structure_parent->liste.at(index)->element;
//            Element = index de la fonction – 1
            nouvelle_operation->element_gauche = index - 1;

//            ajouter cette opération à la liste des opérations
            this->operations_liste.append(nouvelle_operation);
            nouvelle_operation = nullptr;

//            mettre dans la liste d’éléments à élément ‘nul’
            elements_liste[index - 1] = "nul";
//            mettre dans la liste d’éléments à résultat ‘resultat’
            elements_liste[index] = "resultat";
        }
        else // SINON c'est une VARIABLES_POSSIBLES
        {
            variable_possible = structure_parent->liste.at(index)->element;

            if(this->variables_possibles_index[this->variables_possibles.indexOf(variable_possible)].at(0) != -1)
                this->variables_possibles_index[this->variables_possibles.indexOf(variable_possible)].append(index);
            else
                this->variables_possibles_index[this->variables_possibles.indexOf(variable_possible)][0] = index;
        }
    }
//    qDebug() << "\noperandes_liste = " << this->operandes_liste;
//    qDebug() << "\nvariables_possibles = " << this->variables_possibles;
//    qDebug() << "\nvariables_possibles_index = " << this->variables_possibles_index;

    return true;
}

void model_formule::afficher_toutes_les_structures(const structure* structure_parent)
{
    static int niveau = 0;
    QString tab(" ");
    QString element;

    for(int index = 0; index < structure_parent->liste.length(); index++)
    {
        element = structure_parent->liste.at(index)->element;

        if(structure_parent->liste.at(index)->variable_dans_liste == true)
            qDebug() << tab.repeated(niveau*10) + "niveau(" + QString::number(niveau) + ")" << "structure(" << index << "): element = " << element << " - VARIABLE DANS LA LISTE";
        else
            qDebug() << tab.repeated(niveau*10) + "niveau(" + QString::number(niveau) + ")" << "structure(" << index << "): element = " << element;

        if(structure_parent->liste.at(index)->liste.length() > 0)
        {
            niveau++;
            this->afficher_toutes_les_structures(structure_parent->liste.at(index));
            niveau--;
        }
    }
}

void model_formule::afficher_les_variables_a_la_fin(const structure* structure_parent, int groupe_avec_variable_debut, int groupe_avec_variable_fin)
{
    static int nombre_affichage = 0;

    nombre_affichage++;

    // qDebug() << "\tgroupe_avec_variable: " << nombre_affichage;

    for(int index = groupe_avec_variable_debut; index < groupe_avec_variable_fin + 1; index++)
        qDebug() << "\tindex_structure = " << QString::number(index) << ", element = " << structure_parent->liste.at(index)->element << "\n";
}

void model_formule::afficher_toutes_les_operations(const QList<operation*>* operations_liste)
{
    for(int index = 0; index < operations_liste->length(); index++)
    {
        if(operations_liste->at(index)->element_droit != -1)
            qDebug() << "index(" + QString::number(index) + ") " << "operation = " << operations_liste->at(index)->operateur_ou_fonction << ", index element gauche = " << operations_liste->at(index)->element_gauche << ", index element droit = " << operations_liste->at(index)->element_droit << ", index resultat = " << operations_liste->at(index)->resutat;
        else
            qDebug() << "index(" + QString::number(index) + ") " << "fonction = " << operations_liste->at(index)->operateur_ou_fonction << ", index element gauche = " << operations_liste->at(index)->element_gauche << ", index resultat = " << operations_liste->at(index)->resutat;
    }
}
