#include "game.h"
#include "random.h"

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <chrono>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace chrono;
using namespace sf;

bool ValidNum(vector<int> mineTiles, int num)
{
    for (int i = 0; i < mineTiles.size(); ++i)
    {
        if (num == mineTiles.at(i))
        {
            return false;
        }
    }
    return true;
}


// StoreTextures
void StoreTextures::LoadTexture(const string& file)
{
    Texture newTexture;
    if (!newTexture.loadFromFile(file))
    {
        cout << "Could not load " << file << endl;
    }
    list[file.substr(13, file.size()-4)] = newTexture;
}
void StoreTextures::LoadNumberTexture(const string& file, int number)
{
    Texture newTexture;
    if (!newTexture.loadFromFile(file))
    {
        cout << "Could not load " << file << endl;
    }
    numList[number] = newTexture;
}
void StoreTextures::LoadAllNumbers()
{
    LoadNumberTexture("files/images/tile_revealed.png", 0);
    LoadNumberTexture("files/images/number_1.png", 1);
    LoadNumberTexture("files/images/number_2.png", 2);
    LoadNumberTexture("files/images/number_3.png", 3);
    LoadNumberTexture("files/images/number_4.png", 4);
    LoadNumberTexture("files/images/number_5.png", 5);
    LoadNumberTexture("files/images/number_6.png", 6);
    LoadNumberTexture("files/images/number_7.png", 7);
    LoadNumberTexture("files/images/number_8.png", 8);
}
void StoreTextures::LoadAll()
{
    LoadTexture("files/images/debug.png");
    LoadTexture("files/images/digits.png");
    LoadTexture("files/images/face_happy.png");
    LoadTexture("files/images/face_lose.png");
    LoadTexture("files/images/face_win.png");
    LoadTexture("files/images/flag.png");
    LoadTexture("files/images/leaderboard.png");
    LoadTexture("files/images/mine.png");
    LoadTexture("files/images/number_1.png");
    LoadTexture("files/images/number_2.png");
    LoadTexture("files/images/number_3.png");
    LoadTexture("files/images/number_4.png");
    LoadTexture("files/images/number_5.png");
    LoadTexture("files/images/number_6.png");
    LoadTexture("files/images/number_7.png");
    LoadTexture("files/images/number_8.png");
    LoadTexture("files/images/pause.png");
    LoadTexture("files/images/play.png");
    LoadTexture("files/images/tile_hidden.png");
    LoadTexture("files/images/tile_revealed.png");
}


// Tile
Tile::Tile()
{
    xPos = 0;
    yPos = 0;
}
Tile::Tile(int xPos, int yPos, int index)
{
    this->xPos = xPos;
    this->yPos = yPos;
    this->index = index;
}
Tile::Tile(const Tile &d)
{
    this->xPos = d.xPos;
    this->yPos = d.yPos;
    this->index = d.index;
    this->mine = d.mine;
    this->flagged = d.mine;
    this->revealed = d.revealed;
    this->adjacentTiles = d.adjacentTiles;
    this->adjacentMines = d.adjacentMines;
}
Tile &Tile::operator=(const Tile &d)
{
    this->xPos = d.xPos;
    this->yPos = d.yPos;
    this->index = d.index;
    this->mine = d.mine;
    this->flagged = d.mine;
    this->revealed = d.revealed;
    this->adjacentTiles = d.adjacentTiles;
    this->adjacentMines = d.adjacentMines;
    return *this;
}
void Tile::DrawTile(RenderWindow &window, Texture &tileTexture) const
{
    Sprite tile;
    tile.setTexture(tileTexture);
    tile.setPosition(Vector2f(xPos, yPos));
    window.draw(tile);
}
void Tile::setMine()
{
    mine = true;
}
void Tile::destroyMine()
{
    mine = false;
}
bool Tile::isMine() const
{
    return mine;
}
void Tile::flag()
{
    flagged = true;
}
void Tile::unFlag()
{
    flagged = false;
}
bool Tile::isFlagged() const
{
    return flagged;
}
void Tile::revealTile()
{
    revealed = true;
}
void Tile::hideTile()
{
    revealed = false;
}
bool Tile::isRevealed() const
{
    return revealed;
}
int Tile::GetX() const
{
    return xPos;
}
int Tile::GetY() const
{
    return yPos;
}
int Tile::GetIndex() const
{
    return index;
}
void Tile::setAdjacentMines()
{
    int mineCount;
    for (auto & adjacentTile : adjacentTiles)
    {
        if (adjacentTile->isMine())
        {
            ++mineCount;
        }
    }
    adjacentMines = mineCount;
}
int Tile::GetAdjacentMines() const
{
    return adjacentMines;
}


