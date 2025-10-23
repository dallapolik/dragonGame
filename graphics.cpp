#include "graphics.h"
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <QGraphicsProxyWidget>
#include <QMessageBox>
#include <QProperty>
#include <QPropertyAnimation>
#include <algorithm>


RedDragonGraphics::RedDragonGraphics(QGraphicsItem*) : hpBarFront(nullptr), hpBarBack(nullptr) {
    setPixmap(QPixmap(":/images/RedDragon.PNG"));

    hpBarFront = new QGraphicsRectItem();
    hpBarFront->setBrush(QBrush(Qt::green));
    hpBarBack = new QGraphicsRectItem();
    hpBarBack->setBrush(QBrush(Qt::red));
}


/*void RedDragonGraphics::attack(HeroGraphics& hero, QGraphicsScene* scene) {
    QGraphicsPixmapItem* projectile = new QGraphicsPixmapItem();
    projectile->setPixmap(QPixmap(":/images/book.PNG"));
    projectile->setScale(0.7);
    projectile->setPos(pos());
    scene->addItem(projectile);

    QTimer* moveTimer = new QTimer(); // Таймер без родителя
    QObject::connect(moveTimer, &QTimer::timeout, [&]() {
        // Двигаем снаряд вправо
        projectile->moveBy(10, -1);

        if (projectile->collidesWithItem(&hero)) {
            Gameunit::attack(hero);  // Предположим, что у дракона есть метод для получения урона
            scene->removeItem(projectile);  // Удаляем снаряд
            delete projectile;  // Освобождаем память
            moveTimer->stop();  // Останавливаем таймер
        }

        // Удаляем снаряд, если он вылетел за экран
        if (projectile->x() > pos().x() && projectile->y() > pos().y()) {
            scene->removeItem(projectile);
            delete projectile;
            moveTimer->stop();
        }
    });
    moveTimer->start(50);

}*/


void RedDragonGraphics::drawHealthBar(QGraphicsScene* scene){

    if (hpBarBack) {
        scene->removeItem(hpBarBack);  // Удаляем старую полосу здоровья
        delete hpBarBack;  // Очищаем память
    }

    if (hpBarFront) {
        scene->removeItem(hpBarFront);  // Удаляем старую полосу здоровья
        delete hpBarFront;  // Очищаем память
    }

    // Создаем новый прямоугольник для полосы здоровья
    hpBarBack = new QGraphicsRectItem(this);
    hpBarBack->setBrush(QBrush(Qt::green));  // Зеленая для здоровья
    //hpBar->setPen(QPen(Qt::black));

    hpBarBack = new QGraphicsRectItem(this);
    hpBarBack->setBrush(QBrush(Qt::red));

    // Устанавливаем позицию и размеры полосы здоровья
    hpBarFront->setRect(QPixmap(":/images/RedDragon.PNG").width() / 3 - 30, -40, getHealth() * 1.5, 20);  // Позиция над персонажем, ширина зависит от здоровья
    hpBarBack->setRect(QPixmap(":/images/RedDragon.PNG").width() / 3 - 30, -40, getMaxHealth() * 1.5, 20);  // Позиция над персонажем, ширина зависит от здоровья


    scene->addItem(hpBarBack);
    scene->addItem(hpBarFront);

}

void RedDragonGraphics::printQue(QGraphicsScene* scene, QFont font){
    que_ = new Expression('+');
    QString str = QString::fromStdString(que_->toStr());

    QGraphicsTextItem* txt = new QGraphicsTextItem(str);
    txt->setFont(font);
    txt->setDefaultTextColor(Qt::darkMagenta);
    scene->addItem(txt);
    txt->setPos(200, 200);
}


void RedDragonGraphics::addTextFieldToScene(QGraphicsScene* scene, QFont font) {
    // Создаем поле для ввода текста
    QLineEdit* textField = new QLineEdit();

    // Устанавливаем начальный текст (можно оставить пустым)
    textField->setPlaceholderText("Введите свой ответ...");
    font.setPointSize(10);
    textField->setFont(font);
    textField->setFixedSize(300, 40);

    QString userInput_ = textField->text();
    std::string userInput = userInput_.toStdString();


    // Добавляем поле на сцену
    QGraphicsProxyWidget* proxy = scene->addWidget(textField);

    proxy->setPos(200, 250);

    QPushButton* saveButton = new QPushButton("Сохранить");
    QGraphicsProxyWidget* saveProxy = scene->addWidget(saveButton);
    saveProxy->setPos(100, 300); // Позиция кнопки

    QAbstractButton::connect(saveButton, &QPushButton::clicked, [=]() {
        // Получаем текст из поля ввода
        std::string userInput_= textField->text().toStdString();
        setAnswer(stoi(userInput));
    });
}

