#include "lib.hpp"

void setting()
{
    cout << "Basic Movement\n";
    cout << "\'w\' - Move Up\n";
    cout << "\'a\' - Move Left\n";
    cout << "\'s\' - Move Down\n";
    cout << "\'d\' - Move Right\n";

    cout << endl;

    cout << "Lobby Arena\n";
    cout << "\'SPACE\' - Interact\n";
    cout << "\'o\' - Open backpack\n";

    cout << endl;

    cout << "Game Arena\n";
    cout << "\'SPACE\' - Shoot\n";
    cout << "\'r\' Reload\n";
    cout << "\'f\' Skill\n";
    cout << "\'1\' Potion\n";
    cout << "\'2\' Max Potion\n";
    cout << "\'3\' Energy Drink\n";
    cout << "\'4\' Max Energy Drink\n";
    cout << "\'g\' Use Bomb\n";

    cout
        << "Press Enter..";
    while(getch()!='\r'){}
    return;
}

void howToPlay()
{
    cout << "Space Invader\n";
    cout << "The object of the game is to shoot the invaders with your shooter\n";
    cout << "while avoiding their shots and preventing an invasion.\n";
    cout << "Amassing a high score is a further objective and one that must be prioritized against your continued survival\n";

    cout << endl;
    cout << "There is no time limit,\n";
    cout << "except for the fact that if you do not shoot them all before they reach\n";
    cout << "the bottom of the screen the game ends.\n";
    cout << endl << "Credit : TinTin Winata \n";

    cout << "Press Enter..";
    while(getch()!='\r'){}
    return;
}