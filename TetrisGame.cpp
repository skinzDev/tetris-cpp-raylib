#include "TetrisGame.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

TetrisGame* TetrisGame::instance = nullptr;

TetrisGame::TetrisGame() {
    instance = this;
    Initialize();
}

void TetrisGame::Initialize() {
    gameOver = false;
    pause = false;
    beginPlay = true;
    pieceActive = false;
    detection = false;
    lineToDelete = false;

    level = 1;
    lines = 0;
    fadingColor = BLUE;

    gravityMovementCounter = 0;
    lateralMovementCounter = 0;
    turnMovementCounter = 0;
    fastFallMovementCounter = 0;
    fadeLineCounter = 0;
    gravitySpeed = 30;

    gameGrid.Initialize();
    nextPiece.Clear();
}

bool TetrisGame::CreatePiece() {
    currentPiece.SetPosition((GRID_HORIZONTAL_SIZE - 4) / 2, 0);

    if (beginPlay) {
        nextPiece.GenerateRandomPiece();
        beginPlay = false;
    }

    currentPiece.CopyFrom(nextPiece);
    nextPiece.GenerateRandomPiece();

    for (int i = currentPiece.GetPositionX(); i < currentPiece.GetPositionX() + 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (currentPiece.GetCell(i - currentPiece.GetPositionX(), j) == GridSquare::MOVING) {
                gameGrid.SetCell(i, j, GridSquare::MOVING);
            }
        }
    }

    return true;
}

void TetrisGame::ResolveFallingMovement() {
    if (detection) {
        gameGrid.ConvertMovingToFull();
        detection = false;
        pieceActive = false;
    }
    else {
        gameGrid.MovePiecesDown();
        currentPiece.MoveY(1);
    }
}

bool TetrisGame::ResolveLateralMovement() {
    bool collision = false;

    if (IsKeyDown(KEY_LEFT)) {
        if (!gameGrid.CheckCollisionLeft()) {
            gameGrid.MovePiecesLeft();
            currentPiece.MoveX(-1);
        }
        else {
            collision = true;
        }
    }
    else if (IsKeyDown(KEY_RIGHT)) {
        if (!gameGrid.CheckCollisionRight()) {
            gameGrid.MovePiecesRight();
            currentPiece.MoveX(1);
        }
        else {
            collision = true;
        }
    }

    return collision;
}

bool TetrisGame::ResolveTurnMovement() {
    if (IsKeyDown(KEY_UP)) {
        gameGrid.ClearMovingPieces();
        currentPiece.Rotate();

        for (int i = currentPiece.GetPositionX(); i < currentPiece.GetPositionX() + 4; i++) {
            for (int j = currentPiece.GetPositionY(); j < currentPiece.GetPositionY() + 4; j++) {
                if (currentPiece.GetCell(i - currentPiece.GetPositionX(), j - currentPiece.GetPositionY()) == GridSquare::MOVING) {
                    gameGrid.SetCell(i, j, GridSquare::MOVING);
                }
            }
        }

        return true;
    }
    return false;
}

void TetrisGame::Update() {
    if (!gameOver) {
        if (IsKeyPressed('P')) pause = !pause;

        if (!pause) {
            if (!lineToDelete) {
                if (!pieceActive) {
                    pieceActive = CreatePiece();
                    fastFallMovementCounter = 0;
                }
                else {
                    fastFallMovementCounter++;
                    gravityMovementCounter++;
                    lateralMovementCounter++;
                    turnMovementCounter++;

                    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)) {
                        lateralMovementCounter = LATERAL_SPEED;
                    }
                    if (IsKeyPressed(KEY_UP)) {
                        turnMovementCounter = TURNING_SPEED;
                    }

                    if (IsKeyDown(KEY_DOWN) && (fastFallMovementCounter >= FAST_FALL_AWAIT_COUNTER)) {
                        gravityMovementCounter += gravitySpeed;
                    }

                    if (gravityMovementCounter >= gravitySpeed) {
                        detection = gameGrid.CheckCollisionDown();
                        ResolveFallingMovement();

                        if (gameGrid.CheckCompletedLines()) {
                            lineToDelete = true;
                        }

                        gravityMovementCounter = 0;
                    }

                    if (lateralMovementCounter >= LATERAL_SPEED) {
                        if (!ResolveLateralMovement()) {
                            lateralMovementCounter = 0;
                        }
                    }

                    if (turnMovementCounter >= TURNING_SPEED) {
                        if (ResolveTurnMovement()) {
                            turnMovementCounter = 0;
                        }
                    }
                }

                if (gameGrid.IsGameOver()) {
                    gameOver = true;
                }
            }
            else {
                fadeLineCounter++;

                if (fadeLineCounter % 8 < 4) {
                    fadingColor = MAROON;
                }
                else {
                    fadingColor = BLUE;
                }

                if (fadeLineCounter >= FADING_TIME) {
                    int deletedLines = gameGrid.DeleteCompletedLines();
                    fadeLineCounter = 0;
                    lineToDelete = false;
                    lines += deletedLines;
                }
            }
        }
    }
    else {
        if (IsKeyPressed(KEY_ENTER)) {
            Initialize();
            gameOver = false;
        }
    }
}

