#include "GameGrid.h"

GameGrid::GameGrid() {
    Initialize();
}

void GameGrid::Initialize() {
    for (int i = 0; i < GRID_HORIZONTAL_SIZE; i++) {
        for (int j = 0; j < GRID_VERTICAL_SIZE; j++) {
            if ((j == GRID_VERTICAL_SIZE - 1) || (i == 0) || (i == GRID_HORIZONTAL_SIZE - 1)) {
                grid[i][j] = GridSquare::BLOCK;
            }
            else {
                grid[i][j] = GridSquare::EMPTY;
            }
        }
    }
}

GridSquare GameGrid::GetCell(int x, int y) const {
    if (x >= 0 && x < GRID_HORIZONTAL_SIZE && y >= 0 && y < GRID_VERTICAL_SIZE) {
        return grid[x][y];
    }
    return GridSquare::BLOCK;
}

void GameGrid::SetCell(int x, int y, GridSquare value) {
    if (x >= 0 && x < GRID_HORIZONTAL_SIZE && y >= 0 && y < GRID_VERTICAL_SIZE) {
        grid[x][y] = value;
    }
}

bool GameGrid::IsGameOver() const {
    for (int j = 0; j < 2; j++) {
        for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++) {
            if (grid[i][j] == GridSquare::FULL) {
                return true;
            }
        }
    }
    return false;
}

void GameGrid::ConvertMovingToFull() {
    for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--) {
        for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++) {
            if (grid[i][j] == GridSquare::MOVING) {
                grid[i][j] = GridSquare::FULL;
            }
        }
    }
}

void GameGrid::ClearMovingPieces() {
    for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--) {
        for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++) {
            if (grid[i][j] == GridSquare::MOVING) {
                grid[i][j] = GridSquare::EMPTY;
            }
        }
    }
}

bool GameGrid::CheckCollisionDown() const {
    for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--) {
        for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++) {
            if ((grid[i][j] == GridSquare::MOVING) &&
                ((grid[i][j + 1] == GridSquare::FULL) || (grid[i][j + 1] == GridSquare::BLOCK))) {
                return true;
            }
        }
    }
    return false;
}

void GameGrid::MovePiecesDown() {
    for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--) {
        for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++) {
            if (grid[i][j] == GridSquare::MOVING) {
                grid[i][j + 1] = GridSquare::MOVING;
                grid[i][j] = GridSquare::EMPTY;
            }
        }
    }
}

bool GameGrid::CheckCollisionLeft() const {
    for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--) {
        for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++) {
            if (grid[i][j] == GridSquare::MOVING) {
                if ((i - 1 == 0) || (grid[i - 1][j] == GridSquare::FULL)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool GameGrid::CheckCollisionRight() const {
    for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--) {
        for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++) {
            if (grid[i][j] == GridSquare::MOVING) {
                if ((i + 1 == GRID_HORIZONTAL_SIZE - 1) || (grid[i + 1][j] == GridSquare::FULL)) {
                    return true;
                }
            }
        }
    }
    return false;
}

void GameGrid::MovePiecesLeft() {
    for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--) {
        for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++) {
            if (grid[i][j] == GridSquare::MOVING) {
                grid[i - 1][j] = GridSquare::MOVING;
                grid[i][j] = GridSquare::EMPTY;
            }
        }
    }
}

void GameGrid::MovePiecesRight() {
    for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--) {
        for (int i = GRID_HORIZONTAL_SIZE - 1; i >= 1; i--) {
            if (grid[i][j] == GridSquare::MOVING) {
                grid[i + 1][j] = GridSquare::MOVING;
                grid[i][j] = GridSquare::EMPTY;
            }
        }
    }
}

bool GameGrid::CheckCompletedLines() {
    for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--) {
        int calculator = 0;
        for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++) {
            if (grid[i][j] == GridSquare::FULL) {
                calculator++;
            }
        }
        if (calculator == GRID_HORIZONTAL_SIZE - 2) {
            for (int z = 1; z < GRID_HORIZONTAL_SIZE - 1; z++) {
                grid[z][j] = GridSquare::FADING;
            }
            return true;
        }
    }
    return false;
}

int GameGrid::DeleteCompletedLines() {
    int deletedLines = 0;

    for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--) {
        while (grid[1][j] == GridSquare::FADING) {
            for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++) {
                grid[i][j] = GridSquare::EMPTY;
            }

            for (int j2 = j - 1; j2 >= 0; j2--) {
                for (int i2 = 1; i2 < GRID_HORIZONTAL_SIZE - 1; i2++) {
                    if (grid[i2][j2] == GridSquare::FULL) {
                        grid[i2][j2 + 1] = GridSquare::FULL;
                        grid[i2][j2] = GridSquare::EMPTY;
                    }
                    else if (grid[i2][j2] == GridSquare::FADING) {
                        grid[i2][j2 + 1] = GridSquare::FADING;
                        grid[i2][j2] = GridSquare::EMPTY;
                    }
                }
            }
            deletedLines++;
        }
    }
    return deletedLines;
}