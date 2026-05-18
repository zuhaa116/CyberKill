#include "AppDashboard.h"
#include <FL/fl_draw.H>
#include <fstream>
#include <sstream>
#include <cstdio>

using namespace std;

// ============ ScoreBar Draw ============
void ScoreBar::draw() {
    // Background (light grey)
    fl_color(FL_LIGHT2);
    fl_rectf(x(), y(), w(), h());
    
    // Score bar (colored fill)
    int fillWidth = (score * w()) / 100;
    
    if (score >= 75) {
        fl_color(FL_GREEN);
    } else if (score >= 40) {
        fl_color(255, 180, 0);  // Orange
    } else {
        fl_color(FL_RED);
    }
    
    fl_rectf(x(), y(), fillWidth, h());
    
    // Border
    fl_color(FL_BLACK);
    fl_rect(x(), y(), w(), h());
}

// ============ AppDashboard Implementation ============

AppDashboard::AppDashboard(string playerName)
    : resolvedThreats(0), showingGameOver(false) {
    
    manager = new PrivacyManager(playerName);
    
    // Create main window
    mainWindow = new Fl_Window(860, 600, "CyberKill - Social Media Privacy Simulator");
    mainWindow->color(FL_LIGHT2);
    
    // ===== LEFT PANEL =====
    // Title
    titleBox = new Fl_Box(10, 10, 280, 40, "CyberKill");
    titleBox->labelfont(FL_HELVETICA_BOLD);
    titleBox->labelsize(28);
    titleBox->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    
    // Player name and followers
    string playerInfo = playerName + " | " + to_string(manager->getUser()->getFollowers()) + " followers";
    playerNameBox = new Fl_Box(10, 55, 280, 20, playerInfo.c_str());
    playerNameBox->labelfont(FL_HELVETICA);
    playerNameBox->labelsize(12);
    playerNameBox->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    
    // Score percentage
    string scoreStr = to_string(manager->getUser()->getScore()) + "%";
    scorePercentBox = new Fl_Box(10, 80, 280, 15, scoreStr.c_str());
    scorePercentBox->labelfont(FL_HELVETICA);
    scorePercentBox->labelsize(14);
    scorePercentBox->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    
    // Score bar
    scoreBar = new ScoreBar(10, 98, 280, 20, manager->getUser()->getScore());
    scoreBar->box(FL_NO_BOX);
    
    // Round and stats info
    string roundStr = "Round: " + to_string(manager->getRoundCount());
    roundInfoBox = new Fl_Box(10, 125, 280, 15, roundStr.c_str());
    roundInfoBox->labelfont(FL_HELVETICA);
    roundInfoBox->labelsize(12);
    roundInfoBox->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    
    string statsStr = "Blocked: " + to_string(manager->getThreatsBlocked()) 
                    + " | Accepted: " + to_string(manager->getThreatsAccepted());
    statsBox = new Fl_Box(10, 143, 280, 15, statsStr.c_str());
    statsBox->labelfont(FL_HELVETICA);
    statsBox->labelsize(12);
    statsBox->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    
    // Settings buttons
    twoFaButton = new Fl_Button(10, 170, 90, 25, "2FA: OFF");
    twoFaButton->callback(staticCallback2FA, this);
    twoFaButton->color(FL_LIGHT1);
    
    privateButton = new Fl_Button(105, 170, 90, 25, "Private: OFF");
    privateButton->callback(staticCallbackPrivate, this);
    privateButton->color(FL_LIGHT1);
    
    appsButton = new Fl_Button(200, 170, 90, 25, "Apps: OFF");
    appsButton->callback(staticCallbackApps, this);
    appsButton->color(FL_LIGHT1);
    
    // Last session
    lastSessionBox = new Fl_Box(10, 205, 280, 20, "");
    lastSessionBox->labelfont(FL_COURIER);
    lastSessionBox->labelsize(10);
    lastSessionBox->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    
    // Activity log
    logBox = new Fl_Box(10, 230, 280, 350, "");
    logBox->labelfont(FL_COURIER);
    logBox->labelsize(10);
    logBox->align(FL_ALIGN_TOP_LEFT);
    
    // ===== RIGHT PANEL =====
    threatsTitle = new Fl_Box(310, 10, 540, 25, "Active Threats");
    threatsTitle->labelfont(FL_HELVETICA_BOLD);
    threatsTitle->labelsize(16);
    threatsTitle->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    
    // Threat 1
    threat1Box = new Fl_Box(320, 45, 520, 80, "");
    threat1Box->box(FL_DOWN_BOX);
    threat1Box->labelfont(FL_COURIER);
    threat1Box->labelsize(11);
    threat1Box->align(FL_ALIGN_TOP_LEFT);
    
    t1OptionABtn = new Fl_Button(320, 130, 250, 25, "");
    t1OptionABtn->callback(staticCallbackThreat1A, this);
    t1OptionABtn->color(FL_LIGHT1);
    
    t1OptionBBtn = new Fl_Button(575, 130, 265, 25, "");
    t1OptionBBtn->callback(staticCallbackThreat1B, this);
    t1OptionBBtn->color(FL_LIGHT1);
    
    // Threat 2
    threat2Box = new Fl_Box(320, 165, 520, 80, "");
    threat2Box->box(FL_DOWN_BOX);
    threat2Box->labelfont(FL_COURIER);
    threat2Box->labelsize(11);
    threat2Box->align(FL_ALIGN_TOP_LEFT);
    
    t2OptionABtn = new Fl_Button(320, 250, 250, 25, "");
    t2OptionABtn->callback(staticCallbackThreat2A, this);
    t2OptionABtn->color(FL_LIGHT1);
    
    t2OptionBBtn = new Fl_Button(575, 250, 265, 25, "");
    t2OptionBBtn->callback(staticCallbackThreat2B, this);
    t2OptionBBtn->color(FL_LIGHT1);
    
    // Round complete message
    roundCompleteBox = new Fl_Box(320, 285, 520, 30, "");
    roundCompleteBox->labelfont(FL_HELVETICA_BOLD);
    roundCompleteBox->labelsize(14);
    roundCompleteBox->align(FL_ALIGN_CENTER);
    
    // Next round button
    nextRoundBtn = new Fl_Button(620, 325, 120, 35, "Next Round");
    nextRoundBtn->callback(staticCallbackNextRound, this);
    nextRoundBtn->color(FL_LIGHT1);
    nextRoundBtn->hide();
    
    // Game over
    gameOverBox = new Fl_Box(320, 150, 520, 150, "");
    gameOverBox->box(FL_ROUNDED_BOX);
    gameOverBox->labelfont(FL_HELVETICA_BOLD);
    gameOverBox->labelsize(18);
    gameOverBox->align(FL_ALIGN_CENTER | FL_ALIGN_WRAP);
    gameOverBox->hide();
    
    playAgainBtn = new Fl_Button(620, 330, 120, 35, "Play Again");
    playAgainBtn->callback(staticCallbackPlayAgain, this);
    playAgainBtn->color(FL_LIGHT1);
    playAgainBtn->hide();
    
    mainWindow->end();
}

