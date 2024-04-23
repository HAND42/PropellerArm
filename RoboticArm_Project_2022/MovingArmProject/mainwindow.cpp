#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // qDebug() << "MainWindow::MainWindow";

    ui->setupUi(this);

    this->myModel_formule = model_formule::getInstance();
    this->myModel_formule->init();

    this->myView_formule = new view_formule(this);
    this->timer = new QBasicTimer();
//    this->temps_ecoule = new QTime();

    connect(this->myView_formule, SIGNAL(view_formule_closed()), this, SLOT(view_formule_closed()));

    connect(this->myView_formule, SIGNAL(tester_la_formule_pushButton_clicked()), this, SLOT(slot_formule_a_tester()));

    connect(this->myView_formule, SIGNAL(formule_en_cours_de_modification()), this, SLOT(view_formule_modification()));

//    connect(this, SIGNAL(plus_clicked()), this, SLOT(timer_tick()));

    this->formule_variables_possibles << "ANALYSE" << "TEST" << "VARIABLE";

this->myModel_formule->recevoir_les_variables_possibles(this->formule_variables_possibles);
}

MainWindow::~MainWindow()
{
    // qDebug() << "MainWindow::~MainWindow";

    delete ui;
}

void MainWindow::slot_formule_a_tester()
{
    // qDebug() << "MainWindow::slot_formule_a_tester";

    this->ui->affichage_debuter_pushButton->setEnabled(true);

    qDebug() << "\nla formule: " << this->myView_formule->formule();

    this->formule_variables_possibles_index = this->myModel_formule->donne_les_variables_possibles_utilisees_index();
    // qDebug() << "formule_variables_possibles_index: " << this->formule_variables_possibles_index;

    this->formule_valeurs.clear();

    // mettre les références des variables possibles dans formule_valeurs
    for(int index = 0; index < this->formule_variables_possibles_index.length(); index++)
    {
        if(this->formule_variables_possibles_index.at(index) == 0)
            this->formule_valeurs.append(&this->variable_possible_analyse);
        else if(this->formule_variables_possibles_index.at(index) == 1)
            this->formule_valeurs.append(&this->variable_possible_test);
        else
            this->formule_valeurs.append(&this->variable_possible_variable);
    }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
//void MainWindow::timer_tick()
//{
    this->formule_resultat = this->myModel_formule->calculer_le_resultat(this->formule_valeurs);

//    qDebug() << "variable_possible_analyse =" << this->variable_possible_analyse;
//    qDebug() << "variable_possible_test =" << this->variable_possible_test;
//    qDebug() << "variable_possible_variable =" << this->variable_possible_variable;
//    qDebug() << "resultat:" << this->formule_resultat << "- temps ecoule: " << this->temps_ecoule->elapsed();
    qDebug() << "resultat:" << QString::number(this->formule_resultat);

    if(this->compteur == 9)
        this->on_affichage_finir_pushButton_clicked();

    this->compteur++;
    this->variable_possible_analyse += 1.0;
    this->variable_possible_test += 10.0;
    this->variable_possible_variable += 100.0;
}

void MainWindow::on_ouvrir_view_formule_pushButton_clicked()
{
    // qDebug() << "MainWindow::on_ouvrir_view_formule_pushButton_clicked";

    this->ui->ouvrir_view_formule_pushButton->setEnabled(false);
    this->ui->affichage_debuter_pushButton->setEnabled(false);
    this->ui->affichage_finir_pushButton->setEnabled(false);

    this->myView_formule->show();
}

void MainWindow::on_affichage_debuter_pushButton_clicked()
{
    // qDebug() << "MainWindow::on_affichage_debuter_pushButton_clicked";

    this->ui->affichage_debuter_pushButton->setEnabled(false);
    this->ui->affichage_finir_pushButton->setEnabled(true);

    this->compteur = 0;
    this->variable_possible_analyse = 0.0;
    this->variable_possible_test = 0.0;
    this->variable_possible_variable = 0.0;

//    for(int index = 0; index < this->formule_variables_possibles_index.length(); index++)
//         qDebug() << "\tindex: " << index << " valeurs = " << *this->formule_valeurs.at(index);

//    this->temps_ecoule->start();
    this->timer->start(100, Qt::PreciseTimer, this);
}

void MainWindow::on_affichage_finir_pushButton_clicked()
{
    // qDebug() << "MainWindow::on_affichage_finir_pushButton_clicked";

    this->timer->stop();

    this->ui->affichage_debuter_pushButton->setEnabled(true);
    this->ui->affichage_finir_pushButton->setEnabled(false);
}

void MainWindow::view_formule_closed()
{
    this->ui->ouvrir_view_formule_pushButton->setEnabled(true);
    this->ui->affichage_debuter_pushButton->setEnabled(false);
    this->ui->affichage_finir_pushButton->setEnabled(false);
}

void MainWindow::view_formule_modification()
{
    this->ui->ouvrir_view_formule_pushButton->setEnabled(false);
    this->ui->affichage_debuter_pushButton->setEnabled(false);
    this->ui->affichage_finir_pushButton->setEnabled(false);
}

void MainWindow::donne_a_la_formule_les_variables_possibles(void)
{
    // qDebug() << "MainWindow::donne_a_la_formule_les_variables_possibles";

    this->myModel_formule->recevoir_les_variables_possibles(this->formule_variables_possibles);
}

//void MainWindow::on_plus_pushButton_clicked()
//{
//    emit(this->plus_clicked());
//}
