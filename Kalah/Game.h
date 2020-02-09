#ifndef Game_hpp
#define Game_hpp
#include "Board.h"
#include "Player.h"
class Game
{
public:
    Game(const Board& b, Player* south, Player* north);
    void display() const;
    void status(bool& over, bool& hasWinner, Side& winner) const;
    bool move();
    void play();
    int beans(Side s, int hole) const;
private:
    Board m_b;
    Player* N;
    Player* S;
    Player* curr_player;
};
#endif /* Game_hpp */
