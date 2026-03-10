#include "lib.hpp"


int displayMenu()
{
    system("cls");
    cout << "New Game" << endl
         << "Load Game" << endl
         << "Setting" << endl
         << "How to play" << endl
         << "Exit" << endl;
    int option = 0;
    setCursor(20, option);
    cout<< '<';
    bool chosen = false;
    while(!chosen)
    {
        if(kbhit())
        {
            switch(getch())
            {
                case 's':
                    setCursor(20, option);
                    cout<< ' ';
                    option = (option + 1)%5;
                    setCursor(20, option);
                    cout << '<';
                    break;
                case 'w':
                    setCursor(20, option);
                    cout << ' ';
                    option -= 1;
                    if(option==-1)
                    {
                        option = 4;
                    }
                    setCursor(20, option);
                    cout << '<';
                    
                    break;
                case '\r':
                    chosen = true;
                    break;
            }
        }
    }
    system("cls");
    return option;
}