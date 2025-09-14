#ifndef GAME_GRID_H
#define GAME_GRID_H

#include "GameConstants.h"
#include <array>

class GameGrid {
private:
    std::array<std::array<GridSquare, GRID_VERTICAL_SIZE>, GRID_HORIZONTAL_SIZE> grid;

public:
    GameGrid();
    void Initialize();
    GridSquare GetCell(int x, int y) const;
    void SetCell(int x, int y, GridSquare value);
    bool IsGameOver() const;
    void ConvertMovingToFull();
    void ClearMovingPieces();
    bool CheckCollisionDown() const;
    void MovePiecesDown();
    bool CheckCollisionLeft() const;
    bool CheckCollisionRight() const;
    void MovePiecesLeft();
    void MovePiecesRight();
    bool CheckCompletedLines();
    int DeleteCompletedLines();
};

#endif