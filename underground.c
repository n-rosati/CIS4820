//
// Created by Nicholas Rosati on 2021-02-17.
//

#include "underground.h"
#include <stdlib.h>
#include <math.h>

Level* generateUndergroundLevel() {
    Level* level = calloc(1, sizeof(Level));
    level->seed = time(NULL);
    srand(level->seed);

    //Build x platform
    for (int x = 0; x < WORLDX; x++)
        for (int z = 0; z < WORLDZ; z++)
            level->world[x][0][z] = ((x + z) % 2) ? DARK_BROWN : LIGHT_BROWN; //Checker board for light/dark grey

    //Create rooms
    for (int l = 0; l < 9; l++)
        level->rooms[l] = createRoom();

    //Place rooms
    int numRooms = 0;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            drawRoom(row, col, rand() % (CELL_SIZE - level->rooms[numRooms]->length.x - 3), rand() % (CELL_SIZE - level->rooms[numRooms]->length.z - 3), level->rooms[numRooms], level->world);
            populateRoom(level->rooms[row * 3 + col], level->world);
            numRooms++;
        }
    }

    //Connect rooms with hallways
    for (int l = 0; l < 3; l++) {
        for (int m = 0; m < 2; m++) {
            drawHallwaysX(level->rooms[l * 3 + m], level->rooms[l * 3 + m + 1], level->world);
            drawHallwaysZ(level->rooms[l + 3 * m], level->rooms[l + 3 * m + 3], level->world);
        }
    }

    //Pick x random room to have the stairs
    //Up stairs
    int upStairsRoom = rand() % NUM_ROOMS;
    level->stairsUp.x = level->rooms[upStairsRoom]->origin.x + ((rand() % (level->rooms[upStairsRoom]->length.x - 2)) + 1);
    level->stairsUp.y = 0;
    level->stairsUp.z = level->rooms[upStairsRoom]->origin.z + ((rand() % (level->rooms[upStairsRoom]->length.z - 2)) + 1);

    //Down stairs
    int downStairsRoom;
    do {
        downStairsRoom = rand() % NUM_ROOMS;
    } while (downStairsRoom == upStairsRoom);
    level->stairsDown.x = level->rooms[downStairsRoom]->origin.x + ((rand() % (level->rooms[downStairsRoom]->length.x - 2)) + 1);
    level->stairsDown.y = 0;
    level->stairsDown.z = level->rooms[downStairsRoom]->origin.z + ((rand() % (level->rooms[downStairsRoom]->length.z - 2)) + 1);

    //Place stairs
    level->world[level->stairsUp.x][level->stairsUp.y][level->stairsUp.z] = WHITE;
    level->world[level->stairsDown.x][level->stairsDown.y][level->stairsDown.z] = GREY;

    //Set the initial view position
    int cameraStartingRoom;
    do {
        cameraStartingRoom = rand() % NUM_ROOMS;
    } while (cameraStartingRoom == upStairsRoom || cameraStartingRoom == downStairsRoom);
    int halfwayX = level->rooms[cameraStartingRoom]->origin.x + (level->rooms[cameraStartingRoom]->length.x / 2);
    int halfwayZ = level->rooms[cameraStartingRoom]->origin.z + (level->rooms[cameraStartingRoom]->length.z / 2);
    level->viewport.x = NEGATE(halfwayX);
    level->viewport.y = -2.0f;
    level->viewport.z = NEGATE(halfwayZ);

    return level;
}

Room* createRoom() {
    Room* room = calloc(1, sizeof(Room));
    room->length.x = (rand() % ROOM_MAX_WIDTH) + ROOM_MIN_WIDTH;
    room->length.z = (rand() % ROOM_MAX_LENGTH) + ROOM_MIN_LENGTH;
    return room;
}

