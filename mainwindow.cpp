#include <QtWidgets>
#include <QDebug>
#include "fruit.h"
#include "knife.h"
#include "mainwindow.h"

FruitView::FruitView()
    : gameScene(new GameScene)
{
    
    resize(SCREEN_WIDTH, SCREEN_HEIGHT);
    setScene(gameScene);
    gameScene->Start();
}

FruitView::~FruitView()
{
    delete gameScene;
}

FruitScene::FruitScene()
    : fruitKnife(new Knife())
{
    fruitKnife->ShowKnife(this);
    setSceneRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

FruitScene::~FruitScene()
{
    delete fruitKnife;
}

GameScene::GameScene()
    : apple(new AppleScoreFruit[5]),
      banana(new BananaScoreFruit[5]),
      peach(new PeachScoreFruit[5]),
      strawberry(new StrawberryScoreFruit[5]),
      background(new QGraphicsPixmapItem(QPixmap(":/images/background.jpg"))),
      score(0),
      timer(new QTimer)
{
    background->setZValue(-1);
    addItem(background);
    for (int i = 0; i < 5; i ++)
    {
        (apple+i)->ShowFruit(this);
        (apple+i)->SetNumber(4*i+0);
        freeFruit.append(4*i+0);

        (banana+i)->ShowFruit(this);
        (banana+i)->SetNumber(4*i+1);
        freeFruit.append(4*i+1);

        (peach+i)->ShowFruit(this);
        (peach+i)->SetNumber(4*i+2);
        freeFruit.append(4*i+2);

        (strawberry+i)->ShowFruit(this);
        (strawberry+i)->SetNumber(4*i+3);
        freeFruit.append(4*i+3);

        connect(apple+i, SIGNAL(AddScore(int)), this, SLOT(ScoreIncrease(int)));
        connect(apple+i, SIGNAL(FruitStop(int)), this, SLOT(FruitReady(int)));

        connect(banana+i, SIGNAL(AddScore(int)), this, SLOT(ScoreIncrease(int)));
        connect(banana+i, SIGNAL(FruitStop(int)), this, SLOT(FruitReady(int)));

        connect(peach+i, SIGNAL(AddScore(int)), this, SLOT(ScoreIncrease(int)));
        connect(peach+i, SIGNAL(FruitStop(int)), this, SLOT(FruitReady(int)));

        connect(strawberry+i, SIGNAL(AddScore(int)), this, SLOT(ScoreIncrease(int)));
        connect(strawberry+i, SIGNAL(FruitStop(int)), this, SLOT(FruitReady(int)));
    }

    connect(timer, SIGNAL(timeout()), this, SLOT(CastFreeFruit()));
}

GameScene::~GameScene()
{
    delete [] apple;
    delete [] banana;
    delete [] peach;
    delete [] strawberry;
    delete background;
    delete timer;
}

void GameScene::FruitReady(int num)
{
    freeFruit.append(num);
}


void GameScene::ScoreIncrease(int addScore)
{
    score += addScore;
}

void GameScene::Start()
{
    qsrand(time(NULL));

    timer->start(2000);
}

void GameScene::CastFreeFruit()
{
    int count = freeFruit.size();
    for (int i = 0; i < 4; i ++)
    {
        int num = qrand() % count;
        qDebug() << "num:" << num;
        int type = freeFruit[num] % 4;
        qDebug() << "type" << type;
        int index = freeFruit[num] / 4;
        switch(type)
        {
            case 0:
                (apple+index)->FruitStart();
            break;

            case 1:
                (banana+index)->FruitStart();
            break;

            case 2:
                (peach+index)->FruitStart();
            break;

            case 3:
                (strawberry+index)->FruitStart();
            break;

            default:
            break;
        }
        freeFruit.removeAt(num);
        count --;
    }
}