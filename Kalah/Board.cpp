#include "Board.h"
#include <iostream>
using namespace std;

Board::Board(int nHoles, int nInitialBeansPerHole)
{
    int a;
    //If nHoles is not positive, act as if it were 1
    if(nHoles<1)
        m_nHoles=1;
    else
        m_nHoles=nHoles;
    allLen=(m_nHoles+1)*2;
    //if nInitialBeansPerHole is negative, act as if it were 0
    if(nInitialBeansPerHole<0)
        a=0;
    else
        a=nInitialBeansPerHole;
    //declaring a dynamic array
    all = new int[allLen];
    //initializing array
    for(int i=0; i<allLen; i++)
    {
        all[i]=a;
    }
    //setting Pot values to 0
    all[m_nHoles]=POT;
    all[allLen-1]=POT;
}
//since the default copy costructor does not work, we declare and implement our own copy constructor
Board::Board(const Board &other)
{
    m_nHoles=other.m_nHoles;
    allLen=other.allLen;
    all= new int[allLen];
    for(int i=0; i<allLen; i++)
    {
        all[i]=other.all[i];
    }
}

int Board::holes() const
{
    return m_nHoles;
}

int Board::beans(Side s, int hole) const
{
    //returns −1 if the hole number is invalid
    if(hole>m_nHoles||hole<0)
        return -1;
    if(s==NORTH)
        return all[m_nHoles-hole];
    if(hole==0)
        return all[allLen-1];
    return all[hole+m_nHoles];
}

int Board::beansInPlay(Side s) const
{
    int total=0;
    if(s==NORTH)
    {
        for(int i=0;i<m_nHoles;i++)
        {
            //add all of North's beans in holes
            total+=all[i];
        }
    }
    else
    {
        for(int i=0;i<m_nHoles;i++)
        {
            //add all of South's beans in holes
            total+=all[m_nHoles+1+i];
        }
    }
    return total;
}
//return total of all the beans on the board
int Board::totalBeans() const
{
    int total=0;
    for(int i=0;i<allLen;i++)
        total+=all[i];
    return total;
}


bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
    
    if(hole>m_nHoles||hole<1)
        return false; //for invalid hole
    int pos;
    int yPot; //yPot is the opponent's Pot, this helps skip it
    if(s==NORTH)
    {
        pos=m_nHoles-hole;
        yPot=allLen-1;
    }
    else
    {
        pos=m_nHoles+hole;
        yPot=m_nHoles;
    }
    int a=all[pos];
    if(a<=0)
        return false; //if hole is empty
    all[pos]=0;
    pos++;//emptying the hole
    while(a>1) //sowing the beans counterclockwise
    {
        all[pos%allLen]++;
        pos++;
        if(pos%allLen==yPot) //skip the opponent's hole
            pos++;
        a--;
    }
    int c=pos%allLen;
    all[c]++;
    //setting the end side and hole
    if(c<=m_nHoles)
    {
        endSide=NORTH;
        endHole=m_nHoles-c;
    }
    else
    {
        endSide=SOUTH;
        if(c!=allLen-1)
            endHole=c-m_nHoles;
        else
            endHole=0;
    }
    return true;
}

bool Board::moveToPot(Side s, int hole, Side potOwner)
{
    if(hole>m_nHoles||hole<1) //for invalid hole, return false
        return false;
    int pos; //position of hole
    if(s==NORTH)
        pos=m_nHoles-hole;
    else
        pos=m_nHoles+hole;
    int mPot; //position of potOwner's pot
    if(potOwner==NORTH)
        mPot=m_nHoles;
    else
        mPot=allLen-1;
    all[mPot]+=all[pos];
    all[pos]=0;
    return true;
}

bool Board::setBeans(Side s, int hole, int beans)
{
    if(hole>m_nHoles||hole<0 || beans<0)
        return false;
    int pos; //find position of hole in array
    if(s==NORTH)
        pos=m_nHoles-hole;
    else if(hole!=0)
        pos=m_nHoles+hole;
    else
        pos=allLen-1;
    all[pos]=beans;
    return true;
}

Board::~Board()
{
    delete []all;
}
