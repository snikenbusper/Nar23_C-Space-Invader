#include "lib.hpp"

vector<string> map;
vector<string> shipSprite;
vector<void *> bulletList;
vector<vector<void *> > enemyList;
vector<string> enemySprites;
vector<vector<bool> > visited(22, vector<bool>(49, false));
int score = 0;

int spawnTime = 1;
int shiftTime = 1; // 1 unit = 20ms   1 sec = 50
int gameTime = 1;
int enemyDamage[3] = {10, 20, 25};
bool gameExit = false;
bool gameLost = false;

void debug(string s)
{
    setCursor(25, 25);
    cout << s << endl;
}

void printMap()
{
    for (int i = 0; i < map.size(); i++)
    {
        cout << map[i] << endl;
    }
}

void alert(string text)
{
    setCursor(52, 18);
    cout << text;
}

class playerEntity
{
public:
    int x;
    int y;
    int bullets;
    int maxHp;
    int hp;
    int maxArmor;
    int armor;
    int maxEnergy;
    double energy;
    int damage;
    bool reloading;
    int reloadStart;
    playerEntity(int maxHp, int maxEnergy, int maxArmor, int damage)
    {
        this->x = ship.x; // top left coord
        this->y = ship.y;
        this->bullets = 10;
        this->maxHp = maxHp;
        this->hp = maxHp;
        this->maxArmor = maxArmor;
        this->armor = maxArmor;
        this->maxEnergy = maxEnergy;
        this->energy = maxEnergy;
        this->damage = damage;
        this->reloading = false;
    }

    void reload(int time)
    {
        if (this->reloading)
        {
            return;
        }
        this->reloading = true;
        this->reloadStart = time;
        setCursor(52, 15);
        cout << "Reloading ... ";
    }
};

void pMap()
{
    for (int i = 0; i < map.size(); i++)
    {
        setCursor(70, i);
        cout << map[i];
    }
};

class enemyEntity
{
public:
    int level;
    int damage;
    int x; // center
    int y;
    string sprite;
    int nextShot;
    enemyEntity(int level, int x)
    {

        this->level = level;

        this->sprite = enemySprites[(this->level) - 1];
        this->damage = enemyDamage[(this->level) - 1];

        this->y = 0;
        this->x = x;
        this->nextShot = (rand() % 50) + 150;

        setCursor((this->x) - 1, (this->y));
        cout << enemySprites[(this->level) - 1][0];
        setCursor((this->x), (this->y));
        cout << enemySprites[(this->level) - 1][1];
        setCursor((this->x) + 1, (this->y));
        cout << enemySprites[(this->level) - 1][2];
        map[this->y][this->x] = 'e';
        map[this->y][this->x + 1] = 'e';
        map[this->y][this->x - 1] = 'e';
    }
    void move(int X, int Y)
    {
        if (map[this->y + Y][(this->x) + X - 1] != ' ' && map[this->y + Y][(this->x) + X + 1] != ' ')
        {
            return;
        }

        setCursor((this->x) - 1, (this->y));
        cout << ' ';
        setCursor((this->x), (this->y));
        cout << ' ';
        setCursor((this->x) + 1, (this->y));
        cout << ' ';
        map[this->y][this->x] = ' ';
        map[this->y][this->x + 1] = ' ';
        map[this->y][this->x - 1] = ' ';

        this->x = this->x + X;
        this->y = this->y + Y;

        setCursor((this->x) - 1, (this->y));
        cout << enemySprites[this->level - 1][0];
        setCursor((this->x), (this->y));
        cout << enemySprites[this->level - 1][1];
        setCursor((this->x) + 1, (this->y));
        cout << enemySprites[this->level - 1][2];
        map[this->y][this->x] = 'e';
        map[this->y][this->x + 1] = 'e';
        map[this->y][this->x - 1] = 'e';
    }
    void removeEnemy()
    {
        setCursor((this->x) - 1, (this->y));
        cout << ' ';
        setCursor((this->x), (this->y));
        cout << ' ';
        setCursor((this->x) + 1, (this->y));
        cout << ' ';
        map[this->y][this->x] = ' ';
        map[this->y][this->x + 1] = ' ';
        map[this->y][this->x - 1] = ' ';
        delete this;
    }
};

int findEnemy(int target)
{
    int l = 0;
    int r = enemyList.size() - 1;
    int mid;
    while (l <= r)
    {
        mid = (l + r) / 2;
        if (((enemyEntity *)enemyList[mid][0])->y == target)
        {
            return mid;
        }
        else if (((enemyEntity *)enemyList[mid][0])->y > target)
        {
            r = mid - 1;
        }
        else
        {
            l = mid + 1;
        }
    }
    return -1;
}

