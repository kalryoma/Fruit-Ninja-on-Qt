#include "knife.h"
#include <QtWidgets>
#include <cmath>


Knife::Knife()
    : timer(new QTimer(this)),
      flash(new QPixmap(":/images/flash.png")),
      cutFlag(false)
{
    connect(timer, SIGNAL(timeout()), this, SLOT(KnifeDisappear()));
    timer->start(20);
}

Knife::~Knife()
{
    delete timer;
    delete flash;
}

void Knife::ComputeKnifeShape()
{
    //todo
}

void Knife::ShowKnife(QGraphicsScene* scene)
{
    scene->addItem(this);
    this->setZValue(1);
}

void Knife::ShowCutFlash()
{
    cutFlag = true;

    update();
}

void Knife::KnifeDisappear()
{
    if (knifeTrack.size() > 0)
    {
        knifeTrack.dequeue();
        update();
    }
}

void Knife::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QPointF pos = event->scenePos();
    while (knifeTrack.size() < 16)
    {
        knifeTrack.enqueue(pos);
    }
}

void Knife::mouseReleaseEvent(QGraphicsSceneMouseEvent* /*event*/)
{
    while (knifeTrack.size() > 0)
    {
        knifeTrack.dequeue();
    }
}

void Knife::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        QPointF pos = event->scenePos();
        while (knifeTrack.size() < 16)
        {
            knifeTrack.enqueue(pos);
        }
        while (knifeTrack.size() > 15)
        {
            knifeTrack.dequeue();
        }
    }
}


void Knife::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (knifeTrack.size() < 2)
    {
        return;
    }

    painter->setRenderHint(QPainter::Antialiasing);

    QPen outPen(Qt::black, 12, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
    QPen inPen(Qt::white, 8, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);

    qreal width = 0;
    qreal unitWidth = 12.0 / knifeTrack.size();

    QQueue<QPointF>::iterator i;
    for (i = knifeTrack.begin(); i != knifeTrack.end() - 1; i ++)
    {
        outPen.setWidth(width+2);
        width += unitWidth;
        painter->setPen(outPen);
        painter->drawLine(*i, *(i+1));
    }
    width = 0;
    for (i = knifeTrack.begin(); i != knifeTrack.end() - 1; i ++)
    {
        inPen.setWidth(width);
        width += unitWidth;
        painter->setPen(inPen);
        painter->drawLine(*i, *(i+1));
    }

    if (cutFlag == true)
    {
        
        QPointF& lastPoint = *(i-1);
        QPointF vector = lastPoint - *(i-2);

        qreal angle = std::atan(vector.y() / vector.x()) * 180 / 3.14;
        painter->translate(-(lastPoint.x()), -(lastPoint.y()));
        painter->rotate(angle);
        painter->translate(lastPoint.x(), lastPoint.y());
        painter->drawPixmap(lastPoint-(flash->rect()).center(), *flash);
    }
    
}

QRectF Knife::boundingRect() const
{
    return QRectF(0, 0, 800, 600);
}