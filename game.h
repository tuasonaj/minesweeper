#pragma once
#include <iostream>
#include <vector>
#include <chrono>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace chrono;
using namespace sf;

class StoreTextures {
public:
    map<string, Texture> list;
    map<int, Texture> numList;
    void LoadTexture(const string& file);
    void LoadNumberTexture(const string& file, int number);
    void LoadAllNumbers();
    void LoadAll();
};

class Tile {
    int xPos;
    int yPos;
    int index{};
    bool mine = false;
    bool flagged = false;
    bool revealed = false;
    int adjacentMines{};
public:
    vector<Tile*> adjacentTiles;
    Tile();
    Tile(int xPos, int yPos, int index);
    Tile(const Tile &d);
    Tile &operator=(const Tile &d);
    void DrawTile(RenderWindow &window, Texture &tileTexture) const;
    void setMine();
    void destroyMine();
    bool isMine() const;
    void flag();
    void unFlag();
    bool isFlagged() const;
    void revealTile();
    void hideTile();
    bool isRevealed() const;
    int GetX() const;
    int GetY() const;
    int GetIndex() const;
    void setAdjacentMines();
    int GetAdjacentMines() const;
};

class Board {
    int cols;
    int rows;
    int numTiles{};
    int numMines;
    vector<int> mineTiles;
    vector<Tile> tiles;
    StoreTextures textures;
public:
    Board();
    Board(int cols, int rows, int numMines);
    void InitializeTextures();
    void CreateTiles();
    static bool TileIsAdjacent(const Tile& mainTile, Tile tile2);
    void SetAdjacentTiles();
    void DrawBoard(RenderWindow &window);
    int SelectedTileIndex(int xMousePos, int yMousePos);
    void FlagUnFlagTile(RenderWindow &window);
    int numFlags();
    void RevealTiles(int selectedIndex);
    bool MineHit(RenderWindow &window);
    void DrawMines(RenderWindow &window);
    void FlagMines();
    void PausedBoard(RenderWindow &window);
    void DebugMode(RenderWindow &window);
    void MineCounter(RenderWindow &window, int mineCount);
    void Timer(RenderWindow &window, duration<long long, ratio<1, 1000000000>> time);
    void ResetBoard();
    bool GameWon();
};
