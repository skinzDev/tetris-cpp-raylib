#include "TetrisPiece.h"
#include "include/raylib.h"

TetrisPiece::TetrisPiece() : positionX(0), positionY(0) {
    Clear();
}

void TetrisPiece::Clear() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            shape[i][j] = GridSquare::EMPTY;
        }
    }
}

void TetrisPiece::GenerateRandomPiece() {
    Clear();
    int random = GetRandomValue(0, 6);

    switch (random) {
    case 0:
        shape[1][1] = GridSquare::MOVING;
        shape[2][1] = GridSquare::MOVING;
        shape[1][2] = GridSquare::MOVING;
        shape[2][2] = GridSquare::MOVING;
        break;
    case 1:
        shape[1][0] = GridSquare::MOVING;
        shape[1][1] = GridSquare::MOVING;
        shape[1][2] = GridSquare::MOVING;
        shape[2][2] = GridSquare::MOVING;
        break;
    case 2:
        shape[1][2] = GridSquare::MOVING;
        shape[2][0] = GridSquare::MOVING;
        shape[2][1] = GridSquare::MOVING;
        shape[2][2] = GridSquare::MOVING;
        break;
    case 3:
        shape[0][1] = GridSquare::MOVING;
        shape[1][1] = GridSquare::MOVING;
        shape[2][1] = GridSquare::MOVING;
        shape[3][1] = GridSquare::MOVING;
        break;
    case 4:
        shape[1][0] = GridSquare::MOVING;
        shape[1][1] = GridSquare::MOVING;
        shape[1][2] = GridSquare::MOVING;
        shape[2][1] = GridSquare::MOVING;
        break;
    case 5:
        shape[1][1] = GridSquare::MOVING;
        shape[2][1] = GridSquare::MOVING;
        shape[2][2] = GridSquare::MOVING;
        shape[3][2] = GridSquare::MOVING;
        break;
    case 6:
        shape[1][2] = GridSquare::MOVING;
        shape[2][2] = GridSquare::MOVING;
        shape[2][1] = GridSquare::MOVING;
        shape[3][1] = GridSquare::MOVING;
        break;
    }
}

void TetrisPiece::SetPosition(int x, int y) {
    positionX = x;
    positionY = y;
}

int TetrisPiece::GetPositionX() const {
    return positionX;
}

int TetrisPiece::GetPositionY() const {
    return positionY;
}

void TetrisPiece::MoveX(int delta) {
    positionX += delta;
}

void TetrisPiece::MoveY(int delta) {
    positionY += delta;
}

GridSquare TetrisPiece::GetCell(int x, int y) const {
    if (x >= 0 && x < 4 && y >= 0 && y < 4) {
        return shape[x][y];
    }
    return GridSquare::EMPTY;
}

void TetrisPiece::SetCell(int x, int y, GridSquare value) {
    if (x >= 0 && x < 4 && y >= 0 && y < 4) {
        shape[x][y] = value;
    }
}

void TetrisPiece::CopyFrom(const TetrisPiece& other) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            shape[i][j] = other.shape[i][j];
        }
    }
}

void TetrisPiece::Rotate() {
    GridSquare aux;

    aux = shape[0][0];
    shape[0][0] = shape[3][0];
    shape[3][0] = shape[3][3];
    shape[3][3] = shape[0][3];
    shape[0][3] = aux;

    aux = shape[1][0];
    shape[1][0] = shape[3][1];
    shape[3][1] = shape[2][3];
    shape[2][3] = shape[0][2];
    shape[0][2] = aux;

    aux = shape[2][0];
    shape[2][0] = shape[3][2];
    shape[3][2] = shape[1][3];
    shape[1][3] = shape[0][1];
    shape[0][1] = aux;

    aux = shape[1][1];
    shape[1][1] = shape[2][1];
    shape[2][1] = shape[2][2];
    shape[2][2] = shape[1][2];
    shape[1][2] = aux;
}