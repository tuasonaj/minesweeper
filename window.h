#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace chrono;
using namespace sf;

class WelcomeWindow {
    int width;
    int height;
    Font font;
    Text welcome;
    FloatRect welcomeRect;
    Text prompt;
    FloatRect promptRect;
    Text playerName;
    FloatRect playerNameRect;
    string input;
public:
    WelcomeWindow(int width, int height, Font &font);
    void InitializeText();
    void DrawWindow(RenderWindow &window);
    void UpdatePlayerName(RenderWindow &window, char letter);
    void DeleteLetter(RenderWindow &window);
    bool PlayerNameEmpty();
    void StorePlayerName();
    string GetPlayerName();
};

class LeaderboardWindow {
    int width;
    int height;
    Font font;
    Text title;
    FloatRect titleRect;
    Text leaderboard;
    FloatRect leaderboardRect;
    vector<string> times;
    vector<string> names;
    string leaderboardText;
public:
    LeaderboardWindow(int width, int height, Font &font);
    void ReadLeaderboard();
    void CreateLeaderboard();
    void InitializeText();
    void DrawWindow(RenderWindow &window);
    void UpdateLeaderboard(duration<long long, ratio<1, 1000000000>> time, string playerName);
};