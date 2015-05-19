#ifndef FRUIT_H
#define FRUIT_H

#include <QGraphicsPixmapItem>
#include "animation.h"

class QGraphicsScene;


/**
 * @brief This class is used to show fruit in screen
 **/
class FruitItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(PosAngle posAngle READ GetPosAngle WRITE SetPosAngle)
public:
    FruitItem();
    FruitItem(const QPixmap& pixmap);
    ~FruitItem();

    /**
     * @brief use to get position and angle
     **/
    PosAngle GetPosAngle() const;
    /**
     * @brief use to set position and angle
     **/
    void SetPosAngle(PosAngle posAngle);
    /**
     * @brief use to state it's whole fruit or not
     **/
    void SetFruitState(bool isWholeFruit);

signals:
    void CutFruitSignal(qreal angle);
    
protected:
    /**
     * @brief if mouse is in fruit, emit a signal 
     **/
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);

private:
    bool isWholeFruit;
};

/**
 * @brief This class is used to create an abstract fruit class
 **/
class AbstractFruit : public QObject
{
    Q_OBJECT
public:
    AbstractFruit();
    virtual ~AbstractFruit();

    /**
     * @brief get an item of fruit 
     **/
    FruitItem* GetWholeFruit();
    FruitItem* GetFirstCuttingFruit();
    FruitItem* GetSecondCuttingFruit();
    QGraphicsPixmapItem* GetFlash();

    /**
     * @brief show fruit in screen
     **/
    virtual void ShowFruit(QGraphicsScene* scene) = 0;

public slots:
    /**
     * @brief use to show when fruit is cut
     **/
    virtual void CutFruitAnimation(qreal angle);
    /**
     * @brief start fouction when fruit is cut
     **/
    virtual void CutFunction();
    void HideFlash();

protected:

    /**
     * @brief set fruit picture
     **/
    void SetWholeFruit(const QPixmap& fruit);
    void SetCuttingFruit(const QPixmap& firstFruit,
        const QPixmap& secondFruit);
    /**
     * @brief set initial fruit cutting angle
     **/
    void SetCutAngle(const qreal angle);

private:
    FruitItem* wholeFruit;
    FruitItem* firstCuttingFruit;
    FruitItem* secondCuttingFruit;
    QGraphicsPixmapItem* flash;
    ParabolicAnimation* firstCutFruitDown;
    ParabolicAnimation* secondCutFruitDown;
    qreal cutAngle;
};

/**
 * @brief This class is used to create an score fruit class
 **/

class ScoreFruit : public AbstractFruit
{
    Q_OBJECT
public:
    ScoreFruit();
    virtual ~ScoreFruit();

    /**
     * @brief override function
     **/
    void ShowFruit(QGraphicsScene* scene);
    void SetNumber(int num);
    void SetRotationAngle(qreal angle);

public slots:
    virtual void CutFunction();
    /**
     * @brief function starts when fruit stop
     **/
    void FinishedFunction();
    void FruitStart();

signals:
    void AddScore(int addScore);
    void FruitStop(int num);

private:
    ParabolicAnimation* castFruit;
    qreal rotationAngle;
    int fruitNum;
};

class AppleScoreFruit : public ScoreFruit
{
    Q_OBJECT
public:
    AppleScoreFruit();

};

class BananaScoreFruit : public ScoreFruit
{
    Q_OBJECT
public:
    BananaScoreFruit();
};

class PeachScoreFruit : public ScoreFruit
{
    Q_OBJECT
public:
    PeachScoreFruit();
};

class StrawberryScoreFruit : public ScoreFruit
{
    Q_OBJECT
public:
    StrawberryScoreFruit();
};




#endif