#include "view_graph.h"

view_graph::view_graph()
{
    this->une_seule_ligne = true;

    this->axis_X = new QValueAxis();
    this->axis_Y = new QValueAxis();
    this->axis_Y->setMax(0.1);
    this->axis_Y->setLabelsFont(QFont("MS Shell Dlg 2", 7, QFont::Normal));
//    qDebug() << this->axis_Y->labelsFont();

    // AXE X
    this->nombre_d_elements = 1;
    this->pas_axe_X = 1;
    this->nombre_d_elements_pour_changement_pas_X = 10;

    // AXE Y
    this->valeur_min_Y = 0;
    this->valeur_max_Y = 0;
    this->remplir_groupes_intervalles_axe_Y();

    // SRIES
    this->series = new QLineSeries();
    this->series->setPen(QPen(Qt::red));

    this->series0 = new QScatterSeries();
    this->series0->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    this->series0->setMarkerSize(5);
    this->series0->setBrush(Qt::red);
    this->series0->setPen(QPen(Qt::red));

    // CHART
    this->chart = new QChart();
    this->chart->legend()->hide();
    this->chart->setMargins(QMargins(0, 0, 0, 0));
//    this->chart->setTitleBrush(Qt::darkGreen);
//    this->chart->setTitleFont(QFont("MS Shell Dlg 2", 10, QFont::Bold));
    this->chart->addAxis(this->axis_X, Qt::AlignBottom);
    this->chart->addAxis(this->axis_Y, Qt::AlignLeft);
    this->chart->addSeries(this->series);

    this->chart->addSeries(this->series0);
    this->series->attachAxis(this->axis_X);
    this->series->attachAxis(this->axis_Y);
    this->series0->attachAxis(this->axis_X);
    this->series0->attachAxis(this->axis_Y);

//    this->chart->setDropShadowEnabled(false);

    this->axis_X->setTickCount(7);
    this->axis_X->setLabelFormat("%.1f");
    this->axis_X->setRange(0.0, 3.0);
    this->axis_Y->setTickCount(2);
    this->axis_Y->setLabelFormat("%.1f");

//    this->axis_X->setTickType(QValueAxis::TicksDynamic);
//    this->axis_Y->setTickType(QValueAxis::TicksDynamic);

    this->chartView = new QChartView(this->chart);
    this->chartView->setRenderHint(QPainter::Antialiasing);
}

QChartView* view_graph::donne_le_qchartview(void)
{
    return this->chartView;
}

void view_graph::definit_le_titre(const QString titre)
{
    this->chart->setTitle(titre);
}

void view_graph::ajoute_une_deuxieme_ligne(void)
{
    // SRIES
    this->series1 = new QLineSeries();
    this->series1->setPen(QPen(Qt::blue));

    this->series2 = new QScatterSeries();
    this->series2->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    this->series2->setMarkerSize(5);
    this->series2->setBrush(Qt::blue);
    this->series2->setPen(QPen(Qt::blue));

    // CHART
    this->chart->addSeries(this->series1);
    this->chart->addSeries(this->series2);

    // AXIS
    this->series1->attachAxis(this->axis_X);
    this->series1->attachAxis(this->axis_Y);
    this->series2->attachAxis(this->axis_X);
    this->series2->attachAxis(this->axis_Y);

    this->une_seule_ligne = false;
}

void view_graph::dessiner_les_points(const QPointF point_1, const QPointF point_2)
{
    if(point_1.y() > point_2.y())
    {
        this->valeur_plus_forte_Y = point_1.y();
        this->valeur_moins_forte_Y = point_2.y();
    }
    else
    {
        this->valeur_plus_forte_Y = point_2.y();
        this->valeur_moins_forte_Y = point_1.y();
    }

    this->point_2 = point_2;
    this->dessiner_les_points(point_1);
}