// Board
Board::Board()
{
    cols = 0;
    rows = 0;
    numMines = 0;
}
Board::Board(int cols, int rows, int numMines)
{
    this->cols = cols;
    this->rows = rows;
    this->numTiles = cols*rows;
    this->numMines = numMines;
}
void Board::InitializeTextures()
{
    textures.LoadAllNumbers();
    textures.LoadAll();
}
void Board::CreateTiles()
{
    while (mineTiles.size() < numMines)
    {
        int random = Random::Int(0, numTiles-1);
        if (ValidNum(mineTiles, random))
        {
            mineTiles.push_back(random);
        }
    }
    int index = 0;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            Tile tile(j*32, i*32, index);
            tiles.push_back(tile);
            ++index;
        }
    }

    for (int i = 0; i < numMines; ++i)
    {
        tiles.at(mineTiles.at(i)).setMine();
    }
}
bool Board::TileIsAdjacent(const Tile& mainTile, Tile tile2)
{
    // check top left
    if (tile2.GetX() == mainTile.GetX()-32 && tile2.GetY() == mainTile.GetY()-32)
    {
        return true;
    }
    // check top middle
    if (tile2.GetX() == mainTile.GetX() && tile2.GetY() == mainTile.GetY()-32)
    {
        return true;
    }
    // check top right
    if (tile2.GetX() == mainTile.GetX()+32 && tile2.GetY() == mainTile.GetY()-32)
    {
        return true;
    }
    // check left
    if (tile2.GetX() == mainTile.GetX()-32 && tile2.GetY() == mainTile.GetY())
    {
        return true;
    }
    // check right
    if (tile2.GetX() == mainTile.GetX()+32 && tile2.GetY() == mainTile.GetY())
    {
        return true;
    }
    // check bottom left
    if (tile2.GetX() == mainTile.GetX()-32 && tile2.GetY() == mainTile.GetY()+32)
    {
        return true;
    }
    // check bottom middle
    if (tile2.GetX() == mainTile.GetX() && tile2.GetY() == mainTile.GetY()+32)
    {
        return true;
    }
    // check bottom right
    if (tile2.GetX() == mainTile.GetX()+32 && tile2.GetY() == mainTile.GetY()+32)
    {
        return true;
    }
    return false;
}
void Board::SetAdjacentTiles()
{
    for (int i = 0; i < numTiles; ++i)
    {
        vector<Tile*> adTiles;
        for (int j = 0; j < numTiles; ++j)
        {
            if (j == i)
            {
                continue;
            }
            if (TileIsAdjacent(tiles.at(i), tiles.at(j)))
            {
                Tile* pTile = &tiles.at(j);
                adTiles.push_back(pTile);
            }
        }
        tiles.at(i).adjacentTiles = adTiles;
        tiles.at(i).setAdjacentMines();
    }
}
void Board::DrawBoard(RenderWindow &window)
{
    for (int i = 0; i < numTiles; ++i)
    {
        if (!tiles.at(i).isRevealed())
        {
            tiles.at(i).DrawTile(window, textures.list["tile_hidden"]);
            if (tiles.at(i).isFlagged())
            {
                tiles.at(i).DrawTile(window, textures.list["flag"]);
            }
        }
        else
        {
            tiles.at(i).DrawTile(window, textures.list["tile_revealed"]);
            tiles.at(i).DrawTile(window, textures.numList[tiles.at(i).GetAdjacentMines()]);
        }
    }
}
int Board::SelectedTileIndex(int xMousePos, int yMousePos)
{
    for (int i = 0; i < numTiles; ++i)
    {
        int tilesX = tiles.at(i).GetX();
        int tilesY = tiles.at(i).GetY();
        if (xMousePos >= tilesX && xMousePos <= tilesX + 32 && yMousePos >= tilesY && yMousePos <= tilesY + 32)
        {
            return i;
        }
    }
}
void Board::FlagUnFlagTile(RenderWindow &window)
{
    int xMousePos = Mouse::getPosition(window).x;
    int yMousePos = Mouse::getPosition(window).y;
    int selectedIndex = SelectedTileIndex(xMousePos, yMousePos);
    if (!tiles.at(selectedIndex).isRevealed())
    {
        if (!tiles.at(selectedIndex).isFlagged())
        {
            tiles.at(selectedIndex).flag();
        }
        else
        {
            tiles.at(selectedIndex).unFlag();
        }
    }
}
int Board::numFlags()
{
    int numFlags;
    for (int i = 0; i < numTiles; ++i)
    {
        if (tiles.at(i).isFlagged())
        {
            ++numFlags;
        }
    }
    return numFlags;
}
void Board::RevealTiles(int selectedIndex)
{
    if (!tiles.at(selectedIndex).isFlagged())
    {
        if (!tiles.at(selectedIndex).isRevealed())
        {
            tiles.at(selectedIndex).revealTile();
            if (tiles.at(selectedIndex).GetAdjacentMines() == 0)
            {
                for (auto & adjacentTile : tiles.at(selectedIndex).adjacentTiles)
                {
                    RevealTiles(adjacentTile->GetIndex());
                }
            }
        }
    }
}
bool Board::MineHit(RenderWindow &window)
{
    int xMousePos = Mouse::getPosition(window).x;
    int yMousePos = Mouse::getPosition(window).y;
    int selectedIndex = SelectedTileIndex(xMousePos, yMousePos);

    if (!tiles.at(selectedIndex).isFlagged())
    {
        if (tiles.at(selectedIndex).isMine())
        {
            return true;
        }
    }
    return false;
}
void Board::DrawMines(RenderWindow &window)
{
    for (int i = 0; i < numTiles; ++i)
    {
        if (tiles.at(i).isMine())
        {
            tiles.at(i).DrawTile(window, textures.list["tile_revealed"]);
            tiles.at(i).DrawTile(window, textures.list["mine"]);;
        }
    }
}
void Board::FlagMines()
{
    for (int i = 0; i < numMines; ++i)
    {
        if (!tiles.at(mineTiles.at(i)).isFlagged())
        {
            tiles.at(mineTiles.at(i)).flag();
        }
    }
}
void Board::PausedBoard(RenderWindow &window)
{
    for (int i = 0; i < numTiles; ++i)
    {
        tiles.at(i).DrawTile(window, textures.list["tile_revealed"]);
    }
}
void Board::DebugMode(RenderWindow &window)
{
    DrawBoard(window);
    for (int i = 0; i < numTiles; ++i)
    {
        if (tiles.at(i).isMine())
        {
            tiles.at(i).DrawTile(window, textures.list["tile_hidden"]);
            tiles.at(i).DrawTile(window, textures.list["mine"]);
        }
    }
}
void Board::MineCounter(RenderWindow &window, int mineCounter) {
    if (mineCounter < 0)
    {
        mineCounter *= -1;
        string mines = to_string(mineCounter);
        if (mines.size() == 1)
        {
            Sprite negativeSign;
            negativeSign.setTexture(textures.list["digits"]);
            negativeSign.setTextureRect(IntRect(10 * 21, 0, 21, 32));
            negativeSign.setPosition(Vector2f(12, 32 * (rows + 0.5f) + 16));
            window.draw(negativeSign);
            Sprite zero;
            zero.setTexture(textures.list["digits"]);
            zero.setTextureRect(IntRect(0, 0, 21, 32));
            zero.setPosition(Vector2f(33, 32 * (rows + 0.5f) + 16));
            window.draw(zero);
            Sprite zero2;
            zero2.setTexture(textures.list["digits"]);
            zero2.setTextureRect(IntRect(0, 0, 21, 32));
            zero2.setPosition(Vector2f(54, 32 * (rows + 0.5f) + 16));
            window.draw(zero2);
            int ones = stoi(mines);
            Sprite num;
            num.setTexture(textures.list["digits"]);
            num.setTextureRect(IntRect(21 * ones, 0, 21, 32));
            num.setPosition(Vector2f(75, 32 * (rows + 0.5f) + 16));
            window.draw(num);
        }
        else if (mines.size() == 2)
        {
            Sprite negativeSign;
            negativeSign.setTexture(textures.list["digits"]);
            negativeSign.setTextureRect(IntRect(10 * 21, 0, 21, 32));
            negativeSign.setPosition(Vector2f(12, 32 * (rows + 0.5f) + 16));
            window.draw(negativeSign);
            Sprite zero;
            zero.setTexture(textures.list["digits"]);
            zero.setTextureRect(IntRect(0, 0, 21, 32));
            zero.setPosition(Vector2f(33, 32 * (rows + 0.5f) + 16));
            window.draw(zero);
            string number = mines.substr(0, 1);
            int secNum = stoi(number);
            Sprite tens;
            tens.setTexture(textures.list["digits"]);
            tens.setTextureRect(IntRect(21 * secNum, 0, 21, 32));
            tens.setPosition(Vector2f(54, 32 * (rows + 0.5f) + 16));
            window.draw(tens);
            string number2 = mines.substr(1, 1);
            int thirdNum = stoi(number2);
            Sprite ones;
            ones.setTexture(textures.list["digits"]);
            ones.setTextureRect(IntRect(21 * thirdNum, 0, 21, 32));
            ones.setPosition(Vector2f(75, 32 * (rows + 0.5f) + 16));
            window.draw(ones);
        }
        else if (mines.size() == 3)
        {
            Sprite negativeSign;
            negativeSign.setTexture(textures.list["digits"]);
            negativeSign.setTextureRect(IntRect(10 * 21, 0, 21, 32));
            negativeSign.setPosition(Vector2f(12, 32 * (rows + 0.5f) + 16));
            window.draw(negativeSign);
            string number = mines.substr(0, 1);
            int secNum = stoi(number);
            Sprite hundreds;
            hundreds.setTexture(textures.list["digits"]);
            hundreds.setTextureRect(IntRect(21 * secNum, 0, 21, 32));
            hundreds.setPosition(Vector2f(33, 32 * (rows + 0.5f) + 16));
            window.draw(hundreds);
            string number2 = mines.substr(1, 1);
            int thirdNum = stoi(number2);
            Sprite tens;
            tens.setTexture(textures.list["digits"]);
            tens.setTextureRect(IntRect(21 * thirdNum, 0, 21, 32));
            tens.setPosition(Vector2f(54, 32 * (rows + 0.5f) + 16));
            window.draw(tens);
            string number3 = mines.substr(2, 1);
            int fourthNum = stoi(number3);
            Sprite ones;
            ones.setTexture(textures.list["digits"]);
            ones.setTextureRect(IntRect(21 * fourthNum, 0, 21, 32));
            ones.setPosition(Vector2f(75, 32 * (rows + 0.5f) + 16));
            window.draw(ones);
        }
    }
    else
    {
        string mines = to_string(mineCounter);
        if (mines.size() == 1)
        {
            Sprite zero;
            zero.setTexture(textures.list["digits"]);
            zero.setTextureRect(IntRect(0, 0, 21, 32));
            zero.setPosition(Vector2f(33, 32 * (rows + 0.5f) + 16));
            window.draw(zero);
            Sprite zero2;
            zero2.setTexture(textures.list["digits"]);
            zero2.setTextureRect(IntRect(0, 0, 21, 32));
            zero2.setPosition(Vector2f(54, 32 * (rows + 0.5f) + 16));
            window.draw(zero2);
            int ones = stoi(mines);
            Sprite num;
            num.setTexture(textures.list["digits"]);
            num.setTextureRect(IntRect(21 * ones, 0, 21, 32));
            num.setPosition(Vector2f(75, 32 * (rows + 0.5f) + 16));
            window.draw(num);
        }
        else if (mines.size() == 2)
        {
            Sprite zero;
            zero.setTexture(textures.list["digits"]);
            zero.setTextureRect(IntRect(0, 0, 21, 32));
            zero.setPosition(Vector2f(33, 32 * (rows + 0.5f) + 16));
            window.draw(zero);
            string number = mines.substr(0, 1);
            int secNum = stoi(number);
            Sprite tens;
            tens.setTexture(textures.list["digits"]);
            tens.setTextureRect(IntRect(21 * secNum, 0, 21, 32));
            tens.setPosition(Vector2f(54, 32 * (rows + 0.5f) + 16));
            window.draw(tens);
            string number2 = mines.substr(1, 1);
            int thirdNum = stoi(number2);
            Sprite ones;
            ones.setTexture(textures.list["digits"]);
            ones.setTextureRect(IntRect(21 * thirdNum, 0, 21, 32));
            ones.setPosition(Vector2f(75, 32 * (rows + 0.5f) + 16));
            window.draw(ones);
        }
    }
}
void Board::Timer(RenderWindow &window, duration<long long, ratio<1, 1000000000>> time)
{
    int total = (int)duration_cast<chrono::seconds>(time).count();
    int mins = total / 60;
    int secs = total % 60;
    if (mins == 0)
    {
        Sprite zero;
        zero.setTexture(textures.list["digits"]);
        zero.setTextureRect(IntRect(0, 0, 21, 32));
        zero.setPosition(Vector2f(cols*32-97, 32 * (rows + 0.5f) + 16));
        window.draw(zero);
        Sprite zero2;
        zero2.setTexture(textures.list["digits"]);
        zero2.setTextureRect(IntRect(0, 0, 21, 32));
        zero2.setPosition(Vector2f(cols*32-76, 32 * (rows + 0.5f) + 16));
        window.draw(zero2);
    }
    else
    {
        string minutes = to_string(mins);
        if (minutes.size() == 1) {
            Sprite zero;
            zero.setTexture(textures.list["digits"]);
            zero.setTextureRect(IntRect(0, 0, 21, 32));
            zero.setPosition(Vector2f(cols*32-97, 32 * (rows + 0.5f) + 16));
            window.draw(zero);
            int ones = stoi(minutes);
            Sprite num;
            num.setTexture(textures.list["digits"]);
            num.setTextureRect(IntRect(21 * ones, 0, 21, 32));
            num.setPosition(Vector2f(cols*32-76, 32 * (rows + 0.5f) + 16));
            window.draw(num);
        }
        else if (minutes.size() == 2)
        {
            string number = minutes.substr(0, 1);
            int num = stoi(number);
            Sprite tens;
            tens.setTexture(textures.list["digits"]);
            tens.setTextureRect(IntRect(21 * num, 0, 21, 32));
            tens.setPosition(Vector2f(cols*32-97, 32 * (rows + 0.5f) + 16));
            window.draw(tens);
            string number2 = minutes.substr(1, 1);
            int secNum = stoi(number2);
            Sprite ones;
            ones.setTexture(textures.list["digits"]);
            ones.setTextureRect(IntRect(21 * secNum, 0, 21, 32));
            ones.setPosition(Vector2f(cols*32-76, 32 * (rows + 0.5f) + 16));
            window.draw(ones);
        }
    }
    if (secs == 0)
    {
        Sprite zero;
        zero.setTexture(textures.list["digits"]);
        zero.setTextureRect(IntRect(0, 0, 21, 32));
        zero.setPosition(Vector2f(cols*32-54, 32 * (rows + 0.5f) + 16));
        window.draw(zero);
        Sprite zero2;
        zero2.setTexture(textures.list["digits"]);
        zero2.setTextureRect(IntRect(0, 0, 21, 32));
        zero2.setPosition(Vector2f(cols*32-33, 32 * (rows + 0.5f) + 16));
        window.draw(zero2);
    }
    else
    {
        string seconds = to_string(secs);
        if (seconds.size() == 1) {
            Sprite zero;
            zero.setTexture(textures.list["digits"]);
            zero.setTextureRect(IntRect(0, 0, 21, 32));
            zero.setPosition(Vector2f(cols*32-54, 32 * (rows + 0.5f) + 16));
            window.draw(zero);
            int ones = stoi(seconds);
            Sprite num;
            num.setTexture(textures.list["digits"]);
            num.setTextureRect(IntRect(21 * ones, 0, 21, 32));
            num.setPosition(Vector2f(cols*32-33, 32 * (rows + 0.5f) + 16));
            window.draw(num);
        }
        else if (seconds.size() == 2)
        {
            string number = seconds.substr(0, 1);
            int num = stoi(number);
            Sprite tens;
            tens.setTexture(textures.list["digits"]);
            tens.setTextureRect(IntRect(21 * num, 0, 21, 32));
            tens.setPosition(Vector2f(cols*32-54, 32 * (rows + 0.5f) + 16));
            window.draw(tens);
            string number2 = seconds.substr(1, 1);
            int secNum = stoi(number2);
            Sprite ones;
            ones.setTexture(textures.list["digits"]);
            ones.setTextureRect(IntRect(21 * secNum, 0, 21, 32));
            ones.setPosition(Vector2f(cols*32-33, 32 * (rows + 0.5f) + 16));
            window.draw(ones);
        }
    }
}
void Board::ResetBoard()
{
    for (int i = 0; i < numTiles; ++i)
    {
        if (tiles.at(i).isFlagged())
        {
            tiles.at(i).unFlag();
        }
        if (tiles.at(i).isMine())
        {
            tiles.at(i).destroyMine();
        }
        if (tiles.at(i).isRevealed())
        {
            tiles.at(i).hideTile();
        }
    }
    vector<int> randnums;

    while (randnums.size() < numMines)
    {
        int random = Random::Int(0, numTiles-1);
        if (ValidNum(randnums, random))
        {
            randnums.push_back(random);
        }
    }
    mineTiles = randnums;
    for (int i = 0; i < numMines; ++i)
    {
        tiles.at(mineTiles.at(i)).setMine();
    }
    SetAdjacentTiles();
}
bool Board::GameWon()
{
    for (int i = 0; i < numTiles; ++i)
    {
        if (!tiles.at(i).isRevealed())
        {
            if (!tiles.at(i).isMine())
            {
                return false;
            }
        }
    }
    return true;
}