//TODO: Use absolute coordinates instead of cells
void drawRoom(int row, int col, int xOffset, int zOffset, Room* room, GLubyte world[100][50][100]) {
    room->origin.x = (col * CELL_SIZE + xOffset);
    room->origin.z = (row * CELL_SIZE + zOffset);

    //Draw in x direction first
    for (int x = (col * CELL_SIZE + xOffset); x < (room->length.x + (col * CELL_SIZE + xOffset)); ++x) {
        for (int y = 1; y <= ROOM_HEIGHT; y++) {
            world[x][y][row * CELL_SIZE + zOffset] = BLACK;
            world[x][y][row * CELL_SIZE + zOffset + room->length.z - 1] = BLACK;
        }
    }

    //Draw in z direction second
    for (int z = (row * CELL_SIZE + zOffset); z < (room->length.z + (row * CELL_SIZE + zOffset)); ++z) {
        for (int y = 1; y <= ROOM_HEIGHT; y++) {
            world[col * CELL_SIZE + xOffset][y][z] = BLACK;
            world[col * CELL_SIZE + xOffset + room->length.x - 1][y][z] = BLACK;
        }
    }

    //Draw the roof
    for (int x = room->origin.x; x < room->origin.x + room->length.x; ++x)
        for (int z = room->origin.z; z < room->origin.z + room->length.z; ++z)
            world[x][ROOM_HEIGHT + 1][z] = DARK_BROWN;
}

void populateRoom(Room* room, GLubyte world[100][50][100]) {
    for (int i = 0; i < 2; i++) {
        int offsetX = rand() % (room->length.x - 2);
        int offsetZ = rand() % (room->length.z - 2);

        //Choose x random colour for the block
        switch (rand() % 3) {
            case 0:
                world[room->origin.x + offsetX + 1][1][room->origin.z + offsetZ + 1] = RED;
                break;
            case 1:
                world[room->origin.x + offsetX + 1][1][room->origin.z + offsetZ + 1] = GREEN;
                break;
            case 2:
                world[room->origin.x + offsetX + 1][1][room->origin.z + offsetZ + 1] = BLUE;
                break;
        }
    }
}

//TODO: Clean up draw[thing]X/Z

void drawHallwaysX(Room* roomOne, Room* roomTwo, GLubyte world[100][50][100]) {
    //Offsets on the walls
    int hallZOffset1 = rand() % (roomOne->length.z - 1 - HALL_RADIUS) + HALL_RADIUS;
    int hallZOffset2 = rand() % (roomTwo->length.z - 1 - HALL_RADIUS) + HALL_RADIUS;

    //Distance between the two walls in X direction
    int roomOneStartX = roomOne->origin.x + roomOne->length.x - 1;
    int roomTwoStartX = roomTwo->origin.x;
    int distanceX = roomTwoStartX - roomOneStartX;
    double halfDistanceX = distanceX / 2.0;

    //Draws the walls of the hallways
    for (int y = 1; y < HALL_HEIGHT; ++y) {
        //Extend out from room one
        for (int i = -1; i <= 1; ++i) {
            drawLineX(roomOneStartX, roomOneStartX + ((int)ceil(halfDistanceX)), y, roomOne->origin.z + hallZOffset1 + i, BEIGE, world);
        }

        //Extend from roomTwo
        for (int i = -1; i <= 1; ++i) {
            drawLineX(roomTwoStartX, roomTwoStartX - ((int)ceil(halfDistanceX)) + (distanceX % 2 ? 1 : 0), y, roomTwo->origin.z + hallZOffset2 + i, BEIGE, world);
        }

        //Connect the two extensions with x perpendicular line
        for (int i = -1; i <= 1; ++i) {
            drawLineZ(roomOneStartX + ((int)ceil(halfDistanceX)) + i, y, roomOne->origin.z + hallZOffset1, roomTwo->origin.z + hallZOffset2, BEIGE, world);
        }
    }

    //Draws the hollow walkway, using the same logic as above
    for (int y = 1; y < HALL_HEIGHT - 1; ++y) {
        drawLineX(roomOneStartX, roomOneStartX + ((int)ceil(halfDistanceX)), y, roomOne->origin.z + hallZOffset1, EMPTY, world);
        drawLineX(roomTwoStartX, roomTwoStartX - ((int)ceil(halfDistanceX)) + (distanceX % 2 ? 1 : 0), y, roomTwo->origin.z + hallZOffset2, EMPTY, world);
        drawLineZ(roomOneStartX + ((int)ceil(halfDistanceX)), y, roomOne->origin.z + hallZOffset1, roomTwo->origin.z + hallZOffset2, EMPTY, world);
    }
}

