//
// Created by Nicholas Rosati on 2021-02-17.
//

#include <math.h>
#include "dungeon.h"

Level* generateMazeLevel() {
    Level* level = calloc(1, sizeof(Level));
    level->seed = time(NULL);
    srand(level->seed);

    level->levelType = MAZE;

    //Build a platform
    for (int x = 0; x < WORLDX; x++) {
        for (int z = 0; z < WORLDZ; z++) {
#ifdef DEBUG
            level->world[x][0][z] = ((x + z) % 2) ? DARK_BROWN : LIGHT_BROWN; //Checker board for light/dark grey
#endif
#ifndef DEBUG
            level->world[x][0][z] = DIRTY_FLOOR;
#endif
        }
    }

    //Make and place rooms
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            int roomNumber = row + col * 3;
            level->rooms[roomNumber] = createRoom();

            int xOffset = rand() % (CELL_SIZE - level->rooms[roomNumber]->length.x - 3);
            int zOffset = rand() % (CELL_SIZE - level->rooms[roomNumber]->length.z - 3);

            level->rooms[roomNumber]->origin.x = col * CELL_SIZE + xOffset;
            level->rooms[roomNumber]->origin.z = row * CELL_SIZE + zOffset;

            drawRoom(level->rooms[roomNumber], level->world);
        }
    }

    generateMobs(level);
    placeKey(level);
    placeChest(level);
    placeCoin(level);
    placeArmour(level);
    placeSword(level);

    //Hallways
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 2; ++col) {
            connectTopBottom(level->rooms[row * 3 + col], level->rooms[row * 3 + col + 1], level->world);
            connectLeftRight(level->rooms[row + 3 * col], level->rooms[row + 3 * col + 3], level->world);
        }
    }

    //Place the viewport in a random room
    int roomNumber = rand() % 9;
    level->viewport.x = -((float) level->rooms[roomNumber]->origin.x + ((float) level->rooms[roomNumber]->length.x / 2.0f));
    level->viewport.y = -2.1f;
    level->viewport.z = -((float) level->rooms[roomNumber]->origin.z + ((float) level->rooms[roomNumber]->length.z / 2.0f));

    placeStairs(level, roomNumber);
    return level;
}

void placeSword(Level* level) {
    int roomNumber = rand() % 9;
    int lengthOffset;
    int widthOffset;

    do {
        lengthOffset = rand() % level->rooms[roomNumber]->length.x;
        widthOffset = rand() % level->rooms[roomNumber]->length.z;
    } while (level->world[level->rooms[roomNumber]->origin.x + lengthOffset][1][level->rooms[roomNumber]->origin.z + widthOffset] != EMPTY);

    level->swordLocation.x = (float) (level->rooms[roomNumber]->origin.x + lengthOffset) + 0.5f;
    level->swordLocation.y = 1.5f;
    level->swordLocation.z = (float) (level->rooms[roomNumber]->origin.z + widthOffset) + 0.5f;
    level->swordFound = false;
}

void placeArmour(Level* level) {
    int roomNumber = rand() % 9;
    int lengthOffset;
    int widthOffset;

    do {
        lengthOffset = rand() % level->rooms[roomNumber]->length.x;
        widthOffset = rand() % level->rooms[roomNumber]->length.z;
    } while (level->world[level->rooms[roomNumber]->origin.x + lengthOffset][1][level->rooms[roomNumber]->origin.z + widthOffset] != EMPTY);

    level->armourLocation.x = (float) (level->rooms[roomNumber]->origin.x + lengthOffset) + 0.5f;
    level->armourLocation.y = 1.5f;
    level->armourLocation.z = (float) (level->rooms[roomNumber]->origin.z + widthOffset) + 0.5f;
    level->armourFound = false;
    level->armourFound = false;
}

void placeCoin(Level* level) {
    int roomNumber = rand() % 9;
    int lengthOffset;
    int widthOffset;

    do {
        lengthOffset = rand() % level->rooms[roomNumber]->length.x;
        widthOffset = rand() % level->rooms[roomNumber]->length.z;
    } while (level->world[level->rooms[roomNumber]->origin.x + lengthOffset][1][level->rooms[roomNumber]->origin.z + widthOffset] != EMPTY);

    level->coin.location.x = (float) (level->rooms[roomNumber]->origin.x + lengthOffset) + 0.5f;
    level->coin.location.y = 1.5f;
    level->coin.location.z = (float) (level->rooms[roomNumber]->origin.z + widthOffset) + 0.5f;
    level->coinFound = false;
}

