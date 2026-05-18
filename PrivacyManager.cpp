#include "PrivacyManager.h"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

PrivacyManager::PrivacyManager(string playerName)
    : user(playerName, 1250, 50), settings(false, false, false),
      roundCount(0), threatsBlocked(0), threatsAccepted(0),
      gameOver(false), gameWon(false) {
    srand(time(0));
}

PrivacyManager::~PrivacyManager() {
    for (int i = 0; i < currentThreats.size(); i++) {
        delete currentThreats[i];
    }
    currentThreats.clear();
}

bool PrivacyManager::isGameOver() const {
    return gameOver;
}

bool PrivacyManager::didWin() const {
    return gameWon;
}

int PrivacyManager::getRoundCount() const {
    return roundCount;
}

int PrivacyManager::getThreatsBlocked() const {
    return threatsBlocked;
}

int PrivacyManager::getThreatsAccepted() const {
    return threatsAccepted;
}

UserProfile* PrivacyManager::getUser() {
    return &user;
}

PrivacySettings* PrivacyManager::getSettings() {
    return &settings;
}

vector<Threat*> PrivacyManager::getCurrentThreats() const {
    return currentThreats;
}

void PrivacyManager::spawnThreats() {
    // Clear old threats
    for (int i = 0; i < currentThreats.size(); i++) {
        delete currentThreats[i];
    }
    currentThreats.clear();
    
    // Spawn 2 different threat types
    int type1 = rand() % 4;
    int type2 = rand() % 4;
    
    while (type2 == type1) {
        type2 = rand() % 4;
    }
    
    // Create threats by type
    for (int type : vector<int>{type1, type2}) {
        if (type == 0) {
            currentThreats.push_back(new FakeFriendRequest());
        } else if (type == 1) {
            currentThreats.push_back(new SuspiciousLogin());
        } else if (type == 2) {
            currentThreats.push_back(new DataLeak());
        } else {
            currentThreats.push_back(new MaliciousApp());
        }
    }
}

void PrivacyManager::resolveThreat(int threatIndex, bool chosenOptionA) {
    if (threatIndex < 0 || threatIndex >= currentThreats.size()) {
        return;
    }
    
    Threat* threat = currentThreats[threatIndex];
    bool isCorrect = (chosenOptionA == threat->isOptionACorrect());
    
    if (isCorrect) {
        int reward = threat->getReward();
        user.updateScore(reward, "Threat resolved correctly: " + threat->getType());
        threatsBlocked++;
        addLog("+ Blocked " + threat->getType());
    } else {
        int penalty = threat->getPenalty();
        user.updateScore(-penalty, "Threat mishandled: " + threat->getType());
        threatsAccepted++;
        addLog("- Accepted " + threat->getType());
    }
    
    user.clampScore();
    
    // Remove this threat
    currentThreats.erase(currentThreats.begin() + threatIndex);
}

bool PrivacyManager::areAllThreatsResolved() const {
    return currentThreats.size() == 0;
}

int PrivacyManager::toggleSetting(int index) {
    bool newState = settings.toggleSetting(index);
    string settingName;
    int scoreDelta = 0;
    
    if (index == 0) {
        settingName = "2FA";
        scoreDelta = newState ? 14 : -14;
    } else if (index == 1) {
        settingName = "Private Profile";
        scoreDelta = newState ? 10 : -10;
    } else if (index == 2) {
        settingName = "App Restrictions";
        scoreDelta = newState ? 8 : -8;
    }
    
    user.updateScore(scoreDelta, "Setting toggled: " + settingName);
    user.clampScore();
    
    addLog(settingName + " " + (newState ? "enabled" : "disabled"));
    return scoreDelta;
}

void PrivacyManager::addLog(string action) {
    activityLog.push_back(action);
}

vector<string> PrivacyManager::getLastActivityLog(int count) const {
    vector<string> result;
    int start = (int)activityLog.size() - count;
    if (start < 0) start = 0;
    
    for (int i = start; i < activityLog.size(); i++) {
        result.push_back(activityLog[i]);
    }
    return result;
}

void PrivacyManager::nextRound() {
    roundCount++;
    
    if (roundCount % 3 == 0 && roundCount > 0) {
        user.updateScore(-6, "Round pressure penalty");
        user.clampScore();
        addLog("Security degrading (-6)");
    }
    
    spawnThreats();
}

void PrivacyManager::applyRoundPenalty() {
    user.updateScore(-6, "Inactivity penalty");
    user.clampScore();
}

void PrivacyManager::endGame(bool won) {
    gameOver = true;
    gameWon = won;
    saveGameLog();
}

void PrivacyManager::saveGameLog() {
    ofstream file("cyberkill_log.txt", ios::app);
    if (file.is_open()) {
        string result = gameWon ? "WIN" : "LOSS";
        file << "Player: " << user.getName() << " | Score: " << user.getScore()
             << "% | Rounds: " << roundCount << " | Blocked: " << threatsBlocked
             << " | Accepted: " << threatsAccepted << " | Result: " << result << endl;
        file.close();
    }
}
