#include "lib.hpp"
#include "game.cpp"

vector<vector<char> > lobby;
string buffer;


char nearby()
{
    if(p.x == 17 && p.y == 10)//start game
    {
        return 's';
    }
    int directions[5] = {0, 1, 0, -1, 0};
    for (int i = 0; i < 4;i++)
    {
        // check adjacent tiles
        char adj = lobby[p.y + directions[i + 1]][p.x + directions[i]];
        if(adj != ' ' && adj != '#' && adj!='\\' && adj!='/' && adj!='.' && adj!='_')//can only interact with 1 obj at a time
        {
            return adj;
        }
    }

    return ' ';
}

vector<pair<string, int> > itemList;
void initText()
{
    setCursor(40, 4);
    cout << "[" + p.info[0] + "]";
    setCursor(40, 5);
    cout << "Level : " + p.info[3];
    setCursor(40, 6);
    cout << "Money : " + p.info[1];

    // 9 for title
    // 10 input shop/upgrade
    // 16 for prompt space

    itemList.push_back(pair<string, int> ("Potions", 5));
    itemList.push_back(pair<string, int> ("Max Potions", 20));
    itemList.push_back(pair<string, int> ("Energy Drink", 3));
    itemList.push_back(pair<string, int> ("Max Energy Drinks", 15));
    itemList.push_back(pair<string, int> ("Bomb", 10));
}

int renderLobby()
{
    
    
    for (int i = 0; i < lobby.size(); i++)
    {
        for (int j = 0; j < lobby[0].size();j++)
        {
            cout << lobby[i][j];
        }
        cout << endl;
    }
    setCursor(p.x, p.y);
    cout << 'P';
    initText();
    return 0;
}


void checkNearby()
{
    setCursor(40, 16);
    char nearb = nearby();
    if (p.y == 10 && p.x == 17)
    {
        cout << "Press Space to start game";
        return;
    }
    if (nearb != ' ')
    {
        if(nearb == '=')
        {
            cout << "Press Space to Exit";
        }
        else if(nearb=='-')
        {
            cout << "You need to be level 10 to enter";
        }
        else
        {
            cout << "Press Space to Interact";
        }
        
    }
    else
    {
        cout << "                                                ";
    }

}





int loadLobby()
{
    ifstream lobbyMap("assets/lobby.txt");
    string row;
    int r = 0;
    while(getline(lobbyMap, row))
    {
        lobby.push_back(vector<char>());
        for (int i = 0; i < row.length(); i++)
        {
            lobby[r].push_back(row[i]);
        }
        r++;
    }
    lobbyMap.close();
    
    lobby[4][26] = 'W'; //spaceship station
    lobby[16][6] = 'U'; //upgrade
    lobby[16][27] = 'I'; // item shop
    if(stringToInt(p.info[3])<10)
    {
        for (int i = 6; i < 10;i++)
        {
            lobby[6][i] = '-';
        }
            
    }
    else
    {
        lobby[7][5] = '/';
        lobby[7][10] = '\\';
    }

    return 0;
}

int movePlayer(int X, int Y)
{
    if(lobby[p.y+Y][p.x+X] != ' ')
    {
        return 0;
    }   
    else
    {
        setCursor(p.x, p.y);
        cout << ' ';
        p.y += Y;
        p.x += X;
        setCursor(p.x, p.y);
        cout << 'P';
        return 0;
    }
}