class bulletEntity
{
public:
    bool friendly;
    int x;
    int y;
    int damage;
    char sprite;
    int skillBullet;
    bulletEntity(bool friendly, int x, int y, int damage, int skillBullet = -1)
    {
        this->x = x;
        this->y = y;
        this->friendly = friendly;
        this->skillBullet = skillBullet;
        this->sprite = friendly ? (skillBullet >= 0 ? 'O' : '^') : '*';
        this->damage = damage;
    }
    int moveBullet()
    {
        if (this->friendly)
        {
            setCursor(this->x, this->y);
            cout << ' ';
            this->y--;
            if (this->skillBullet == 1)
            {
                if (this->x > 1)
                {
                    this->x -= 1;
                }
                else
                {
                    this->removeBullet();
                    return 1;
                }
            }
            if (this->skillBullet == 2)
            {
                if (this->x < 47)
                {
                    this->x += 1;
                }
                else
                {
                    this->removeBullet();
                    return 1;
                }
            }
            setCursor(this->x, this->y);
            cout << this->sprite;
        }
        else
        {
            setCursor(this->x, this->y);
            cout << ' ';
            this->y++;
            setCursor(this->x, this->y);
            cout << this->sprite;
        }
    }
    void removeBullet()
    {
        setCursor(this->x, this->y);
        cout << ' ';
        delete this;
    }

    int collision(playerEntity *curr)
    {
        if (this->friendly)
        {
            if (map[(this->y - 1)][this->x] == 'e')
            {
                int enemyYIdx = findEnemy(this->y - 1);
                for (int i = 0; i < enemyList[enemyYIdx].size(); i++)
                {

                    int collidedX = ((enemyEntity *)enemyList[enemyYIdx][i])->x;
                    if (collidedX + 1 == this->x || collidedX - 1 == this->x || collidedX == this->x)
                    {

                        ((enemyEntity *)enemyList[enemyYIdx][i])->removeEnemy();
                        enemyList[enemyYIdx].erase(enemyList[enemyYIdx].begin() + i);
                        if (enemyList[enemyYIdx].empty())
                        {
                            enemyList.erase(enemyList.begin() + enemyYIdx);
                        }
                        score += 10;
                        this->removeBullet();
                        return 1;
                    }
                }
            }
        }
        else
        {
            if (map[this->y - 1][this->x] == 'p')
            {
                this->removeBullet();
                if (curr->armor > 0)
                {
                    curr->armor -= 1;
                }
                else
                {
                    curr->hp -= this->damage;
                    if (curr->hp <= 0)
                    {
                        gameLost = true;
                        curr->hp = 0;
                    }
                }
                return 1;
            }
        }
    }
};

void playerShoot(playerEntity *curr)
{
    if (curr->bullets == 0 || curr->reloading)
    {
        return;
    }

    curr->bullets--;
    bulletList.push_back(new bulletEntity(true, curr->x + ship.core[0], (curr->y) - 1, 1));
}

void skill(playerEntity *curr)
{
    if (curr->bullets < 3 || curr->reloading)
    {
        return;
    }
    if (curr->energy < 30)
    {
        alert("Not enough Energy");
        return;
    }
    curr->bullets -= 3;
    curr->energy -= 30;
    bulletList.push_back(new bulletEntity(true, curr->x + ship.core[0] - 1, (curr->y) - 1, 1, 1));
    bulletList.push_back(new bulletEntity(true, curr->x + ship.core[0], (curr->y) - 1, 1, 0));
    bulletList.push_back(new bulletEntity(true, curr->x + ship.core[0] + 1, (curr->y) - 1, 1, 2));
}

void enemyShoot(enemyEntity *curr)
{
    bulletList.push_back(new bulletEntity(false, curr->x, curr->y + 1, curr->damage));
}

void enemyCheck(int time, enemyEntity *curr)
{
    if (time >= curr->nextShot)
    {
        enemyShoot(curr);
        curr->nextShot = 1000;
    }
    if (time == 1)
    {
        curr->nextShot = (rand() % 50) + 150;
    }
}

int loadMap()
{

    ifstream mapString("assets/game.txt");
    string row;
    while (getline(mapString, row))
    {
        map.push_back(row);
    }
    return 0;
}

