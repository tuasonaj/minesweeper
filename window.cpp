#include "window.h"
#include <chrono>
#include "SFML/Graphics.hpp"
using namespace std;
using namespace chrono;
using namespace sf;

WelcomeWindow::WelcomeWindow(int width, int height, Font &font)
{
    this->width = static_cast<int>(width * 1.5);
    this->height = static_cast<int>(height * 1.5);
    this->font = font;
}
void WelcomeWindow::InitializeText()
{
    welcome.setFont(font);
    welcome.setString("WELCOME TO MINESWEEPER!");
    welcome.setStyle(Text::Bold | Text::Underlined);
    welcome.setFillColor(Color::White);
    welcome.setCharacterSize(24);
    welcomeRect = welcome.getGlobalBounds();
    welcome.setOrigin(welcomeRect.width/2, welcomeRect.height/2);
    welcome.setPosition((float)width/2.0f, (float)height/2.0f-150);

    prompt.setFont(font);
    prompt.setString("Enter your name:");
    prompt.setStyle(Text::Bold);
    prompt.setFillColor(Color::White);
    prompt.setCharacterSize(20);
    promptRect = prompt.getGlobalBounds();
    prompt.setOrigin(promptRect.width/2, promptRect.height/2);
    prompt.setPosition((float)width/2.0f, (float)height/2.0f-75);

    string playerNameText = "|";
    playerName.setFont(font);
    playerName.setString(playerNameText);
    playerName.setStyle(Text::Bold);
    playerName.setFillColor(Color::Yellow);
    playerName.setCharacterSize(18);
    playerNameRect = playerName.getGlobalBounds();
    playerName.setOrigin(playerNameRect.width/2, playerNameRect.height/2);
    playerName.setPosition((float)width/2.0f, (float)height/2.0f-45);
}
void WelcomeWindow::DrawWindow(RenderWindow &window)
{
    window.clear(Color::Blue);
    window.draw(welcome);
    window.draw(prompt);
    window.draw(playerName);
    window.display();
}
void WelcomeWindow::UpdatePlayerName(RenderWindow &window, char letter)
{
    if (input.size() < 10)
    {
        input = input + letter;
        playerName.setString(input + '|');
        playerNameRect = playerName.getGlobalBounds();
        playerName.setOrigin(playerNameRect.width/2, playerNameRect.height/2);
        window.draw(playerName);
    }
}
void WelcomeWindow::DeleteLetter(RenderWindow &window)
{
    if (!input.empty())
    {
        input = input.substr(0, input.size()-1);
        playerName.setString(input + '|');
        playerNameRect = playerName.getGlobalBounds();
        playerName.setOrigin(playerNameRect.width/2, playerNameRect.height/2);
        window.draw(playerName);
    }
}
bool WelcomeWindow::PlayerNameEmpty()
{
    if (input.empty())
    {
        return true;
    }
    return false;
}
void WelcomeWindow::StorePlayerName()
{
    input.at(0) = (char)toupper(input.at(0));
    for (int i = 1; i < input.size(); ++i)
    {
        input.at(i) = (char)tolower(input.at(i));
    }
}
string WelcomeWindow::GetPlayerName()
{
    return input;
}


LeaderboardWindow::LeaderboardWindow(int width, int height, Font &font)
{
    this->width = width/2;
    this->height = height/2;
    this->font = font;
}
void LeaderboardWindow::ReadLeaderboard()
{
    fstream file("files/leaderboard.txt");
    string Line;
    while (getline(file, Line))
    {
        string time;
        string name;
        istringstream stream(Line);
        getline(stream, time, ',');
        getline(stream, name);
        times.push_back(time);
        names.push_back(name);
    }
}
void LeaderboardWindow::CreateLeaderboard()
{
    string newLeaderboard;
    for (int i = 0; i < 5; ++i)
    {
        if (i == 4)
        {
            newLeaderboard += to_string(i+1) + ".\t" + times.at(i) + "\t" + names.at(i);
        }
        else
        {
            newLeaderboard += to_string(i+1) + ".\t" + times.at(i) + "\t" + names.at(i) + "\n\n";
        }
    }
    leaderboardText = newLeaderboard;
}
void LeaderboardWindow::InitializeText()
{
    title.setFont(font);
    title.setString("LEADERBOARD");
    title.setStyle(Text::Bold | Text::Underlined);
    title.setFillColor(Color::White);
    title.setCharacterSize(20);
    titleRect = title.getGlobalBounds();
    title.setOrigin(titleRect.width/2, titleRect.height/2);
    title.setPosition(width/2.0f, height/2.0f-120);

    leaderboard.setFont(font);
    leaderboard.setString(leaderboardText);
    leaderboard.setStyle(Text::Bold);
    leaderboard.setFillColor(Color::White);
    leaderboard.setCharacterSize(18);
    leaderboardRect = leaderboard.getGlobalBounds();
    leaderboard.setOrigin(leaderboardRect.width/2, leaderboardRect.height/2);
    leaderboard.setPosition(Vector2f(width/2.0f, height/2.0f+20));
}
void LeaderboardWindow::DrawWindow(RenderWindow &window)
{
    window.clear(Color::Blue);
    window.draw(title);
    window.draw(leaderboard);
    window.display();
}
void LeaderboardWindow::UpdateLeaderboard(duration<long long, ratio<1, 1000000000>> time, string playerName)
{
    int intMin = (int)duration_cast<chrono::seconds>(time).count()/60;
    int intSec = (int)duration_cast<chrono::seconds>(time).count()%60;
    int intTime = intMin*60 + intSec;
    string winnerMin;
    string winnerSec;
    string winnerTime;
    if (intMin/10 == 0)
    {
        winnerMin = "0" + to_string(intMin);
    }
    else
    {
        winnerMin = to_string(intMin);
    }
    if (intSec/10 == 0)
    {
        winnerSec = "0" + to_string(intSec);
    }
    else
    {
        winnerSec = to_string(intSec);
    }
    winnerTime = winnerMin + ":" + winnerSec;
    vector<string> updatedTimes;
    vector<string> updatedNames;
    int index = -1;
    for (int i = 0; i < 5; ++i)
    {
        int minutes = stoi(times.at(i).substr(0, 2));
        int seconds = stoi(times.at(i).substr(3, 2));
        int total = minutes*60 + seconds;
        if (intTime < total)
        {
            index = i;
            break;
        }
    }
    if (index != -1)
    {
        for (int j = 0; j < 5; ++j)
        {
            if (j == index)
            {
                updatedTimes.push_back(winnerTime);
                updatedNames.push_back(playerName);
                updatedTimes.push_back(times.at(j));
                updatedNames.push_back(names.at(j));
            }
            else
            {
                updatedTimes.push_back(times.at(j));
                updatedNames.push_back(names.at(j));
            }
        }
        times = updatedTimes;
        names = updatedNames;
    }
    string newLeaderboard;
    for (int i = 0; i < 5; ++i)
    {
        if (i == index)
        {
            if (i == 4)
            {
                newLeaderboard += to_string(i+1) + ".\t" + times.at(i) + "\t" + names.at(i) + "*";
            }
            else
            {
                newLeaderboard += to_string(i+1) + ".\t" + times.at(i) + "\t" + names.at(i) + "*\n\n";
            }
        }
        else
        {
            if (i == 4)
            {
                newLeaderboard += to_string(i+1) + ".\t" + times.at(i) + "\t" + names.at(i);
            }
            else
            {
                newLeaderboard += to_string(i+1) + ".\t" + times.at(i) + "\t" + names.at(i) + "\n\n";
            }
        }
    }
    leaderboardText = newLeaderboard;
}