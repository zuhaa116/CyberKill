#include "UserProfile.h"

using namespace std;

UserProfile::UserProfile(string playerName, int initialFollowers, int initialScore)
    : name(playerName), followers(initialFollowers), privacyScore(initialScore) {
}

string UserProfile::getName() const {
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

void UserProfile::updateScore(int delta, string reason) {
    privacyScore += delta;
    // The reason is logged by PrivacyManager, we just apply the delta here
}

void UserProfile::clampScore() {
    if (privacyScore < 0) privacyScore = 0;
    if (privacyScore > 100) privacyScore = 100;
}
