//
// Created by Nicholas Rosati on 2021-01-12.
//

#ifndef CIS4820_ELEMENTS_H
#define CIS4820_ELEMENTS_H

#include "graphics.h"

//Arbitrary room constants
#define ROOM_HEIGHT 3
#define ROOM_MIN_WIDTH 7
#define ROOM_MAX_WIDTH 18
#define ROOM_MIN_LENGTH 7
#define ROOM_MAX_LENGTH 18
#define CELL_SIZE 33

//Hallways stuff
#define HALL_RADIUS 2
#define HALL_HEIGHT 4

#define NEGATE(x) (x * -1)

//Colours
#define EMPTY       0
#define GREEN       1
#define BLUE        2
#define RED         3
#define BLACK       4
#define WHITE       5
#define PURPLE      6
#define ORANGE      7
#define YELLOW      8
#define LIGHT_BROWN 9
#define DARK_BROWN  10
#define BEIGE       11

#define GRAVITY_AMT 0.1f

/**Room details.*/
typedef struct Room {
    /**Length along the x axis*/
    int width;
    /**Length along the z axis*/
    int length;
    /**Absolute starting coordinate of the room*/
    int startX, startZ;
} Room;

/**Level details.*/
typedef  struct Level {
    /**Rooms in the level*/
    Room *rooms[9];
    /**Seed used to generate the level*/
    int seed;
    /**Viewport X.*/
    float viewportX;
    /**Viewport Y.*/
    float viewportY;
    /**Viewport Z.*/
    float viewportZ;
} Level;

Room* createRoom();
void deleteRoom(Room* room);
void drawRoom(int row, int col, int xOffset, int zOffset, Room* room, GLubyte world[100][50][100]);
void populateRoom(Room* room, GLubyte world[100][50][100]);
void drawHallwaysX(Room* roomOne, Room* roomTwo, GLubyte world[100][50][100]);
void drawHallwaysZ(Room* roomOne, Room* roomTwo, GLubyte world[100][50][100]);
void drawLineX(int xStart, int xEnd, int y, int z, int cubeColour, GLubyte world[100][50][100]);
void drawLineZ(int x, int y, int zStart, int zEnd, int cubeColour, GLubyte world[100][50][100]);

void setUserColourRGBA(int colourNumber, int red, int green, int blue, float alpha);

#endif //CIS4820_ELEMENTS_H