void view_graph::dessiner_les_points(const QPointF point_1)
{
    this->valeur_X = point_1.x();

    if(this->une_seule_ligne)
    {
        this->valeur_plus_forte_Y = point_1.y();
        this->valeur_moins_forte_Y = point_1.y();
    }
    // TEST POUR ACCELERER L'AFFICHAGE DES GRAPHIQUES

//    qDebug() << "****************************************";
//    qDebug() << "element " << QString::number(this->nombre_d_elements) << " ----- X = " << QString::number(this->valeur_X) << " \tY = " << QString::number(this->valeur_plus_forte_Y);
/*
    // AXE X
    // CALCUL POUR axis_X->setTickCount ET axis_X->setMax
    if(this->nombre_d_elements > (this->nombre_d_elements_pour_changement_pas_X * this->pas_axe_X))
    {
//        qDebug() << "this->nombre_d_elements = " << this->nombre_d_elements_pour_changement_pas_X * this->pas_axe_X;
        if(this->pas_axe_X > 2)
            this->pas_axe_X *= 2;
        else if(this->pas_axe_X == 1)
            this->pas_axe_X = 2;
        else
            this->pas_axe_X = 5;

        // AJUSTER L'AFFICHAGE DES DECIMALES
        if(this->pas_axe_X > 5)
            this->axis_X->setLabelFormat("%.0f");
    }

//        CONDITION POUR MODIFIER    axis_X->setTickCount ET axis_X->setMax
    if(this->nombre_d_elements % this->pas_axe_X == 1 || this->nombre_d_elements < 11)
    {
//        qDebug() << "this->nombre_d_elements = " << this->nombre_d_elements;
//        qDebug() << "this->pas_axe_X = " << this->pas_axe_X;
//        qDebug() << "this->nombre_d_elements % this->pas_axe_X = " << this->nombre_d_elements % this->pas_axe_X;
//            this->axis_X->setMax(this->valeur_X + qreal(this->pas_axe_X) / 10.0 - 0.1);
        this->axis_X->setMax(this->valeur_X + (qreal(this->pas_axe_X) - 1) / 10.0);

        this->axis_X->setTickCount((this->nombre_d_elements + this->pas_axe_X - 1) / this->pas_axe_X + 1);
    }
//    qDebug() << "this->axis_X->max = " << this->axis_X->max();
//    qDebug() << "this->axis_X->tickCount = " << this->axis_X->tickCount();
*/
    // AXE Y
    // CALCUL POUR axis_Y->setTickCount ET axis_Y->setMax
    // VOIR FICHIER INFOS.TXT
    // TROUVER LE PAS EN FONCTION DE LA VALEUR Y
    if(this->valeur_plus_forte_Y >= this->axis_Y->max())
    {
        this->valeur_max_Y = this->valeur_plus_forte_Y;
        this->valeur_inferieure_a_min_OU_superieure_a_max_Y = true;
    }
    if(this->valeur_moins_forte_Y <= this->axis_Y->min())
    {
        this->valeur_min_Y = this->valeur_moins_forte_Y;
        this->valeur_inferieure_a_min_OU_superieure_a_max_Y = true;
    }

    if(this->valeur_inferieure_a_min_OU_superieure_a_max_Y == true)
    {
        this->ecart_min_max_Y = this->valeur_max_Y - this->valeur_min_Y;
//        qDebug() << "ecart_min_max_Y = " << this->ecart_min_max_Y;

        for(int index = 0; index < this->groupes_intervalles_Y.length() - 1; index++)
        {
            if(this->ecart_min_max_Y >= this->groupes_intervalles_Y.at(index).at(INDEX_VALEUR_MINIMUM_X_Y) && this->ecart_min_max_Y < this->groupes_intervalles_Y.at(index + 1).at(INDEX_VALEUR_MAXIMUM_X_Y))
            {
        // DEFINIR LE PAS ET LE NOMBRE DE LIGNES POUR LA VALEUR Y
//                    qDebug() << "DEBUT  ==>> axis_Y->min = " << this->axis_Y->min();
//                    qDebug() << "DEBUT  ==>> axis_Y->max = " << this->axis_Y->max();
                if(this->valeur_plus_forte_Y >= this->axis_Y->max() || this->valeur_moins_forte_Y <= this->axis_Y->min())
                {
                    this->pas_axe_Y = this->groupes_intervalles_Y.at(index).at(INDEX_PAS_X_Y);
//                    qDebug() << "pas_axe_Y = " << this->pas_axe_Y;

                    // AJUSTER L'AFFICHAGE DES DECIMALES
                    if(this->pas_axe_Y > 0.9)
                        this->axis_Y->setLabelFormat("%.0f");

                    this->axis_Y->setMax((qFloor(this->valeur_max_Y / this->pas_axe_Y) + 1) * this->pas_axe_Y);
//                    qDebug() << "this->axis_Y->max = " << this->axis_Y->max();
                    this->axis_Y->setMin((qCeil(this->valeur_min_Y / this->pas_axe_Y)) * this->pas_axe_Y - this->pas_axe_Y);
//                    qDebug() << "this->axis_Y->min = " << this->axis_Y->min();

//                    qDebug() << "nombre_de_lignes_horizontales AU DESSOUS 0 = " << int(qCeil(qFabs(this->axis_Y->min()) / this->pas_axe_Y));
//                    qDebug() << "nombre_de_lignes_horizontales AU DESSUS 0 = " << int(qCeil(this->axis_Y->max() / this->pas_axe_Y));
                    this->nombre_de_lignes_horizontales = int(qFloor(qFabs(this->axis_Y->min()) / this->pas_axe_Y) + qFloor(this->axis_Y->max() / this->pas_axe_Y)) + 1;
//                    qDebug() << "nombre_de_lignes_horizontales = " << this->nombre_de_lignes_horizontales;

                    this->axis_Y->setTickCount(this->nombre_de_lignes_horizontales);
//                    qDebug() << "this->axis_Y->tickCount = " << this->axis_Y->tickCount();
                }
//                    qDebug() << "FIN  ==>> axis_Y->min = " << this->axis_Y->min();
//                    qDebug() << "FIN  ==>> axis_Y->max = " << this->axis_Y->max();
                break;
            }
        }
        this->valeur_inferieure_a_min_OU_superieure_a_max_Y = false;
    }

    this->series->append(point_1);
    this->series0->append(point_1);

    if(!this->une_seule_ligne)
    {
//        qDebug() << "2 lignes dans le graphique";
        this->series1->append(this->point_2);
        this->series2->append(this->point_2);
    }

    this->nombre_d_elements++;
}

