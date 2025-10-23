#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLineEdit>
#include "backend.h"

class HeroGraphics;

class RedDragonGraphics : public QGraphicsPixmapItem, public RedDragon {
public:
    RedDragonGraphics(QGraphicsItem* parent = nullptr);

    void drawHealthBar(QGraphicsScene*);
    void attack(HeroGraphics&, QGraphicsScene*);
    void printQue(QGraphicsScene*, QFont);
    void addTextFieldToScene(QGraphicsScene*, QFont);

public slots:
    void checkAnswer(QGraphicsScene*, QLineEdit*, HeroGraphics&, bool&);

protected:
    QGraphicsRectItem* hpBarFront = nullptr;
    QGraphicsRectItem* hpBarBack = nullptr;
};



class HeroGraphics : public QGraphicsPixmapItem, public Hero {
public:
    HeroGraphics(std::string, int, int);
    void drawHealthBar(QGraphicsScene*);
    void attack(RedDragonGraphics&, QGraphicsScene*);
    //void attack(GreenDragonGraphics&, QGraphicsScene*);
    //void attack(BlackDragonGraphics&, QGraphicsScene*);
private:
    QGraphicsRectItem* hpBar = nullptr;

};


#endif // GRAPHICS_H
