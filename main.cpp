#include <iostream>
#include <string>
#include "AppDashboard.h"

int main(int argc, char* argv[]) {
    std::cout << "================================" << std::endl;
    std::cout << "  CyberKill - Privacy Simulator" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << std::endl;
    
    std::string playerName;
    std::cout << "Enter your username: ";
    std::getline(std::cin, playerName);
    
    if (playerName.empty()) {
        playerName = "Player";
    }
    
    std::cout << "Starting game for " << playerName << "..." << std::endl << std::endl;
    
    AppDashboard dashboard(playerName);
    dashboard.show();
    
    Fl::run();
    
    return 0;
}
