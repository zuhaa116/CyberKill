#include "PrivacySettings.h"

using namespace std;

PrivacySettings::PrivacySettings(bool initial2FA, bool initialPrivate, bool initialAppsRestricted)
    : twoFactor(initial2FA), profilePrivate(initialPrivate), appsRestricted(initialAppsRestricted) {
}

bool PrivacySettings::getTwoFactor() const {
    return twoFactor;
}

bool PrivacySettings::getProfilePrivate() const {
    return profilePrivate;
}

bool PrivacySettings::getAppsRestricted() const {
    return appsRestricted;
}

bool PrivacySettings::toggleSetting(int index) {
    if (index == 0) {
        twoFactor = !twoFactor;
        return twoFactor;
    } else if (index == 1) {
        profilePrivate = !profilePrivate;
        return profilePrivate;
    } else if (index == 2) {
        appsRestricted = !appsRestricted;
        return appsRestricted;
    }
    return false;
}

bool PrivacySettings::toggleSetting(string name) {
    if (name == "2fa") {
        twoFactor = !twoFactor;
        return twoFactor;
    } else if (name == "private") {
        profilePrivate = !profilePrivate;
        return profilePrivate;
    } else if (name == "apps") {
        appsRestricted = !appsRestricted;
        return appsRestricted;
    }
    return false;
}
