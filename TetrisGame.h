#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include "GameGrid.h"
#include "TetrisPiece.h"
#include "include/raylib.h"

class TetrisGame {
private:
    static constexpr int screenWidth = 800;
    static constexpr int screenHeight = 450;

    GameGrid gameGrid;
    TetrisPiece currentPiece;
    TetrisPiece nextPiece;

    bool gameOver;
    bool pause;
    bool beginPlay;
    bool pieceActive;
    bool detection;
    bool lineToDelete;

    Color fadingColor;
    int level;
    int lines;

    int gravityMovementCounter;
    int lateralMovementCounter;
    int turnMovementCounter;
    int fastFallMovementCounter;
    int fadeLineCounter;
    int gravitySpeed;

    static TetrisGame* instance;

public:
    TetrisGame();
    void Initialize();
    bool CreatePiece();
    void ResolveFallingMovement();
    bool ResolveLateralMovement();
    bool ResolveTurnMovement();
    void Update();
    void Draw();
    void Run();
    static void UpdateDrawFrameCallback();
};

#endif