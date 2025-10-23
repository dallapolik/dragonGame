#include "mainwindow.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QProperty>
#include <QPropertyAnimation>
#include <QEasingCurve>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    scene(new QGraphicsScene(this)),
    view(new QGraphicsView(scene, this)),
    introTimer(new QTimer(this)),
    currentLineIndex(0),
    currentDragonIndex(0),
    introIndex(0),
    font(QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/images/pixel.ttf")).at(0), 16)
{
    font.setStyleStrategy(QFont::NoAntialias);

    setCentralWidget(view);
    setGeometry(QGuiApplication::primaryScreen()->geometry());

    QRect screenSize = QGuiApplication::primaryScreen()->geometry();
    scene->setSceneRect(0, 0, screenSize.width(), screenSize.height());
    view->showFullScreen();

    connect(introTimer, &QTimer::timeout, this, &MainWindow::updateIntroAnimation);
    showIntroScene();
}

MainWindow::~MainWindow()
{
    //delete ui;
}


void MainWindow::showIntroScene() {
    scene->clear();

    scene->setBackgroundBrush(QBrush(QPixmap(":/images/gym.PNG")));

    introLines = {"Джимбро, тебя приветствует dallapolik", "Стероиды пагубно влияют на организм ",
                  "Человека, в особенности на его мозг", "Небезызвестный факт, что большинство качков глупые",
                  "Тебе не надоело быть подтверждением стереотипа?", "Пора что-то менять", "\n",
                  "Именно для таких, как ты, я созвал", "В этих локациях умнейших животных этого мира",
                  "На твоем пути могут встретиться арифметические", "Драконы, тролли, и другие злобные твари",
                  "Ты спросишь, можно ли победить их твоей", "Грудой мышц, ответ - нет….",
                  "Их можно атаковать только силой твоей мысли", "Их броня ослабевает, как только ты отвечаешь",
                  "На их вопрос, но будь осторожен, при неверном ответе,", "Твоя броня тоже пропадает на время, что",
                  "Делает тебя уязвимым для врагов", "\n", "Если сможешь победить их всех, станешь умнее, если нет….",
                  " ", "Желаю удачи ;)"};
    currentLineIndex = 0;
    activeLines.clear();

    introTimer->start(2);
}

void MainWindow::updateIntroAnimation() {

    static int frameCounter = 0;
    frameCounter++;

    if (frameCounter % 40 == 0 && currentLineIndex < introLines.size()) {
        auto *line = scene->addText(introLines[currentLineIndex], font);
        line->setDefaultTextColor(QColor(180, 50, 255));
        QGraphicsDropShadowEffect* glow = new QGraphicsDropShadowEffect();
        glow->setBlurRadius(30);

        glow->setColor(QColor(180, 0, 255));
        glow->setOffset(0, 0);
        line->setGraphicsEffect(glow);

        line->setPos((scene->sceneRect().width() - line->boundingRect().width()) / 2, scene->sceneRect().height());
        activeLines.push_back(line);
        currentLineIndex++;
    }
    for (auto* line : activeLines) {
        line->setY(line->y() - 2);
    }

    for (int i = 0; i < activeLines.size(); i++) {
        if (activeLines[i]->y() < -50) {
            scene->removeItem(activeLines[i]);
            delete activeLines[i];
            activeLines.remove(i);
            i--;
        }
    }


    if (currentLineIndex == introLines.size() && activeLines.isEmpty()) {
        introTimer->stop();

        /*QPushButton *nextButton = new QPushButton("Начать");
        nextButton->setFixedSize(800, 600);
        nextButton->setStyleSheet(
            "QPushButton {"
            "    border: none;"
            "    background-image: url(:/images/button.PNG);"
            "    background-repeat: no-repeat;"
            "    background-position: center;"
            "    color: black;"             // цвет текста
            "    font: bold 160px 'Press Start 2P';" // пиксельный шрифт
            "}"
            );
        QGraphicsProxyWidget *proxy = scene->addWidget(nextButton);
        proxy->setScale(0.15);
        proxy->setPos(scene->sceneRect().width() / 2 - 50, scene->sceneRect().height()/2);
        connect(nextButton, &QPushButton::clicked, this, [this]() {
            QTimer::singleShot(0, this, [this]() {
                this->showGameScene();
            });
        });*/


        showGameScene();



    }
}

void resethpBar(QGraphicsRectItem& hpBar, int delta, int x, int y){
    int width = hpBar.rect().width();
    int height = hpBar.rect().height();

    hpBar.setRect(x, y, width - delta, height);
}





void MainWindow::showGameScene() {
    scene->disconnect();
    scene->clear();
    QCoreApplication::processEvents();

    char op;
    //Hero* hero = new Hero("Warrior", 20, 120);
    if (!hero) hero = new Hero("Warrior", 150, 120);

    //else {delete hero; hero = new Hero("Warrior", 150, 120);}

    if (drag) {delete drag; drag = nullptr;}// удалить старого
    if(dragGraphics) {scene->removeItem(this->dragGraphics); delete this->dragGraphics; this->dragGraphics = nullptr;}

    if (currentDragonIndex == 0){
        drag = new RedDragon();
        dragGraphics = new QGraphicsPixmapItem(QPixmap(":/images/RedDragon.PNG"));
        op = '+';
    }
    else if (currentDragonIndex == 1){
        drag = new GreenDragon();
        dragGraphics = new QGraphicsPixmapItem(QPixmap(":/images/GreenDragon.PNG"));
        op = '-';
    }
    else {
        drag = new BlackDragon();
        dragGraphics = new QGraphicsPixmapItem(QPixmap(":/images/BlackDragon.PNG"));
        op = '*';
    }



    QGraphicsPixmapItem* heroGraphics = new QGraphicsPixmapItem(QPixmap(":/images/Hero.png"));
    heroGraphics->setScale(0.7);
    heroGraphics->setPos(100, 300);
    scene->addItem(heroGraphics);

    QGraphicsRectItem* hpBarBackHero = new QGraphicsRectItem();
    hpBarBackHero->setRect(170, 235, 2 * hero->getHealth(), 20);
    hpBarBackHero->setBrush(Qt::red);
    scene->addItem(hpBarBackHero);

    QGraphicsRectItem* hpBarFrontHero = new QGraphicsRectItem();
    hpBarFrontHero->setRect(170, 235, 2 * hero->getMaxHealth(), 20);
    hpBarFrontHero->setBrush(Qt::green);
    scene->addItem(hpBarFrontHero);





    dragGraphics->setScale(0.7);
    dragGraphics->setPos(800, 220);
    scene->addItem(dragGraphics);

    QGraphicsRectItem* hpBarBackRd = new QGraphicsRectItem();
    hpBarBackRd->setRect(920, 175, 2 * drag->getHealth(), 20);
    hpBarBackRd->setBrush(Qt::red);
    scene->addItem(hpBarBackRd);

    QGraphicsRectItem* hpBarFrontRd = new QGraphicsRectItem();
    hpBarFrontRd->setRect(920, 175, 2 * drag->getMaxHealth(), 20);
    hpBarFrontRd->setBrush(Qt::green);
    scene->addItem(hpBarFrontRd);




    // текст вопроса
    QGraphicsTextItem* questionText = new QGraphicsTextItem(QString::fromStdString(drag->question(op)));
    questionText->setFont(font);
    questionText->setDefaultTextColor(Qt::darkMagenta);
    questionText->setPos(500, 100);
    scene->addItem(questionText);

    // ввод ответа
    QLineEdit* answerInput = new QLineEdit();
    QPushButton* checkButton = new QPushButton("Проверить ответ");
    QGraphicsProxyWidget* inputProxy = scene->addWidget(answerInput);
    QGraphicsProxyWidget* buttonProxy = scene->addWidget(checkButton);
    inputProxy->setPos(500, 200);
    buttonProxy->setPos(700, 200);

    // соединение кнопки с логикой боя
    connect(checkButton, &QPushButton::clicked, this, [=]() mutable {
        this->check(*drag, *hero, questionText, answerInput, hpBarFrontRd, hpBarFrontHero, op);
    });



}


void MainWindow::check(Dragon& rdrag, Hero& hero,
                       QGraphicsTextItem* questionText, QLineEdit* answerInput,
                       QGraphicsRectItem* hpBarFrontRd, QGraphicsRectItem* hpBarFrontHero, char op)
{
    if (!rdrag.isAlive() || !hero.isAlive()) {
        questionText->setPlainText(rdrag.isAlive() ? "💀 Герой пал..." : "✅ Дракон побежден!");
        return;
    }

    bool ok;
    long long answer = answerInput->text().toLongLong(&ok);
    if (!ok) return;

    rdrag.setAnswer(answer);
    bool correct = rdrag.checkAnswer();

    // создаём снаряд
    QGraphicsPixmapItem* projectile;
    if(!correct) projectile = new QGraphicsPixmapItem(QPixmap(":/images/book.PNG"));
    else projectile = new QGraphicsPixmapItem(QPixmap(":/images/protein.PNG"));
    projectile->setScale(0.2);
    scene->addItem(projectile);

    qreal startX, startY = 300, endX;
    if (correct) { startX = 200; endX = 900; startY = 400;}   // герой атакует
    else { startX = 900; endX = 200; }           // дракон атакует

    projectile->setPos(startX, startY);

    // анимация через QVariantAnimation
    QVariantAnimation* anim = new QVariantAnimation();
    anim->setDuration(500);
    anim->setStartValue(startX);
    anim->setEndValue(endX);
    anim->setEasingCurve(QEasingCurve::InOutQuad);

    // обработка каждого кадра — двигаем снаряд
    connect(anim, &QVariantAnimation::valueChanged, this, [=](const QVariant &value) {
        qreal x = value.toReal();
        projectile->setPos(x, startY);
    });

    // при завершении полёта наносим урон
    connect(anim, &QVariantAnimation::finished, this, [=, &rdrag, &hero]() mutable {
        scene->removeItem(projectile);
        delete projectile;

        if (correct) {
            hero.attack(rdrag);
            double newWidth = std::max(0.0, hpBarFrontRd->rect().width() - hero.getAttack() * 2.0);
            hpBarFrontRd->setRect(hpBarFrontRd->rect().x(), hpBarFrontRd->rect().y(), newWidth, hpBarFrontRd->rect().height());
            questionText->setPlainText("🎯 Верно! Дракон получает урон!");
        } else {
            rdrag.attack(hero);
            double newWidth = std::max(0.0, hpBarFrontHero->rect().width() - rdrag.getAttack() * 2.0);
            hpBarFrontHero->setRect(hpBarFrontHero->rect().x(), hpBarFrontHero->rect().y(), newWidth, hpBarFrontHero->rect().height());
            questionText->setPlainText("🔥 Неверно! Дракон атакует тебя!");
        }

        // конец боя
        if (!rdrag.isAlive() || !hero.isAlive()) {
            if (!rdrag.isAlive()) {

                questionText->setPlainText("✅ Дракон побежден!");

                if (dragGraphics) {
                    scene->removeItem(dragGraphics);
                    delete dragGraphics;
                    dragGraphics = nullptr;
                }

                QTimer::singleShot(1000, this, [=]() mutable {
                    currentDragonIndex++;
                    if (currentDragonIndex < 3) {
                        this->showGameScene();
                    } else {
                        //questionText->setPlainText("🎉 Все драконы повержены! Ты победил!");
                        scene->clear();


                        QPushButton* restartButton = new QPushButton("Ещё раз");
                        QGraphicsProxyWidget* buttonProxy = scene->addWidget(restartButton);
                        buttonProxy->setPos(700, 700);

                        QGraphicsTextItem* questionText = new QGraphicsTextItem(QString::fromStdString("Репутация качков восстановлена, благодарим тебя.\n Драконы не против посоревноваться вновь!"));
                        questionText->setFont(font);
                        questionText->setDefaultTextColor(Qt::darkMagenta);
                        questionText->setPos(10, 100);
                        scene->addItem(questionText);

                        connect(restartButton, &QPushButton::clicked, this, [this, restartButton]() {
                            restartButton->deleteLater();

                            currentDragonIndex = 0;
                            this->hero = nullptr;
                            this->drag = nullptr;
                            this->dragGraphics = nullptr;

                            // 👇 немного подождать, чтобы сцена успела очиститься
                            QTimer::singleShot(0, this, [this]() {
                                showIntroScene();
                            });
                        });
                    }
                });
            } else {
                //questionText->setPlainText("💀 Герой пал...");
                scene->clear();


                QPushButton* restartButton = new QPushButton("Ещё раз");
                QGraphicsProxyWidget* buttonProxy = scene->addWidget(restartButton);
                buttonProxy->setPos(700, 700);

                QGraphicsTextItem* questionText = new QGraphicsTextItem(QString::fromStdString("Ты пока что остался тупым качком, но не волнуйся, я дам тебе еще шанс!\n Не подведи братство качков"));
                questionText->setFont(font);
                questionText->setDefaultTextColor(Qt::darkMagenta);
                questionText->setPos(10, 100);
                scene->addItem(questionText);

                connect(restartButton, &QPushButton::clicked, this, [this, restartButton]() {
                    restartButton->deleteLater();

                    currentDragonIndex = 0;
                    this->hero = nullptr;
                    this->drag = nullptr;
                    this->dragGraphics = nullptr;

                    // 👇 немного подождать, чтобы сцена успела очиститься
                    QTimer::singleShot(0, this, [this]() {
                        showIntroScene();
                    });
                });
            }




            return;
        }

        answerInput->clear();

        questionText->setPlainText(QString::fromStdString(rdrag.question(op)));
        //questionText->setPlainText(QString::fromStdString());

    });

    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
