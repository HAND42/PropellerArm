#ifndef MODEL_DRAW_H
#define MODEL_DRAW_H

#include <QWidget>
#include <QDebug>
#include <QTimer>
#include <QPaintEvent>
#include <QString>
#include <QBrush>
#include <QFont>
#include <QPen>
#include <QPainter>
#include <QPainterPath>
#include <QRect>
#include <QPoint>
#include <QImage>
#include <QPicture>
#include <QPixmap>
#include <QPolygon>
#include <QRectF>
#include <QPointF>

#define LARGEUR 741
#define HAUTEUR 661

class model_draw : public QWidget
{
    Q_OBJECT

    public:
        model_draw(QWidget *parent);
        ~model_draw() override;

        void definit_la_taille(const QSize taille);
        void adapte_la_taille(const QSize taille);
//        void definit_la_puissance(const qreal puissance);
//        void definit_la_rotation_du_bras(const qreal angle);
        void definit_la_puissance_et_la_rotation_du_bras(const qreal puissance, const qreal angle);

    public slots:
//        void animate();

    protected:
        void paintEvent(QPaintEvent *event) override;

    private:
        int elapsed;
        int difference_largeur;
        int difference_hauteur;
        int difference_taille;
        qreal scale_X;
        qreal scale_Y;
        qreal scale_valeur;

        qreal puissance;
        qreal angle_bras;

        double rapport_largeur;

        QBrush background;
        QBrush circleBrush;
        QFont textFont;
        QPen circlePen;
        QPen textPen;
        QPainter painter;

        // IMAGES
        QImage fond_ecran;
        QImage texture_bois;
        QImage texture_alu_noir;
        QImage texture_aluminium;

        // BRAS HELICE
        QRect table_cote;
        QPolygon table_dessus;
        QRect poteau;
        QRect attache_bras;
        QRect bras;
        QPolygon helice;
        QPolygonF fleche;
        QPixmap test;

        void redessine_la_fleche(const qreal puissance);
};

#endif
