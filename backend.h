//#include <iostream>
#include <string>
#include <vector>
//#include <algorithm>
#include <bits/stdc++.h>


class Gameunit {
public:
    Gameunit(int, int);
    bool isAlive();
    void attack(Gameunit&);
    int getAttack();
    int getHealth();
    void setHealth(int);
    void setMaxHealth(int);
    int getMaxHealth();
protected:
    int attack_, health_;
    int maxhealth_;
};


enum class Enemy_type{Dragon, Trolle};


class Enemy : public Gameunit {
public:
    Enemy(Enemy_type type, int, int);
private:
    Enemy_type type_;
};


enum class Color { Red, Green, Black };

class Expression {
public:
    Expression(char op);
    long long calculate();
    void print();
    std::string toStr();
private:
    std::vector<int> var_;
    char op_;
};

class Dragon : public Enemy {
public:
    Dragon(Color color);
    void updateData();
    void setAnswer(long long);
    bool checkAnswer();
    virtual std::string question(char);
    Color getColor();
protected:

    Color color_;
    long long answer_;
    Expression* que_;
};

class RedDragon : public Dragon {
public:
    RedDragon();
    std::string question(char op = '+');
};

class GreenDragon : public Dragon {
public:
    GreenDragon();
    std::string question(char op = '-');
};

class BlackDragon : public Dragon {
public:
    BlackDragon();
    std::string question(char op = '*');
};


class Hero : public Gameunit {
public:
    Hero(std::string, int, int);
    void levelUp(int, int, int);
    int getExp();
    void attack(Enemy&);
protected:
    std::string name_;
    int exp_;
};
