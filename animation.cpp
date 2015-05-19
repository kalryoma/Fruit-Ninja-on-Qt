#include "animation.h"
#include <QDebug>
#include <cmath>

PosAngle::PosAngle() : x(0), y(0), angle(0)
{

}

PosAngle::PosAngle(const PosAngle& other)
{
    x = other.GetX();
    y = other.GetY();
    angle = other.GetRotation();
}

PosAngle::~PosAngle()
{

}

PosAngle::PosAngle(qreal x, qreal y, qreal angle) : x(x), y(y), angle(angle)
{

}

qreal PosAngle::GetX() const
{
    return x;
}

qreal PosAngle::GetY() const
{
    return y;
}

qreal PosAngle::GetRotation() const
{
    return angle;
}

void PosAngle::SetX(qreal x)
{
    this->x = x;
}

void PosAngle::SetY(qreal y)
{
    this->y = y;
}

void PosAngle::SetRotation(qreal angle)
{
    this->angle = angle;
}

ParabolicAnimation::ParabolicAnimation()
    : QPropertyAnimation(NULL, "posAngle"),
      curve(new QEasingCurve(QEasingCurve::OutInQuad)),
      keyPoint(0, 0, 0),
      highPoint(0, 0, 0)
{

}

ParabolicAnimation::ParabolicAnimation(QObject* target)
    : QPropertyAnimation(target, "posAngle"),
      curve(new QEasingCurve(QEasingCurve::OutInQuad)),
      keyPoint(0, 0, 0),
      highPoint(0, 0, 0)
{

}

ParabolicAnimation::~ParabolicAnimation()
{
    delete curve;
}

void ParabolicAnimation::SetKeyPoint(qreal keyX, qreal keyY, qreal keyAngle)
{
    keyPoint.SetX(keyX);
    keyPoint.SetY(keyY);
    keyPoint.SetRotation(keyAngle);
}

void ParabolicAnimation::SetHighPoint(qreal highX, qreal highY, qreal highAngle)
{
    highPoint.SetX(highX);
    highPoint.SetY(highY);
    highPoint.SetRotation(highAngle);
}

void ParabolicAnimation::SetRandomPath(qreal angle)
{
    qreal beginX = qrand() % SCREEN_WIDTH;
    qreal endX = qrand() % SCREEN_WIDTH;
    qreal highY = qrand() % (SCREEN_HEIGHT / 3);

    keyPoint.SetX(beginX);
    keyPoint.SetY(STOP_Y);
    keyPoint.SetRotation(0);
    highPoint.SetX(beginX + (endX - beginX) / 2);
    highPoint.SetY(highY);
    highPoint.SetRotation(angle);
}

void ParabolicAnimation::ComputePathAndStart(qreal step)
{
    int animationTime = static_cast<int>(std::sqrt(STOP_Y - highPoint.GetY()) * 150);
    setDuration(animationTime);
    qreal keyX = keyPoint.GetX();
    qreal keyY = keyPoint.GetY();
    qreal highX = highPoint.GetX();
    qreal highY = highPoint.GetY();
    qreal keyAngle = keyPoint.GetRotation();
    qreal highAngle = highPoint.GetRotation();
    qreal factor = std::sqrt((STOP_Y-highY) / (keyY-highY));

    qreal beginX = highX - (highX - keyX) * factor;
    qreal beginY = STOP_Y;
    qreal beginAngle = highAngle - (highAngle - keyAngle) * factor;
    setStartValue(QVariant::fromValue(PosAngle(beginX, beginY, beginAngle)));
    setEndValue(QVariant::fromValue(highPoint));
    start();
    setCurrentTime(static_cast<int>(animationTime * step));
}

QVariant ParabolicAnimation::interpolated(const QVariant& from, const QVariant& to, qreal progress) const
{
    PosAngle f = from.value<PosAngle>();
    PosAngle t = to.value<PosAngle>();

    qreal value;
    if (progress < 0.5)
    {
        value = 2 * curve->valueForProgress(progress);
    }
    else
    {
        value = 2 * (1 - curve->valueForProgress(progress)); 
    }
    qreal x = f.GetX() + (t.GetX() - f.GetX()) * progress;
    qreal y = f.GetY() + (t.GetY() - f.GetY()) * value;
    qreal angle = f.GetRotation() + (t.GetRotation() - f.GetRotation()) * progress;

    return QVariant::fromValue(PosAngle(x, y, angle));
}