AppDashboard::~AppDashboard() {
    delete manager;
    delete mainWindow;
}

void AppDashboard::show() {
    loadLastSession();
    manager->nextRound();
    updateDisplay();
    mainWindow->show();
}

void AppDashboard::loadLastSession() {
    ifstream file("cyberkill_log.txt");
    if (file.is_open()) {
        string lastLine;
        string line;
        while (getline(file, line)) {
            lastLine = line;
        }
        file.close();
        
        if (!lastLine.empty()) {
            // Parse and display
            string display = "Last: " + lastLine.substr(0, min(70, (int)lastLine.length()));
            if (lastLine.length() > 70) display += "...";
            lastSessionBox->copy_label(display.c_str());
        }
    }
}

void AppDashboard::updateDisplay() {
    // Update score box
    string scoreStr = to_string(manager->getUser()->getScore()) + "%";
    scorePercentBox->copy_label(scoreStr.c_str());
    scoreBar->setScore(manager->getUser()->getScore());
    
    // Update round info
    string roundStr = "Round: " + to_string(manager->getRoundCount());
    roundInfoBox->copy_label(roundStr.c_str());
    
    // Update stats
    string statsStr = "Blocked: " + to_string(manager->getThreatsBlocked()) 
                    + " | Accepted: " + to_string(manager->getThreatsAccepted());
    statsBox->copy_label(statsStr.c_str());
    
    // Update activity log
    vector<string> lastLogs = manager->getLastActivityLog(10);
    string logText = "";
    for (int i = 0; i < lastLogs.size(); i++) {
        logText += lastLogs[i] + "\n";
    }
    logBox->copy_label(logText.c_str());
    
    // Check win/lose
    if (manager->getUser()->getScore() >= 100) {
        showGameOver(true);
    } else if (manager->getUser()->getScore() <= 0) {
        showGameOver(false);
    }
    
    updateThreatsDisplay();
    mainWindow->redraw();
}

void AppDashboard::updateThreatsDisplay() {
    vector<Threat*> threats = manager->getCurrentThreats();
    
    if (threats.size() >= 1) {
        Threat* t1 = threats[0];
        string t1Text = t1->getType() + "\n" + t1->getDescription() + "\n" + t1->getDetail();
        threat1Box->copy_label(t1Text.c_str());
        
        t1OptionABtn->copy_label(t1->getOptionA().c_str());
        t1OptionABtn->show();
        
        t1OptionBBtn->copy_label(t1->getOptionB().c_str());
        t1OptionBBtn->show();
    } else {
        threat1Box->copy_label("");
        t1OptionABtn->hide();
        t1OptionBBtn->hide();
    }
    
    if (threats.size() >= 2) {
        Threat* t2 = threats[1];
        string t2Text = t2->getType() + "\n" + t2->getDescription() + "\n" + t2->getDetail();
        threat2Box->copy_label(t2Text.c_str());
        
        t2OptionABtn->copy_label(t2->getOptionA().c_str());
        t2OptionABtn->show();
        
        t2OptionBBtn->copy_label(t2->getOptionB().c_str());
        t2OptionBBtn->show();
    } else {
        threat2Box->copy_label("");
        t2OptionABtn->hide();
        t2OptionBBtn->hide();
    }
    
    if (manager->areAllThreatsResolved()) {
        roundCompleteBox->copy_label("Round Complete!");
        roundCompleteBox->show();
        nextRoundBtn->show();
    } else {
        roundCompleteBox->hide();
        nextRoundBtn->hide();
    }
}

