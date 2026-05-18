#ifndef PRIVACYSETTINGS_H
#define PRIVACYSETTINGS_H

#include <string>

class PrivacySettings {
private:
    bool twoFactor;
    bool profilePrivate;
    bool appsRestricted;

public:
    PrivacySettings(bool initial2FA, bool initialPrivate, bool initialAppsRestricted);
    
    bool getTwoFactor() const;
    bool getProfilePrivate() const;
    bool getAppsRestricted() const;
    
    // Overload 1: toggle by index (0=2FA, 1=private, 2=apps)
    bool toggleSetting(int index);
    
    // Overload 2: toggle by name ("2fa", "private", "apps")
    bool toggleSetting(std::string name);
};

#endif
