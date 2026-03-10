#include <iostream>
#pragma once
#include <string>
#include <conio.h>
#include <typeinfo>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <utility>
#include <cmath>
#include <Windows.h>
#include <cstdlib>
#include <time.h>

using namespace std;
bool gameEnd = false;
const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);




string errorMessage = "";


void setCursor(int x, int y)
{
    COORD coord = {x, y};
    SetConsoleCursorPosition(hOut, coord);
}


vector<string> split(string s, char c)
{
    vector<string> arr;
    ;

    while(s.find(c) != string::npos)
    {
        arr.push_back(s.substr(0, s.find(c)));
        s.erase(0, s.find(c)+1);
    }
    arr.push_back(s.substr(0, s.length()));
    return arr;
}
string join(vector<string> s, char c)
{
    string res;
    while(!s.empty())
    {
        res += s.front()+'#';
        s.erase(s.begin());
    }
    return res.substr(0, res.size()-1);
}

bool findSubstr(string a, string find)
{
    setCursor(27, 27);
    if(a.size()<find.size())
    {
        return false;
    }

    for (int i = 0; i <= (a.size()-find.size());i++)
    {
        if(a.substr(i, (find.size())).compare(find) == 0)
        
        {
            return true;
        }
    }
    return false;
}

void displayError()
{
    cout << errorMessage << endl;
    cout << "Press any key to continue.." << endl;
    getch();
    errorMessage = "";
}

class player
{
    public:
        
        vector<string> info;
        int x;
        int y;
        int* items;
        player(int x = 17, int y = 19)
        {
            this->x = x;
            this->y = y;
            this->info = info;
            this->items = (int *)calloc(5, sizeof(int));
            return;
        }
}p;



int shipSize[4][3] = {{5, 5, 3}, {7, 5, 3}, {3, 4, 2}, {7, 5, 3}}; // width, height, coreY


class spaceShip
{
    public:
    int type;
    int core[2];
    int x;
    int y;
    string name;
    spaceShip(int type = 0, int x = 24, int y = 18)
    {
        this->type = type;
        this->core[0] = shipSize[type][0] / 2;
        this->core[1] = shipSize[type][2];
        this->x = x - core[0]; // 1-47
        this->y = y - core[1]; // 1-21
        // top left coordinates
        
        switch(type)
        {
            case 0:
                this->name = "Default Spaceship";
                break;
            case 1:
                this->name = "Great Spaceship";
                break;
            case 2:
                this->name = "Little Spaceship";
                break;
            case 3:
                this->name = "Rocket Spaceship";
                break;
        }
        
    }
}
ship;

void saveData()
{
    ifstream playerList("database/player.dat");
    vector<string> tmp;
    string playerInfo;
    while(getline(playerList, playerInfo))
    {
        if(split(playerInfo, '#')[0].compare(p.info[0])==0)
        {
            playerInfo = join(p.info, '#');
        }
        tmp.push_back(playerInfo);
    }

    playerList.close();
    //add data

    ofstream playerData("database/player.dat");
    while(!tmp.empty())
    {
        playerData << tmp.front() << endl;
        tmp.erase(tmp.begin());
    }
    playerData.close();
}



void bsortr(vector<pair<int, string > >& arr)
{
    int N = arr.size();
    for (int i = 0; i < N;i++)
    {
        for (int j = 0; j < N - 1; j++)
        {
            if(arr[j].first <= arr[j+1].first)
            {
                pair<int, string> tmp;
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
    return;
}

int stringToInt(string s)
{
    int num = 0;
    int p = 0;
    int m = (s[0] == '-') ? -1 : 1;
    for (int i = s.size() - 1; i >= (s[0]=='-')?1:0; i--)
    {
        num += (s[i]-'0')*pow(10, p);
        p++;
    }
    return num*m;
}
string intToString(int num)
{
    if(num==0)
    {
        return "0";
    }
    string res;
    while(num)
    {
        int digit = num % 10;
        res = (char)(digit+'0') + res;
        num /= 10;
    }
    return res;
}

bool isnum(string s)
{
    int i = 0;
    if (s[0] == '-'){
        if(s.size()==1)
        {
            return false;
        }
        i++;
    }
    for (; i < s.size(); i++)
    {
        if(!isdigit(s[i])){
            return false;
        }
    }
    return true;
}




