#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define ANALYSE_INDEX  0
#define TEST_INDEX     1
#define VARIABLE_INDEX 2

#include <QMainWindow>
#include <QDebug>
#include <QStringList>
#include <QList>
#include <QBasicTimer>
//#include <QTime>

#include "model_formule.h"
#include "view_formule.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow() override;

    public slots:
        void slot_formule_a_tester(void);

    protected:
        void timerEvent(QTimerEvent *event) override;

//    signals:
//        void plus_clicked(void);

    private slots:
//        void timer_tick(void);

        void on_ouvrir_view_formule_pushButton_clicked(void);

        void on_affichage_debuter_pushButton_clicked();

        void on_affichage_finir_pushButton_clicked();

        void view_formule_closed(void);

        void view_formule_modification(void);

//        void on_plus_pushButton_clicked();

    private:
        Ui::MainWindow *ui;

        model_formule* myModel_formule;
        view_formule* myView_formule;

        QStringList formule_variables_possibles;
        QList<int> formule_variables_possibles_index;
        QList<double*> formule_valeurs;
        double formule_resultat;

        // TIMER
        QBasicTimer* timer;
        int compteur;
        double variable_possible_analyse;
        double variable_possible_test;
        double variable_possible_variable;

        // TEMPS ECOULE
//        QTime* temps_ecoule;

        void donne_a_la_formule_les_variables_possibles(void);
};

#endif // MAINWINDOW_H
