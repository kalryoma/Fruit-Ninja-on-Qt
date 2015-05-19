#ifndef ANIMATION_H
#define ANIMATION_H

#include <QPropertyAnimation>
#include <QPointF>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int STOP_Y = 750;

class PosAngle
{
public:
    PosAngle();
    PosAngle(const PosAngle& other);
    ~PosAngle();

    PosAngle(qreal x, qreal y, qreal angle);

    qreal GetX() const;
    qreal GetY() const;
    qreal GetRotation() const;
    void SetX(qreal x);
    void SetY(qreal y);
    void SetRotation(qreal angle);

private:
    qreal x;
    qreal y;
    qreal angle;
};

Q_DECLARE_METATYPE(PosAngle);

/**
 * @brief use to let fruit run a parabolic animation
 **/

class ParabolicAnimation : public QPropertyAnimation
{
    Q_OBJECT
public:
    ParabolicAnimation();
    ParabolicAnimation(QObject* target);
    ~ParabolicAnimation();

    void SetKeyPoint(qreal keyX, qreal keyY, qreal keyAngle);
    void SetHighPoint(qreal highX, qreal highY, qreal highAngle);
    void SetRandomPath(qreal angle);

public slots:
    void ComputePathAndStart(qreal step = 0);

protected:
    QVariant interpolated(const QVariant& from, const QVariant& to,
        qreal progress) const;

private:
    QEasingCurve* curve;
    PosAngle keyPoint;
    PosAngle highPoint;
};

#endif