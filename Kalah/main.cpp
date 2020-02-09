#include "Game.h"
#include "Player.h"
#include "Board.h"
#include "Side.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    int numplayers = -1;
    cout<<"Please select one of the following\n1: Play against a friend\n2: Play against the computer\n3: Observe a game\n";
    cin>>numplayers;
    while ( numplayers != 1 && numplayers != 2 && numplayers != 3)
    {
        cout<<"Please Enter a Valid Answer (1/2/3)";
        cin>>numplayers;
    }
    
    
    char custom;
    cout<<"Would you like to customize your game? y/n \n";
    cin>>custom;
    while ( custom != 'y' && custom != 'n')
    {
        cout<<"Please Enter a Valid Answer (y/n)";
        cin>>custom;
    }
    int numMarbles = 3;
    int numHoles = 6;
    if (custom == 'y')
    {
        cout<<"Please enter the number of holes\n";
        cin>>numHoles;
        cout<<"Please enter the number of marbles in each hole\n";
        cin>>numMarbles;
    }
    Board b(numHoles, numMarbles);
        
    if (numplayers == 1)
    {
        HumanPlayer bp1("Player 1");
        HumanPlayer bp2("Player 2");
        Game g(b, &bp1, &bp2);
        g.play();
    }
        
        
    if (numplayers == 2)
    {
        int level;
        cout<<"Please select a level of difficulty\n1: Easy\n2: Hard\n";
        cin>>level;
        while ( level != 1 && level != 2)
        {
            cout<<"Please Enter a Valid Answer (1/2)";
            cin>>level;
        }
        
        char first;
        cout<<"Would you like to go first? y/n \n";
        cin>>first;
        while ( first != 'y' && first != 'n')
        {
            cout<<"Please Enter a Valid Answer (y/n)";
            cin>>first;
        }
        if (first == 'y' && level == 1)
        {
           HumanPlayer bp1("Player 1");
           BadPlayer bp2("Computer");
           Game g(b, &bp1, &bp2);
           g.play();
        }
        if (first == 'y' && level == 2)
        {
           HumanPlayer bp1("Player 1");
           SmartPlayer bp2("Computer");
            Game g(b, &bp1, &bp2);
            g.play();
        }
        if (first == 'n' && level == 1)
        {
           HumanPlayer bp2("Player 1");
           BadPlayer bp1("Computer");
            Game g(b, &bp1, &bp2);
            g.play();
        }
        if (first == 'n' && level == 2)
        {
           HumanPlayer bp2("Player 1");
           SmartPlayer bp1("Computer");
            Game g(b, &bp1, &bp2);
            g.play();
        }
        
    }
    if (numplayers == 3)
    {
        int cnt;
        cout<<"Please select one of the following options:\n1: Smart Player vs Smart Player\n2: Smart Player vs Regular Player\n3: Regular Player vs Regular Player\n";
        cin>>cnt;
        while ( cnt != 1 && cnt != 2 && cnt != 3)
        {
            cout<<"Please Enter a Valid Answer (1/2/3)";
            cin>>cnt;
        }
        if (cnt == 1)
        {
            SmartPlayer bp2("Smart Player 1");
            SmartPlayer bp1("Smart Player 2");
            Game g(b, &bp1, &bp2);
            g.play();
        }
        if (cnt == 2)
        {
            SmartPlayer bp2("Smart Player 1");
            BadPlayer bp1("Regular Player 2");
            Game g(b, &bp1, &bp2);
            g.play();
        }
        if (cnt == 2)
        {
            BadPlayer bp2("Regular Player 1");
            BadPlayer bp1("Regular Player 2");
            Game g(b, &bp1, &bp2);
            g.play();
        }
    }
}

