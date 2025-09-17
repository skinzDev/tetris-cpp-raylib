#ifndef TETRIS_PIECE_H
#define TETRIS_PIECE_H

#include "BaseGameObject.h"
#include <array>

class TetrisPiece : public BaseGameObject {
private:
    std::array<std::array<GridSquare, 4>, 4> shape;
    int pieceType;

public:
    TetrisPiece() : BaseGameObject(0, 0), pieceType(0) {
        std::cout << "TetrisPiece constructor called - inheriting from BaseGameObject" << std::endl;
        Clear();
    }
    
    virtual void Update() override {
        std::cout << "TetrisPiece Update() - overridden from BaseGameObject" << std::endl;
        BaseGameObject::Update();
    }
    
    virtual void Draw() const override {
        std::cout << "TetrisPiece Draw() - overridden from BaseGameObject" << std::endl;
    }
    
    virtual void Initialize() override {
        std::cout << "TetrisPiece Initialize() - overridden from BaseGameObject" << std::endl;
        BaseGameObject::Initialize();
        Clear();
        GenerateRandomPiece();
    }
    
    virtual void ProcessLogic() override {
        std::cout << "TetrisPiece ProcessLogic() - implementing pure virtual function" << std::endl;
        if (!IsActive()) {
            std::cout << "Piece is inactive, skipping logic processing" << std::endl;
            return;
        }
    }
    
    void Clear();
    void GenerateRandomPiece();
    void MoveX(int delta);
    void MoveY(int delta);
    GridSquare GetCell(int x, int y) const;
    void SetCell(int x, int y, GridSquare value);
    void CopyFrom(const TetrisPiece& other);
    void Rotate();
    
    void MoveToPosition(int x, int y) {
        SetPosition(x, y);
        std::cout << "Piece moved to position using inherited SetPosition()" << std::endl;
    }
    
    int GetPieceType() const { return pieceType; }
};

#endif