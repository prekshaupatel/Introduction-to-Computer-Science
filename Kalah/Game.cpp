#include "Game.h"
#include <iostream>
using namespace std;

Game::Game(const Board& b, Player* south, Player* north):m_b(b)
{
    S=south;
    N=north;
    curr_player=S;
}

void Game::display() const
{
    cout<<"            "<<N->name()<<endl;
    cout<<"    ";
    for(int i=1;i<=m_b.holes();i++)
        cout<<m_b.beans(NORTH, i)<<"  ";
    cout<<endl<<m_b.beans(NORTH,0)<<"                        "<<m_b.beans(SOUTH,0)<<endl;
    cout<<"    ";
    for(int i=1;i<=m_b.holes();i++)
        cout<<m_b.beans(SOUTH, i)<<"  ";
    cout<<endl<<"            "<<S->name()<<endl<<endl;
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
    Side z=NORTH;
    if(curr_player==S)
        z=SOUTH;
    if (m_b.beansInPlay(z)!=0 && m_b.beansInPlay(opponent(z))!=0 )
    {
        over=false;
        return;
    }
    over=true;
    int n=m_b.beansInPlay(NORTH)+m_b.beans(NORTH,0);
    int s=m_b.beansInPlay(SOUTH)+m_b.beans(SOUTH,0);
    if(n==s)
    {
        hasWinner=false;
        return;
    }
    hasWinner=true;
    if(n>s)
        winner=NORTH;
    else
        winner=SOUTH;
}

bool Game::move()
{
    Side z=NORTH;
    if(curr_player==S)
        z=SOUTH;
    int m=curr_player->chooseMove(m_b,z);
    if (m==-1)
    {
        for(int i=1; i<=m_b.holes(); i++)
        {
            m_b.moveToPot(NORTH, i, NORTH);
            m_b.moveToPot(SOUTH, i, SOUTH);
        }
        return false;
    }
    Side endSide;
    int endHole;
    m_b.sow(z, m, endSide, endHole);
    cout<<curr_player->name()<<" sowed from "<<m<<endl;
    display();
    if(endSide==z)
    {
        if(endHole==0)
        {
            if(move()==false)
                return false;
            //another turn!
        }
        else if(m_b.beans(z,endHole)==1 && m_b.beans(opponent(z),endHole)!=0)
        {
            //capture!!
            m_b.moveToPot(NORTH, endHole, z);
            m_b.moveToPot(SOUTH, endHole, z);
	    display();
        }
    }

    if(z==NORTH)
        curr_player=S;
    else
        curr_player=N;
    return true;
}

void Game::play()
{
    bool over;
    bool hasWinner;
    Side winner;
    status(over, hasWinner, winner);
    display();
    while(!over)
    {
        move();
        status(over, hasWinner, winner);
    }
    
    for(int i=1; i<=m_b.holes(); i++)
    {
        m_b.moveToPot(NORTH, i, NORTH);
        m_b.moveToPot(SOUTH, i, SOUTH);
    }
    display();
    if(!hasWinner)
        cout<<"Tie!";
    else if(winner==NORTH)
        cout<<N->name()<<" won!";
    else
        cout<<S->name()<<" won!";
    cout<<endl;
}
    //
//If neither player is interactive, then to keep the display from quickly scrolling through the whole game, it would be reasonable periodically to prompt the viewer to press ENTER to continue and not proceed until ENTER is pressed. (The ignore function for input streams is useful here.)
int Game::beans(Side s, int hole) const
{
    return m_b.beans(s,hole);
}


//
//cout << "Press enter to continue . . . ";
//cin.sync();
//cin.ignore(numeric_limits<streamsize>::max(), '\n');