void placeChest(Level* level) {
    int roomNumber = rand() % 9;
    int lengthOffset;
    int widthOffset;

    do {
        lengthOffset = rand() % level->rooms[roomNumber]->length.x;
        widthOffset = rand() % level->rooms[roomNumber]->length.z;
    } while (level->world[level->rooms[roomNumber]->origin.x + lengthOffset][1][level->rooms[roomNumber]->origin.z + widthOffset] != EMPTY);

    level->chestLocation.x = (float) (level->rooms[roomNumber]->origin.x + lengthOffset) + 0.5f;
    level->chestLocation.y = 1.0f;
    level->chestLocation.z = (float) (level->rooms[roomNumber]->origin.z + widthOffset) + 0.5f;
    level->chestFound = false;
}

void placeKey(Level* level) {
    int roomNumber = rand() % 9;
    int lengthOffset;
    int widthOffset;

    do {
        lengthOffset = rand() % level->rooms[roomNumber]->length.x;
        widthOffset = rand() % level->rooms[roomNumber]->length.z;
    } while (level->world[level->rooms[roomNumber]->origin.x + lengthOffset][1][level->rooms[roomNumber]->origin.z + widthOffset] != EMPTY);

    level->keyLocation.x = (float) (level->rooms[roomNumber]->origin.x + lengthOffset) + 0.5f;
    level->keyLocation.y = 1.5f;
    level->keyLocation.z = (float) (level->rooms[roomNumber]->origin.z + widthOffset) + 0.5f;
    level->keyFound = false;
}

void generateMobs(Level* level) {
    for (int i = 0; i < 9; ++i) {
        level->mobs[i].id = i;

        //Place the mob
        do {
            level->mobs[i].position.x = ((float)((rand() % (level->rooms[i]->length.x - 4)) + 2 + level->rooms[i]->origin.x) + 0.5f);
            level->mobs[i].position.z = ((float)((rand() % (level->rooms[i]->length.z - 4)) + 2 + level->rooms[i]->origin.z) + 0.5f);
        } while (level->world[(int) floorf(level->mobs[i].position.x)][1][(int) floorf(level->mobs[i].position.x)] != 0);

        //Pick a target for the mob
        //HACK: Should be actual pathfinding
        do {
            level->mobs[i].targetPosition.x = (float)(rand() % (level->rooms[i]->length.x - 4) + 2 + level->rooms[i]->origin.x) + 0.5f;
            level->mobs[i].targetPosition.z = (float)(rand() % (level->rooms[i]->length.z - 4) + 2 + level->rooms[i]->origin.z) + 0.5f;
        } while (level->world[(int) floorf(level->mobs[i].targetPosition.x)][1][(int) floorf(level->mobs[i].targetPosition.z)] != 0);

        switch (rand() % 3) {
            case 0:
                level->mobs[i].type = FISH;
                level->mobs[i].scale = 0.5f;
                level->mobs[i].position.y = 1.50f;
                break;
            case 1:
                level->mobs[i].type = BAT;
                level->mobs[i].scale = 0.4f;
                level->mobs[i].position.y = 1.00f;
                break;
            case 2:
                level->mobs[i].type = CACTUS;
                level->mobs[i].scale = 0.5f;
                level->mobs[i].position.y = 1.00f;
        }

        level->mobs[i].isVisible = false;
        level->mobs[i].doMovement = true;
    }
}

