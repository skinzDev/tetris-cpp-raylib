#ifndef GAME_GRID_H
#define GAME_GRID_H

#include "BaseGameObject.h"
#include <array>
#include <memory>
#include <fstream>

struct LineBreakRecord {
    int level;
    int totalLines;
    int timestamp;
    
    LineBreakRecord(int lvl, int lines, int time) 
        : level(lvl), totalLines(lines), timestamp(time) {}
};

class GameGrid : public BaseGameObject {
private:
    std::array<std::array<GridSquare, GRID_VERTICAL_SIZE>, GRID_HORIZONTAL_SIZE> grid;
    
    LineBreakRecord** lineBreakHistory;
    int* lineBreaksPerLevel;
    int maxLevels;
    int historyCount;
    int historyCapacity;
    
    void resizeHistory();
    void saveLineBreaksToFile() const;

public:
    GameGrid();
    virtual ~GameGrid();
    
    virtual void Update() override;
    virtual void Draw() const override;
    virtual void Initialize() override;
    virtual void ProcessLogic() override;
    
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
    
    void RecordLineBreak(int level, int totalLines);
    int GetLineBreaksForLevel(int level) const;
    void LoadLineBreaksFromFile();
    void PrintLineBreakStatistics() const;
    void CleanupMemory();
    
    void ResetGrid() {
        std::cout << "Resetting grid using inherited Initialize()" << std::endl;
        Initialize();
        SetActive(true);
    }
};

#endif