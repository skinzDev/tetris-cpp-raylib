#ifndef TETRIS_PIECE_H
#define TETRIS_PIECE_H

#include "GameConstants.h"
#include <array>

class TetrisPiece {
private:
    std::array<std::array<GridSquare, 4>, 4> shape;
    int positionX;
    int positionY;

public:
    TetrisPiece();
    void Clear();
    void GenerateRandomPiece();
    void SetPosition(int x, int y);
    int GetPositionX() const;
    int GetPositionY() const;
    void MoveX(int delta);
    void MoveY(int delta);
    GridSquare GetCell(int x, int y) const;
    void SetCell(int x, int y, GridSquare value);
    void CopyFrom(const TetrisPiece& other);
    void Rotate();
};

#endif