void view_graph::remplir_groupes_intervalles_axe_Y()
{
    this->groupes_intervalles_Y = {{0.0, 0.1}, {0.7, 0.2},
                                   {1.4, 0.4}, {2.8, 0.5},
                                   {3.5, 1}, {7, 2},
                                   {14, 4}, {28, 5},
                                   {35, 10}, {70, 20},
                                   {140, 40}, {280, 50},
                                   {350, 100}, {700, 200},
                                   {1400, 400}, {2800, 400}, {3500}};
}

void view_graph::clear(void)
{
    this->series->clear();
    this->series0->clear();

    if(!this->une_seule_ligne)
    {
        this->series1->clear();
        this->series2->clear();
    }

    this->nombre_d_elements = 1;
    this->pas_axe_X = 1;

    this->valeur_min_Y = 0;
    this->valeur_max_Y = 0;

//    this->axis_X->setTickCount(2);
//    this->axis_X->setLabelFormat("%.1f");
//    this->axis_X->setMin(0.0);
//    this->axis_X->setMax(0.2);
    this->axis_X->setTickCount(7);
    this->axis_X->setLabelFormat("%.1f");
    this->axis_X->setRange(0.0, 3.0);

    this->axis_Y->setTickCount(2);
    this->axis_Y->setLabelFormat("%.1f");
    this->axis_Y->setMin(-0.1);
    this->axis_Y->setMax(0.1);
}