int openBackpack()
{
    cout << p.info[0] << "'s backpack" << endl;
    cout << "========================================" << endl;
    for (int i = 0; i < 5;i++)
    {
        int *ptr = p.items;
        if(ptr[i] > 0)
        {
            switch(i)
            {
                case 0:
                    cout << "-Potion : " << ptr[i] << endl;
                    break;
                case 1:
                    cout << "-Max Potion : " << ptr[i] << endl;
                    break;
                case 2:
                    cout << "-Energy Drink : " << ptr[i] << endl;
                    break;
                case 3:
                    cout << "-Max Energy Drink : " << ptr[i] << endl;
                    break;
                case 4:
                    cout << "-Bomb : " << ptr[i] << endl;
                    break;
            }
        }
    }
    cout << "press any key to go back .. ";
    getch();
    system("cls");
    renderLobby();
    return 0;
}
void upStat(int stat)
{
    if(stringToInt(p.info[1])<50)
    {
        cout << "You don't have enough money" << endl;
        cout << "Press any key...";
        getch();
        return;
    }
    if(stringToInt(p.info[4+stat]) == (stat==0?300:((stat==1)?500:30)))
    {
        cout << "You are already maxed" << endl;
        cout << "Press any key...";
        getch();
        return;
    }

    p.info[1] = intToString(stringToInt(p.info[1])-50);
    p.info[4+stat] = intToString(stringToInt(p.info[4+stat])+1);
    return;
}
void upgradeStats()
{

    setCursor(40, 9);
    cout << "Welcome to upgrade shop";
    setCursor(40, 9);
    cout << "Do you want to upgrade your stats? [y/n]";
    
    char option;
    option = getch();
    if(option=='y')
    {
        int ptr = 0;
        bool end = false;
        while(true)
        {
            system("cls");
            cout << "Upgrade your stats : " << endl;
            cout << "========================" << endl;
            cout << (ptr==0?">":" ") << "1. HP " << p.info[4] << "/300 - $50" << (ptr == 0 ? "<":"") << endl;
            cout << (ptr==1?">":" ") << "2. Armor " << p.info[5] << "/500 - $50" << (ptr == 1 ? "<":"") << endl;
            cout << (ptr==2?">":" ") << "3. Energy " << p.info[6] << "/30 - $50" << (ptr == 2 ? "<":"") << endl;
            cout << (ptr==3?">":" ") << "4. Back " << (ptr == 3 ? "<":"") << endl;
            cout << "Money Left : $" << p.info[1] << endl;
            cout << "Press enter..." << endl;
            char p;
            p = getch();
            if(p == 'w')
            {
                ptr = max((ptr - 1) % 4, 0);
            }
            else if(p == 's')
            {
                ptr = (ptr + 1) % 4;
            }
            else if(p == '\r')
            {
                if(ptr==3)
                {
                    break;
                }
                else
                {
                    upStat(ptr);
                }
            }
        }
        
    }
    system("cls");
    renderLobby();
    return;

}




int buy(int n)
{
    string c;
    cout << "How many " << itemList[n].first << " do you want to buy?" << endl;
    cout << "- ";
    cin >> c;
    if(!isnum(c) || stringToInt(c)<=0)
    {
        cout << "Input valid digit" << endl;
        cout << "Press enter to continue..";
        while(getch() != '\r'){}
        return -1;
    }
    if(stringToInt(p.info[1]) < itemList[n].second*stringToInt(c))
    {
        cout << "Not enough money" << endl;
        cout << "Press enter to continue..";
        while(getch() != '\r'){}
        return -1;
    }
    cout << "Purchase Succesful" << endl;
    cout << "Press enter to continue..";
    while(getch() != '\r'){}
    p.items[n] += stringToInt(c);
    p.info[1] = intToString(stringToInt(p.info[1]) - stringToInt(c) * itemList[n].second);
    return 0;
}

