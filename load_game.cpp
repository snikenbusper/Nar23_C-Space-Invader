#include "lib.hpp"

int selectSaveData()
{
    cout << "Save Data : " << endl;
    ifstream players("database/player.dat");
    vector< vector<string> > playersList;
    string playerInfo;
    int idx = 1;
    while(getline(players,playerInfo))
    {
        vector<string> arr = split(playerInfo, '#');
        playersList.push_back(arr);
        cout << idx << ". " << arr[0] << " [Level : " << arr[3] << "]" << endl;
        idx++;
    }
    cout << idx << ". Back" << endl;
    players.close();
    setCursor(30, 1);
    cout << '<';
    int option = 1;
    bool chosen = false;
    while(!chosen)
    {
        if(kbhit())
        {
            switch(getch())
            {
                case 's':
                    setCursor(30, option);
                    cout<< ' ';
                    option = (option % (idx)) + 1;
                    setCursor(30, option);
                    cout << '<';
                    break;
                case 'w':
                    setCursor(30, option);
                    cout << ' ';
                    option -= 1;
                    if(option==0)
                    {
                        option = idx;
                    }
                    setCursor(30, option);
                    cout << '<';
                    
                    break;
                case '\r':
                    chosen = true;
                    break;
            }
        }
    }
    

    if(option==idx)
    {
        return -1;
    }
    
    p.info = playersList[option-1];
    system("cls");
    return 0;
}