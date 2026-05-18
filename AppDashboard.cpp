#include "AppDashboard.h"
#include <FL/fl_draw.H>
#include <fstream>
#include <sstream>
#include <cstdio>

using namespace std;

// ============ ScoreBar Draw ============
void ScoreBar::draw() {
    // Background (light grey)
    fl_color(240, 240, 240);
    fl_rectf(x(), y(), w(), h());
    
    // Score bar (colored fill)
    int fillWidth = (score * w()) / 100;
    
    if (score >= 75) {
        fl_color(46, 204, 113);  // Green
    } else if (score >= 40) {
        fl_color(241, 196, 15);  // Gold
    } else {
        fl_color(231, 76, 60);   // Red
    }
    
    fl_rectf(x(), y(), fillWidth, h());
    
    // Border
    fl_color(52, 73, 94);  // Dark blue-grey
    fl_rect(x(), y(), w(), h());
}

// ============ AppDashboard Implementation ============

AppDashboard::AppDashboard(string playerName)
    : resolvedThreats(0), showingGameOver(false) {
    
    manager = new PrivacyManager(playerName);
    
    // Create main window - fullscreen
    mainWindow = new Fl_Window(1400, 800, "CyberKill - Social Media Privacy Simulator");
    mainWindow->color(236, 240, 241);  // Light grey background
    
    // ===== TOP HEADER SECTION =====
    titleBox = new Fl_Box(20, 15, 300, 50, "CyberKill");
    titleBox->labelfont(FL_HELVETICA_BOLD);
    titleBox->labelsize(36);
    titleBox->labelcolor(52, 73, 94);  // Dark blue-grey
    titleBox->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    
    string playerInfo = playerName + " | " + to_string(manager->getUser()->getFollowers()) + " followers";
    playerNameBox = new Fl_Box(20, 68, 350, 20, playerInfo.c_str());
    playerNameBox->labelfont(FL_HELVETICA);
    playerNameBox->labelsize(12);
    playerNameBox->labelcolor(44, 62, 80);
    playerNameBox->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    
    string scoreStr = to_string(manager->getUser()->getScore()) + "%";
    scorePercentBox = new Fl_Box(20, 90, 100, 25, scoreStr.c_str());
    scorePercentBox->labelfont(FL_HELVETICA_BOLD);
    scorePercentBox->labelsize(20);
    scorePercentBox->labelcolor(46, 204, 113);  // Green
    scorePercentBox->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    
    scoreBar = new ScoreBar(130, 90, 240, 25, manager->getUser()->getScore());
    scoreBar->box(FL_BORDER_BOX);
    
    string roundStr = "Round: " + to_string(manager->getRoundCount());
    roundInfoBox = new Fl_Box(400, 68, 150, 20, roundStr.c_str());
    roundInfoBox->labelfont(FL_HELVETICA);
    roundInfoBox->labelsize(14);
    roundInfoBox->labelcolor(44, 62, 80);
    roundInfoBox->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    
    string statsStr = "Blocked: " + to_string(manager->getThreatsBlocked()) 
                    + " | Accepted: " + to_string(manager->getThreatsAccepted());
    statsBox = new Fl_Box(400, 90, 250, 20, statsStr.c_str());
    statsBox->labelfont(FL_HELVETICA);
    statsBox->labelsize(12);
    statsBox->labelcolor(44, 62, 80);
    statsBox->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    
    // Settings buttons
    twoFaButton = new Fl_Button(700, 68, 100, 35, "2FA: OFF");
    twoFaButton->callback(staticCallback2FA, this);
    twoFaButton->color(189, 195, 199);  // Light grey
    twoFaButton->labelfont(FL_HELVETICA);
    twoFaButton->labelsize(11);
    
    privateButton = new Fl_Button(810, 68, 100, 35, "Private: OFF");
    privateButton->callback(staticCallbackPrivate, this);
    privateButton->color(189, 195, 199);
    privateButton->labelfont(FL_HELVETICA);
    privateButton->labelsize(11);
    
    appsButton = new Fl_Button(920, 68, 100, 35, "Apps: OFF");
    appsButton->callback(staticCallbackApps, this);
    appsButton->color(189, 195, 199);
    appsButton->labelfont(FL_HELVETICA);
    appsButton->labelsize(11);
    
    // ===== ACTIVITY LOG (LEFT SIDE) =====
    logBox = new Fl_Box(20, 130, 350, 650, "");
    logBox->box(FL_BORDER_BOX);
    logBox->color(255, 255, 255);  // White
    logBox->labelfont(FL_COURIER);
    logBox->labelsize(11);
    logBox->align(FL_ALIGN_TOP_LEFT);
    
    // ===== THREATS SECTION (CENTER/RIGHT) =====
    threatsTitle = new Fl_Box(390, 130, 980, 30, "Active Threats");
    threatsTitle->labelfont(FL_HELVETICA_BOLD);
    threatsTitle->labelsize(18);
    threatsTitle->labelcolor(52, 73, 94);
    threatsTitle->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    
    // Threat 1
    threat1Box = new Fl_Box(390, 165, 980, 110, "");
    threat1Box->box(FL_BORDER_BOX);
    threat1Box->color(52, 152, 219);  // Nice blue
    threat1Box->labelfont(FL_COURIER);
    threat1Box->labelsize(11);
    threat1Box->labelcolor(255, 255, 255);  // White text
    threat1Box->align(FL_ALIGN_TOP_LEFT | FL_ALIGN_WRAP);
    
    t1OptionABtn = new Fl_Button(390, 280, 485, 35, "");
    t1OptionABtn->callback(staticCallbackThreat1A, this);
    t1OptionABtn->color(46, 204, 113);  // Green
    t1OptionABtn->labelfont(FL_HELVETICA);
    t1OptionABtn->labelsize(11);
    t1OptionABtn->labelcolor(255, 255, 255);
    
    t1OptionBBtn = new Fl_Button(885, 280, 485, 35, "");
    t1OptionBBtn->callback(staticCallbackThreat1B, this);
    t1OptionBBtn->color(231, 76, 60);  // Red
    t1OptionBBtn->labelfont(FL_HELVETICA);
    t1OptionBBtn->labelsize(11);
    t1OptionBBtn->labelcolor(255, 255, 255);
    
    // Threat 2
    threat2Box = new Fl_Box(390, 320, 980, 110, "");
    threat2Box->box(FL_BORDER_BOX);
    threat2Box->color(155, 89, 182);  // Purple
    threat2Box->labelfont(FL_COURIER);
    threat2Box->labelsize(11);
    threat2Box->labelcolor(255, 255, 255);
    threat2Box->align(FL_ALIGN_TOP_LEFT | FL_ALIGN_WRAP);
    
    t2OptionABtn = new Fl_Button(390, 435, 485, 35, "");
    t2OptionABtn->callback(staticCallbackThreat2A, this);
    t2OptionABtn->color(46, 204, 113);  // Green
    t2OptionABtn->labelfont(FL_HELVETICA);
    t2OptionABtn->labelsize(11);
    t2OptionABtn->labelcolor(255, 255, 255);
    
    t2OptionBBtn = new Fl_Button(885, 435, 485, 35, "");
    t2OptionBBtn->callback(staticCallbackThreat2B, this);
    t2OptionBBtn->color(231, 76, 60);  // Red
    t2OptionBBtn->labelfont(FL_HELVETICA);
    t2OptionBBtn->labelsize(11);
    t2OptionBBtn->labelcolor(255, 255, 255);
    
    // Round complete message
    roundCompleteBox = new Fl_Box(390, 480, 980, 50, "");
    roundCompleteBox->box(FL_BORDER_BOX);
    roundCompleteBox->color(46, 204, 113);  // Green
    roundCompleteBox->labelfont(FL_HELVETICA_BOLD);
    roundCompleteBox->labelsize(16);
    roundCompleteBox->labelcolor(255, 255, 255);
    roundCompleteBox->align(FL_ALIGN_CENTER);
    
    // Next round button
    nextRoundBtn = new Fl_Button(1185, 540, 185, 40, "Next Round >>>");
    nextRoundBtn->callback(staticCallbackNextRound, this);
    nextRoundBtn->color(52, 73, 94);  // Dark blue-grey
    nextRoundBtn->labelfont(FL_HELVETICA_BOLD);
    nextRoundBtn->labelsize(12);
    nextRoundBtn->labelcolor(255, 255, 255);
    nextRoundBtn->hide();
    
    // Game over
    gameOverBox = new Fl_Box(450, 250, 850, 200, "");
    gameOverBox->box(FL_ROUNDED_BOX);
    gameOverBox->color(52, 73, 94);  // Dark blue-grey
    gameOverBox->labelfont(FL_HELVETICA_BOLD);
    gameOverBox->labelsize(28);
    gameOverBox->labelcolor(255, 255, 255);
    gameOverBox->align(FL_ALIGN_CENTER | FL_ALIGN_WRAP);
    gameOverBox->hide();
    
    playAgainBtn = new Fl_Button(1185, 540, 185, 40, "Play Again");
    playAgainBtn->callback(staticCallbackPlayAgain, this);
    playAgainBtn->color(46, 204, 113);  // Green
    playAgainBtn->labelfont(FL_HELVETICA_BOLD);
    playAgainBtn->labelsize(12);
    playAgainBtn->labelcolor(255, 255, 255);
    playAgainBtn->hide();
    
    mainWindow->end();
}

