#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

constexpr int SQUARE_SIZE = 20;
constexpr int GRID_HORIZONTAL_SIZE = 12;
constexpr int GRID_VERTICAL_SIZE = 20;
constexpr int LATERAL_SPEED = 10;
constexpr int TURNING_SPEED = 12;
constexpr int FAST_FALL_AWAIT_COUNTER = 30;
constexpr int FADING_TIME = 33;
constexpr int LINES_PER_LEVEL = 10;

enum class GridSquare {
    EMPTY,
    MOVING,
    FULL,
    BLOCK,
    FADING
};

#endif