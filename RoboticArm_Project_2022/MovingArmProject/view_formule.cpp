#include "view_formule.h"
#include "ui_view_formule.h"

view_formule::view_formule(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::view_formule)
{
    // qDebug() << "view_formule::view_formule";

    ui->setupUi(this);

    this->myModel_formule = model_formule::getInstance();
    this->myView_formule_info = new view_formule_info();

    this->verifier_la_formule_pushButton_actif = false;
    this->erreur_label_deja_invisible = true;
    this->ui->erreur_titre_label->setVisible(false);
    this->ui->erreur_label->setVisible(false);
}

view_formule::~view_formule()
{
    // qDebug() << "view_formule::~view_formule";
    delete ui;
}

QString view_formule::formule()
{
    // qDebug() << "view_formule::formule";

    return this->ui->formule_lineEdit->text();
}

void view_formule::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)

    // qDebug() << "view_formule::closeEvent";
    if(this->myView_formule_info->isVisible())
        this->myView_formule_info->close();

    emit(this->view_formule_closed());
}

void view_formule::keyPressEvent(QKeyEvent *keyEvent)
{
//     qDebug() << "view_formule::keyPressEvent";
    if(keyEvent->key() == Qt::Key_Return)
     {
         if(this->ui->verifier_la_formule_pushButton->hasFocus())
            this->on_verifier_la_formule_pushButton_clicked();

         else if(this->ui->tester_la_formule_pushButton->hasFocus())
             this->on_tester_la_formule_pushButton_clicked();

         else if(this->ui->formule_info_pushButton->hasFocus())
             this->on_formule_info_pushButton_clicked();
     }
}

void view_formule::verify_return_slot()
{
     if(this->ui->verifier_la_formule_pushButton->hasFocus())
         this->on_verifier_la_formule_pushButton_clicked();

     else if(this->ui->tester_la_formule_pushButton->hasFocus())
         this->on_tester_la_formule_pushButton_clicked();

     else if(this->ui->formule_info_pushButton->hasFocus())
         this->on_formule_info_pushButton_clicked();
}

void view_formule::on_verifier_la_formule_pushButton_clicked()
{
//     qDebug() << "view_formule::on_verifier_la_formule_pushButton_clicked";

//     qDebug() << "texte =" << this->ui->formule_lineEdit->text();

     this->myModel_formule->preparer_la_formule_pour_le_calcul(this->ui->formule_lineEdit->text());

    if(!this->myModel_formule->est_elle_sans_erreur())
        this->erreur_message_a_afficher(this->myModel_formule->donne_l_erreur());

    else
    {
        this->ui->verifier_la_formule_pushButton->setEnabled(false);
        this->ui->tester_la_formule_pushButton->setEnabled(true);
        this->ui->tester_la_formule_pushButton->setFocus();
    }
}

void view_formule::on_tester_la_formule_pushButton_clicked()
{
    // qDebug() << "view_formule::on_tester_la_formule_pushButton_clicked";

    this->ui->tester_la_formule_pushButton->setEnabled(false);

    emit(this->tester_la_formule_pushButton_clicked());
}

void view_formule::erreur_message_a_afficher(const QString texte)
{
    // qDebug() << "view_formule::erreur_message_a_afficher";

    this->ui->verifier_la_formule_pushButton->setEnabled(false);

    this->ui->erreur_titre_label->setVisible(true);
    this->ui->erreur_label->setVisible(true);
    this->ui->erreur_label->setText(texte);

//    this->erreur_label_deja_invisible = false;
}

void view_formule::erreur_message_a_cacher()
{
    // qDebug() << "view_formule::erreur_message_a_cacher";

    this->ui->erreur_titre_label->setVisible(false);
    this->ui->erreur_label->setVisible(false);
    this->ui->erreur_label->setText("");
}

void view_formule::on_formule_lineEdit_textChanged(const QString &arg1)
{
    // qDebug() << "view_formule::on_formule_lineEdit_textChanged";
//    qDebug() << "texte =" << arg1;

    emit(this->formule_en_cours_de_modification());

    if(!arg1.isEmpty())
    {
//        qDebug() << "1";
        if(!this->ui->verifier_la_formule_pushButton->isEnabled())
        {
//            qDebug() << "2";
            this->ui->verifier_la_formule_pushButton->setEnabled(true);
        }

        if(this->ui->tester_la_formule_pushButton->isEnabled())
        {
//            qDebug() << "2-3";
            this->ui->tester_la_formule_pushButton->setEnabled(false);
            this->ui->verifier_la_formule_pushButton->setEnabled(true);
        }
        this->erreur_message_a_cacher();
    }
    else
    {
//        qDebug() << "3";
        if(this->ui->verifier_la_formule_pushButton->isEnabled())
        {
//            qDebug() << "4";
            this->ui->verifier_la_formule_pushButton->setEnabled(false);
            this->erreur_message_a_afficher("La formule ne doit pas être vide");
        }
    }

}

void view_formule::on_formule_info_pushButton_clicked()
{
    this->myView_formule_info->show();
}
