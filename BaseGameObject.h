#ifndef BASE_GAME_OBJECT_H
#define BASE_GAME_OBJECT_H

#include "GameConstants.h"
#include <iostream>


class BaseGameObject {
protected:
    int positionX;
    int positionY;
    bool isActive;
    
public:
    BaseGameObject(int x = 0, int y = 0) : positionX(x), positionY(y), isActive(true) {
        std::cout << "BaseGameObject constructor called at position (" << x << ", " << y << ")" << std::endl;
    }
    
    virtual ~BaseGameObject() {
        std::cout << "BaseGameObject destructor called" << std::endl;
    }
    
    virtual void Update() {
        std::cout << "BaseGameObject Update() called" << std::endl;
    }
    
    virtual void Draw() const {
        std::cout << "BaseGameObject Draw() called" << std::endl;
    }
    
    virtual void Initialize() {
        isActive = true;
        std::cout << "BaseGameObject Initialize() called" << std::endl;
    }
    
    void SetPosition(int x, int y) {
        positionX = x;
        positionY = y;
        std::cout << "Position set to (" << x << ", " << y << ")" << std::endl;
    }
    
    int GetPositionX() const { return positionX; }
    int GetPositionY() const { return positionY; }
    
    void SetActive(bool active) { 
        isActive = active; 
        std::cout << "Object active state set to: " << (active ? "true" : "false") << std::endl;
    }
    
    bool IsActive() const { return isActive; }
    

    virtual void ProcessLogic() = 0;
};

#endif