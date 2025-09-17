#include "TetrisGame.h"
#include <cstdlib>
#include <iostream>
#include <fstream>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

TetrisGame* TetrisGame::instance = nullptr;

TetrisGame::TetrisGame() : levelProgression(nullptr), maxAchievedLevel(1), levelUpAchieved(false) {
    instance = this;
    
    try {
        levelProgression = (int*)calloc(20, sizeof(int));
        if (!levelProgression) {
            std::cerr << "Greska: Memory allocation failed for TetrisGame level progression" << std::endl;
            throw std::bad_alloc();
        }
        
        LoadHighScore();
        Initialize();
        
    } catch (const std::exception& e) {
        std::cerr << "Greska pri kreiranju igre: " << e.what() << std::endl;
        if (levelProgression) {
            free(levelProgression);
            levelProgression = nullptr;
        }
        throw;
    }
}

TetrisGame::~TetrisGame() {
    try {
        SaveHighScore();
        gameGrid.PrintLineBreakStatistics();
        
        if (levelProgression) {
            free(levelProgression);
        }
    } catch (const std::exception& e) {
        std::cerr << "Greska pri zatvaranju igre: " << e.what() << std::endl;
    }
}

void TetrisGame::Initialize() {
    try {
        gameOver = false;
        pause = false;
        beginPlay = true;
        pieceActive = false;
        detection = false;
        lineToDelete = false;
        levelUpAchieved = false;

        level = 1;
        lines = 0;
        score = 0;
        fadingColor = GRAY;

        gravityMovementCounter = 0;
        lateralMovementCounter = 0;
        turnMovementCounter = 0;
        fastFallMovementCounter = 0;
        fadeLineCounter = 0;
        gravitySpeed = 30;

        gameGrid.Initialize();
        nextPiece.Initialize();
        
        if (levelProgression) {
            for (int i = 0; i < 20; i++) {
                levelProgression[i] = 0;
            }
        }
        
        std::cout << "Game initialized using inheritance" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Greska pri inicijalizaciji igre: " << e.what() << std::endl;
        throw;
    }
}

bool TetrisGame::CreatePiece() {
    try {
        if (beginPlay) {
            nextPiece.GenerateRandomPiece();
            beginPlay = false;
        }

        currentPiece.CopyFrom(nextPiece);
        
        int centerX = 4;
        currentPiece.SetPosition(centerX, 0);
        
        std::cout << "Creating piece at position: " << centerX << std::endl;
        
        nextPiece.GenerateRandomPiece();

        for (int i = currentPiece.GetPositionX(); i < currentPiece.GetPositionX() + 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (currentPiece.GetCell(i - currentPiece.GetPositionX(), j) == GridSquare::MOVING) {
                    std::cout << "Setting grid cell at (" << i << ", " << j << ") to MOVING" << std::endl;
                    gameGrid.SetCell(i, j, GridSquare::MOVING);
                }
            }
        }
        
        currentPiece.SetActive(true);

        return true;
    } catch (const std::exception& e) {
        std::cerr << "Greska pri kreiranju figura: " << e.what() << std::endl;
        return false;
    }
}

void TetrisGame::ResolveFallingMovement() {
    if (detection) {
        gameGrid.ConvertMovingToFull();
        detection = false;
        pieceActive = false;
        
        currentPiece.SetActive(false);
        std::cout << "Piece landed and deactivated" << std::endl;
    }
    else {
        gameGrid.MovePiecesDown();
        currentPiece.MoveY(1);
    }
}

bool TetrisGame::ResolveLateralMovement() {
    bool collision = false;

    try {
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
    } catch (const std::exception& e) {
        std::cerr << "Greska pri lateralnom pomeranju: " << e.what() << std::endl;
        collision = true;
    }

    return collision;
}

bool TetrisGame::ResolveTurnMovement() {
    try {
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
    } catch (const std::exception& e) {
        std::cerr << "Greska pri rotaciji: " << e.what() << std::endl;
    }
    return false;
}

void TetrisGame::UpdateLevel() {
    int newLevel = 1 + lines / 10;
    if (newLevel > level) {
        level = newLevel;
        gravitySpeed = 30 - (level * 2);
        if (gravitySpeed < 5) gravitySpeed = 5;
        
        levelUpAchieved = true;
        if (level > maxAchievedLevel) {
            maxAchievedLevel = level;
        }
        
        std::cout << "LEVEL UP! Novi level: " << level << std::endl;
    }
}

void TetrisGame::LoadHighScore() {
    try {
        std::ifstream file("highscore.dat");
        if (file.is_open()) {
            file >> highScore;
            file.close();
        } else {
            highScore = 0;
        }
    } catch (const std::exception& e) {
        std::cerr << "Greska pri citanju high score: " << e.what() << std::endl;
        highScore = 0;
    }
}

