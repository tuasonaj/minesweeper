#include <iostream>
#include <random>
#include <chrono>
#include <SFML/Graphics.hpp>
#include "game.h"
#include "window.h"
using namespace std;
using namespace chrono;
using namespace sf;

bool MouseOverButton(Sprite &button, RenderWindow &window)
{
    int mX = Mouse::getPosition(window).x;
    int mY = Mouse::getPosition(window).y;

    float buttonX = button.getPosition().x;
    float buttonY = button.getPosition().y;
    float buttonXWidth = button.getPosition().x + button.getLocalBounds().width;
    float buttonYHeight = button.getPosition().y + button.getLocalBounds().height;

    if ((float)mX < buttonXWidth && (float)mX > buttonX && (float)mY < buttonYHeight && (float)mY > buttonY) {
        return true;
    }
    return false;
}
bool MouseOverBoard(int cols, int rows, RenderWindow &window)
{
    int width = cols*32;
    int height = rows*32;
    int mX = Mouse::getPosition(window).x;
    int mY = Mouse::getPosition(window).y;

    if (mX >= 0 && mX <= width && mY >= 0 && mY <= height)
    {
        return true;
    }
    return false;
}

int main()
{
    // get board size
    int cols, rows, mines;
    fstream config("files/config.cfg");
    config >> cols >> rows >> mines;
    int boardWidth = cols*32;
    int boardHeight = rows*32+100;
    // load font
    Font font;
    if (!font.loadFromFile("files/font.ttf"))
    {
        cout << "Could not load font" << endl;
    }
    // welcome window
    sf::RenderWindow welcomeWindow(sf::VideoMode(800, 600), "Minesweeper", Style::Close);
    WelcomeWindow WWindow(800, 600, font);
    WWindow.InitializeText();
    while (welcomeWindow.isOpen())
    {
        Event event;
        while (welcomeWindow.pollEvent(event))
        {
            if (event.type == Event::TextEntered)
            {
                if (isalpha((char)event.text.unicode))
                {
                    WWindow.UpdatePlayerName(welcomeWindow, (char)event.text.unicode);
                }
            }
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Enter)
                {
                    if (!WWindow.PlayerNameEmpty())
                    {
                        WWindow.StorePlayerName();
                        welcomeWindow.close();
                    }
                }
                if (event.key.code == Keyboard::BackSpace)
                {
                    WWindow.DeleteLetter(welcomeWindow);
                }
            }
            if (event.type == Event::Closed)
            {
                welcomeWindow.close();
                return 0;
            }
        }
        WWindow.DrawWindow(welcomeWindow);
    }
    // game window
    sf::RenderWindow gameWindow(VideoMode(boardWidth, boardHeight), "Minesweeper", Style::Close);
    // textures
    StoreTextures textures;
    textures.LoadAll();
    // sprites
    Sprite HappyFaceButton;
    HappyFaceButton.setTexture(textures.list["face_happy"]);
    HappyFaceButton.setPosition(cols/2.0*32-32, 32*(rows+0.5f));
    Sprite WinFace;
    WinFace.setTexture(textures.list["face_win"]);
    WinFace.setPosition(cols/2.0*32-32, 32*(rows+0.5f));
    Sprite LoseFace;
    LoseFace.setTexture(textures.list["face_lose"]);
    LoseFace.setPosition(cols/2.0*32-32, 32*(rows+0.5f));
    Sprite DebugButton;
    DebugButton.setTexture(textures.list["debug"]);
    DebugButton.setPosition(cols*32-304, 32*(rows+0.5f));
    Sprite pause;
    pause.setTexture(textures.list["pause"]);
    pause.setPosition(cols*32-240, 32*(rows+0.5f));
    Sprite play;
    play.setTexture(textures.list["play"]);
    play.setPosition(cols*32-240, 32*(rows+0.5f));
    Sprite LeaderboardButton;
    LeaderboardButton.setTexture(textures.list["leaderboard"]);
    LeaderboardButton.setPosition(cols*32-176, 32*(rows+0.5f));
    // leaderboard
    LeaderboardWindow LWindow(boardWidth, boardHeight, font);
    LWindow.ReadLeaderboard();
    LWindow.CreateLeaderboard();
    LWindow.InitializeText();
    // board
    Board board(cols, rows, mines);
    board.InitializeTextures();
    board.CreateTiles();
    board.SetAdjacentTiles();
    int flagCount;
    // game states
    bool gameOver = false;
    bool win = false;
    bool paused = false;
    bool leaderboardOpen = false;
    bool debugMode = false;
    //timer
    auto pauseStart = high_resolution_clock::now();
    auto pauseTime = 0s;
    auto leaderboardStart = high_resolution_clock::now();
    auto leaderboardTime = 0s;
    auto startTime = high_resolution_clock::now();
    auto elapsedTime = high_resolution_clock::now() - startTime - pauseTime;
    while (gameWindow.isOpen())
    {
        Event event;
        while (gameWindow.pollEvent(event))
        {
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                if (MouseOverBoard(cols, rows, gameWindow))
                {
                    if (!gameOver)
                    {
                        if (board.MineHit(gameWindow))
                        {
                            gameOver = true;
                        }
                        else
                        {
                            int xMousePos = Mouse::getPosition(gameWindow).x;
                            int yMousePos = Mouse::getPosition(gameWindow).y;
                            int selectedIndex = board.SelectedTileIndex(xMousePos, yMousePos);
                            board.RevealTiles(selectedIndex);
                            if (board.GameWon())
                            {
                                gameOver = true;
                                win = true;
                                board.FlagMines();
                                LWindow.UpdateLeaderboard(elapsedTime, WWindow.GetPlayerName());
                                LWindow.InitializeText();
                                leaderboardOpen = true;
                            }
                        }
                    }
                }
                if (MouseOverButton(pause, gameWindow))
                {
                    if (!paused)
                    {
                        paused = true;
                        pauseStart = high_resolution_clock::now();
                    }
                    else
                    {
                        paused = false;
                        auto pauseEnd = high_resolution_clock::now() - pauseStart;
                        pauseTime = pauseTime + duration_cast<chrono::seconds>(pauseEnd);
                    }
                }
                if (MouseOverButton(LeaderboardButton, gameWindow))
                {
                    paused = true;
                    leaderboardOpen = true;
                    pauseStart = high_resolution_clock::now();
                }
                if (MouseOverButton(DebugButton, gameWindow))
                {
                    if (!paused)
                    {
                        if (!debugMode)
                        {
                            debugMode = true;
                        }
                        else
                        {
                            debugMode = false;
                        }
                    }
                }
                if (MouseOverButton(HappyFaceButton, gameWindow))
                {
                    paused = false;
                    leaderboardOpen = false;
                    debugMode = false;
                    gameOver = false;
                    win = false;
                    LWindow.CreateLeaderboard();
                    LWindow.InitializeText();
                    board.ResetBoard();
                    board.DrawBoard(gameWindow);
                    startTime = high_resolution_clock::now();
                    pauseTime = 0s;
                }
            }
            if (Mouse::isButtonPressed(Mouse::Right))
            {
                if (!gameOver)
                {
                    if (MouseOverBoard(cols, rows, gameWindow))
                    {
                        board.FlagUnFlagTile(gameWindow);
                    }
                }
            }
            if (event.type == Event::Closed)
            {
                gameWindow.close();
                return 0;
            }
        }
        gameWindow.clear(Color::White);
        gameWindow.draw(DebugButton);
        gameWindow.draw(pause);
        gameWindow.draw(LeaderboardButton);
        flagCount = mines - board.numFlags();
        board.MineCounter(gameWindow, flagCount);
        if (!gameOver)
        {
            gameWindow.draw(HappyFaceButton);
            if (paused)
            {
                auto pausedDisplay = pauseStart - startTime - pauseTime - leaderboardTime;
                board.Timer(gameWindow, pausedDisplay);
                board.PausedBoard(gameWindow);
                gameWindow.draw(play);
            }
            else
            {
                elapsedTime = high_resolution_clock::now() - startTime - pauseTime;
                board.Timer(gameWindow, elapsedTime);
                if (debugMode)
                {
                    board.DebugMode(gameWindow);
                }
                else
                {
                    board.DrawBoard(gameWindow);
                }
            }
        }
        else
        {
            if (win)
            {
                gameWindow.draw(WinFace);
                board.Timer(gameWindow, elapsedTime);
                board.DrawBoard(gameWindow);
            }
            else
            {
                gameWindow.draw(LoseFace);
                board.Timer(gameWindow, elapsedTime);
                board.DrawBoard(gameWindow);
                board.DrawMines(gameWindow);
            }
        }
        gameWindow.display();

        if (leaderboardOpen)
        {
            RenderWindow leaderboardWindow(VideoMode(boardWidth/2, boardHeight/2), "Minesweeper", Style::Close);
            while (leaderboardWindow.isOpen())
            {
                Event event2;
                while (leaderboardWindow.pollEvent(event2))
                {
                    if (event2.type == Event::Closed)
                    {
                        paused = false;
                        auto pauseEnd = high_resolution_clock::now() - pauseStart;
                        pauseTime = pauseTime + duration_cast<chrono::seconds>(pauseEnd);
                        leaderboardOpen = false;
                        leaderboardWindow.close();
                    }
                }
                LWindow.DrawWindow(leaderboardWindow);
            }
        }
    }

    return 0;
}