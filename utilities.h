//
// Created by Nicholas Rosati on 2021-02-17.
//

#ifndef CIS4820_UTILITIES_H
#define CIS4820_UTILITIES_H

#include <time.h>
#include "graphics.h"
#include "LinkedListAPI.h"

#define NEGATE(x) (x * -1)
#define MAX(x, y) (x > y ? x : y)
#define MIN(x, y) (x < y ? x : y)

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
extern void setMeshID(int id, int meshNumber, float xpos, float ypos, float zpos);
#endif
#ifndef UNSET_MESH_ID
#define UNSET_MESH_ID
extern void unsetMeshID(int);
#endif
#ifndef SET_TRANSLATE_MESH
#define SET_TRANSLATE_MESH
extern void setTranslateMesh(int id, float xpos, float ypos, float zpos);
#endif
#ifndef SET_ROTATE_MESH
#define SET_ROTATE_MESH
extern void setRotateMesh(int id, float x, float y, float z);
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
#ifndef SET_MOB_POS
#define SET_MOB_POS
extern void setMobPosition(int id, float x, float y, float z, float rotation);
#endif

//Colours
typedef enum {
    EMPTY = 0,
    GREEN = 1,
    BLUE = 2,
    RED = 3,
    BLACK = 4,
    WHITE = 5,
    PURPLE = 6,
    ORANGE = 7,
    YELLOW = 8,
    LIGHT_BROWN = 9,
    DARK_BROWN = 10,
    BEIGE = 11,
    GREY = 12
} Colour;

static float* CACTUS_MAP = (float[]){0.80f, 0.20f, 0.55f, 1.0f};
static float* BAT_MAP = (float[]){0.54f, 0.25f, 0.55f, 1.0f};
static float* FISH_MAP = (float[]){0.01f, 0.45f, 0.55f, 1.0f};

//Textures
typedef enum {
    GRASS = 20,
    DIRT = 21,
    STONE_BRICK = 22,
    DIRTY_FLOOR = 23,
    ASPHALT = 24,
    SUN_MOON_BOX = 26,
    FLOWER_BOX = 27,
    TREE_BOX = 28,
    SNOW = 29,
    CAVE_STONE = 55
} Texture;

//Meshes
typedef enum {
    FISH = 1,
    BAT = 2,
    CACTUS = 3,
    OPEN_CHEST = 5,
    KEY = 8,
    COIN = 11,
    SWORD = 4,
    ARMOUR = 6
} Mesh;

//Mesh IDs
typedef enum {
    KEY_MESH_ID = 10,
    ARMOUR_MESH_ID = 13,
    SWORD_MESH_ID = 14,
    CHEST_MESH_ID = 11,
    COIN_MESH_ID = 12
} StaticMeshID;

//Rotations
static const float NORTH = 90.0f;
static const float SOUTH = 270.0f;
static const float EAST = 0.0f;
static const float WEST = 180.0f;

static const float MESH_OFFSET = 0.15f;

//Other
static const float GRAVITY_AMT = 0.1f;
static const float MAP_SCALE = 1.1f; //Bigger number = smaller map
static const int HIT_CHANCE = 2; //The chance a player has to hit a mob; 1 in X chance

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

/**Mob details*/
typedef struct {
    /**Mob ID*/
    int id;
    /**Mob type*/
    Mesh type;
    bool isVisible;
    bool isDead;
    /**Mob location*/
    ThreeTupleFloat position;
    ThreeTupleFloat targetPosition;
    bool doMovement;
    /**Mob rotation in Y axis*/
    float rotation;
    /**Mob scale*/
    float scale;
} Mob;

/**Coin details*/
typedef struct {
    ThreeTupleFloat location;
    ThreeTupleFloat rotation;
} Coin;

/**Room details.*/
typedef struct {
    /**Lengths of the room*/
    TwoTupleInt length;
    /**Absolute starting coordinate of the room*/
    TwoTupleInt origin;
    /**Marker to check if the user has been to this room before*/;
    bool visited;
} Room;

/**Level types*/
typedef enum {
    OUTSIDE = 0,
    CAVE = 1,
    MAZE = 2
} LevelType;

/**Level details.*/
typedef struct {
    LevelType levelType;
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
    /**Level's mobs*/
    Mob mobs[9];
    /**Key location*/
    ThreeTupleFloat keyLocation;
    /**Key found*/
    bool keyFound;
    /**Chest location*/
    ThreeTupleFloat chestLocation;
    bool chestFound;
    /**Coin details*/
    Coin coin;
    bool coinFound;
    /**Sword details*/
    ThreeTupleFloat swordLocation;
    bool swordFound;
    /**Armour details*/
    ThreeTupleFloat armourLocation;
    bool armourFound;
} Level;

/**Player inventory*/
typedef struct {
    bool hasKey;
    bool hasArmour;
    bool hasSword;
    bool hasBow;
} Inventory;

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

/**
 * Returns the integer coordinate of a floating poit coordinate
 * @param coordinate Floating point coordinate
 * @return Integer coordinate
 */
ThreeTupleInt getIntPosFromFloat3Tuple(ThreeTupleFloat coordinate);

/**
 * Gets a integer position value from a floating point position.
 * @param position Floating point position
 * @return Integer position
 */
int getIntPosFromFloat (float position);

/**
 * Clamps a number
 * @param num Number to clamp.
 * @param min Minimum value.
 * @param max Maximum value.
 * @return Number that is : min >= num <= max
 */
int clamp(int num, int min, int max);

/**
 * Clamps a number
 * @param num Number to clamp.
 * @param min Minimum value.
 * @param max Maximum value.
 * @return Number that is : min >= num <= max
 */
float clampf(float num, float min, float max);

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
