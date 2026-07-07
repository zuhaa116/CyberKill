#include "AppDashboard.h"
#include <FL/fl_draw.H>
#include <fstream>
#include <iostream>

// Color palette
#define COL_BG       fl_rgb_color(18, 18, 18)
#define COL_PANEL    fl_rgb_color(56, 30, 75)
#define COL_ACCENT   fl_rgb_color(58, 65, 95)
#define COL_MUTED    fl_rgb_color(180, 168, 170)
#define COL_TEXT     fl_rgb_color(220, 215, 225)

// ScoreBar draw
void ScoreBar::draw() {
    draw_box();
    
    // Draw background for the bar
    fl_color(COL_PANEL);
    fl_rectf(x() + 2, y() + 2, w() - 4, h() - 4);
    
    // Compute fill width from score
    int fillWidth = (score * (w() - 4)) / 100;
    
    if (score >= 75) {
        fl_color(fl_rgb_color(76, 175, 80));
    } else if (score >= 40) {
        fl_color(fl_rgb_color(255, 193, 7));
    } else {
        fl_color(fl_rgb_color(244, 67, 54));
    }
    
    fl_rectf(x() + 2, y() + 2, fillWidth, h() - 4);
    
    // Draw border
    fl_color(COL_ACCENT);
    fl_rect(x(), y(), w(), h());
}

