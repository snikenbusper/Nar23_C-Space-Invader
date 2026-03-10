#include "lib.hpp"
#include "main_menu.cpp"
#include "new_game.cpp"
#include "load_game.cpp"
#include "lobby.cpp"
#include "setting_howto.cpp"



int main()
{
    srand(time(0));
    bool end=false;
    while(!end)
    {
        switch(displayMenu())
        {
            case 0: // new game
                system("cls");
                if(inputPlayer()==-1){
                    displayError();
                }
                break;
            case 1: // load game
                system("cls");
                if(selectSaveData()==0)
                {
                    loadLobby();
                    system("cls");
                    renderLobby();
                    initText();
                    while(lobbyInput()==0)
                    {
                    }
                    saveData();
                    lobby.clear();
                    ship = spaceShip();
                    p = player();
                    
                }
                
                break;
            case 2: // setting
                setting();
                break;
            case 3: // how to play
                howToPlay();
                break;
            case 4: // exit
                end = true;
                break;
        }
        system("cls");

    }
    return 0;
}

//Justine#3580 #500#6    #100#50    #1
// name  #money#exp#level#hp #energy#armor#damage