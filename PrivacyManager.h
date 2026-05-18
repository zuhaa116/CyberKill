#ifndef PRIVACYMANAGER_H
#define PRIVACYMANAGER_H

#include <vector>
#include <string>
#include "UserProfile.h"
#include "PrivacySettings.h"
#include "Threat.h"

class PrivacyManager {
private:
    UserProfile user;
    PrivacySettings settings;
    std::vector<Threat*> currentThreats;
    std::vector<std::string> activityLog;
    int roundCount;
    int threatsBlocked;
    int threatsAccepted;
    bool gameOver;
    bool gameWon;

public:
    PrivacyManager(std::string playerName);
    ~PrivacyManager();
    
    // Game state
    bool isGameOver() const;
    bool didWin() const;
    int getRoundCount() const;
    int getThreatsBlocked() const;
    int getThreatsAccepted() const;
    
    // User access
    UserProfile* getUser();
    PrivacySettings* getSettings();
    
    // Threat management
    std::vector<Threat*> getCurrentThreats() const;
    void spawnThreats();
    void resolveThreat(int threatIndex, bool chosenOptionA);
    bool areAllThreatsResolved() const;
    
    // Settings changes
    int toggleSetting(int index);
    
    // Activity logging
    void addLog(std::string action);
    std::vector<std::string> getLastActivityLog(int count) const;
    
    // Game progression
    void nextRound();
    void applyRoundPenalty();
    void endGame(bool won);
    void saveGameLog();
};

#endif
