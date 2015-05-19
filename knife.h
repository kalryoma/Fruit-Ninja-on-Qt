#ifndef KNIFE_H
#define KNIFE_H

#include <QGraphicsItem>
#include <QObject>
#include <QQueue>

class QGraphicsScene;
class QTimer;

/**
 * @brief use to show knife in screen
 **/
class Knife : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Knife();
    virtual ~Knife();

    void ComputeKnifeShape();
    virtual void ShowKnife(QGraphicsScene* scene);

public slots:
    void KnifeDisappear();
    void ShowCutFlash();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void paint(QPainter *painter, 
        const QStyleOptionGraphicsItem *option, 
        QWidget *widget = 0);    
    QRectF boundingRect() const;

private:
    QQueue<QPointF> knifeTrack;
    QQueue<QPointF> knifeBlade;
    QTimer* timer;
    QPixmap* flash;
    bool cutFlag;
};

#endif