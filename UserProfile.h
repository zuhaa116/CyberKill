#ifndef USERPROFILE_H
#define USERPROFILE_H

#include <string>

class UserProfile {
private:
    std::string name;
    int followers;
    int privacyScore;

public:
    UserProfile(std::string playerName, int initialFollowers, int initialScore);
    
    std::string getName() const;
    int getFollowers() const;
    int getScore() const;
    
    // Overload 1: just delta
    void updateScore(int delta);
    
    // Overload 2: delta with reason (for logging)
    void updateScore(int delta, std::string reason);
    
    // Clamp score between 0 and 100
    void clampScore();
};

#endif
