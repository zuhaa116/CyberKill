#include "PrivacyManager.h"
#include <cstdlib>
#include <fstream>
#include <iostream>

PrivacyManager::PrivacyManager(std::string playerName)
    : user(playerName, 1250, 50), settings(false, false, false),
      roundCount(0), threatsBlocked(0), threatsAccepted(0),
      gameOver(false), gameWon(false) {
    std::srand(std::time(0));
}

PrivacyManager::~PrivacyManager() {
    for (size_t i = 0; i < currentThreats.size(); ++i) {
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

std::vector<Threat*> PrivacyManager::getCurrentThreats() const {
    return currentThreats;
}

void PrivacyManager::spawnThreats() {
    // Remove previous threats
    for (size_t i = 0; i < currentThreats.size(); ++i) {
        delete currentThreats[i];
    }
    currentThreats.clear();
    
    int type1 = std::rand() % 4;
    int type2 = std::rand() % 4;
    while (type2 == type1) {
        type2 = std::rand() % 4;
    }
    
    std::vector<int> types = {type1, type2};
    for (int type : types) {
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
    if (threatIndex < 0 || threatIndex >= static_cast<int>(currentThreats.size())) {
        return;
    }
    
    Threat* threat = currentThreats[threatIndex];
    bool isCorrect = (chosenOptionA == threat->isOptionACorrect());
    
    if (isCorrect) {
        int reward = threat->getReward();
        user.updateScore(reward, std::string("Threat resolved correctly: ") + threat->getType());
        threatsBlocked++;
        addLog(std::string("+ Blocked ") + threat->getType());
    } else {
        int penalty = threat->getPenalty();
        user.updateScore(-penalty, std::string("Threat mishandled: ") + threat->getType());
        threatsAccepted++;
        addLog(std::string("- Accepted ") + threat->getType());
    }
    
    user.clampScore();
    
    // Remove threat from the list but do not delete here because destructor handles cleanup
    currentThreats.erase(currentThreats.begin() + threatIndex);
}

bool PrivacyManager::areAllThreatsResolved() const {
    return currentThreats.size() == 0;
}

int PrivacyManager::toggleSetting(int index) {
    bool newState = settings.toggleSetting(index);
    std::string settingName;
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
    
    user.updateScore(scoreDelta, std::string("Setting toggled: ") + settingName);
    user.clampScore();
    
    addLog(settingName + (newState ? " enabled" : " disabled"));
    return scoreDelta;
}

void PrivacyManager::addLog(std::string action) {
    activityLog.push_back(action);
}

std::vector<std::string> PrivacyManager::getLastActivityLog(int count) const {
    std::vector<std::string> result;
    int start = static_cast<int>(activityLog.size()) - count;
    if (start < 0) start = 0;
    
    for (int i = start; i < static_cast<int>(activityLog.size()); ++i) {
        result.push_back(activityLog[i]);
    }
    return result;
}

void PrivacyManager::nextRound() {
    roundCount++;
    
    if (roundCount % 3 == 0 && roundCount > 0) {
        user.updateScore(-6, std::string("Round pressure penalty"));
        user.clampScore();
        addLog(std::string("Security degrading ( -6 )"));
    }
    
    spawnThreats();
}

void PrivacyManager::applyRoundPenalty() {
    user.updateScore(-6, std::string("Inactivity penalty"));
    user.clampScore();
}

void PrivacyManager::endGame(bool won) {
    gameOver = true;
    gameWon = won;
    saveGameLog();
}

void PrivacyManager::saveGameLog() {
    std::ofstream file("cyberkill_log.txt", std::ios::app);
    if (file.is_open()) {
        std::string result = gameWon ? "WIN" : "LOSS";
        file << "Player: " << user.getName() << " | Score: " << user.getScore()
             << "% | Rounds: " << roundCount << " | Blocked: " << threatsBlocked
             << " | Accepted: " << threatsAccepted << " | Result: " << result << std::endl;
        file.close();
    }
}