// AppDashboard implementation
AppDashboard::AppDashboard(std::string playerName)
    : resolvedThreats(0), showingGameOver(false) {
    
    manager = new PrivacyManager(playerName);
    
    // Create main window and set background color
    mainWindow = new Fl_Window(1400, 800, "CyberKill - Social Media Privacy Simulator");
    mainWindow->color(COL_BG);
    
    // Title and header info
    titleBox = new Fl_Box(20, 15, 300, 50, "CyberKill");
    titleBox->box(FL_NO_BOX);
    titleBox->labelfont(FL_HELVETICA_BOLD);
    titleBox->labelsize(36);
    titleBox->labelcolor(COL_TEXT);
    titleBox->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    
    std::string playerInfo = playerName + " | " + std::to_string(manager->getUser()->getFollowers()) + " followers";
    playerNameBox = new Fl_Box(20, 68, 350, 20, playerInfo.c_str());
    playerNameBox->box(FL_NO_BOX);
    playerNameBox->labelfont(FL_HELVETICA);
    playerNameBox->labelsize(12);
    playerNameBox->labelcolor(COL_MUTED);
    playerNameBox->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    
    std::string scoreStr = std::to_string(manager->getUser()->getScore()) + "%";
    scorePercentBox = new Fl_Box(20, 90, 100, 25, scoreStr.c_str());
    scorePercentBox->box(FL_NO_BOX);
    scorePercentBox->labelfont(FL_HELVETICA_BOLD);
    scorePercentBox->labelsize(20);
    scorePercentBox->labelcolor(fl_rgb_color(76, 175, 80));
    scorePercentBox->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    
    scoreBar = new ScoreBar(130, 90, 240, 25, manager->getUser()->getScore());
    scoreBar->box(FL_BORDER_BOX);
    
    std::string roundStr = "Round: " + std::to_string(manager->getRoundCount());
    roundInfoBox = new Fl_Box(400, 68, 150, 20, roundStr.c_str());
    roundInfoBox->box(FL_NO_BOX);
    roundInfoBox->labelfont(FL_HELVETICA);
    roundInfoBox->labelsize(14);
    roundInfoBox->labelcolor(COL_MUTED);
    roundInfoBox->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    
    std::string statsStr = "Blocked: " + std::to_string(manager->getThreatsBlocked()) 
                    + " | Accepted: " + std::to_string(manager->getThreatsAccepted());
    statsBox = new Fl_Box(400, 90, 250, 20, statsStr.c_str());
    statsBox->box(FL_NO_BOX);
    statsBox->labelfont(FL_HELVETICA);
    statsBox->labelsize(12);
    statsBox->labelcolor(COL_MUTED);
    statsBox->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    
    // Settings buttons
    twoFaButton = new Fl_Button(700, 68, 100, 35, "2FA: OFF");
    twoFaButton->callback(staticCallback2FA, this);
    twoFaButton->color(COL_ACCENT);
    twoFaButton->labelcolor(COL_TEXT);
    twoFaButton->labelfont(FL_HELVETICA);
    twoFaButton->labelsize(11);
    
    privateButton = new Fl_Button(810, 68, 100, 35, "Private: OFF");
    privateButton->callback(staticCallbackPrivate, this);
    privateButton->color(COL_ACCENT);
    privateButton->labelcolor(COL_TEXT);
    privateButton->labelfont(FL_HELVETICA);
    privateButton->labelsize(11);
    
    appsButton = new Fl_Button(920, 68, 100, 35, "Apps: OFF");
    appsButton->callback(staticCallbackApps, this);
    appsButton->color(COL_ACCENT);
    appsButton->labelcolor(COL_TEXT);
    appsButton->labelfont(FL_HELVETICA);
    appsButton->labelsize(11);
    
    // Activity log area
    logBox = new Fl_Box(20, 130, 350, 620, "");
    logBox->box(FL_BORDER_BOX);
    logBox->color(COL_PANEL);
    logBox->labelfont(FL_COURIER);
    logBox->labelsize(11);
    logBox->labelcolor(COL_TEXT);
    logBox->align(FL_ALIGN_TOP_LEFT | FL_ALIGN_INSIDE);
    
    // Last session info
    lastSessionBox = new Fl_Box(20, 755, 350, 20, "");
    lastSessionBox->box(FL_NO_BOX);
    lastSessionBox->labelfont(FL_HELVETICA);
    lastSessionBox->labelsize(10);
    lastSessionBox->labelcolor(COL_MUTED);
    lastSessionBox->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    
    // Threats header
    threatsTitle = new Fl_Box(390, 130, 980, 30, "Active Threats");
    threatsTitle->box(FL_NO_BOX);
    threatsTitle->labelfont(FL_HELVETICA_BOLD);
    threatsTitle->labelsize(18);
    threatsTitle->labelcolor(COL_TEXT);
    threatsTitle->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    
    // Threat 1
    threat1Box = new Fl_Box(390, 165, 980, 110, "");
    threat1Box->box(FL_BORDER_BOX);
    threat1Box->color(COL_PANEL);
    threat1Box->labelfont(FL_COURIER);
    threat1Box->labelsize(11);
    threat1Box->labelcolor(COL_TEXT);
    threat1Box->align(FL_ALIGN_TOP_LEFT | FL_ALIGN_WRAP | FL_ALIGN_INSIDE);
    
    t1OptionABtn = new Fl_Button(390, 280, 485, 35, "");
    t1OptionABtn->callback(staticCallbackThreat1A, this);
    t1OptionABtn->color(COL_ACCENT);
    t1OptionABtn->labelfont(FL_HELVETICA);
    t1OptionABtn->labelsize(11);
    t1OptionABtn->labelcolor(COL_TEXT);
    
    t1OptionBBtn = new Fl_Button(885, 280, 485, 35, "");
    t1OptionBBtn->callback(staticCallbackThreat1B, this);
    t1OptionBBtn->color(COL_ACCENT);
    t1OptionBBtn->labelfont(FL_HELVETICA);
    t1OptionBBtn->labelsize(11);
    t1OptionBBtn->labelcolor(COL_TEXT);
    
    // Threat 2
    threat2Box = new Fl_Box(390, 320, 980, 110, "");
    threat2Box->box(FL_BORDER_BOX);
    threat2Box->color(COL_PANEL);
    threat2Box->labelfont(FL_COURIER);
    threat2Box->labelsize(11);
    threat2Box->labelcolor(COL_TEXT);
    threat2Box->align(FL_ALIGN_TOP_LEFT | FL_ALIGN_WRAP | FL_ALIGN_INSIDE);
    
    t2OptionABtn = new Fl_Button(390, 435, 485, 35, "");
    t2OptionABtn->callback(staticCallbackThreat2A, this);
    t2OptionABtn->color(COL_ACCENT);
    t2OptionABtn->labelfont(FL_HELVETICA);
    t2OptionABtn->labelsize(11);
    t2OptionABtn->labelcolor(COL_TEXT);
    
    t2OptionBBtn = new Fl_Button(885, 435, 485, 35, "");
    t2OptionBBtn->callback(staticCallbackThreat2B, this);
    t2OptionBBtn->color(COL_ACCENT);
    t2OptionBBtn->labelfont(FL_HELVETICA);
    t2OptionBBtn->labelsize(11);
    t2OptionBBtn->labelcolor(COL_TEXT);
    
    // Round complete message
    roundCompleteBox = new Fl_Box(390, 480, 980, 50, "");
    roundCompleteBox->box(FL_BORDER_BOX);
    roundCompleteBox->color(COL_ACCENT);
    roundCompleteBox->labelfont(FL_HELVETICA_BOLD);
    roundCompleteBox->labelsize(16);
    roundCompleteBox->labelcolor(COL_TEXT);
    roundCompleteBox->align(FL_ALIGN_CENTER);
    
    // Next round button
    nextRoundBtn = new Fl_Button(1185, 540, 185, 40, "Next Round >>>");
    nextRoundBtn->callback(staticCallbackNextRound, this);
    nextRoundBtn->color(COL_ACCENT);
    nextRoundBtn->labelfont(FL_HELVETICA_BOLD);
    nextRoundBtn->labelsize(12);
    nextRoundBtn->labelcolor(COL_TEXT);
    nextRoundBtn->hide();
    
    // Game over display
    gameOverBox = new Fl_Box(450, 250, 850, 200, "");
    gameOverBox->box(FL_ROUNDED_BOX);
    gameOverBox->color(COL_PANEL);
    gameOverBox->labelfont(FL_HELVETICA_BOLD);
    gameOverBox->labelsize(28);
    gameOverBox->labelcolor(COL_TEXT);
    gameOverBox->align(FL_ALIGN_CENTER | FL_ALIGN_WRAP);
    gameOverBox->hide();
    
    playAgainBtn = new Fl_Button(1185, 540, 185, 40, "Play Again");
    playAgainBtn->callback(staticCallbackPlayAgain, this);
    playAgainBtn->color(COL_ACCENT);
    playAgainBtn->labelfont(FL_HELVETICA_BOLD);
    playAgainBtn->labelsize(12);
    playAgainBtn->labelcolor(COL_TEXT);
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
    std::ifstream file("cyberkill_log.txt");
    if (!file.is_open()) return;
    
    std::string line;
    std::string lastLine;
    while (std::getline(file, line)) {
        if (!line.empty()) lastLine = line;
    }
    file.close();
    
    if (!lastLine.empty()) {
        std::string display = "Last: " + lastLine.substr(0, 60);
        lastSessionBox->copy_label(display.c_str());
    }
}