void loadShip()
{
    string path = "assets/space_" + intToString(ship.type + 1) + ".txt";
    ifstream sprite(path.c_str());
    string row;
    while (getline(sprite, row))
    {
        shipSprite.push_back(row);
    }
    sprite.close();

    for (int i = 1; i < 4; i++)
    {
        path = "assets/enemy_" + intToString(i) + ".txt";
        ifstream enemy(path.c_str());
        string enemySprite;
        while (getline(enemy, enemySprite))
        {
            enemySprites.push_back(enemySprite);
        }
        enemy.close();
    }
}

void move(int X, int Y, playerEntity *curr)
{
    if (curr->x + X <= 0 || curr->y + Y < 0 || curr->x + shipSize[ship.type][0] + X >= 48 || curr->y + shipSize[ship.type][1] + Y >= 22)
    {
        return;
    }

    for (int i = 0; i < shipSize[ship.type][1]; i++)
    {
        for (int j = 0; j < shipSize[ship.type][0]; j++)
        {
            setCursor(j + curr->x, i + curr->y);
            map[i + curr->y][j + curr->x] = ' ';
            cout << ' ';
        }
    }
    curr->x += X;
    curr->y += Y;
    for (int i = 0; i < shipSize[ship.type][1]; i++)
    {
        for (int j = 0; j < shipSize[ship.type][0]; j++)
        {
            setCursor(j + curr->x, i + curr->y);
            map[i + curr->y][j + curr->x] = 'p';
            cout << shipSprite[i][j];
        }
    }
}

void useItem(int item, playerEntity *curr)
{
    switch (item)
    {
    case 1:
        if (p.items[0] > 0)
        {
            curr->hp += 50;
            p.items[0] -= 1;
            alert("You used a potion to restore 50 HP");
        }
        else
        {
            alert("You don't have enough potions");
        }
        break;
    case 2:
        if (p.items[1] > 0)
        {
            curr->hp = curr->maxHp;
            p.items[1] -= 1;
            alert("You used a max potion to restore to full HP");
        }
        else
        {
            alert("You don't have enough max potions");
        }
        break;
    case 3:
        if (p.items[2] > 0)
        {
            curr->energy += 50;
            p.items[2] -= 1;
            alert("You used an energy drink to replenish 50 Energy");
        }
        else
        {
            alert("You don't have enough energy drinks");
        }
        break;
    case 4:
        if (p.items[3] > 0)
        {
            curr->energy += curr->maxEnergy;
            p.items[3] -= 1;
            alert("You used a max energy drink to replenish your energy");
        }
        else
        {
            alert("You don't have enough max energy drinks");
        }
        break;
    }
}

void bomb(vector<pair<int, int> > &q, int currLength, char c)
{
    while (!q.empty())
    {
        while (currLength)
        {
            pair<int, int> curr = q.front();
            q.erase(q.begin());
            currLength--;
            int x = curr.first;
            int y = curr.second;
            if (x <= 0 || y <= 0 || x >= 48 || y >= 21 || visited[y][x])
            {
                continue;
            }
            visited[y][x] = true;
            setCursor(x, y);
            cout << c;
            q.push_back(pair<int, int>(x + 1, y));
            q.push_back(pair<int, int>(x - 1, y));
            q.push_back(pair<int, int>(x, y + 1));
            q.push_back(pair<int, int>(x, y - 1));
        }
        currLength = q.size();
    }
    return;
}

void renderMap(playerEntity *curr)
{
    for (int i = 0; i < map.size(); i++)
    {
        cout << map[i] << endl;
    }
    for (int i = 0; i < shipSize[ship.type][1]; i++)
    {
        for (int j = 0; j < shipSize[ship.type][0]; j++)
        {
            setCursor(j + curr->x, i + curr->y);
            map[i + curr->y][j + curr->x] = shipSprite[i][j] != ' ' ? 'p' : ' ';
            cout << shipSprite[i][j];
        }
    }

    return;
}

