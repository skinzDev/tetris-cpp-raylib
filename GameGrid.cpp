#include "GameGrid.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cstring>

GameGrid::GameGrid() : BaseGameObject(0, 0), lineBreakHistory(nullptr), lineBreaksPerLevel(nullptr), 
                      maxLevels(10), historyCount(0), historyCapacity(100) {
    std::cout << "GameGrid constructor - inheriting from BaseGameObject" << std::endl;
    try {
        lineBreakHistory = (LineBreakRecord**)malloc(historyCapacity * sizeof(LineBreakRecord*));
        if (!lineBreakHistory) {
            std::cerr << "Greska: Memory allocation failed for GameGrid history array" << std::endl;
            throw std::bad_alloc();
        }
        
        lineBreaksPerLevel = (int*)calloc(maxLevels, sizeof(int));
        if (!lineBreaksPerLevel) {
            free(lineBreakHistory);
            std::cerr << "Greska: Memory allocation failed for GameGrid level counter" << std::endl;
            throw std::bad_alloc();
        }
        
        Initialize();
        LoadLineBreaksFromFile();
    } catch (const std::exception& e) {
        std::cerr << "Greska pri inicijalizaciji GameGrid: " << e.what() << std::endl;
        CleanupMemory();
        throw;
    }
}

GameGrid::~GameGrid() {
    std::cout << "GameGrid destructor called" << std::endl;
    saveLineBreaksToFile();
    CleanupMemory();
}

void GameGrid::CleanupMemory() {
    if (lineBreakHistory) {
        for (int i = 0; i < historyCount; i++) {
            delete lineBreakHistory[i];
        }
        free(lineBreakHistory);
        lineBreakHistory = nullptr;
    }
    
    if (lineBreaksPerLevel) {
        free(lineBreaksPerLevel);
        lineBreaksPerLevel = nullptr;
    }
}

void GameGrid::Initialize() {
    std::cout << "GameGrid Initialize() - overridden from BaseGameObject" << std::endl;
    BaseGameObject::Initialize();
    
    try {
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
    } catch (const std::exception& e) {
        std::cerr << "Greska pri inicijalizaciji grid-a: " << e.what() << std::endl;
        throw;
    }
}

void GameGrid::Update() {
    std::cout << "GameGrid Update() - overridden from BaseGameObject" << std::endl;
    BaseGameObject::Update();
}

void GameGrid::Draw() const {
    std::cout << "GameGrid Draw() - overridden from BaseGameObject" << std::endl;
}

void GameGrid::ProcessLogic() {
    std::cout << "GameGrid ProcessLogic() - implementing pure virtual function" << std::endl;
    if (!IsActive()) {
        std::cout << "Grid is inactive, skipping logic processing" << std::endl;
        return;
    }
}

GridSquare GameGrid::GetCell(int x, int y) const {
    if (x >= 0 && x < GRID_HORIZONTAL_SIZE && y >= 0 && y < GRID_VERTICAL_SIZE) {
        return grid[x][y];
    }
    
    std::cerr << "Greska: Pristup van granica grid-a na poziciji (" << x << ", " << y << ")" << std::endl;
    return GridSquare::BLOCK;
}

void GameGrid::SetCell(int x, int y, GridSquare value) {
    if (x >= 0 && x < GRID_HORIZONTAL_SIZE && y >= 0 && y < GRID_VERTICAL_SIZE) {
        grid[x][y] = value;
    } else {
        std::cerr << "Greska: Pokusaj postavljanja van granica na (" << x << ", " << y << ")" << std::endl;
    }
}

bool GameGrid::IsGameOver() const {
    try {
        for (int j = 0; j < 2; j++) {
            for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++) {
                if (grid[i][j] == GridSquare::FULL) {
                    return true;
                }
            }
        }
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Greska pri proveri game over: " << e.what() << std::endl;
        return true;
    }
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