void AppDashboard::updateDisplay() {
    // Update score display
    std::string scoreStr = std::to_string(manager->getUser()->getScore()) + "%";
    scorePercentBox->copy_label(scoreStr.c_str());
    scoreBar->setScore(manager->getUser()->getScore());
    
    // Update round and stats
    std::string roundStr = "Round: " + std::to_string(manager->getRoundCount());
    roundInfoBox->copy_label(roundStr.c_str());
    
    std::string statsStr = "Blocked: " + std::to_string(manager->getThreatsBlocked()) 
                    + " | Accepted: " + std::to_string(manager->getThreatsAccepted());
    statsBox->copy_label(statsStr.c_str());
    
    // Update activity log
    std::vector<std::string> lastLogs = manager->getLastActivityLog(20);
    std::string logText;
    for (size_t i = 0; i < lastLogs.size(); ++i) {
        logText += lastLogs[i] + "\n";
    }
    logBox->copy_label(logText.c_str());
    
    // Check win or lose
    if (manager->getUser()->getScore() >= 100) {
        showGameOver(true);
    } else if (manager->getUser()->getScore() <= 0) {
        showGameOver(false);
    }
    
    updateThreatsDisplay();
    mainWindow->redraw();
}

void AppDashboard::updateThreatsDisplay() {
    std::vector<Threat*> threats = manager->getCurrentThreats();
    
    if (threats.size() >= 1) {
        Threat* t1 = threats[0];
        std::string t1Text = "[!] " + t1->getType() + "\n" + t1->getDescription() + "\n" + t1->getDetail();
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
        std::string t2Text = "[!] " + t2->getType() + "\n" + t2->getDescription() + "\n" + t2->getDetail();
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
    std::string resultText = won ? "YOU WIN!\nPrivacy Score: 100%" : "GAME OVER\nPrivacy Score: 0%";
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
    manager->endGame(won);
    mainWindow->redraw();
}

void AppDashboard::resetGame() {
    std::string oldName = manager->getUser()->getName();
    delete manager;
    manager = new PrivacyManager(oldName);
    resolvedThreats = 0;
    showingGameOver = false;
    
    threat1Box->show();
    threat2Box->show();
    gameOverBox->hide();
    playAgainBtn->hide();
    
    manager->nextRound();
    updateDisplay();
}

// Callbacks for UI buttons
void AppDashboard::staticCallback2FA(Fl_Widget* w, void* data) {
    AppDashboard* dash = static_cast<AppDashboard*>(data);
    dash->manager->toggleSetting(0);
    bool is2FA = dash->manager->getSettings()->getTwoFactor();
    dash->twoFaButton->copy_label(is2FA ? "2FA: ON" : "2FA: OFF");
    dash->twoFaButton->color(is2FA ? fl_rgb_color(78, 88, 120) : COL_ACCENT);
    dash->updateDisplay();
}

void AppDashboard::staticCallbackPrivate(Fl_Widget* w, void* data) {
    AppDashboard* dash = static_cast<AppDashboard*>(data);
    dash->manager->toggleSetting(1);
    bool isPrivate = dash->manager->getSettings()->getProfilePrivate();
    dash->privateButton->copy_label(isPrivate ? "Private: ON" : "Private: OFF");
    dash->privateButton->color(isPrivate ? fl_rgb_color(78, 88, 120) : COL_ACCENT);
    dash->updateDisplay();
}

void AppDashboard::staticCallbackApps(Fl_Widget* w, void* data) {
    AppDashboard* dash = static_cast<AppDashboard*>(data);
    dash->manager->toggleSetting(2);
    bool isRestricted = dash->manager->getSettings()->getAppsRestricted();
    dash->appsButton->copy_label(isRestricted ? "Apps: ON" : "Apps: OFF");
    dash->appsButton->color(isRestricted ? fl_rgb_color(78, 88, 120) : COL_ACCENT);
    dash->updateDisplay();
}

void AppDashboard::staticCallbackThreat1A(Fl_Widget* w, void* data) {
    AppDashboard* dash = static_cast<AppDashboard*>(data);
    dash->manager->resolveThreat(0, true);
    dash->updateDisplay();
}

void AppDashboard::staticCallbackThreat1B(Fl_Widget* w, void* data) {
    AppDashboard* dash = static_cast<AppDashboard*>(data);
    dash->manager->resolveThreat(0, false);
    dash->updateDisplay();
}

void AppDashboard::staticCallbackThreat2A(Fl_Widget* w, void* data) {
    AppDashboard* dash = static_cast<AppDashboard*>(data);
    size_t s = dash->manager->getCurrentThreats().size();
    int idx = s > 1 ? 1 : 0;
    dash->manager->resolveThreat(idx, true);
    dash->updateDisplay();
}

void AppDashboard::staticCallbackThreat2B(Fl_Widget* w, void* data) {
    AppDashboard* dash = static_cast<AppDashboard*>(data);
    size_t s = dash->manager->getCurrentThreats().size();
    int idx = s > 1 ? 1 : 0;
    dash->manager->resolveThreat(idx, false);
    dash->updateDisplay();
}

void AppDashboard::staticCallbackNextRound(Fl_Widget* w, void* data) {
    AppDashboard* dash = static_cast<AppDashboard*>(data);
    if (dash->manager->getUser()->getScore() >= 100) {
        dash->showGameOver(true);
    } else if (dash->manager->getUser()->getScore() <= 0) {
        dash->showGameOver(false);
    } else {
        dash->manager->nextRound();
        dash->updateDisplay();
    }
}

void AppDashboard::staticCallbackPlayAgain(Fl_Widget* w, void* data) {
    AppDashboard* dash = static_cast<AppDashboard*>(data);
    dash->resetGame();
}
