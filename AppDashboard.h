#ifndef APPDASHBOARD_H
#define APPDASHBOARD_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Scroll.H>
#include <string>
#include <vector>
#include "PrivacyManager.h"

// Custom widget for score bar
class ScoreBar : public Fl_Box {
private:
    int score;

public:
    ScoreBar(int x, int y, int w, int h, int initialScore)
        : Fl_Box(x, y, w, h), score(initialScore) {}
    
    void setScore(int s) {
        score = s;
        redraw();
    }
    
    int getScore() const {
        return score;
    }
    
    void draw() override;
};

// Main dashboard window
class AppDashboard {
private:
    Fl_Window* mainWindow;
    PrivacyManager* manager;
    
    // Top header
    Fl_Box* titleBox;
    Fl_Box* playerNameBox;
    Fl_Box* scorePercentBox;
    ScoreBar* scoreBar;
    Fl_Box* roundInfoBox;
    Fl_Box* statsBox;
    
    // Settings buttons
    Fl_Button* twoFaButton;
    Fl_Button* privateButton;
    Fl_Button* appsButton;
    
    // Activity log
    Fl_Box* logBox;
    
    // Threat display
    Fl_Box* threatsTitle;
    Fl_Box* threat1Box;
    Fl_Button* t1OptionABtn;
    Fl_Button* t1OptionBBtn;
    
    Fl_Box* threat2Box;
    Fl_Button* t2OptionABtn;
    Fl_Button* t2OptionBBtn;
    
    Fl_Box* roundCompleteBox;
    Fl_Button* nextRoundBtn;
    
    Fl_Box* gameOverBox;
    Fl_Button* playAgainBtn;
    
    // State tracking
    int resolvedThreats;
    bool showingGameOver;
    
    // Helper methods
    void loadLastSession();
    void updateDisplay();
    void updateThreatsDisplay();
    void showGameOver(bool won);
    void resetGame();
    
public:
    AppDashboard(std::string playerName);
    ~AppDashboard();
    
    void show();
    
    // FLTK callback bridge
    static void staticCallback2FA(Fl_Widget* w, void* data);
    static void staticCallbackPrivate(Fl_Widget* w, void* data);
    static void staticCallbackApps(Fl_Widget* w, void* data);
    
    static void staticCallbackThreat1A(Fl_Widget* w, void* data);
    static void staticCallbackThreat1B(Fl_Widget* w, void* data);
    static void staticCallbackThreat2A(Fl_Widget* w, void* data);
    static void staticCallbackThreat2B(Fl_Widget* w, void* data);
    
    static void staticCallbackNextRound(Fl_Widget* w, void* data);
    static void staticCallbackPlayAgain(Fl_Widget* w, void* data);
};

#endif
