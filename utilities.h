//
// Created by Nicholas Rosati on 2021-02-17.
//

#ifndef CIS4820_UTILITIES_H
#define CIS4820_UTILITIES_H

#include <time.h>
#include "graphics.h"
#include "LinkedListAPI.h"

#define NEGATE(x) (x * -1)
#define MAX(x,y) (x > y ? x : y)
#define MIN(x,y) (x < y ? x : y)

//OpenGL functions
#ifndef SET_USR_COLOUR
#define SET_USR_COLOUR
extern int setUserColour(int, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
#endif
#ifndef SET_VPOS
#define SET_VPOS
extern void setViewPosition(float, float, float);
#endif
#ifndef SET_OLD_VPOS
#define SET_OLD_VPOS
extern void setOldViewPosition(float, float, float);
#endif
#ifndef GET_VPOS
#define GET_VPOS
extern void getViewPosition(float*, float*, float*);
#endif
#ifndef SET_TEXTURE
#define SET_TEXTURE
extern int setAssignedTexture(int, int);
#endif
#ifndef SET_MESH_ID
#define SET_MESH_ID
extern void setMeshID(int, int, float, float, float);
#endif
#ifndef UNSET_MESH_ID
#define UNSET_MESH_ID
extern void unsetMeshID(int);
#endif
#ifndef SET_TRANSLATE_MESH
#define SET_TRANSLATE_MESH
extern void setTranslateMesh(int, float, float, float);
#endif
#ifndef SET_ROTATE_MESH
#define SET_ROTATE_MESH
extern void setRotateMesh(int, float, float, float);
#endif
#ifndef SET_SCALE_MESH
#define SET_SCALE_MESH
extern void setScaleMesh(int, float);
#endif
#ifndef DRAW_MESH
#define DRAW_MESH
extern void drawMesh(int);
#endif
#ifndef HIDE_MESH
#define HIDE_MESH
extern void hideMesh(int);
#endif
extern int PointInFrustum( float x, float y, float z );


//Colours
static const GLubyte EMPTY = 0;
static const GLubyte GREEN = 1;
static const GLubyte BLUE = 2;
static const GLubyte RED = 3;
static const GLubyte BLACK = 4;
static const GLubyte WHITE = 5;
static const GLubyte PURPLE = 6;
static const GLubyte ORANGE = 7;
static const GLubyte YELLOW = 8;

static const GLubyte LIGHT_BROWN = 9;
static const GLubyte DARK_BROWN = 10;
static const GLubyte BEIGE = 11;
static const GLubyte GREY = 12;

//Textures
static const GLubyte GRASS = 20;
static const GLubyte DIRT = 21;
static const GLubyte STONE_BRICK = 22;
static const GLubyte DIRTY_FLOOR = 23;
static const GLubyte ASPHALT = 24;
static const GLubyte CONCRETE = 25;
static const GLubyte SUN_MOON_BOX = 26;
static const GLubyte FLOWER_BOX = 27;
static const GLubyte TREE_BOX = 28;
static const GLubyte SNOW = 29;

//Meshes
static const int COW = 0;
static const int FISH = 1;
static const int BAT = 2;
static const int CACTUS = 3;

//Other
static const float GRAVITY_AMT = 0.1f;
static const float MAP_SCALE = 1.5f;

/**
 * Two tuple of integers x and z
 */
typedef struct {
    int x, z;
} TwoTupleInt;

/**
 * Three tuple of integers x, z, and z
 */
typedef struct {
    int x, y, z;
} ThreeTupleInt;

/**
 * Two tuple of floats x and z
 */
typedef struct {
    float x, z;
} TwoTupleFloat;

/**
 * Three tuple of floats x, z, and z
 */
typedef struct {
    float x, y, z;
} ThreeTupleFloat;

/**Room details.*/
typedef struct {
    /**Lengths of the room*/
    TwoTupleInt length;
    /**Absolute starting coordinate of the room*/
    TwoTupleInt origin;
    /**Mob ID*/
    int mobID;
    /**Mob location*/
    ThreeTupleFloat mobPos;
    /**Mob visibility*/
    bool isMobVisible;
} Room;

/**Level details.*/
typedef struct {
    /**World associated with level*/
    GLubyte world[100][50][100];
    /**Rooms in level. NULL if outdoor level.*/
    Room* rooms[9];
    /**Seed used to generate the level*/
    int seed;
    /**Viewport location*/
    ThreeTupleFloat viewport;
    /**Stairs down location*/
    ThreeTupleInt stairsDown;
    /**Stairs up location*/
    ThreeTupleInt stairsUp;
} Level;

/**
 * Sets x user colour using RGBA instead of x number between 0 and 1.
 * @param colourNumber The colour number to set in OpenGL
 * @param red Red value [0-255]
 * @param green Green value [0-255]
 * @param blue Blue value [0-255]
 * @param alpha Alpha value [0-1]
 * @return
 */
void setUserColourRGBA(int colourNumber, int red, int green, int blue, float alpha);

void setTexture(int textureID, int colourID);

/**
 * Removes all blocks in the world.
 * @param world World to clear.
 */
void clearWorld(GLubyte world[100][50][100]);

/**
 * Moves the player to the next level (down).
 * @param levels Main levels list.
 * @param world World to put the level into.
 * @param newLevel The new level to add to the levels stack.
 */
void moveDown(List* levels, GLubyte world[100][50][100], Level* newLevel);

/**
 * Moves the player to the next level (up).
 * @param levels Main levels list.
 * @param world World to put the level into.
 */
void moveUp(List* levels, GLubyte world[100][50][100]);

/**
 * Will load the level into the active world.
 * @param level Level to load.
 * @param world World to load the level into.
 */
void loadLevel(Level* level, GLubyte world[100][50][100]);

/**
 * Returns the coordinate on the screen that a block starts at.
 * @param mapSize One of the sizes of the screen, length or width.
 * @param blockPosition The position in the world the block is at.
 * @return The coordinate of the start and end.
 */
TwoTupleInt get2DScreenPosFromBlock(int mapSize, int blockPosition);


//Functions for the LinkedListAPI
/**Dummy function. No functionality.*/
char* printLevel(void* level);

/**
 * Delete x level.
 * @param data Void pointer to level data.
 */
void deleteLevel(void* data);

/**Dummy function. No functionality.*/
int compareLevels(const void* level1, const void* level2);

#endif //CIS4820_UTILITIES_H
