#ifndef board_hpp
#define board_hpp
#include "Side.h"

class Board
{
public:
    
    
    Board(int nHoles, int nInitialBeansPerHole);
    Board(const Board &other);
    int holes() const;
    int beans(Side s, int hole) const;
    int beansInPlay(Side s) const;
    int totalBeans() const;
    bool sow(Side s, int hole, Side& endSide, int& endHole);
    bool moveToPot(Side s, int hole, Side potOwner);
    bool setBeans(Side s, int hole, int beans);
    
    ~Board();
private:
    
    int m_nHoles;
    int* all;
    int allLen;
};

#endif /* board_hpp */