void AppDashboard::showGameOver(bool won) {
    string resultText = won ? "YOU WIN!\nPrivacy Score: 100%+" : "GAME OVER\nPrivacy Score: 0%";
    gameOverBox->copy_label(resultText.c_str());
    gameOverBox->show();
    playAgainBtn->show();
    
    threat1Box->hide();
    threat2Box->hide();
    t1OptionABtn->hide();
    t1OptionBBtn->hide();
    t2OptionABtn->hide();
    t2OptionBBtn->hide();
    roundCompleteBox->hide();
    nextRoundBtn->hide();
    
    showingGameOver = true;
    mainWindow->redraw();
}

void AppDashboard::resetGame() {
    delete manager;
    manager = new PrivacyManager(manager->getUser()->getName());
    resolvedThreats = 0;
    showingGameOver = false;
    
    threat1Box->show();
    threat2Box->show();
    gameOverBox->hide();
    playAgainBtn->hide();
    
    manager->nextRound();
    updateDisplay();
}

// ============ Callbacks ============

void AppDashboard::staticCallback2FA(Fl_Widget* w, void* data) {
    AppDashboard* dash = (AppDashboard*)data;
    dash->manager->toggleSetting(0);
    
    bool is2FA = dash->manager->getSettings()->getTwoFactor();
    dash->twoFaButton->copy_label(is2FA ? "2FA: ON" : "2FA: OFF");
    dash->twoFaButton->color(is2FA ? FL_GRAY : FL_LIGHT1);
    
    dash->updateDisplay();
}

void AppDashboard::staticCallbackPrivate(Fl_Widget* w, void* data) {
    AppDashboard* dash = (AppDashboard*)data;
    dash->manager->toggleSetting(1);
    
    bool isPrivate = dash->manager->getSettings()->getProfilePrivate();
    dash->privateButton->copy_label(isPrivate ? "Private: ON" : "Private: OFF");
    dash->privateButton->color(isPrivate ? FL_GRAY : FL_LIGHT1);
    
    dash->updateDisplay();
}

void AppDashboard::staticCallbackApps(Fl_Widget* w, void* data) {
    AppDashboard* dash = (AppDashboard*)data;
    dash->manager->toggleSetting(2);
    
    bool isRestricted = dash->manager->getSettings()->getAppsRestricted();
    dash->appsButton->copy_label(isRestricted ? "Apps: ON" : "Apps: OFF");
    dash->appsButton->color(isRestricted ? FL_GRAY : FL_LIGHT1);
    
    dash->updateDisplay();
}

void AppDashboard::staticCallbackThreat1A(Fl_Widget* w, void* data) {
    AppDashboard* dash = (AppDashboard*)data;
    dash->manager->resolveThreat(0, true);
    dash->updateDisplay();
}

void AppDashboard::staticCallbackThreat1B(Fl_Widget* w, void* data) {
    AppDashboard* dash = (AppDashboard*)data;
    dash->manager->resolveThreat(0, false);
    dash->updateDisplay();
}

void AppDashboard::staticCallbackThreat2A(Fl_Widget* w, void* data) {
    AppDashboard* dash = (AppDashboard*)data;
    dash->manager->resolveThreat(0, true);  // threats shift after removal
    dash->updateDisplay();
}

void AppDashboard::staticCallbackThreat2B(Fl_Widget* w, void* data) {
    AppDashboard* dash = (AppDashboard*)data;
    dash->manager->resolveThreat(0, false);
    dash->updateDisplay();
}

void AppDashboard::staticCallbackNextRound(Fl_Widget* w, void* data) {
    AppDashboard* dash = (AppDashboard*)data;
    if (dash->manager->getUser()->getScore() >= 100) {
        dash->manager->endGame(true);
        dash->showGameOver(true);
    } else if (dash->manager->getUser()->getScore() <= 0) {
        dash->manager->endGame(false);
        dash->showGameOver(false);
    } else {
        dash->manager->nextRound();
        dash->updateDisplay();
    }
}

void AppDashboard::staticCallbackPlayAgain(Fl_Widget* w, void* data) {
    AppDashboard* dash = (AppDashboard*)data;
    dash->resetGame();
}