void TetrisGame::Draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (!gameOver) {
        Vector2 offset = {
            screenWidth / 2.0f - (GRID_HORIZONTAL_SIZE * SQUARE_SIZE / 2.0f) - 50,
            screenHeight / 2.0f - ((GRID_VERTICAL_SIZE - 1) * SQUARE_SIZE / 2.0f) + SQUARE_SIZE * 2 - 50
        };

        int controller = static_cast<int>(offset.x);

        for (int j = 0; j < GRID_VERTICAL_SIZE; j++) {
            for (int i = 0; i < GRID_HORIZONTAL_SIZE; i++) {
                GridSquare cell = gameGrid.GetCell(i, j);

                switch (cell) {
                case GridSquare::EMPTY:
                    DrawLine(static_cast<int>(offset.x), static_cast<int>(offset.y),
                        static_cast<int>(offset.x) + SQUARE_SIZE, static_cast<int>(offset.y), DARKBLUE);
                    DrawLine(static_cast<int>(offset.x), static_cast<int>(offset.y),
                        static_cast<int>(offset.x), static_cast<int>(offset.y) + SQUARE_SIZE, DARKBLUE);
                    DrawLine(static_cast<int>(offset.x) + SQUARE_SIZE, static_cast<int>(offset.y),
                        static_cast<int>(offset.x) + SQUARE_SIZE, static_cast<int>(offset.y) + SQUARE_SIZE, DARKBLUE);
                    DrawLine(static_cast<int>(offset.x), static_cast<int>(offset.y) + SQUARE_SIZE,
                        static_cast<int>(offset.x) + SQUARE_SIZE, static_cast<int>(offset.y) + SQUARE_SIZE, DARKBLUE);
                    break;
                case GridSquare::FULL:
                    DrawRectangle(static_cast<int>(offset.x), static_cast<int>(offset.y), SQUARE_SIZE, SQUARE_SIZE, YELLOW);
                    break;
                case GridSquare::MOVING:
                    DrawRectangle(static_cast<int>(offset.x), static_cast<int>(offset.y), SQUARE_SIZE, SQUARE_SIZE, DARKBLUE);
                    break;
                case GridSquare::BLOCK:
                    DrawRectangle(static_cast<int>(offset.x), static_cast<int>(offset.y), SQUARE_SIZE, SQUARE_SIZE, DARKBLUE);
                    break;
                case GridSquare::FADING:
                    DrawRectangle(static_cast<int>(offset.x), static_cast<int>(offset.y), SQUARE_SIZE, SQUARE_SIZE, fadingColor);
                    break;
                }
                offset.x += SQUARE_SIZE;
            }
            offset.x = controller;
            offset.y += SQUARE_SIZE;
        }

        offset.x = 500;
        offset.y = 45;
        int controler = static_cast<int>(offset.x);

        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 4; i++) {
                GridSquare cell = nextPiece.GetCell(i, j);

                if (cell == GridSquare::EMPTY) {
                    DrawLine(static_cast<int>(offset.x), static_cast<int>(offset.y),
                        static_cast<int>(offset.x) + SQUARE_SIZE, static_cast<int>(offset.y), LIGHTGRAY);
                    DrawLine(static_cast<int>(offset.x), static_cast<int>(offset.y),
                        static_cast<int>(offset.x), static_cast<int>(offset.y) + SQUARE_SIZE, LIGHTGRAY);
                    DrawLine(static_cast<int>(offset.x) + SQUARE_SIZE, static_cast<int>(offset.y),
                        static_cast<int>(offset.x) + SQUARE_SIZE, static_cast<int>(offset.y) + SQUARE_SIZE, LIGHTGRAY);
                    DrawLine(static_cast<int>(offset.x), static_cast<int>(offset.y) + SQUARE_SIZE,
                        static_cast<int>(offset.x) + SQUARE_SIZE, static_cast<int>(offset.y) + SQUARE_SIZE, LIGHTGRAY);
                }
                else if (cell == GridSquare::MOVING) {
                    DrawRectangle(static_cast<int>(offset.x), static_cast<int>(offset.y), SQUARE_SIZE, SQUARE_SIZE, GRAY);
                }
                offset.x += SQUARE_SIZE;
            }
            offset.x = controler;
            offset.y += SQUARE_SIZE;
        }

        DrawText("INCOMING:", controler, static_cast<int>(offset.y) - 100, 10, GRAY);
        DrawText(TextFormat("LINES:      %04i", lines), controler, static_cast<int>(offset.y) + 20, 10, GRAY);

        if (pause) {
            DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, GRAY);
        }
    }
    else {
        DrawText("PRESS [ENTER] TO PLAY AGAIN",
            GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2,
            GetScreenHeight() / 2 - 50, 20, GRAY);
    }

    EndDrawing();
}

void TetrisGame::Run() {
    InitWindow(screenWidth, screenHeight, "Classic Game: Tetris (OOP C++)");

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrameCallback, 60, 1);
#else
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        Update();
        Draw();
    }
#endif

    CloseWindow();
}

void TetrisGame::UpdateDrawFrameCallback() {
    if (instance) {
        instance->Update();
        instance->Draw();
    }
}