void buyItems()
{
    setCursor(40, 9);
    cout << "Welcome to item shop";
    setCursor(40, 10);
    cout << "Do you want to buy items? [y/n]";
    
    char option;
    option = getch();
    if(option=='y')
    {
        int ptr = 0;
        bool end = false;
        while(true)
        {
            system("cls");
            cout << "Buy Items : " << endl;
            cout << "===============================================" << endl;
            cout << (ptr==0?">":" ") << "1. Potions           $5 " << (ptr == 0 ? "<":"") << endl;
            cout << (ptr==1?">":" ") << "2. Max Potions       $20"<< (ptr == 1 ? "<":"") << endl;
            cout << (ptr==2?">":" ") << "3. Energy Drink      $3 " << (ptr == 2 ? "<":"") << endl;
            cout << (ptr==3?">":" ") << "4. Max Energy Drink  $15"<< (ptr == 3 ? "<":"") << endl;
            cout << (ptr==4?">":" ") << "5. Bomb              $10"<< (ptr == 4 ? "<":"") << endl;
            cout << (ptr==5?">":" ") << "6. Back                 "<< (ptr == 5 ? "<":"") << endl;
            cout << "Money Left : $" << p.info[1] << endl;
            cout << "Press enter..." << endl;
            char p;
            p = getch();
            if(p == 'w')
            {
                ptr = max((ptr - 1) % 6, 0);
            }
            else if(p == 's')
            {
                ptr = (ptr + 1) % 6;
            }
            else if(p == '\r')
            {
                if(ptr==5)
                {
                    break;
                }
                else
                {
                    buy(ptr);
                }
            }
        }
        
    }
    system("cls");
    renderLobby();
    return;
}
void checkCheat()
{
    if(findSubstr(buffer, "wasdwasdwasdwasd"))
    {
        p.items[4] = 99;
        setCursor(40, 17);
        buffer = "";
        cout << "BOMB CHEAT ACTIVATED";
        Sleep(100);
    }
    else if(findSubstr(buffer, "asdasdasdasd"))
    {
        for (int i = 0; i < 4;i++)
        {
            p.items[i] = 99;
        }
        setCursor(40, 17);
        buffer = "";
        cout << "ITEM CHEAT ACTIVATED";
        Sleep(100);
    }
    else if(findSubstr(buffer, "wswswsadadad"))
    {
        p.info[4] = "300";
        p.info[5] = "50";
        p.info[6] = "30";
        buffer = "";
        setCursor(40, 17);
        cout << "MAX STATS CHEAT";
        Sleep(100);
    }
    
    setCursor(40, 17);
    cout << "                                        ";
}
int exit()
{
    return 1;
}
void showScore()
{
    system("cls");
    vector<pair<int, string> > scoreData;
    ifstream playerScores("database/score.dat");
    string data;
    while(getline(playerScores, data))
    {
        vector<string> tmp = split(data, '#');
        scoreData.push_back(pair<int, string>(stringToInt(tmp[1]), tmp[0]));
    }
    playerScores.close();
    cout << "Score board" << endl;
    cout << "========================================" << endl;
    bsortr(scoreData);
    for (int i = 0; i < scoreData.size();i++)
    {
    cout << "- " << scoreData[i].second << " = " << scoreData[i].first << endl;
    }
    cout << "press any key to go back .. ";
    getch();
    system("cls");
    renderLobby();
}
void changeShip()
{
    system("cls");
    vector<vector<string> > shipSprites;
    
    for (int i = 1; i < 5;i++)
    {
        string path = "assets/space_" + intToString(i) + ".txt";
        ifstream sprite(path.c_str());
        vector<string> tmp;
        string row;

        while(getline(sprite, row))
        {
            tmp.push_back(row);
        }
        shipSprites.push_back(tmp);
        sprite.close();

    }

    int op = 1; // 1- 4

    cout << "Choose your ship          " << endl;
    cout << "==========================" << endl;
    for (int i = 0; i < shipSprites[op - 1].size(); i++)
    {
        cout << shipSprites[op - 1][i] << "          " << endl;
    }
    cout << endl << endl;
    if(ship.type == op-1)
    {
    cout << "Current Ship" << endl;
    }
    else
    {
        cout << endl;
    }
    cout << "press A or D to swipe    " << endl;
    cout << "press Q to go back       " << endl;
    cout << "Press Enter to choose    " << endl;
    while (true)
    {
        if(kbhit())
        {
            char key = getch();
            if(key=='a')
            {
                op = op - 1;
                if(op==0)
                {
                    op = 4;
                }
            }
            else if(key=='d')
            {
                op = op + 1;
                if(op==5)
                {
                    op = 1;
                }
            }
            else if(key=='\r')
            {
                setCursor(0, 19);
                cout << "Succesfully picked spaceship" << endl;
                ship = spaceShip(op - 1);
                cout << "press enter to go to lobby" << endl;
                while(getch()!='\r'){}
                system("cls");
                renderLobby();
                return;
            }
            else if(key=='q')
            {
                system("cls");
                renderLobby();
                return;
            }
            for (int i = 0; i < shipSprites[op - 1].size(); i++)
            {
                setCursor(0, 2 + i);
                cout << shipSprites[op - 1][i] << "          ";
            }
            setCursor(0, 9);
            if(ship.type == op-1)
            {
            cout << "Current Ship" << endl;
            }
            else
            {
                cout << "                   " << endl;
            }
        }

    }
    

}

int pressSpace(char obj)
{
    switch(obj)
    {
        case '=':
            return exit();
            break;
        case 'U':
            upgradeStats();
            break;
        case 'I':
            buyItems();
            break;
        case 'W':
            changeShip();
            break;
        case '|':
            showScore();
            break;
        case 's':
            startGame();
            renderLobby();
            break;
    }
    return 0;
};





int lobbyInput()
{
    
    char key;
    if(kbhit())
    {
        switch(key = getch())
        {
            case 'w':
                movePlayer(0, -1);
                break;
            case 'a':
                movePlayer(-1, 0);
                break;
            case 's':
                movePlayer(0, 1);
                break;
            case 'd':
                movePlayer(1, 0);
                break;
            case 'o': //backpack
                system("cls");
                openBackpack();
                break;
            case 32:
                return pressSpace(nearby());
            default:
                break;
        }
        buffer = buffer + key;
        if(buffer.length()>16)
        {
            buffer = buffer.substr(1,buffer.size());
        }
        checkCheat();

        checkNearby();
    }
    
    
    return 0;
}