void GameGrid::resizeHistory() {
    if (historyCount >= historyCapacity) {
        try {
            int newCapacity = historyCapacity * 2;
            LineBreakRecord** newHistory = (LineBreakRecord**)realloc(lineBreakHistory, 
                                                                     newCapacity * sizeof(LineBreakRecord*));
            if (!newHistory) {
                std::cerr << "Greska: Memory allocation failed in resizeHistory" << std::endl;
                throw std::bad_alloc();
            }
            lineBreakHistory = newHistory;
            historyCapacity = newCapacity;
        } catch (const std::exception& e) {
            std::cerr << "Greska pri prosirenju memorije: " << e.what() << std::endl;
        }
    }
}

void GameGrid::RecordLineBreak(int level, int totalLines) {
    try {
        resizeHistory();
        
        if (level >= maxLevels) {
            int newMaxLevels = level + 5;
            int* newLevelCounter = (int*)realloc(lineBreaksPerLevel, newMaxLevels * sizeof(int));
            if (!newLevelCounter) {
                std::cerr << "Greska: Memory allocation failed for RecordLineBreak level expansion" << std::endl;
                throw std::bad_alloc();
            }
            
            for (int i = maxLevels; i < newMaxLevels; i++) {
                newLevelCounter[i] = 0;
            }
            
            lineBreaksPerLevel = newLevelCounter;
            maxLevels = newMaxLevels;
        }
        
        LineBreakRecord* record = new LineBreakRecord(level, totalLines, static_cast<int>(time(nullptr)));
        lineBreakHistory[historyCount++] = record;
        lineBreaksPerLevel[level]++;
        
        std::cout << "Linija obrisana! Level: " << level << ", Ukupno linija: " << totalLines << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Greska pri zapisivanju brisanja linije: " << e.what() << std::endl;
    }
}

int GameGrid::GetLineBreaksForLevel(int level) const {
    if (level >= 0 && level < maxLevels && lineBreaksPerLevel) {
        return lineBreaksPerLevel[level];
    }
    return 0;
}

void GameGrid::LoadLineBreaksFromFile() {
    try {
        std::ifstream file("tetris_stats.dat", std::ios::binary);
        if (!file.is_open()) {
            return;
        }
        
        int savedCount;
        file.read(reinterpret_cast<char*>(&savedCount), sizeof(int));
        
        for (int i = 0; i < savedCount && file.good(); i++) {
            int level, totalLines, timestamp;
            file.read(reinterpret_cast<char*>(&level), sizeof(int));
            file.read(reinterpret_cast<char*>(&totalLines), sizeof(int));
            file.read(reinterpret_cast<char*>(&timestamp), sizeof(int));
            
            RecordLineBreak(level, totalLines);
        }
        
        file.close();
        std::cout << "Ucitano " << historyCount << " rekorda iz fajla." << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Greska pri citanju statistika: " << e.what() << std::endl;
    }
}

void GameGrid::saveLineBreaksToFile() const {
    try {
        std::ofstream file("tetris_stats.dat", std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Greska: Ne mogu da kreiran fajl za statistike" << std::endl;
            return;
        }
        
        file.write(reinterpret_cast<const char*>(&historyCount), sizeof(int));
        
        for (int i = 0; i < historyCount; i++) {
            if (lineBreakHistory[i]) {
                file.write(reinterpret_cast<const char*>(&lineBreakHistory[i]->level), sizeof(int));
                file.write(reinterpret_cast<const char*>(&lineBreakHistory[i]->totalLines), sizeof(int));
                file.write(reinterpret_cast<const char*>(&lineBreakHistory[i]->timestamp), sizeof(int));
            }
        }
        
        file.close();
        
    } catch (const std::exception& e) {
        std::cerr << "Greska pri cuvanju statistika: " << e.what() << std::endl;
    }
}

void GameGrid::PrintLineBreakStatistics() const {
    std::cout << "\n=== TETRIS STATISTIKE ===" << std::endl;
    std::cout << "Ukupno brisanja linija: " << historyCount << std::endl;
    
    for (int level = 0; level < maxLevels; level++) {
        if (lineBreaksPerLevel[level] > 0) {
            std::cout << "Level " << level << ": " << lineBreaksPerLevel[level] << " brisanja" << std::endl;
        }
    }
    std::cout << "========================\n" << std::endl;
}