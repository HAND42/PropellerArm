#ifndef VIEW_FORMULE_H
#define VIEW_FORMULE_H

#include <QMainWindow>
#include <QDebug>
#include <QString>
#include <QKeyEvent>

#include "model_formule.h"
#include "view_formule_info.h"

namespace Ui {
    class view_formule;
}

class view_formule : public QMainWindow
{
        Q_OBJECT

    public:
        explicit view_formule(QWidget *parent = nullptr);
        ~view_formule() override;

        QString formule(void);

    protected:
        void closeEvent(QCloseEvent *event) override;
        void keyPressEvent(QKeyEvent *keyEvent) override;

    signals:
        void view_formule_closed(void);
        void tester_la_formule_pushButton_clicked(void);
        void formule_en_cours_de_modification(void);

    private slots:
        void on_verifier_la_formule_pushButton_clicked();

        void on_tester_la_formule_pushButton_clicked();

        void on_formule_lineEdit_textChanged(const QString &arg1);

        void on_formule_info_pushButton_clicked();

        void verify_return_slot();

    private:
        Ui::view_formule *ui;

        model_formule* myModel_formule;
        view_formule_info* myView_formule_info;

        int calcul_pret;
        bool verifier_la_formule_pushButton_actif;
        bool erreur_label_deja_invisible;

        void erreur_message_a_afficher(const QString texte);
        void erreur_message_a_cacher(void);

        void formules_a_tester_a_remplir(void); // A SUPPRIMER
};

#endif // VIEW_FORMULE_H
