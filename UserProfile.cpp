#include "UserProfile.h"

UserProfile::UserProfile(std::string playerName, int initialFollowers, int initialScore)
    : name(playerName), followers(initialFollowers), privacyScore(initialScore) {
}

std::string UserProfile::getName() const {
    return name;
}

int UserProfile::getFollowers() const {
    return followers;
}

int UserProfile::getScore() const {
    return privacyScore;
}

void UserProfile::updateScore(int delta) {
    privacyScore += delta;
}

void UserProfile::updateScore(int delta, std::string reason) {
    privacyScore += delta;
    // The reason is stored by PrivacyManager for logging purposes
}

void UserProfile::clampScore() {
    if (privacyScore < 0) privacyScore = 0;
    if (privacyScore > 100) privacyScore = 100;
}
