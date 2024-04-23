#include "model_draw.h"

model_draw::model_draw(QWidget *parent)
    : QWidget(parent)
{
    this->elapsed = 0;

    this->difference_largeur = 0;
    this->difference_hauteur = 0;
    this->difference_taille = 0;

    this->background = QBrush(QColor(64, 32, 64));
    this->fond_ecran = QImage("images/fond_ecran2.png");
//    this->fond_ecran = QImage("fond_ecran3.png");
    this->texture_bois = QImage("images/texture_wood.png");
    this->texture_alu_noir = QImage("images/texture_blackAlu.png");

    this->table_cote.setRect(190, 590, 400, 30);
    this->table_dessus << QPoint(190, 590) << QPoint(590, 590) << QPoint(550, 545) << QPoint(230, 545);
    this->poteau.setRect(225, 300, 45, 275);
    this->attache_bras.setRect(270, 320, 15, 30);
//    this->bras.setRect(273, 335, 9, 205);
//    this->helice << QPoint(285, 500) << QPoint(290, 500) << QPoint(288, 517) << QPoint(290, 535) << QPoint(285, 535) << QPoint(287, 517);
    this->bras.setRect(-4, 0, 9, 205);
    this->helice << QPoint(8, 165) << QPoint(13, 165) << QPoint(11, 182) << QPoint(13, 200) << QPoint(8, 200) << QPoint(10, 182);
    this->puissance = 0.0;
    this->angle_bras = 0.0;
}

model_draw::~model_draw()
{
}

void model_draw::definit_la_taille(const QSize taille)
{
    this->setFixedSize(taille);
    this->move(9, 21);
//    qDebug() << "model_draw->rect = " << this->rect();
}

void model_draw::adapte_la_taille(const QSize taille)
{
    this->difference_largeur = taille.width() - this->width();
//    qDebug() << "difference_largeur = " << QString::number(this->difference_largeur);
    this->difference_hauteur = taille.height() - this->height();
//    qDebug() << "difference_hauteur = " << QString::number(this->difference_hauteur);

    if(this->difference_largeur < this->difference_hauteur)
    {
        this->difference_taille = this->difference_largeur;
        this->move(0 , (this->difference_hauteur - this->difference_largeur) / 2);
    }
    else
    {
        this->difference_taille = this->difference_hauteur;
        this->move((this->difference_largeur - this->difference_hauteur) / 2, 0);
    }
//    qDebug() << "difference_taille = " << QString::number(this->difference_taille);

    this->setFixedSize(this->width() + this->difference_taille, this->height() + this->difference_taille);
}

//void model_draw::definit_la_puissance(const qreal puissance)
//{
//    this->puissance = puissance * 30.0;
//    this->repaint();
//}

//void model_draw::definit_la_rotation_du_bras(const qreal angle)
//{
//    this->angle_bras = -angle;
//    this->repaint();
//}

void model_draw::definit_la_puissance_et_la_rotation_du_bras(const qreal puissance, const qreal angle)
{
    this->puissance = puissance * 30.0;
    this->angle_bras = -angle;
    this->repaint();
}

//void model_draw::animate()
//{
//    this->elapsed = (this->elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
//    this->update();
//}

void model_draw::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

//    qDebug() << "painter ****";
//    qDebug() << "AVANT taille model_draw : " << this->size();

    this->painter.begin(this);
//    this->painter.fillRect(event->rect(), this->background);
    this->painter.setRenderHint(QPainter::Antialiasing);

    this->painter.drawImage(this->rect(), this->fond_ecran, this->fond_ecran.rect());
    this->scale_X = qreal(this->width()) / qreal(LARGEUR);
    this->scale_Y = qreal(this->height()) / qreal(HAUTEUR);
    if(this->scale_X < this->scale_Y)
        this->scale_valeur = this->scale_X;
    else
        this->scale_valeur = this->scale_Y;
//    qDebug() << "taille actuelle: " << this->size();
//    qDebug() << "scale x: " << this->scale_X;
//    qDebug() << "scale y: " << this->scale_Y;

    if(this->scale_valeur > 1.0)
        this->painter.setTransform(QTransform::fromScale(this->scale_valeur, this->scale_valeur), false);

    this->painter.setPen(Qt::NoPen);
    this->painter.setBrush(this->texture_bois);
    this->painter.drawRect(this->table_cote);
    this->painter.drawPolygon(this->table_dessus);

    this->painter.save();

    this->painter.translate(this->attache_bras.x() + 7, this->attache_bras.y() + 15);
    this->painter.rotate(this->angle_bras);
    this->painter.setBrush(Qt::gray);
    this->painter.drawRect(this->bras);
    this->painter.drawPolygon(this->helice);

    this->painter.setBrush(Qt::green);
    this->redessine_la_fleche(this->puissance);
    this->painter.drawPolygon(this->fleche, Qt::WindingFill); // Qt::WindingFill INDISPENSABLE POUR BIEN DESSINER LA FLECHE

    this->painter.restore();

    this->painter.setBrush(this->texture_alu_noir);
    this->painter.drawRect(this->poteau);
    this->painter.drawRect(this->attache_bras);

    this->painter.end();
}

void model_draw::redessine_la_fleche(const qreal puissance)
{
    this->fleche.clear(); // 273, 335
//    this->fleche << QPointF(295.0 + puissance, 504.0) << QPointF(307.0 + puissance, 516.0) << QPointF(295.0 + puissance, 528.0) << QPointF(295.0 + puissance, 522.0) << QPointF(295.0, 522.0) << QPointF(295.0, 510.0) << QPointF(295.0 + puissance, 510.0);
    this->fleche << QPointF(18.0 + puissance, 169.0) << QPointF(30.0 + puissance, 181.0) << QPointF(18.0 + puissance, 193.0) << QPointF(18.0 + puissance, 187.0) << QPointF(18.0, 187.0) << QPointF(18.0, 175.0) << QPointF(18.0 + puissance, 175.0);
}
