#include "fruit.h"
#include <QtWidgets>
#include <cmath>
#include <QDebug>


FruitItem::FruitItem()
    : isWholeFruit(true)
{
    QRectF bounding = boundingRect();
    setTransformOriginPoint(bounding.center());
    setAcceptHoverEvents(isWholeFruit);

}

FruitItem::FruitItem(const QPixmap& pixmap)
    : QGraphicsPixmapItem(pixmap),
      isWholeFruit(true)
{
    QRectF bounding = boundingRect();
    setTransformOriginPoint(bounding.center());
    setAcceptHoverEvents(isWholeFruit);
}

FruitItem::~FruitItem()
{

}


PosAngle FruitItem::GetPosAngle() const
{
    return PosAngle(x(), y(), rotation());
}


void FruitItem::SetPosAngle(PosAngle posAngle)
{
    setPos(posAngle.GetX(), posAngle.GetY());
    setRotation(posAngle.GetRotation());
}

void FruitItem::SetFruitState(bool isWholeFruit)
{
    this->isWholeFruit = isWholeFruit;
    setAcceptHoverEvents(isWholeFruit);
}

void FruitItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    if (!isWholeFruit)
    {
        return;
    }
    else
    {
        qDebug() << "emit";
        qreal angle;
        QPointF vector = event->scenePos() - event->lastScenePos();
        angle = std::atan(vector.y() / vector.x()) * 180 / 3.14;
        emit CutFruitSignal(angle);
    }
}

AbstractFruit::AbstractFruit()
    : wholeFruit(new FruitItem(QPixmap(":/images/fruit/apple.png"))),
      firstCuttingFruit(new FruitItem(QPixmap(":/images/fruit/apple-1.png"))),
      secondCuttingFruit(new FruitItem(QPixmap(":/images/fruit/apple-2.png"))),
      flash(new QGraphicsPixmapItem(QPixmap(":/images/flash.png"))),
      firstCutFruitDown(new ParabolicAnimation(firstCuttingFruit)),
      secondCutFruitDown(new ParabolicAnimation(secondCuttingFruit)),
      cutAngle(0)
{
    firstCuttingFruit->SetFruitState(false);
    secondCuttingFruit->SetFruitState(false);
    connect(wholeFruit, SIGNAL(CutFruitSignal(qreal)), this, SLOT(CutFruitAnimation(qreal)));
    connect(wholeFruit, SIGNAL(CutFruitSignal(qreal)), this, SLOT(CutFunction()));
}

AbstractFruit::~AbstractFruit()
{
    delete wholeFruit;
    delete firstCuttingFruit;
    delete secondCuttingFruit;
    delete flash;
    delete firstCutFruitDown;
    delete secondCutFruitDown;
}

FruitItem* AbstractFruit::GetWholeFruit()
{
    return wholeFruit;
}

FruitItem* AbstractFruit::GetFirstCuttingFruit()
{
    return firstCuttingFruit;
}

FruitItem* AbstractFruit::GetSecondCuttingFruit()
{
    return secondCuttingFruit;
}

QGraphicsPixmapItem* AbstractFruit::GetFlash()
{
    return flash;
}

/**
 *@brief 
 */
void AbstractFruit::CutFruitAnimation(qreal angle)
{
    qreal x = wholeFruit->GetPosAngle().GetX();
    qreal y = wholeFruit->GetPosAngle().GetY();
    QPointF flashCenter = (flash->boundingRect()).center();
    flash->setTransformOriginPoint(flashCenter);
    flash->setRotation(angle);
    flash->setPos(QPointF(x, y)-flashCenter);
    flash->show();
    QTimer::singleShot(50, this, SLOT(HideFlash()));

    wholeFruit->hide();
    firstCuttingFruit->show();
    secondCuttingFruit->show();
    
    qreal fruitAngle = angle - cutAngle;
    qreal offset = 30;
    firstCutFruitDown->SetHighPoint(x-2*offset, y, fruitAngle);
    firstCutFruitDown->SetKeyPoint(x+offset, STOP_Y, fruitAngle);
    secondCutFruitDown->SetHighPoint(x+2*offset, y, fruitAngle);
    secondCutFruitDown->SetKeyPoint(x-offset, STOP_Y, fruitAngle);

    firstCutFruitDown->ComputePathAndStart(0.5);
    secondCutFruitDown->ComputePathAndStart(0.5);
}

