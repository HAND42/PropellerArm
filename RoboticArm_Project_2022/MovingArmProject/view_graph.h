#ifndef VIEW_GRAPH_H
#define VIEW_GRAPH_H

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <QList>
#include <QPointF>
#include <QRectF>
#include <QMargins>
#include <QPen>
#include <QFont>

#include <QDebug>
#include <QString>
#include <QtMath>

#define INDEX_VALEUR_MINIMUM_X_Y 0
#define INDEX_VALEUR_MAXIMUM_X_Y 0
#define INDEX_PAS_X_Y 1


class view_graph
{
    public:
        view_graph();

        QChartView* donne_le_qchartview(void);
        void definit_le_titre(const QString titre);
        void ajoute_une_deuxieme_ligne(void);
        void dessiner_les_points(const QPointF point_1);
        void dessiner_les_points(const QPointF point_1, const QPointF point_2);
        void clear(void);

    private:
    // ATTRIBUTS
        bool une_seule_ligne;
        QLineSeries *series;
        QScatterSeries *series0;
        QLineSeries *series1;
        QScatterSeries *series2;
        QChart *chart;
        QChartView *chartView;

        // VALEUR X ET Y
        QPointF point_2;
        QList <QPointF> valeurs;
        int nombre_d_elements;

        // AXES X ET Y
        bool valeur_inferieure_a_min_OU_superieure_a_max_Y;

        // AXE X
        QValueAxis *axis_X;
        qreal valeur_X;
        int pas_axe_X;
        int nombre_d_elements_pour_changement_pas_X;

        // AXE Y
        QValueAxis *axis_Y;
        QList<QList<qreal>> groupes_intervalles_Y;
        qreal valeur_1_Y;
        qreal valeur_plus_forte_Y;
        qreal valeur_moins_forte_Y;
        qreal valeur_min_Y;
        qreal valeur_max_Y;
        qreal pas_axe_Y;
        int nombre_de_lignes_horizontales;
        bool valeur_max_Y_egal_0;
        qreal ecart_min_max_Y;

    // FONCTIONS
        void remplir_groupes_intervalles_axe_Y(void);
};

#endif // VIEW_GRAPH_H