void connectTopBottom(Room* roomOne, Room* roomTwo, GLubyte world[100][50][100]) {
    int hallwayOneStartX = ((rand() % (roomOne->length.x - 2)) + 1) + roomOne->origin.x;
    int hallwayTwoStartX = ((rand() % (roomTwo->length.x - 2)) + 1) + roomTwo->origin.x;
    int distanceOneTwo = (int) floorf((float) (roomTwo->origin.z - (roomOne->origin.z + roomOne->length.z)) / 2.0f);
    int distanceTwoOne = (int) ceilf((float) (roomTwo->origin.z - (roomOne->origin.z + roomOne->length.z)) / 2.0f);

    //Draws a solid hallway
    for (int x = -1; x <= 1; ++x) {
        for (int y = 1; y <= 3; ++y) {
            drawLineZ(hallwayOneStartX + x, y, roomOne->origin.z + roomOne->length.z - 1, roomTwo->origin.z - 1 - distanceOneTwo, world, STONE_BRICK);
            drawLineZ(hallwayTwoStartX + x, y, roomTwo->origin.z, roomOne->origin.z + roomOne->length.z + 1 + distanceTwoOne, world, STONE_BRICK);
            drawLineX(hallwayOneStartX + (hallwayOneStartX <= hallwayTwoStartX ? -1 : 1), hallwayTwoStartX + (hallwayTwoStartX < hallwayOneStartX ? -1 : 1), y, distanceOneTwo + roomOne->origin.z + roomOne->length.z + abs(distanceOneTwo - distanceTwoOne) + x, world, STONE_BRICK);
        }
    }

    //Hollows out a walkable area for the hallway
    for (int y = 1; y <= 2; ++y) {
        drawLineZ(hallwayOneStartX, y, roomOne->origin.z + roomOne->length.z - 1, roomTwo->origin.z - 1 - distanceOneTwo, world, EMPTY);
        drawLineZ(hallwayTwoStartX, y, roomTwo->origin.z, roomOne->origin.z + roomOne->length.z + 1 + distanceTwoOne, world, EMPTY);
        drawLineX(hallwayOneStartX, hallwayTwoStartX, y, roomOne->origin.z + roomOne->length.z + distanceOneTwo + abs(distanceOneTwo - distanceTwoOne), world, EMPTY);
    }
}

void connectLeftRight(Room* roomOne, Room* roomTwo, GLubyte world[100][50][100]) {
    int hallwayOneStartZ = ((rand() % (roomOne->length.z - 2)) + 1) + roomOne->origin.z;
    int hallwayTwoStartZ = ((rand() % (roomTwo->length.z - 2)) + 1) + roomTwo->origin.z;
    int distanceOneTwo = (int) floorf((float) (roomTwo->origin.x - (roomOne->origin.x + roomOne->length.x)) / 2.0f);
    int distanceTwoOne = (int) ceilf((float) (roomTwo->origin.x - (roomOne->origin.x + roomOne->length.x)) / 2.0f);

    //Draws a solid hallway
    for (int z = -1; z <= 1; ++z) {
        for (int y = 1; y <= 3; ++y) {
            drawLineX(roomOne->origin.x + roomOne->length.x, roomTwo->origin.x - 1 - distanceOneTwo, y, hallwayOneStartZ + z, world, STONE_BRICK);
            drawLineX(roomTwo->origin.x, roomOne->origin.x + roomOne->length.x + 1 + distanceTwoOne, y, hallwayTwoStartZ + z, world, STONE_BRICK);
            drawLineZ(roomOne->origin.x + roomOne->length.x + distanceOneTwo + abs(distanceOneTwo - distanceTwoOne) + z, y, hallwayOneStartZ + (hallwayOneStartZ <= hallwayTwoStartZ ? -1 : 1), hallwayTwoStartZ + (hallwayTwoStartZ < hallwayOneStartZ ? -1 : 1), world, STONE_BRICK);
        }
    }

    //Hollows out a walkable area for the hallway
    for (int y = 1; y <= 2; ++y) {
        drawLineX(roomOne->origin.x + roomOne->length.x - 1, roomTwo->origin.x - 1 - distanceOneTwo, y, hallwayOneStartZ, world, EMPTY);
        drawLineX(roomTwo->origin.x, roomOne->origin.x + roomOne->length.x + 1 + distanceTwoOne, y, hallwayTwoStartZ, world, EMPTY);
        drawLineZ(roomOne->origin.x + roomOne->length.x + distanceOneTwo + abs(distanceOneTwo - distanceTwoOne), y, hallwayOneStartZ, hallwayTwoStartZ, world, EMPTY);
    }

}

Room* createRoom() {
    Room* room = calloc(1, sizeof(Room));
    room->length.x = (rand() % ROOM_MAX_WIDTH) + ROOM_MIN_WIDTH;
    room->length.z = (rand() % ROOM_MAX_LENGTH) + ROOM_MIN_LENGTH;
    return room;
}

