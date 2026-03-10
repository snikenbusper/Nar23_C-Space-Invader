#include "lib.hpp"

int inputPlayer()
{
    
    string username;
    cout << "Input new username : ";
    cin >> username;
    
    

    if(username.length()<=3)
    {
        errorMessage = "Username must be longer than 3 characters";
        return -1;
    
    }

    ifstream players("database/player.dat");
    string playerInfo;
    while(getline(players, playerInfo))
    {
        if(split(playerInfo, '#')[0] == username)
        {
            errorMessage = "Username taken";
            return -1;
        }
    }
    players.close();
    //add data

    ofstream playerData("database/player.dat", std::ios_base::app);
    playerData << username << "#0#0#0#100#50#1#1" << "\n";
    cout << "Successfully added username to database (press any key to continue)" << endl;
    getch();
    playerData.close();
    return 0;
}
