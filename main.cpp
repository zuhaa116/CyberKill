#include <iostream>
#include <string>
#include "AppDashboard.h"

using namespace std;

int main(int argc, char* argv[]) {
    cout << "================================" << endl;
    cout << "  CyberKill - Privacy Simulator" << endl;
    cout << "================================" << endl;
    cout << endl;
    
    string playerName;
    cout << "Enter your username: ";
    getline(cin, playerName);
    
    if (playerName.empty()) {
        playerName = "Player";
    }
    
    cout << "Starting game for " << playerName << "..." << endl << endl;
    
    AppDashboard dashboard(playerName);
    dashboard.show();
    
    Fl::run();
    
    return 0;
}
