#ifndef Player_hpp
#define Player_hpp
#include <string>
#include "Board.h"

class Player
{
public:
    Player(std::string name);
    std::string name() const;
    virtual bool isInteractive() const;
//    Return false if the player is a computer player. Return true if the player is human. Most kinds of players will be computer players.
    virtual int chooseMove(const Board& b, Side s) const = 0;
//    Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose. If no move is possible, return −1.
    virtual ~Player();

private:
    std::string m_name;
};

class HumanPlayer:public Player
{
public:
    HumanPlayer(std::string name);
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const;
};


class BadPlayer:public Player
{
public:
    BadPlayer(std::string name);
    virtual int chooseMove(const Board& b, Side s) const;
};

class SmartPlayer:public Player
{
public:
    SmartPlayer(std::string name);
    virtual int chooseMove(const Board& b, Side s) const;
};



#endif /* Player_hpp */

