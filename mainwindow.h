#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QGraphicsTextItem>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QStringList>
#include <QVector>
#include <QFontDatabase>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QBrush>
#include <QColor>
#include <QPixmap>
#include <QScreen>
#include <QFont>
#include <QGraphicsDropShadowEffect>
#include <QLineEdit>
#include "backend.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum class GameState {Intro, Shop, Menu, Playing, Pause};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showIntroScene();
    void updateIntroAnimation();   // для анимации титров
    //void spawnDragon(int index, Hero* hero, QGraphicsRectItem* hpBarFrontHero, QGraphicsTextItem* questionText, QLineEdit* answerInput, QPushButton*);
    void showGameScene();
    void check(Dragon& rdrag, Hero& hero, QGraphicsTextItem* questionText, QLineEdit* answerInput, QGraphicsRectItem* hpBarFrontRd, QGraphicsRectItem* hpBarFrontHero, char op);

private:
    //Ui::MainWindow *ui;
    Hero* hero = nullptr;
    Dragon* drag = nullptr;
    QGraphicsPixmapItem* dragGraphics = nullptr;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QTimer *introTimer;
    QStringList introLines;
    QVector<QGraphicsTextItem*> activeLines;
    int currentLineIndex;
    int currentDragonIndex;
    int introIndex;
    QFont font;

    //void resethpBar(QGraphicsRectItem& hpBar, int delta, int x, int y);



};
#endif // MAINWINDOW_H
