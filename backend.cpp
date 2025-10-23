#include "backend.h"
#include <random>
#include <chrono>
#include <limits>
#include <QDebug>
#include <sstream>

Gameunit::Gameunit(int attack, int health) : attack_(attack), health_(health), maxhealth_(health) {}

int Gameunit::getAttack() {
    return attack_;
}

int Gameunit::getHealth() {
    return health_;
}

void Gameunit::setHealth(int health){
    health_ = health; //, maxhealth_ = health;
}

void Gameunit::attack(Gameunit& rhs) {
    rhs.setHealth(std::max(0,rhs.getHealth() - attack_));
}

bool Gameunit::isAlive() {
    return health_ > 0;
}

int Gameunit::getMaxHealth(){ return maxhealth_; }


Enemy::Enemy(Enemy_type type, int x, int y) : Gameunit(x, y), type_(type) {}


std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());

int rand(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}
std::map<Color, std::pair<int, int>> DragonCharachteristics = {
    {Color::Red, {30, 200}},
    {Color::Green, {50, 300}},
    {Color::Black, {70, 500}},
    };

Expression::Expression(char op) : op_(op) {
    if(op_ == '*') {
        var_.resize(rand(2, 3));
    } else {
        var_.resize(rand(2, 6));
    }
    if(op_ == '-'){
        var_[0] = rand(200, 400);
        for(unsigned int i = 1; i < var_.size(); ++i){
            var_[i] = rand(10, 30);
        }
    } else{
        for(unsigned int i = 0; i < var_.size(); ++i){
            var_[i] = rand(10, 30);
        }
    }
}

long long Expression::calculate(){
    long long res(var_[0]);
    for(unsigned int i = 1; i < var_.size(); ++i){
        if(op_ == '*') res *= var_[i];
        else if(op_ == '+') res += var_[i];
        else res -= var_[i];
    }
    return res;
}

std::string Expression::toStr(){
    std::ostringstream str;
    for(unsigned int i = 0; i < var_.size() - 1; ++i){
        str << var_[i] << ' ' << op_ << ' ';
    }
    str << var_[var_.size() - 1] << " = ";
    return str.str();
}


Dragon::Dragon(Color color) : Enemy(Enemy_type::Dragon, DragonCharachteristics[color].first, DragonCharachteristics[color].second), color_(color), que_(0) {}

void Dragon::setAnswer(long long ans) {answer_ = ans;}

void Dragon::updateData() {attack_ = DragonCharachteristics[color_].first;}//, health_ = DragonCharachteristics[color_].second;}

bool Dragon::checkAnswer() {return (answer_ == que_->calculate());}

std::string Dragon::question(char op){
    que_ = new Expression(op);
    updateData();
    //que_->print();
    std::string res = que_->toStr();
    return res;
}

Color Dragon::getColor(){
    return color_;
}

RedDragon::RedDragon() : Dragon(Color::Red) {}
std::string RedDragon::question(char op){
    std::string res = "Дракон сложения задает вопрос: " + Dragon::question(op);
    return res;
}

GreenDragon::GreenDragon() : Dragon(Color::Green) {}
std::string GreenDragon::question(char op){
    std::string res = "Дракон вычитания задает вопрос: " + Dragon::question(op);
    return res;
}

BlackDragon::BlackDragon() : Dragon(Color::Black) {}
std::string BlackDragon::question(char op){
    std::string res = "Дракон умножения задает вопрос: " + Dragon::question(op);
    return res;
}









Hero::Hero(std::string name, int x, int y) : Gameunit(x, y), name_(name), exp_(0) {}

void Hero::levelUp(int attack, int health, int lvl){
    attack_ = attack;
    health_ = health;
    exp_ += lvl * 100;
}

int Hero::getExp(){ return exp_; }

void Hero::attack(Enemy& enemy){
    enemy.setHealth(enemy.getHealth() - getAttack());
}

