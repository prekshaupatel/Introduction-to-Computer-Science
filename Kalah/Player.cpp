#include "Player.h"
#include <iostream>
int value(const Board& A, Side s);
void chooseM(Board& b,Side s, int i, int& BestHole, int& Value);
void printnode(Board b);

Player::Player(std::string name)
{
    m_name=name;
}
std::string Player::name() const
{
    return m_name;
}
bool Player::isInteractive() const
{
    return false;
    //we overwrite function in base class if the Player isInteractive
}
Player::~Player(){}



HumanPlayer::HumanPlayer(std::string name):Player(name)
{
}
//overwriting function since HumanPlayer is Interactive
bool HumanPlayer::isInteractive() const
{
    return true;
}
int HumanPlayer::chooseMove(const Board& b, Side s) const
{
    int a=0;
    //if there are no beans, there are no possible moves, so return -1
    if(b.beansInPlay(s)==0)
        return -1;
    while(a<1 || a> b.holes() || b.beans(s,a)==0)
    {
        //prompts for move till the move is valid
        std::cerr<<"Make a move!"<<std::endl;
        std::cin>>a;
    }
    return a;
}




BadPlayer::BadPlayer(std::string name):Player(name)
{
}
int BadPlayer::chooseMove(const Board& b, Side s) const
{
    int a=1;
    //if there are no beans, there are no possible moves, so return -1
    if(b.beansInPlay(s)==0)
        return -1;
    //returns the first possible valid move
    while(a<=b.holes() && b.beans(s,a)==0)
        a++;
    return a;
}





SmartPlayer::SmartPlayer(std::string name):Player(name)
{
   //player does not have a default constructor, so it must be initialized in the initializer list
}
int SmartPlayer::chooseMove(const Board& b, Side s) const
{
    int BestHole=-1;
    int Value=-1000000;
    int i=0;
    Board c=b;
    if(b.beansInPlay(s)==0 || b.beansInPlay(opponent(s))==0)
        return -1;
    chooseM(c, s, i, BestHole, Value);
//    std::cout<<Value<<", "<<BestHole<<std::endl;
    
    return BestHole;
}


void chooseM(Board& b,Side s, int i, int& BestHole, int& Value)
{
    i++;
    if( i>10 || b.beansInPlay(s)==0 || b.beansInPlay(opponent(s))==0)
    {
        BestHole=-1;
        if(b.beansInPlay(s)==0)
        {
            for(int i=1;i<=b.holes();i++)
                b.moveToPot(opponent(s), i, opponent(s));
        }
        if(b.beansInPlay(opponent(s))==0)
        {
            for(int i=1;i<=b.holes();i++)
                b.moveToPot(s, i, s);
        }
        Value=value(b, s);
//        std::cout<<"Winning position is "<<Value<<" for "<<s<<" on "<<i<<std::endl;
        return;
        
    }
    Value=-1000000;
    int h,v;
    Side endSide;
    int endHole;
    for(int j=1;j<=b.holes();j++)
    {
        if(b.beans(s,j)!=0)
        {
            
            
//            std::cout<<i<<"..............................................."<<std::endl; //remove
//            std::cout<<"moved: "<<s<<std::endl;
//            printnode(b);
            

            Board c = b;
            c.sow(s,j, endSide, endHole);
            if(endSide==s && endHole!=0 && c.beans(s,endHole)==1 && c.beans(opponent(s),endHole)!=0)
            {
                c.moveToPot(NORTH, endHole, s);
                c.moveToPot(SOUTH, endHole, s);
            }
//
//            printnode(c);
//            printnode(b);
            if(endSide==s && endHole==0)
            {
                chooseM( c, s, i, h, v);
                if(v>=Value)
                {
                    Value=v;
                    BestHole=j;
                }
            }
            else
            {
                chooseM(c,opponent(s), i, h, v);
                if(-v>=Value)
                {
                    Value=-v;
                    BestHole=j;
                }
            }
            
            
        }
        
        
    }
}


int value(const Board& A, Side s)
{
    int diff= A.beans(NORTH, 0) -A.beans(SOUTH, 0) ;
    if(A.beansInPlay(SOUTH)==0 && A.beansInPlay(NORTH)==0)
    {
        if(diff>0)
            diff=1000000;
        else if(diff<0)
            diff=-1000000;
    }
    if(s==SOUTH)
    {
        diff=0-diff;
    }
    return diff;
}



void printnode(Board b)
{
    
    std::cout<<"   ";
    for(int i=1;i<=b.holes();i++)
        std::cout<<b.beans(NORTH, i)<<"  ";
    std::cout<<std::endl<<b.beans(NORTH,0)<<"           "<<b.beans(SOUTH,0)<<std::endl;
    std::cout<<"    ";
    for(int i=1;i<=b.holes();i++)
        std::cout<<b.beans(SOUTH, i)<<"  ";
    std::cout<<std::endl<<std::endl;
}
