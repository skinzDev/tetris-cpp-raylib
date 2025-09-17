#include "TetrisPiece.h"
#include "include/raylib.h"

void TetrisPiece::Clear() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            shape[i][j] = GridSquare::EMPTY;
        }
    }
    std::cout << "Piece shape cleared" << std::endl;
}

void TetrisPiece::GenerateRandomPiece() {
    Clear();
    pieceType = GetRandomValue(0, 6);
    
    std::cout << "Generating random piece type: " << pieceType << std::endl;

    switch (pieceType) {
    case 0:
        shape[1][1] = GridSquare::MOVING;
        shape[2][1] = GridSquare::MOVING;
        shape[1][2] = GridSquare::MOVING;
        shape[2][2] = GridSquare::MOVING;
        std::cout << "Generated Square piece" << std::endl;
        break;
    case 1:
        shape[1][0] = GridSquare::MOVING;
        shape[1][1] = GridSquare::MOVING;
        shape[1][2] = GridSquare::MOVING;
        shape[2][2] = GridSquare::MOVING;
        std::cout << "Generated L piece" << std::endl;
        break;
    case 2:
        shape[1][2] = GridSquare::MOVING;
        shape[2][0] = GridSquare::MOVING;
        shape[2][1] = GridSquare::MOVING;
        shape[2][2] = GridSquare::MOVING;
        std::cout << "Generated Reverse L piece" << std::endl;
        break;
    case 3:
        shape[0][1] = GridSquare::MOVING;
        shape[1][1] = GridSquare::MOVING;
        shape[2][1] = GridSquare::MOVING;
        shape[3][1] = GridSquare::MOVING;
        std::cout << "Generated Line piece" << std::endl;
        break;
    case 4:
        shape[1][0] = GridSquare::MOVING;
        shape[1][1] = GridSquare::MOVING;
        shape[1][2] = GridSquare::MOVING;
        shape[2][1] = GridSquare::MOVING;
        std::cout << "Generated T piece" << std::endl;
        break;
    case 5:
        shape[1][1] = GridSquare::MOVING;
        shape[2][1] = GridSquare::MOVING;
        shape[2][2] = GridSquare::MOVING;
        shape[3][2] = GridSquare::MOVING;
        std::cout << "Generated S piece" << std::endl;
        break;
    case 6:
        shape[1][2] = GridSquare::MOVING;
        shape[2][2] = GridSquare::MOVING;
        shape[2][1] = GridSquare::MOVING;
        shape[3][1] = GridSquare::MOVING;
        std::cout << "Generated Z piece" << std::endl;
        break;
    }
    
    SetActive(true);
}

void TetrisPiece::MoveX(int delta) {
    positionX += delta;
    std::cout << "Piece moved X by " << delta << " using inherited positionX" << std::endl;
}

void TetrisPiece::MoveY(int delta) {
    positionY += delta;
    std::cout << "Piece moved Y by " << delta << " using inherited positionY" << std::endl;
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
    
    positionX = other.positionX;
    positionY = other.positionY;
    isActive = other.isActive;
    pieceType = other.pieceType;
    
    std::cout << "Piece copied including inherited properties" << std::endl;
}

void TetrisPiece::Rotate() {
    if (!IsActive()) {
        std::cout << "Cannot rotate inactive piece" << std::endl;
        return;
    }
    
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
    
    std::cout << "Piece rotated at position (" << GetPositionX() << ", " << GetPositionY() << ")" << std::endl;
}