void spawnEnemy()
{

    if (enemyList.size() >= 21)
    {
        gameLost = true;
        return;
    }

    vector<vector<void *> > tmp;
    // for (int i = 0; i < enemyList.size();i++)
    // {
    //     vector<void*> temp = enemyList.front();
    //     for (int j = 0;j<temp.size();j++)
    //     {
    //         setCursor(70, (i*temp.size())+j);
    //         cout << i << " " << j << "  " <<((enemyEntity *)temp[i])->x << " | " << ((enemyEntity *)temp[i])->y;
    //     }
    // }
    while (!enemyList.empty())
    {
        // debug("MOVE");
        vector<void *> temp = enemyList.front();
        // debug("front  "+intToString(temp.size()));
        enemyList.erase(enemyList.begin());
        for (int i = 0; i < temp.size(); i++)
        {
            ((enemyEntity *)temp[i])->move(0, 1);
        }

        tmp.push_back(temp);
    }
    enemyList = tmp;

    // for (int i = 0; i < enemyList.size();i++)
    // {
    //     vector<void*> temp = enemyList.front();
    //     for (int j = 0;j<temp.size();j++)
    //     {
    //         setCursor(100, (i*temp.size())+j);
    //         cout << i << " " << j << "  " <<((enemyEntity *)temp[i])->x << " | " << ((enemyEntity *)temp[i])->y;
    //     }
    // }
    tmp.clear();

    int x = (rand() % 38) + 2;
    vector<void *> tmp1;
    for (int i = 0; i < 3; i++)
    {
        tmp1.push_back(new enemyEntity((rand() % 3) + 1, (x + (i * 3)) + 1));
    }

    enemyList.insert(enemyList.begin(), tmp1);
    tmp1.clear();

    // for (int i = 0; i < enemyList.size();i++)
    // {
    //     for (int j = 0;j<enemyList[i].size();j++)
    //     {
    //         setCursor(70, (i*enemyList[i].size())+j);
    //         cout << i << " " << j << "  " <<((enemyEntity *)enemyList[i][j])->x << " | " << ((enemyEntity *)enemyList[i][j])->y;
    //     }
    // }
}

void shiftEnemy()
{
    vector<vector<void *> > temp;
    while (!enemyList.empty())
    {
        vector<void *> currGroup = enemyList.front();
        int dir = 1 + (-2 * (rand() % 2));
        if (dir > 0)
        {
            for (int i = currGroup.size() - 1; i >= 0; i--)
            {
                ((enemyEntity *)currGroup[i])->move(dir, 0);
            }
        }
        else
        {
            for (int i = 0; i < currGroup.size(); i++)
            {
                ((enemyEntity *)currGroup[i])->move(dir, 0);
            }
        }
        enemyList.erase(enemyList.begin());
        temp.push_back(currGroup);
    }
    enemyList = temp;
}

void printText(playerEntity *curr)
{
    setCursor(52, 6);
    cout << "C space Invader";
    setCursor(52, 7);
    cout << ship.name;
    setCursor(52, 9);
    cout << "Score : " << score;
    setCursor(52, 10);
    cout << "Bullets " << curr->bullets << "/10  ";
    setCursor(52, 12);
    cout << "HP [" << curr->hp << "/" << curr->maxHp << "]";
    setCursor(52, 13);
    cout << "Armor [" << curr->armor << "/" << curr->maxArmor << "]";
    setCursor(52, 14);
    cout << "Energy [" << curr->energy << "/" << curr->maxEnergy << "]";
}

int gamePause()
{
    setCursor(0, 0);
    cout << "Game Paused" << endl;
    cout << "===================" << endl;
    cout << "Current Score : " << score << endl;
    cout << "Exp gained  : " << (score * 3) / 4 << endl;
    cout << "Do you want to continue ? [y/n]" << endl;
    cout << "(Progress won't be saved if you quit now)" << endl;
    int key;
    while (key = getch())
    {
        switch (key)
        {
        case 'y':
            return 1;
        case 'n':
            gameEnd = true;
            return 0;
        default:
            break;
        }
    }
    return 0;
}

void lose()
{
    setCursor(0, 0);
    cout << "You Lose" << endl;
    cout << "===================" << endl;
    cout << "Final Score : " << score << endl;
    cout << "Exp gained  : " << (score * 3) / 4 << endl;
    cout << "Press Enter to go back to Lobby..";

    vector<string> scoreStrings;
    string tempString;
    ifstream scores("database/score.dat");
    bool found = false;
    while (getline(scores, tempString))
    {
        string res;
        vector<string> scoreData = split(tempString, '#');
        if (scoreData[0].compare(p.info[0]) == 0)
        {
            scoreData[1] = intToString(max(stringToInt(scoreData[1]), score));
            found = true;
        }
        res = join(scoreData, '#');
        scoreStrings.push_back(res);
    }
    if (!found)
    {
        scoreStrings.push_back(p.info[0] + "#" + intToString(score));
    }
    scores.close();
    fstream scoreWrite("database/score.dat");
    while (!scoreStrings.empty())
    {
        scoreWrite << scoreStrings.front() << endl;
        scoreStrings.erase(scoreStrings.begin());
    }
    scoreWrite.close();
    p.info[3] = intToString(stringToInt(p.info[3])+(score / 100));
    p.info[2] = intToString(score % 100);
    while (getch() != '\r')
    {
    }
    return;
}