void RedDragonGraphics::checkAnswer(QGraphicsScene* scene, QLineEdit* answerField, HeroGraphics& hero, bool& flag) {
    // Получение ответа пользователя
    std::string userAnswer = answerField->text().toStdString(); // answerField — это поле для ввода ответа
    setAnswer(stoi(userAnswer)); // Это твой метод для проверки ответа
    bool isAnswerCorrect = Dragon::checkAnswer();

    // Если ответ верный
    if (isAnswerCorrect) {
        // Наносим урон по дракону
        hero.HeroGraphics::attack(*this, scene);

        // Если здоровье дракона становится 0 или меньше, создаём нового дракона
        if (getHealth() <= 0) {
            scene->removeItem(this);  // Убираем старого дракона с экрана
            flag = 0;
        }
    } else {
        // Если ответ неверный, наносим урон по герою
        //attack(hero, scene);

        // Если здоровье героя становится 0 или меньше, показываем экран поражения
        if (hero.getHealth() <= 0) {
            //showGameOverScreen();  // Показываем экран поражения
        }
    }

    // Обновляем полосы здоровья для героя и дракона
    hero.HeroGraphics::drawHealthBar(scene);
    drawHealthBar(scene);

    // Очищаем поле ввода после проверки ответа
    answerField->clear();
}
































HeroGraphics::HeroGraphics(std::string name, int x, int y)
    : Hero(name, x, y), hpBar(nullptr) {
    setPixmap(QPixmap(":/images/Hero.png"));

    //setPos(100, 100);

}

void HeroGraphics::attack(RedDragonGraphics& dragon, QGraphicsScene* scene){
    QGraphicsPixmapItem* projectile = new QGraphicsPixmapItem();
    projectile->setPixmap(QPixmap(":/images/protein.PNG"));
    projectile->setPos(x(), y());
    scene->addItem(projectile);
    projectile->setScale(0.1);


    QTimer* moveTimer = new QTimer(); // Таймер без родителя
    QObject::connect(moveTimer, &QTimer::timeout, [&]() {
        // Двигаем снаряд вправо
        projectile->moveBy(10, -1);

        // Проверяем столкновение с драконом
        if (projectile->collidesWithItem(&dragon)) {
            // Уменьшаем здоровье дракона
            Gameunit::attack(dragon);  // Предположим, что у дракона есть метод для получения урона
            dragon.drawHealthBar(scene);
            scene->removeItem(projectile);  // Удаляем снаряд
            delete projectile;  // Освобождаем память
            moveTimer->stop();  // Останавливаем таймер
        }

        // Удаляем снаряд, если он вылетел за экран
        if (projectile->x() > scene->sceneRect().width()) {
            scene->removeItem(projectile);
            delete projectile;
            moveTimer->stop();
        }
    });
    moveTimer->start(50);
}


void HeroGraphics::drawHealthBar(QGraphicsScene* scene) {
    if (hpBar) {
        scene->removeItem(hpBar);  // Удаляем старую полосу здоровья
        delete hpBar;  // Очищаем память
    }

    // Создаем новый прямоугольник для полосы здоровья
    hpBar = new QGraphicsRectItem(this);
    hpBar->setBrush(QBrush(Qt::green));  // Зеленая для здоровья
    hpBar->setPen(QPen(Qt::black));

    // Устанавливаем размеры полосы здоровья в зависимости от текущего здоровья
    float healthRatio = (float) getHealth() / getMaxHealth();
    float width = 100 * healthRatio;  // Ширина полосы здоровья

    // Устанавливаем позицию и размеры полосы здоровья
    hpBar->setRect(QPixmap(":/images/Hero.PNG").width() / 9 + 10, -40, width * 4, 20);  // Позиция над персонажем, ширина зависит от здоровья

    scene->addItem(hpBar);  // Добавляем на сцену
}