void AbstractFruit::CutFunction()
{
    return;
}

void AbstractFruit::HideFlash()
{
    flash->hide();
}

void AbstractFruit::SetWholeFruit(const QPixmap& fruit)
{
    wholeFruit->setPixmap(fruit);
}

void AbstractFruit::SetCuttingFruit(const QPixmap& firstFruit, const QPixmap& secondFruit)
{
    firstCuttingFruit->setPixmap(firstFruit);
    secondCuttingFruit->setPixmap(secondFruit);
}

void AbstractFruit::SetCutAngle(const qreal angle)
{
    cutAngle = angle;
}

ScoreFruit::ScoreFruit()
    : castFruit(new ParabolicAnimation(GetWholeFruit())),
      rotationAngle(0),
      fruitNum(0)
{
    castFruit->SetRandomPath(rotationAngle);
    connect(castFruit, SIGNAL(finished()), this, SLOT(FinishedFunction()));
}

ScoreFruit::~ScoreFruit()
{
    delete castFruit;
}

void ScoreFruit::ShowFruit(QGraphicsScene* scene)
{
    scene->addItem(GetWholeFruit());
    scene->addItem(GetFirstCuttingFruit());
    scene->addItem(GetSecondCuttingFruit());
    scene->addItem(GetFlash());

    GetFlash()->hide();
    GetFirstCuttingFruit()->hide();
    GetSecondCuttingFruit()->hide();
}


void ScoreFruit::SetNumber(int num)
{
    fruitNum = num;
}

void ScoreFruit::SetRotationAngle(qreal angle)
{
    rotationAngle = angle;
}

void ScoreFruit::CutFunction()
{
    emit AddScore(1);
}

void ScoreFruit::FinishedFunction()
{
    qDebug() << "fruitstop";
    emit FruitStop(fruitNum);
}

void ScoreFruit::FruitStart()
{
    castFruit->SetRandomPath(rotationAngle);
    GetFirstCuttingFruit()->hide();
    GetSecondCuttingFruit()->hide();
    GetWholeFruit()->show();
    castFruit->ComputePathAndStart();

}


AppleScoreFruit::AppleScoreFruit()
{
    SetWholeFruit(QPixmap(":/images/fruit/apple.png"));
    SetCuttingFruit(QPixmap(":/images/fruit/apple-1.png"), 
        QPixmap(":/images/fruit/apple-2.png"));
    SetCutAngle(60);
    SetRotationAngle(720);
}

BananaScoreFruit::BananaScoreFruit()
{
    SetWholeFruit(QPixmap(":/images/fruit/banana.png"));
    SetCuttingFruit(QPixmap(":/images/fruit/banana-1.png"), 
        QPixmap(":/images/fruit/banana-2.png"));
    SetCutAngle(0);
    SetRotationAngle(0);

}

PeachScoreFruit::PeachScoreFruit()
{
    SetWholeFruit(QPixmap(":/images/fruit/peach.png"));
    SetCuttingFruit(QPixmap(":/images/fruit/peach-1.png"),
        QPixmap(":/images/fruit/peach-2.png"));
    SetCutAngle(60);
    SetRotationAngle(360);
}

StrawberryScoreFruit::StrawberryScoreFruit()
{
    SetWholeFruit(QPixmap(":/images/fruit/strawberry.png"));
    SetCuttingFruit(QPixmap(":/images/fruit/strawberry-1.png"),
        QPixmap(":/images/fruit/strawberry-2.png"));
    SetCutAngle(120);
    SetRotationAngle(360);
}