void startGame()
{
    
    score = 0;
    system("cls");
    playerEntity *curr = new playerEntity(stringToInt(p.info[4]), stringToInt(p.info[5]), stringToInt(p.info[6]), stringToInt(p.info[7]));
    loadMap();
    loadShip();
    renderMap(curr);
    printText(curr);
    gameEnd = false;
    gameLost = false;
    while (!gameLost && !gameEnd)
    {
        move(0, 0, curr);
        if (kbhit())
        {
            switch (getch())
            {
            case 'w':
                move(0, -1, curr);
                break;
            case 'a':
                move(-1, 0, curr);
                break;
            case 's':
                move(0, 1, curr);
                break;
            case 'd':
                move(1, 0, curr);
                break;
            case 32:
                playerShoot(curr);
                break;
            case 'r':
                curr->reload(gameTime);
                break;
            case 'f': // skill
                skill(curr);
                break;
            case 'g': // bomb
                if (p.items[4] > 0)
                {
                    visited = vector<vector<bool> >(22, vector<bool>(49, false));
                    vector<pair<int, int> > q;
                    q.push_back(pair<int, int>(curr->x + ship.core[0], curr->y + ship.core[1]));
                    p.items[4] -= 1;
                    for (int i = 0; i < enemyList.size(); i++)
                    {
                        for (int j = 0; j < enemyList[i].size(); j++)
                        {
                            ((enemyEntity *)enemyList[i][j])->removeEnemy();
                        }
                    }
                    enemyList.clear();
                    alert("Boom!");
                    bomb(q, 1, '.');
                    visited = vector<vector<bool> >(22, vector<bool>(49, false));
                    q.push_back(pair<int, int>(curr->x + ship.core[0], curr->y + ship.core[1]));
                    bomb(q, 1, ' ');
                    visited.clear();
                }
                else
                {
                    alert("Not enough bombs");
                }

                break;
            case 'q': // quit
                system("cls");
                int res;
                res = gamePause();
                system("cls");
                if (res == 1)
                {
                    renderMap(curr);
                    printText(curr);
                }
                break;
            case '1':
                useItem(1, curr);
                break;
            case '2':
                useItem(2, curr);
                break;
            case '3':
                useItem(3, curr);
                break;
            case '4':
                useItem(4, curr);
                break;
            }
        }
        if (spawnTime > 0 && spawnTime % 150 == 0)
        {
            spawnEnemy();
        }
        if (shiftTime % 200 == 0)
        {
            shiftEnemy();
        }

        vector<void *> tmp;

        while (!bulletList.empty())
        {
            bulletEntity *ptr = (bulletEntity *)bulletList.back();
            bulletList.pop_back();
            if ((ptr->y <= 0 || ptr->y >= 20))
            {
                ptr->removeBullet();
                continue;
            }
            int res = ptr->collision(curr);
            if (res == 1) // bullet collided
            {
                continue;
            }
            if (ptr->moveBullet() == 1) // bullet deleted
            {
                continue;
            }
            tmp.push_back(ptr);
        }
        bulletList = tmp;
        for (int i = 0; i < enemyList.size(); i++)
        {
            for (int j = 0; j < enemyList[i].size(); j++)
            {
                enemyCheck(shiftTime, (enemyEntity *)enemyList[i][j]);
                ((enemyEntity *)enemyList[i][j])->move(0, 0);
            }
        }

        Sleep(20);
        spawnTime = (spawnTime % 150) + 1;
        shiftTime = (shiftTime % 200) + 1;
        gameTime = (gameTime % 50) + 1;

        setCursor(52, 14);
        cout << "Energy [" << (int)curr->energy << "/" << curr->maxEnergy << "]";
        curr->energy = min((double)curr->maxEnergy, curr->energy + 0.1);
        if (curr->reloading)
        {
            if (curr->reloadStart == gameTime)
            {
                curr->reloading = false;
                curr->bullets = 10;
                curr->reloadStart = 0;
                setCursor(52, 15);
                cout << "                                            ";
            }
        }
        setCursor(52, 9);
        cout << "Score : " << score;
        setCursor(52, 10);
        cout << "Bullets " << curr->bullets << "/10  ";
        setCursor(52, 12);
        cout << "HP [" << curr->hp << "/" << curr->maxHp << "]    ";
        setCursor(52, 13);
        cout << "Armor [" << curr->armor << "/" << curr->maxArmor << "]   ";
        if (gameTime % 50 == 0)
        {
            alert("                                                     ");
        }
    }
    if (gameLost)
    {
        system("cls");
        lose();
    }
    system("cls");
    delete curr;
    map.clear();
    bulletList.clear();
    enemyList.clear();
    shipSprite.clear();
    return;
};