AppDashboard::~AppDashboard() {
    delete manager;
    delete mainWindow;
}

void AppDashboard::show() {
    manager->nextRound();
    updateDisplay();
    mainWindow->show();
}

void AppDashboard::loadLastSession() {
    // Not used in new version, but kept for compatibility
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
    vector<string> lastLogs = manager->getLastActivityLog(20);
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
        string t1Text = "⚠ " + t1->getType() + "\n" + t1->getDescription() + "\n" + t1->getDetail();
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
        string t2Text = "⚠ " + t2->getType() + "\n" + t2->getDescription() + "\n" + t2->getDetail();
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
        roundCompleteBox->copy_label("✓ Round Complete!");
        roundCompleteBox->show();
        nextRoundBtn->show();
    } else {
        roundCompleteBox->hide();
        nextRoundBtn->hide();
    }
}

void AppDashboard::showGameOver(bool won) {
    string resultText = won ? "🎉 YOU WIN! 🎉\nPrivacy Score: 100%" : "💀 GAME OVER 💀\nPrivacy Score: 0%";
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
    dash->twoFaButton->color(is2FA ? 46 : 189, is2FA ? 204 : 195, is2FA ? 113 : 199);
    
    dash->updateDisplay();
}

void AppDashboard::staticCallbackPrivate(Fl_Widget* w, void* data) {
    AppDashboard* dash = (AppDashboard*)data;
    dash->manager->toggleSetting(1);
    
    bool isPrivate = dash->manager->getSettings()->getProfilePrivate();
    dash->privateButton->copy_label(isPrivate ? "Private: ON" : "Private: OFF");
    dash->privateButton->color(isPrivate ? 46 : 189, isPrivate ? 204 : 195, isPrivate ? 113 : 199);
    
    dash->updateDisplay();
}

void AppDashboard::staticCallbackApps(Fl_Widget* w, void* data) {
    AppDashboard* dash = (AppDashboard*)data;
    dash->manager->toggleSetting(2);
    
    bool isRestricted = dash->manager->getSettings()->getAppsRestricted();
    dash->appsButton->copy_label(isRestricted ? "Apps: ON" : "Apps: OFF");
    dash->appsButton->color(isRestricted ? 46 : 189, isRestricted ? 204 : 195, isRestricted ? 113 : 199);
    
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
    dash->manager->resolveThreat(0, true);
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