void TetrisGame::SaveHighScore() {
    try {
        if (score > highScore) {
            std::ofstream file("highscore.dat");
            if (file.is_open()) {
                file << score;
                file.close();
                std::cout << "Novi high score: " << score << "!" << std::endl;
            } else {
                std::cerr << "Greska: Ne mogu da sacuvam high score" << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Greska pri cuvanju high score: " << e.what() << std::endl;
    }
}

void TetrisGame::DemonstrateInheritance() {
    std::cout << "\n=== INHERITANCE DEMONSTRATION ===" << std::endl;
    
    std::cout << "This demonstrates how inheritance and polymorphism work:" << std::endl;
    BaseGameObject* objects[] = { &currentPiece, &gameGrid };
    
    for (int i = 0; i < 2; i++) {
        std::cout << "Object " << i << " (calling through base class pointer):" << std::endl;
        objects[i]->Update();
        objects[i]->ProcessLogic();
        std::cout << "Position: (" << objects[i]->GetPositionX() << ", " << objects[i]->GetPositionY() << ")" << std::endl;
        std::cout << "Active: " << (objects[i]->IsActive() ? "Yes" : "No") << std::endl;
        std::cout << std::endl;
    }
    
    std::cout << "================================\n" << std::endl;
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
                    SaveHighScore();
                }
            }
            else {
                fadeLineCounter++;

                if (fadeLineCounter % 8 < 4) {
                    fadingColor = MAROON;
                }
                else {
                    fadingColor = GRAY;
                }

                if (fadeLineCounter >= FADING_TIME) {
                    try {
                        int deletedLines = gameGrid.DeleteCompletedLines();
                        fadeLineCounter = 0;
                        lineToDelete = false;
                        lines += deletedLines;
                        
                        int lineScore = deletedLines * 100 * level;
                        score += lineScore;
                        
                        for (int i = 0; i < deletedLines; i++) {
                            gameGrid.RecordLineBreak(level, lines);
                        }
                        
                        UpdateLevel();
                        
                    } catch (const std::exception& e) {
                        std::cerr << "Greska pri brisanju linija: " << e.what() << std::endl;
                        lineToDelete = false;
                    }
                }
            }
        }
    }
    else {
        if (IsKeyPressed(KEY_ENTER)) {
            try {
                Initialize();
                gameOver = false;
            } catch (const std::exception& e) {
                std::cerr << "Greska pri restartovanju igre: " << e.what() << std::endl;
            }
        }
        
        if (IsKeyPressed('S')) {
            gameGrid.PrintLineBreakStatistics();
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
                        static_cast<int>(offset.x) + SQUARE_SIZE, static_cast<int>(offset.y), LIGHTGRAY);
                    DrawLine(static_cast<int>(offset.x), static_cast<int>(offset.y),
                        static_cast<int>(offset.x), static_cast<int>(offset.y) + SQUARE_SIZE, LIGHTGRAY);
                    DrawLine(static_cast<int>(offset.x) + SQUARE_SIZE, static_cast<int>(offset.y),
                        static_cast<int>(offset.x) + SQUARE_SIZE, static_cast<int>(offset.y) + SQUARE_SIZE, LIGHTGRAY);
                    DrawLine(static_cast<int>(offset.x), static_cast<int>(offset.y) + SQUARE_SIZE,
                        static_cast<int>(offset.x) + SQUARE_SIZE, static_cast<int>(offset.y) + SQUARE_SIZE, LIGHTGRAY);
                    break;
                case GridSquare::FULL:
                    DrawRectangle(static_cast<int>(offset.x), static_cast<int>(offset.y), SQUARE_SIZE, SQUARE_SIZE, GRAY);
                    break;
                case GridSquare::MOVING:
                    DrawRectangle(static_cast<int>(offset.x), static_cast<int>(offset.y), SQUARE_SIZE, SQUARE_SIZE, DARKGRAY);
                    break;
                case GridSquare::BLOCK:
                    DrawRectangle(static_cast<int>(offset.x), static_cast<int>(offset.y), SQUARE_SIZE, SQUARE_SIZE, LIGHTGRAY);
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
        DrawText(TextFormat("LEVEL:      %04i", level), controler, static_cast<int>(offset.y) + 35, 10, GRAY);
        DrawText(TextFormat("SCORE:      %06i", score), controler, static_cast<int>(offset.y) + 50, 10, GRAY);
        DrawText(TextFormat("HIGH SCORE: %06i", highScore), controler, static_cast<int>(offset.y) + 65, 10, GRAY);
        
        if (levelUpAchieved) {
            DrawText("LEVEL UP!", controler, static_cast<int>(offset.y) + 90, 12, RED);
        }

        if (pause) {
            DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, GRAY);
        }
    }
    else {
        DrawText("PRESS [ENTER] TO PLAY AGAIN",
            GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2,
            GetScreenHeight() / 2 - 50, 20, GRAY);
        DrawText("PRESS [S] FOR STATISTICS",
            GetScreenWidth() / 2 - MeasureText("PRESS [S] FOR STATISTICS", 16) / 2,
            GetScreenHeight() / 2 - 20, 16, GRAY);
        DrawText(TextFormat("FINAL SCORE: %06i", score),
            GetScreenWidth() / 2 - MeasureText(TextFormat("FINAL SCORE: %06i", score), 16) / 2,
            GetScreenHeight() / 2 + 30, 16, GRAY);
    }

    EndDrawing();
}

void TetrisGame::Run() {
    try {
        InitWindow(screenWidth, screenHeight, "Enhanced Tetris with Inheritance - Dynamic Memory & OOP");

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
    } catch (const std::exception& e) {
        std::cerr << "Greska pri pokretanju igre: " << e.what() << std::endl;
        if (IsWindowReady()) {
            CloseWindow();
        }
    }
}

void TetrisGame::UpdateDrawFrameCallback() {
    if (instance) {
        try {
            instance->Update();
            instance->Draw();
        } catch (const std::exception& e) {
            std::cerr << "Greska u game loop: " << e.what() << std::endl;
        }
    }
}