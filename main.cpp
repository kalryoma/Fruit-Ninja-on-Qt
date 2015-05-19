#include "fruit.h"
#include "animation.h"
#include "knife.h"
#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    FruitView* view = new FruitView();

    view->show(); 
    return app.exec();

}