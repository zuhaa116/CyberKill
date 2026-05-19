#ifndef THREAT_H
#define THREAT_H

#include <string>
#include <cstdlib>
#include <ctime>

// Abstract base class
class Threat {
protected:
    bool correctIsA;

public:
    Threat() {
        correctIsA = (rand() % 2 == 0);
    }
    virtual ~Threat() {}
    
    virtual std::string getType() const = 0;
    virtual std::string getDescription() const = 0;
    virtual std::string getDetail() const = 0;
    virtual std::string getOptionA() const = 0;
    virtual std::string getOptionB() const = 0;
    virtual int getPenalty() const = 0;
    virtual int getReward() const = 0;
    virtual bool isOptionACorrect() const = 0;
    virtual void display() const = 0;
};

// ============================================
// FAKE FRIEND REQUEST
// ============================================
class FakeFriendRequest : public Threat {
private:
    int penalty;
    int reward;

public:
    FakeFriendRequest() : Threat() {
        penalty = 13;
        reward = 9;
    }
    
    std::string getType() const override {
        return "Fake Friend Request";
    }
    
    std::string getDescription() const override {
        return "Suspicious profile sent you a friend request.";
    }
    
    std::string getDetail() const override {
        return "The profile has no mutual friends and a stock photo.";
    }
    
    std::string getOptionA() const override {
        return correctIsA ? "Dismiss request" : "Confirm follow request";
    }
    
    std::string getOptionB() const override {
        return correctIsA ? "Confirm follow request" : "Dismiss request";
    }
    
    int getPenalty() const override {
        return penalty;
    }
    
    int getReward() const override {
        return reward;
    }
    
    bool isOptionACorrect() const override {
        return correctIsA;
    }
    
    void display() const override {}
};

// ============================================
// SUSPICIOUS LOGIN
// ============================================
class SuspiciousLogin : public Threat {
private:
    int penalty;
    int reward;

public:
    SuspiciousLogin() : Threat() {
        penalty = 22;
        reward = 14;
    }
    
    std::string getType() const override {
        return "Suspicious Login";
    }
    
    std::string getDescription() const override {
        return "Login attempt from unknown location.";
    }
    
    std::string getDetail() const override {
        return "Beijing, China at 3:47 AM. Device: Android.";
    }
    
    std::string getOptionA() const override {
        return correctIsA ? "Deny this session" : "Approve this session";
    }
    
    std::string getOptionB() const override {
        return correctIsA ? "Approve this session" : "Deny this session";
    }
    
    int getPenalty() const override {
        return penalty;
    }
    
    int getReward() const override {
        return reward;
    }
    
    bool isOptionACorrect() const override {
        return correctIsA;
    }
    
    void display() const override {}
};

// ============================================
// DATA LEAK
// ============================================
class DataLeak : public Threat {
private:
    int penalty;
    int reward;

public:
    DataLeak() : Threat() {
        penalty = 28;
        reward = 18;
    }
    
    std::string getType() const override {
        return "Data Leak";
    }
    
    std::string getDescription() const override {
        return "Your password may have been leaked in a data breach.";
    }
    
    std::string getDetail() const override {
        return "A security website detected your email on a leaked database.";
    }
    
    std::string getOptionA() const override {
        return correctIsA ? "Reset my password" : "Keep existing credentials";
    }
    
    std::string getOptionB() const override {
        return correctIsA ? "Keep existing credentials" : "Reset my password";
    }
    
    int getPenalty() const override {
        return penalty;
    }
    
    int getReward() const override {
        return reward;
    }
    
    bool isOptionACorrect() const override {
        return correctIsA;
    }
    
    void display() const override {}
};

// ============================================
// MALICIOUS APP
// ============================================
class MaliciousApp : public Threat {
private:
    int penalty;
    int reward;

public:
    MaliciousApp() : Threat() {
        penalty = 16;
        reward = 10;
    }
    
    std::string getType() const override {
        return "Malicious App";
    }
    
    std::string getDescription() const override {
        return "An app is requesting permission to access your data.";
    }
    
    std::string getDetail() const override {
        return "Unknown publisher, asks for full account access and location.";
    }
    
    std::string getOptionA() const override {
        return correctIsA ? "Revoke app access" : "Allow app permissions";
    }
    
    std::string getOptionB() const override {
        return correctIsA ? "Allow app permissions" : "Revoke app access";
    }
    
    int getPenalty() const override {
        return penalty;
    }
    
    int getReward() const override {
        return reward;
    }
    
    bool isOptionACorrect() const override {
        return correctIsA;
    }
    
    void display() const override {}
};

#endif