void drawRoom(Room* room, GLubyte world[100][50][100]) {
    for (int y = 1; y <= ROOM_HEIGHT; ++y) {
        drawLineX(room->origin.x, room->origin.x + room->length.x - 1, y, room->origin.z, world, STONE_BRICK);
        drawLineX(room->origin.x, room->origin.x + room->length.x - 1, y, room->origin.z + room->length.z - 1, world, STONE_BRICK);
        drawLineZ(room->origin.x, y, room->origin.z, room->origin.z + room->length.z - 1, world, STONE_BRICK);
        drawLineZ(room->origin.x + room->length.x - 1, y, room->origin.z, room->origin.z + room->length.z - 1, world, STONE_BRICK);
#ifndef DEBUG
        //Roof
        if (y == ROOM_HEIGHT) {
            for (int zOffset = 0; zOffset <= room->length.z - 1; ++zOffset) {
                drawLineX(room->origin.x, room->origin.x + room->length.x - 1, y, room->origin.z + zOffset, world, ASPHALT);
            }
        }
#endif
    }

    populateRoom(room, world);
}

void populateRoom(Room* room, GLubyte world[100][50][100]) {
    //Place cubes
    for (int i = 0; i < 2; i++) {
        int offsetX = rand() % (room->length.x - 2);
        int offsetZ = rand() % (room->length.z - 2);

        //Choose x random colour for the block
        switch (rand() % 3) {
            case 0:
                world[room->origin.x + offsetX + 1][1][room->origin.z + offsetZ + 1] = SUN_MOON_BOX;
                break;
            case 1:
                world[room->origin.x + offsetX + 1][1][room->origin.z + offsetZ + 1] = FLOWER_BOX;
                break;
            case 2:
                world[room->origin.x + offsetX + 1][1][room->origin.z + offsetZ + 1] = TREE_BOX;
                break;
        }
    }
}

void placeStairs(Level* level, int roomNumber) {
    //Add stairs up
    level->stairsUp.x = (rand() % level->rooms[roomNumber]->length.x) + level->rooms[roomNumber]->origin.x;
    level->stairsUp.y = 0;
    level->stairsUp.z = (rand() % level->rooms[roomNumber]->length.z) + level->rooms[roomNumber]->origin.z;
    level->world[level->stairsUp.x][level->stairsUp.y][level->stairsUp.z] = WHITE;

    //Add stairs down
    roomNumber = rand() % 9;
    level->stairsDown.x = (rand() % level->rooms[roomNumber]->length.x) + level->rooms[roomNumber]->origin.x;
    level->stairsDown.y = 0;
    level->stairsDown.z = (rand() % level->rooms[roomNumber]->length.z) + level->rooms[roomNumber]->origin.z;
    level->world[level->stairsDown.x][level->stairsDown.y][level->stairsDown.z] = GREY;
}

void drawLineX(int xStart, int xEnd, int y, int z, GLubyte world[100][50][100], GLubyte color) {
    if (xStart < xEnd) {
        for (int x = xStart; x <= xEnd; ++x) {
            world[x][y][z] = color;
        }
    } else {
        for (int x = xStart; x >= xEnd; --x) {
            world[x][y][z] = color;
        }
    }
}

void drawLineZ(int x, int y, int zStart, int zEnd, GLubyte world[100][50][100], GLubyte color) {
    if (zStart < zEnd) {
        for (int z = zStart; z <= zEnd; ++z) {
            world[x][y][z] = color;
        }
    } else {
        for (int z = zStart; z >= zEnd; --z) {
            world[x][y][z] = color;
        }
    }
}

int isInRoom(TwoTupleInt coordinate, Level* level) {
    for (int i = 0; i < NUM_ROOMS; ++i) {
        TwoTupleInt roomStart = {.x = level->rooms[i]->origin.x, .z = level->rooms[i]->origin.z};
        TwoTupleInt roomEnd = {.x = level->rooms[i]->origin.x + level->rooms[i]->length.x - 1, .z = level->rooms[i]->origin.z + level->rooms[i]->length.z - 1};

        if (isInArea(coordinate, roomStart, roomEnd)) {
            return i;
        }
    }

    return -1;
}

bool isInArea(TwoTupleInt coordinate, TwoTupleInt startCoordinate, TwoTupleInt endCoordinate) {
    bool isInX = coordinate.x >= startCoordinate.x && coordinate.x <= endCoordinate.x;
    bool isInZ = coordinate.z >= startCoordinate.z && coordinate.z <= endCoordinate.z;

    return isInX && isInZ;
}
