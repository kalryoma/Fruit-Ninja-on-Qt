#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsView>
#include <QGraphicsScene>

class QGraphicsScene;
class ScoreFruit;
class AppleScoreFruit;
class BananaScoreFruit;
class PeachScoreFruit;
class StrawberryScoreFruit;
class Knife;
class QPoinF;

class FruitScene : public QGraphicsScene
{
    Q_OBJECT
public:
    FruitScene();
    virtual ~FruitScene();

public slots:

    virtual void Start() = 0;
protected:
private:
    Knife* fruitKnife;
};

class GameScene : public FruitScene
{
    Q_OBJECT
public:
    GameScene();
    ~GameScene();

public slots:
    void FruitReady(int fruitNum);
    void ScoreIncrease(int addScore);
    void Start();
    void CastFreeFruit();

private:
    AppleScoreFruit* apple;
    BananaScoreFruit* banana;
    PeachScoreFruit* peach;
    StrawberryScoreFruit* strawberry;
    QGraphicsPixmapItem* background;
    int score;
    QList<int> freeFruit;
    QTimer* timer;
};


class FruitView : public QGraphicsView
{
public:
    FruitView();
    ~FruitView();
protected:
private:
    FruitScene* gameScene;
};

#endif