void drawHallwaysZ(Room* roomOne, Room* roomTwo, GLubyte world[100][50][100]) {
    //Offsets on the walls
    int hallXOffset1 = rand() % (roomOne->length.x - 1 - HALL_RADIUS) + HALL_RADIUS;
    int hallXOffset2 = rand() % (roomTwo->length.x - 1 - HALL_RADIUS) + HALL_RADIUS;

    //Get the distance between the two walls in Z direction
    int roomOneStartZ = roomOne->origin.z + roomOne->length.z - 1;
    int roomTwoStartZ = roomTwo->origin.z;
    int distanceZ = roomTwoStartZ - roomOneStartZ;
    double halfDistanceZ = distanceZ / 2.0;

    //Draws the walls of the hallways
    for (int y = 1; y < HALL_HEIGHT; ++y) {
        //Extend out from room one
        for (int i = -1; i <= 1; ++i) {
            drawLineZ(roomOne->origin.x + hallXOffset1 + i, y, roomOneStartZ, roomOneStartZ + ((int)ceil(halfDistanceZ)), BEIGE, world);
        }

        //Extend from room two
        for (int i = -1; i <= 1; ++i) {
            drawLineZ(roomTwo->origin.x + hallXOffset2 + i, y, roomTwoStartZ, roomTwoStartZ - ((int)ceil(halfDistanceZ)) + (distanceZ % 2 ? 1 : 0), BEIGE, world);
        }

        //Connect the two extensions with x perpendicular line
        for (int i = -1; i <= 1; ++i) {
            drawLineX(roomOne->origin.x + hallXOffset1, roomTwo->origin.x + hallXOffset2, y, roomOneStartZ + ((int)ceil(halfDistanceZ)) + i, BEIGE, world);
        }
    }

    //Draws empty line that is the walkway, using the same logic as above
    for (int y = 1; y < HALL_HEIGHT - 1; ++y) {
        drawLineZ(roomOne->origin.x + hallXOffset1, y, roomOneStartZ, roomOneStartZ + ((int)ceil(halfDistanceZ)), EMPTY, world);
        drawLineZ(roomTwo->origin.x + hallXOffset2, y, roomTwoStartZ, roomTwoStartZ - ((int)ceil(halfDistanceZ)) + (distanceZ % 2 ? 1 : 0), EMPTY, world);
        drawLineX(roomOne->origin.x + hallXOffset1, roomTwo->origin.x + hallXOffset2, y, roomOneStartZ + ((int)ceil(halfDistanceZ)), EMPTY, world);
    }
}

void drawLineX(int xStart, int xEnd, int y, int z, int cubeColour, GLubyte world[100][50][100]) {
    if (xStart < xEnd) {
        for (int x = xStart; x < xEnd + 1; ++x)
            world[x][y][z] = cubeColour;
    } else {
        for (int x = xStart; x > xEnd - 1; --x)
            world[x][y][z] = cubeColour;
    }
}

void drawLineZ(int x, int y, int zStart, int zEnd, int cubeColour, GLubyte world[100][50][100]) {
    if (zStart < zEnd) {
        for (int z = zStart; z < zEnd + 1; ++z)
            world[x][y][z] = cubeColour;
    } else {
        for (int z = zStart; z > zEnd - 1; --z) {
            world[x][y][z] = cubeColour;
        }
    }
}
