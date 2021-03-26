
/* Derived from scene.c in the The OpenGL Programming Guide */
/* Keyboard and mouse rotation taken from Swiftless Tutorials #23 Part 2 */
/* http://www.swiftless.com/tutorials/opengl/camera2.html */

/* Frames per second code taken from : */
/* http://www.lighthouse3d.com/opengl/glut/index.php?fps */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "graphics.h"

#include "utilities.h"
#include "outside.h"
#include "underground.h"
#include "graphics.h"
#include "LinkedListAPI.h"

extern GLubyte  world[WORLDX][WORLDY][WORLDZ];

/* mouse function called by GLUT when a button is pressed or released */
void mouse(int, int, int, int);

/* initialize graphics library */
extern void graphicsInit(int *, char **);

/* lighting control */
extern void setLightPosition(GLfloat, GLfloat, GLfloat);
extern GLfloat* getLightPosition();

/* viewpoint control */
#ifndef SET_VPOS
extern void setViewPosition(float, float, float);
#endif
#ifndef GET_VPOS
extern void getViewPosition(float *, float *, float *);
#endif
extern void getOldViewPosition(float *, float *, float *);
#ifndef SET_OLD_VPOS
extern void setOldViewPosition(float, float, float);
#endif
extern void setViewOrientation(float, float, float);
extern void getViewOrientation(float *, float *, float *);

/* add cube to display list so it will be drawn */
extern void addDisplayList(int, int, int);

/* mob controls */
extern void createMob(int, float, float, float, float);
#ifndef SET_MOB_POS
extern void setMobPosition(int, float, float, float, float);
#endif
extern void hideMob(int);
extern void showMob(int);

/* player controls */
extern void createPlayer(int, float, float, float, float);
extern void setPlayerPosition(int, float, float, float, float);
extern void hidePlayer(int);
extern void showPlayer(int);

/* tube controls */
extern void createTube(int, float, float, float, float, float, float, int);
extern void hideTube(int);
extern void showTube(int);

/* 2D drawing functions */
extern void  draw2Dline(int x1, int y1, int x2, int y2, int lineWidth);
extern void  draw2Dbox(int x1, int y1, int x2, int y2);
extern void  draw2Dtriangle(int x1, int y1, int x2, int y2, int x3, int y3);
extern void  set2Dcolour(float colourv[]);

/* texture functions */
#ifndef SET_TEXTURE
extern int setAssignedTexture(int, int);
#endif
extern void unsetAssignedTexture(int);
extern int getAssignedTexture(int);
extern void setTextureOffset(int, float, float);


/* flag which is set to 1 when flying behaviour is desired */
extern int flycontrol;
/* flag used to indicate that the test world should be used */
extern int testWorld;
/* flag to print out frames per second */
extern int fps;
/* flag to indicate the space bar has been pressed */
extern int space;
/* flag indicates the program is a client when set = 1 */
extern int netClient;
/* flag indicates the program is a server when set = 1 */
extern int netServer;
/* size of the window in pixels */
extern int screenWidth, screenHeight;
/* flag indicates if map is to be printed */
extern int displayMap;
/* flag indicates use of a fixed viewpoint */
extern int fixedVP;

/* frustum corner coordinates, used for visibility determination  */
extern float corners[4][3];

/* determine which cubes are visible e.g. in view frustum */
extern void ExtractFrustum();
extern void tree(float, float, float, float, float, float, int);

/* allows users to define colours */
extern int setUserColour(int, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void unsetUserColour(int);
extern void getUserColour(int, GLfloat *, GLfloat *, GLfloat *, GLfloat *, GLfloat *, GLfloat *, GLfloat *, GLfloat *);

/* mesh creation, translatio, rotation functions */
#ifndef SET_MESH_ID
extern void setMeshID(int id, int meshNumber, float xpos, float ypos, float zpos);
#endif
#ifndef UNSET_MESH_ID
extern void unsetMeshID(int);
#endif
#ifndef SET_TRANSLATE_MESH
extern void setTranslateMesh(int, float, float, float);
#endif
#ifndef SET_ROTATE_MESH
extern void setRotateMesh(int, float, float, float);
#endif
#ifndef SET_SCALE_MESH
extern void setScaleMesh(int, float);
#endif
#ifndef DRAW_MESH
extern void drawMesh(int);
#endif
#ifndef HIDE_MESH
extern void hideMesh(int);
#endif

/********* end of extern variable declarations **************/

List* levels;
bool playerTurn = true;
bool moveTracked = false;
Inventory inventory = {.hasKey = false, .hasArmour = false, .hasSword = false, .hasBow = false};

/*** collisionResponse() ***/
/* -performs collision detection and response */
/*  sets new xyz  to position of the viewpoint after collision */
/* -can also be used to implement gravity by updating y position of vp*/
/* note that the world coordinates returned from getViewPosition()
   will be the negative value of the array indices */
void collisionResponse() {
#ifndef DEBUG
    ThreeTupleFloat oldVP;
    getOldViewPosition(&oldVP.x, &oldVP.y, &oldVP.z);
    oldVP.x = NEGATE(oldVP.x);
    oldVP.y = NEGATE(oldVP.y);
    oldVP.z = NEGATE(oldVP.z);

    ThreeTupleFloat newVP;
    getViewPosition(&newVP.x, &newVP.y, &newVP.z);
    newVP.x = NEGATE(newVP.x);
    newVP.y = NEGATE(newVP.y);
    newVP.z = NEGATE(newVP.z);
    ThreeTupleInt newVPInt = getIntPosFromFloat3Tuple(newVP);

    ThreeTupleFloat moveVector = {.x = newVP.x - oldVP.x, .y = newVP.y - oldVP.y, .z = newVP.z - oldVP.z};

    ThreeTupleFloat constructedVP = {.x = newVP.x + (moveVector.x * 1.5f), .y = newVP.y, .z = newVP.z + (moveVector.z * 1.5f)};
    ThreeTupleInt constructedVPInt = getIntPosFromFloat3Tuple(constructedVP);

    //Out of bounds checks
    if (constructedVP.x <= 0.0f || constructedVP.x >= 100.0f) newVP.x -= moveVector.x;
    if (constructedVP.y <= 0.0f || constructedVP.x >= 50.0f) newVP.y -= moveVector.y;
    if (constructedVP.z <= 0.0f || constructedVP.z >= 100.0f) newVP.z -= moveVector.z;

    if (world[constructedVPInt.x][constructedVPInt.y][constructedVPInt.z] != EMPTY) {
        //Ability to climb blocks
        if (world[constructedVPInt.x][constructedVPInt.y + 1][constructedVPInt.z] == EMPTY) {
            newVP.x += moveVector.x;
            newVP.y += 1.0f;
            newVP.z += moveVector.z;
            goto skipSlide;
        }

        //Negate movement in a direction if it hits a block
        if (world[constructedVPInt.x][constructedVPInt.y][newVPInt.z] != EMPTY) newVP.x -= moveVector.x;
        if (world[newVPInt.x][constructedVPInt.y][constructedVPInt.z] != EMPTY) newVP.z -= moveVector.z;
        skipSlide:;
    }

    Level* currentLevel = (Level*)(levels->head->data);
    if (!currentLevel->isOutside) {
        //Test for mob collisions
        for (int i = 0; i < 9; ++i) {
            Mob* mob = &currentLevel->mobs[i];
            if (mob->isDead) continue;

            ThreeTupleFloat mobPosition = (ThreeTupleFloat){.x = mob->position.x, .y = mob->position.y, .z = mob->position.z};
            if ((fabsf(mobPosition.x - constructedVP.x) < 1.00f && fabsf(mobPosition.z - constructedVP.z) < 1.00f)) {
                //Player colliding with a mob
                newVP.x -= moveVector.x;
                newVP.y -= moveVector.y;
                newVP.z -= moveVector.z;
                mob->doMovement = false;

                //Random change to hit or miss
                switch (rand() % HIT_CHANCE) {
                    case 0:
                        printf("Player attacked mob with ID %d!\n", mob->id);
                        mob->isDead = true;
                        hideMesh(mob->id);
                        break;
                    default:
                        printf("Player misses on mob with ID %d!\n", mob->id);
                        break;
                }
                playerTurn = false;
            } else if (!mob->doMovement) {
                mob->doMovement = true;
            }
        }
    }

    setViewPosition(NEGATE(newVP.x), NEGATE(newVP.y), NEGATE(newVP.z));
#endif
}


/******* draw2D() *******/
/* draws 2D shapes on screen */
/* use the following functions: 			*/
/*	draw2Dline(int x1, int y1, int x2, int y2, int lineWidth);		*/
/*	draw2Dbox(int, int, int, int);			*/
/*	draw2Dtriangle(int, int, int, int, int, int);	*/
/*	set2Dcolour(float[] colour); 				*/
/* colour must be set before other functions are called	*/
//(0,0) is bottom left of screen, (screenWidth, screenHeight) is top right of screen
void draw2D() {
    if (testWorld) {
        /* draw some sample 2d shapes */
        if (displayMap == 1) {
            GLfloat green[] = {0.0, 0.5, 0.0, 0.5};
            set2Dcolour(green);
            draw2Dline(0, 0, 500, 500, 15);
            draw2Dtriangle(0, 0, 200, 200, 0, 200);

            GLfloat black[] = {0.0, 0.0, 0.0, 0.5};
            set2Dcolour(black);
            draw2Dbox(500, 380, 524, 388);
        }
    } else {
        int screenMax = MIN(screenWidth, screenHeight);
        int mapDimension = (int) floorf((float)(screenMax) / MAP_SCALE);
        Level *currentLevel = levels->head->data;

        //Get player position
        ThreeTupleFloat player;
        getViewPosition(&player.x, &player.y, &player.z);
        ThreeTupleInt playerInt = getIntPosFromFloat3Tuple(player);

        if (displayMap == 1 || displayMap == 2) {
            if (!currentLevel->isOutside) {
                //Draw rooms
                for (int i = 0; i < NUM_ROOMS; ++i) {
                    if (displayMap == 2) {
                        if (displayMap == 2 && !currentLevel->rooms[i]->visited) continue;

                        //Draw the room
                        Room* currentRoom = currentLevel->rooms[i];
                        for (int x = currentRoom->origin.x; x < currentRoom->origin.x + currentRoom->length.x; ++x) {
                            for (int z = currentRoom->origin.z; z < currentRoom->origin.z + currentRoom->length.z; ++z) {
                                if (world[x][1][z] == EMPTY) continue;
                                TwoTupleInt blockX = get2DScreenPosFromBlock(mapDimension, x);
                                TwoTupleInt blockY = get2DScreenPosFromBlock(mapDimension, z);
                                switch (world[x][1][z]) {
                                    case STONE_BRICK:
                                        set2Dcolour((float[]){0.38f, 0.33f, 0.28f, 1.00f});
                                        break;
                                    case FLOWER_BOX:
                                        set2Dcolour((float[]){0.74f, 0.73f, 0.00f, 1.00f});
                                        break;
                                    case TREE_BOX:
                                        set2Dcolour((float[]){0.36f, 0.53f, 0.08f, 1.00f});
                                        break;
                                    case SUN_MOON_BOX:
                                        set2Dcolour((float[]){0.17f, 0.49f, 0.55f, 1.00f});
                                        break;
                                    default:
                                        break;
                                }
                                draw2Dbox(blockX.x, blockY.x, blockX.z, blockY.z);
                            }

                            //Draw stairs up
                            TwoTupleInt stairsUpX = get2DScreenPosFromBlock(mapDimension, (currentLevel->stairsUp.x));
                            TwoTupleInt stairsUpY = get2DScreenPosFromBlock(mapDimension, (currentLevel->stairsUp.z));
                            set2Dcolour((float[]){1.0f, 1.00f, 1.00f, 1.00f});
                            draw2Dbox(stairsUpX.x, stairsUpY.x, stairsUpX.z, stairsUpY.z);

                            //Draw stairs down
                            TwoTupleInt stairsDownX = get2DScreenPosFromBlock(mapDimension, (currentLevel->stairsDown.x));
                            TwoTupleInt stairsDownY = get2DScreenPosFromBlock(mapDimension, (currentLevel->stairsDown.z));
                            set2Dcolour((float[]){0.1f, 0.1f, 0.1f, 1.0f});
                            draw2Dbox(stairsDownX.x, stairsDownY.x, stairsDownX.z, stairsDownY.z);
                        }
                    } else {
                        //Draw dungeons
                        if (!currentLevel->isOutside) {
                            for (int x = 0; x < WORLDX; ++x) {
                                for (int z = 0; z < WORLDZ; ++z) {
                                    TwoTupleInt blockX = get2DScreenPosFromBlock(mapDimension, x);
                                    TwoTupleInt blockY = get2DScreenPosFromBlock(mapDimension, z);
                                    switch(world[x][1][z]) {
                                        case STONE_BRICK:
                                            set2Dcolour((float[]){0.38f, 0.33f, 0.28f, 1.0f});
                                            draw2Dbox(blockX.x, blockY.x, blockX.z, blockY.z);
                                            break;
                                        case FLOWER_BOX:
                                            set2Dcolour((float[]){0.74f, 0.73f, 0.0f, 1.0f});
                                            draw2Dbox(blockX.x, blockY.x, blockX.z, blockY.z);
                                            break;
                                        case TREE_BOX:
                                            set2Dcolour((float[]){0.36f, 0.525f, 0.08f, 1.0f});
                                            draw2Dbox(blockX.x, blockY.x, blockX.z, blockY.z);
                                            break;
                                        case SUN_MOON_BOX:
                                            set2Dcolour((float[]){0.17f, 0.49f, 0.55f, 1.0f});
                                            draw2Dbox(blockX.x, blockY.x, blockX.z, blockY.z);
                                            break;
                                        default:
                                            break;
                                    }
                                }
                            }

                            //Draw stairs up
                            TwoTupleInt stairsUpX = get2DScreenPosFromBlock(mapDimension, (currentLevel->stairsUp.x));
                            TwoTupleInt stairsUpY = get2DScreenPosFromBlock(mapDimension, (currentLevel->stairsUp.z));
                            set2Dcolour((float[]){1.0f, 1.00f, 1.00f, 1.00f});
                            draw2Dbox(stairsUpX.x, stairsUpY.x, stairsUpX.z, stairsUpY.z);

                            //Draw stairs down
                            TwoTupleInt stairsDownX = get2DScreenPosFromBlock(mapDimension, (currentLevel->stairsDown.x));
                            TwoTupleInt stairsDownY = get2DScreenPosFromBlock(mapDimension, (currentLevel->stairsDown.z));
                            set2Dcolour((float[]){0.1f, 0.1f, 0.1f, 1.0f});
                            draw2Dbox(stairsDownX.x, stairsDownY.x, stairsDownX.z, stairsDownY.z);
                        }
                    }
                }

                //Mobs. Only show close mobs if map type is 2
                {
                    float roomDiagonal = sqrtf(powf((float) ROOM_MAX_LENGTH, 2) + powf((float) ROOM_MAX_WIDTH, 2));
                    for (int i = 0; i < 9; ++i) {
                        if (currentLevel->mobs[i].isDead) continue;

                        switch (currentLevel->mobs[i].type) {
                            case CACTUS:
                                set2Dcolour(CACTUS_MAP);
                                break;
                            case BAT:
                                set2Dcolour(BAT_MAP);
                                break;
                            case FISH:
                                set2Dcolour(FISH_MAP);
                                break;
                        }

                        if (displayMap == 2) {
                            if (sqrtf(powf((currentLevel->viewport.x - currentLevel->mobs[i].position.x), 2) + powf((currentLevel->viewport.z - currentLevel->mobs[i].position.z), 2)) <= roomDiagonal) {
                                TwoTupleInt mobX = get2DScreenPosFromBlock(mapDimension, (int)floorf(currentLevel->mobs[i].position.x));
                                TwoTupleInt mobY = get2DScreenPosFromBlock(mapDimension, (int)floorf(currentLevel->mobs[i].position.z));
                                draw2Dbox(mobX.x, mobY.x, mobX.z, mobY.z);
                            }
                        } else {
                            TwoTupleInt mobX = get2DScreenPosFromBlock(mapDimension, (int)floorf(currentLevel->mobs[i].position.x));
                            TwoTupleInt mobY = get2DScreenPosFromBlock(mapDimension, (int)floorf(currentLevel->mobs[i].position.z));
                            draw2Dbox(mobX.x, mobY.x, mobX.z, mobY.z);
                        }
                    }
                }
            } else {
                //Draw stairs down
                TwoTupleInt stairsDownX = get2DScreenPosFromBlock(mapDimension, (currentLevel->stairsDown.x));
                TwoTupleInt stairsDownY = get2DScreenPosFromBlock(mapDimension, (currentLevel->stairsDown.z));
                set2Dcolour((float[]){0.1f, 0.1f, 0.1f, 1.0f});
                draw2Dbox(stairsDownX.x, stairsDownY.x, stairsDownX.z, stairsDownY.z);
            }
        }

        //Common things
        if (displayMap == 1 || displayMap == 2) {
            //Draw player
            set2Dcolour((float[]){0.4f, 0.0f, 1.0f, 1.0f});
            TwoTupleInt playerPositionX = get2DScreenPosFromBlock(mapDimension, playerInt.x);
            TwoTupleInt playerPositionY = get2DScreenPosFromBlock(mapDimension, playerInt.z);
            draw2Dbox(playerPositionX.x, playerPositionY.x, playerPositionX.z, playerPositionY.z);

            //Background for map
            set2Dcolour((float[]){0.1f, 0.1f, 0.1f, 0.75f});
            draw2Dbox(0, 0, mapDimension, mapDimension);
        }

        //Inventory
        {
            //Stairs key
            if (inventory.hasKey) {
                set2Dcolour((float[]) {0.80f, 0.75f, 0.12f, 1.00f});
                int size = (int) floorf(0.125f * (float) (screenMax));
                draw2Dbox(screenWidth - size, 0 + size, screenWidth, 0); //FIXME: Make this an actual key shape
            }
        }
    }
}


/*** update() ***/
/* background process, it is called when there are no other events */
/* -used to control animations and perform calculations while the  */
/*  system is running */
/* -gravity must also implemented here, duplicate collisionResponse */
void update() {
    int i, j, k;
    float *la;
    float x, y, z;

    /* sample animation for the testworld, don't remove this code */
    /* demo of animating mobs */
    if (testWorld) {

        /* update old position so it contains the correct value */
        /* -otherwise view position is only correct after a key is */
        /*  pressed and keyboard() executes. */

        getViewPosition(&x, &y, &z);
        setOldViewPosition(x,y,z);

        /* sample of rotation and positioning of mob */
        /* coordinates for mob 0 */
        static float mob0x = 50.0, mob0y = 25.0, mob0z = 52.0;
        static float mob0ry = 0.0;
        static int increasingmob0 = 1;
        /* coordinates for mob 1 */
        static float mob1x = 50.0, mob1y = 25.0, mob1z = 52.0;
        static float mob1ry = 0.0;
        static int increasingmob1 = 1;
        /* counter for user defined colour changes */
        static int colourCount = 0;
        static GLfloat offset = 0.0;

        /* offset counter for animated texture */
        static float textureOffset = 0.0;

        /* scaling values for fish mesh */
        static float fishScale = 1.0;
        static int scaleCount = 0;
        static GLfloat scaleOffset = 0.0;

        /* move mob 0 and rotate */
        /* set mob 0 position */
        setMobPosition(0, mob0x, mob0y, mob0z, mob0ry);

        /* move mob 0 in the x axis */
        if (increasingmob0 == 1)
            mob0x += 0.2;
        else
            mob0x -= 0.2;
        if (mob0x > 50) increasingmob0 = 0;
        if (mob0x < 30) increasingmob0 = 1;

        /* rotate mob 0 around the y axis */
        mob0ry += 1.0;
        if (mob0ry > 360.0) mob0ry -= 360.0;

        /* move mob 1 and rotate */
        setMobPosition(1, mob1x, mob1y, mob1z, mob1ry);

        /* move mob 1 in the z axis */
        /* when mob is moving away it is visible, when moving back it */
        /* is hidden */
        if (increasingmob1 == 1) {
            mob1z += 0.2;
            showMob(1);
        } else {
            mob1z -= 0.2;
            hideMob(1);
        }
        if (mob1z > 72) increasingmob1 = 0;
        if (mob1z < 52) increasingmob1 = 1;

        /* rotate mob 1 around the y axis */
        mob1ry += 1.0;
        if (mob1ry > 360.0) mob1ry -= 360.0;

        /* change user defined colour over time */
        if (colourCount == 1) offset += 0.05;
        else offset -= 0.01;
        if (offset >= 0.5) colourCount = 0;
        if (offset <= 0.0) colourCount = 1;
        setUserColour(9, 0.7, 0.3 + offset, 0.7, 1.0, 0.3, 0.15 + offset, 0.3, 1.0);

        /* sample tube creation  */
        /* draws a purple tube above the other sample objects */
        createTube(1, 45.0, 30.0, 45.0, 50.0, 30.0, 50.0, 6);

        /* move texture for lava effect */
        textureOffset -= 0.01;
        setTextureOffset(18, 0.0, textureOffset);

        /* make fish grow and shrink (scaling) */
        if (scaleCount == 1) scaleOffset += 0.01;
        else scaleOffset -= 0.01;
        if (scaleOffset >= 0.5) scaleCount = 0;
        if (scaleOffset <= 0.0) scaleCount = 1;
        setScaleMesh(1, 0.5 + scaleOffset);

        /* make cow with id == 2 appear and disappear */
        /* use scaleCount as switch to flip draw/hide */
        /* rotate cow while it is visible */
        if (scaleCount == 0) {
            drawMesh(2);
            setRotateMesh(2, 0.0, 180.0 + scaleOffset * 100.0, 0.0);
        } else {
            hideMesh(2);
        }

        /* end testworld animation */


    } else {
        Level* currentLevel = levels->head->data;

        ThreeTupleFloat oldView;
        getOldViewPosition(&oldView.x, &oldView.y, &oldView.z);
        ThreeTupleInt oldViewInt = getIntPosFromFloat3Tuple(oldView);

        ThreeTupleFloat newView;
        getOldViewPosition(&newView.x, &newView.y, &newView.z);
        ThreeTupleInt newViewInt = getIntPosFromFloat3Tuple(newView);

#ifndef DEBUG
        //Check if the current block and block under the view port are air
        //If so, do gravity
        if (world[oldViewInt.x][oldViewInt.y][oldViewInt.z] == 0 && world[oldViewInt.x][oldViewInt.y - 1][oldViewInt.z] == 0) {
            setViewPosition(oldView.x, oldView.y + GRAVITY_AMT, oldView.z);
        }
#endif

        //Check if the player is on stairs
        if ((oldViewInt.x == currentLevel->stairsDown.x && (oldViewInt.y - 1) == currentLevel->stairsDown.y && oldViewInt.z == currentLevel->stairsDown.z)) {
            if (currentLevel->isOutside) {
                moveDown(levels, world, levels->head->next == NULL ? generateUndergroundLevel() : levels->head->next->data);
            } else if (currentLevel->keyFound == true) {
                moveDown(levels, world, levels->head->next == NULL ? generateUndergroundLevel() : levels->head->next->data);
            }
        } else if (oldViewInt.x == currentLevel->stairsUp.x && (oldViewInt.y - 1) == currentLevel->stairsUp.y && oldViewInt.z == currentLevel->stairsUp.z) {
            moveUp(levels, world);
        }

        //Timing
        const long minUpdateTime = 1000000 / 64; //1000000 microsecond = 1 second; Update 64 times per second
        struct timespec currentTime;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &currentTime);
        static struct timespec lastUpdate = (struct timespec) {.tv_sec = 0, .tv_nsec = 0};
        static long long deltaT = 0;

        deltaT += ((currentTime.tv_nsec - lastUpdate.tv_nsec) + (1000000000 * (currentTime.tv_sec - lastUpdate.tv_sec))) / 1000; //1000 ns = 1 μs

        float roomDiagonal = sqrtf(powf((float) ROOM_MAX_LENGTH, 2) + powf((float) ROOM_MAX_WIDTH, 2));

        while (deltaT >= minUpdateTime) { //Do the timing loop
            deltaT -= minUpdateTime;

            if (currentLevel->isOutside) {
                //Clouds
                {
                    static int cloudTick = 0;
                    if (cloudTick++ == 32) {
                        for (int a = 0; a < WORLDX; ++a) {
                            GLubyte end = world[a][49][WORLDZ - 1];
                            for (int b = WORLDZ - 1; b > 0; --b) {
                                world[a][49][b] = world[a][49][b - 1];
                            }
                            world[a][49][0] = end;
                        }
                        cloudTick = 0;
                    }
                }
            } else {
                //Mob movement
                if (!playerTurn) {
                    for (int l = 0; l < 9; ++l) {
                        Mob* mob = &currentLevel->mobs[l];
                        if (mob->isDead) continue;

                        //Mob arrived at target. Pick a new one.
                        if (mob->type != CACTUS && (fabsf(mob->position.x - mob->targetPosition.x) < 0.1f && fabsf(mob->position.z - mob->targetPosition.z) < 0.1f)) {
                            do {
                                mob->targetPosition.x = (float)(rand() % (currentLevel->rooms[l]->length.x - 4) + 2 + currentLevel->rooms[l]->origin.x) + 0.5f;
                                mob->targetPosition.z = (float)(rand() % (currentLevel->rooms[l]->length.z - 4) + 2 + currentLevel->rooms[l]->origin.z) + 0.5f;
                            } while (currentLevel->world[(int) floorf(currentLevel->mobs[l].targetPosition.x)][1][(int) floorf(currentLevel->mobs[l].targetPosition.z)] != 0);
                        }

                        //Player collision detection; Player and mob within 1 unit
                        if (fabsf(mob->position.x - fabsf(newView.x)) < 1.10f && fabsf(mob->position.z - fabsf(newView.z)) < 1.10f) {
                            //Collision with player
                            printf("Mob with ID %d hits the player!\n", mob->id);
                            mob->doMovement = false;
                        } else if (!mob->doMovement){
                            mob->doMovement = true;
                        }

                        if (mob->type != CACTUS && mob->doMovement) {
                            for (int m = 0; m < 100; ++m) {
                                if (m % 10) {
                                    if (fabsf(mob->position.x - mob->targetPosition.x) < 0.1f) {
                                        mob->position.x += mob->position.x < mob->targetPosition.x ? 0.1f : -0.1f;
                                        mob->rotation = mob->position.x < mob->targetPosition.x ? NORTH : SOUTH;
                                    } else {
                                        mob->position.z += mob->position.z < mob->targetPosition.z ? 0.1f : -0.1f;
                                        mob->rotation = mob->position.z < mob->targetPosition.z ? EAST : WEST;
                                    }

                                    //Update mob rotations and translations in OpenGL (if underground)
                                    setTranslateMesh(currentLevel->mobs[l].id, currentLevel->mobs[l].position.x, currentLevel->mobs[l].position.y, currentLevel->mobs[l].position.z);
                                    setRotateMesh(currentLevel->mobs[l].id, 0, currentLevel->mobs[l].rotation, 0);
                                }
                            }
                        }
                    }
                    playerTurn = true;
                }


                //Mob visibility checking
                {
                    for (int l = 0; l < 9; ++l) {
                        Mob* mob = &currentLevel->mobs[l];
                        float distanceMobPlayer = sqrtf(powf(mob->position.x - NEGATE(oldView.x), 2) + powf(mob->position.z - NEGATE(oldView.z), 2)); //Pythagorean theorem
                        if (distanceMobPlayer <= roomDiagonal) {
                            if (!mob->isDead && PointInFrustum(mob->position.x, mob->position.y, mob->position.z)) {
                                if (!mob->isVisible) {
                                    drawMesh(mob->id);
                                    mob->isVisible = true;
                                    switch (mob->type) {
                                        case FISH:
                                            printf("Fish mesh #%d now visible.\n", mob->id);
                                            break;
                                        case BAT:
                                            printf("Bat mesh #%d now visible.\n", mob->id);
                                            break;
                                        case CACTUS:
                                            printf("Cactus mesh #%d now visible.\n", mob->id);
                                            break;
                                    }
                                }
                            } else if (mob->isVisible) {
                                hideMesh(mob->id);
                                mob->isVisible = false;
                                switch (mob->type) {
                                    case FISH:
                                        printf("Fish mesh #%d now hidden.\n", mob->id);
                                        break;
                                    case BAT:
                                        printf("Bat mesh #%d now hidden.\n", mob->id);
                                        break;
                                    case CACTUS:
                                        printf("Cactus mesh #%d now hidden.\n", mob->id);
                                        break;
                                }
                            }
                        }
                    }
                }

                //Player tracking
                {
                    ThreeTupleFloat playerPos;
                    getViewPosition(&playerPos.x, &playerPos.y, &playerPos.z);
                    ThreeTupleInt playerPosInt = getIntPosFromFloat3Tuple(playerPos);

                    int roomNumber = isInRoom((TwoTupleInt){.x = playerPosInt.x, .z = playerPosInt.z}, currentLevel);
                    if (roomNumber != -1) {
                        currentLevel->rooms[roomNumber]->visited = true;
                    }

                    ThreeTupleFloat oldPosition;
                    getOldViewPosition(&oldPosition.x, &oldPosition.y, &oldPosition.z);
                    ThreeTupleInt oldPositionInt = getIntPosFromFloat3Tuple(oldPosition);

                    if (playerPosInt.x == oldPositionInt.x && playerPosInt.y == oldPositionInt.y && playerPosInt.z == oldPositionInt.z) {
                        moveTracked = false;
                    }

                    if (!(playerPosInt.x == oldPositionInt.x && playerPosInt.y == oldPositionInt.y && playerPosInt.z == oldPositionInt.z) && !moveTracked) {
                        playerTurn = false;
                        moveTracked = true;
                    }

                    ThreeTupleInt keyLocation = getIntPosFromFloat3Tuple(currentLevel->keyLocation);
                    if ((playerPosInt.x == keyLocation.x && playerPosInt.z == keyLocation.z) && !currentLevel->keyFound) {
                        inventory.hasKey = true;
                        currentLevel->keyFound = true;
                        unsetMeshID(10);
                    }
                }
            }
        }

        lastUpdate = currentTime;
    }
}


/* called by GLUT when a mouse button is pressed or released */
/* -button indicates which button was pressed or released */
/* -state indicates a button down or button up event */
/* -x,y are the screen coordinates when the mouse is pressed or */
/*  released */
void mouse(int button, int state, int x, int y) {
    if (testWorld) {
        if (button == GLUT_LEFT_BUTTON)
            printf("left button - ");
        else if (button == GLUT_MIDDLE_BUTTON)
            printf("middle button - ");
        else
            printf("right button - ");
        if (state == GLUT_UP)
            printf("up - ");
        else
            printf("down - ");
        printf("%d %d\n", x, y);
    }

    if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) {
        float camX = 0, camY = 0, camZ = 0;
        getViewPosition(&camX, &camY, &camZ);
        printf("\nFloat cam: X: %.2f\tY: %.2f\tZ: %.2f\n", camX, camY, camZ);
        ThreeTupleInt camLocation = getIntPosFromFloat3Tuple((ThreeTupleFloat) {.x = camX, .y = camY, .z = camZ});
        printf("Int cam: X: %d\tY: %d\tZ: %d\n", camLocation.x, camLocation.y, camLocation.z);
//        printf("Stairs down at: X: %d\tY: %d\tZ: %d\n\n", ((Level*)(levels->head->data))->stairsDown.x, ((Level*)(levels->head->data))->stairsDown.y, ((Level*)(levels->head->data))->stairsDown.z);
    }

#ifdef DEBUG
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        moveDown(levels, world, levels->head->next == NULL ? generateUndergroundLevel() : levels->head->next->data);
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
        moveUp(levels, world);
    }
#endif
}



int main(int argc, char** argv)
{
    int i, j, k;
    /* initialize the graphics system */
    graphicsInit(&argc, argv);


    /* the first part of this if statement builds a sample */
    /* world which will be used for testing */
    /* DO NOT remove this code. */
    /* Put your code in the else statment below */
    /* The testworld is only guaranteed to work with a world of
        with dimensions of 100,50,100. */
    if (testWorld == 1) {
        /* initialize world to empty */
        for(i=0; i<WORLDX; i++)
            for(j=0; j<WORLDY; j++)
                for(k=0; k<WORLDZ; k++)
                    world[i][j][k] = 0;

        /* some sample objects */
        /* build a red platform */
        for(i=0; i<WORLDX; i++) {
            for(j=0; j<WORLDZ; j++) {
                world[i][24][j] = 3;
            }
        }
        /* create some green and blue cubes */
        world[50][25][50] = 1;
        world[49][25][50] = 1;
        world[49][26][50] = 1;
        world[52][25][52] = 2;
        world[52][26][52] = 2;

        /* create user defined colour and draw cube */
        setUserColour(9, 0.7, 0.3, 0.7, 1.0, 0.3, 0.15, 0.3, 1.0);
        world[54][25][50] = 9;


        /* blue box shows xy bounds of the world */
        for(i=0; i<WORLDX-1; i++) {
            world[i][25][0] = 2;
            world[i][25][WORLDZ-1] = 2;
        }
        for(i=0; i<WORLDZ-1; i++) {
            world[0][25][i] = 2;
            world[WORLDX-1][25][i] = 2;
        }

        /* create two sample mobs */
        /* these are animated in the update() function */
        createMob(0, 50.0, 25.0, 52.0, 0.0);
        createMob(1, 50.0, 25.0, 52.0, 0.0);

        /* create sample player */
        createPlayer(0, 52.0, 27.0, 52.0, 0.0);

        /* texture examples */

        /* create textured cube */
        /* create user defined colour with an id number of 11 */
        setUserColour(11, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
        /* attach texture 22 to colour id 11 */
        setAssignedTexture(11, 22);
        /* place a cube in the world using colour id 11 which is texture 22 */
        world[59][25][50] = 11;

        /* create textured cube */
        setUserColour(12, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
        setAssignedTexture(12, 27);
        world[61][25][50] = 12;

        /* create textured cube */
        setUserColour(10, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
        setAssignedTexture(10, 26);
        world[63][25][50] = 10;

        /* create textured floor */
        setUserColour(13, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
        setAssignedTexture(13, 8);
        for (i=57; i<67; i++)
            for (j=45; j<55; j++)
                world[i][24][j] = 13;

        /* create textured wall */
        setUserColour(14, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
        setAssignedTexture(14, 18);
        for (i=57; i<67; i++)
            for (j=0; j<4; j++)
                world[i][24+j][45] = 14;

        /* create textured wall */
        setUserColour(15, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
        setAssignedTexture(15, 42);
        for (i=45; i<55; i++)
            for (j=0; j<4; j++)
                world[57][24+j][i] = 15;

        // two cubes using the same texture but one is offset
        // cube with offset texture 33
        setUserColour(16, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
        setAssignedTexture(16, 33);
        world[65][25][50] = 16;
        setTextureOffset(16, 0.5, 0.5);
        // cube with non-offset texture 33
        setUserColour(17, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
        setAssignedTexture(17, 33);
        world[66][25][50] = 17;

        // create some lava textures that will be animated
        setUserColour(18, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
        setAssignedTexture(18, 24);
        world[62][24][55] = 18;
        world[63][24][55] = 18;
        world[64][24][55] = 18;
        world[62][24][56] = 18;
        world[63][24][56] = 18;
        world[64][24][56] = 18;

        // draw cow mesh and rotate 45 degrees around the y axis
        // game id = 0, cow mesh id == 0
        setMeshID(0, 0, 48.0, 26.0, 50.0);
        setRotateMesh(0, 0.0, 45.0, 0.0);

        // draw fish mesh and scale to half size (0.5)
        // game id = 1, fish mesh id == 1
        setMeshID(1, 1, 51.0, 28.0, 50.0);
        setScaleMesh(1, 0.5);

        // draw cow mesh and rotate 45 degrees around the y axis
        // game id = 2, cow mesh id == 0
        setMeshID(2, 0, 59.0, 26.0, 47.0);

        // draw bat
        // game id = 3, bat mesh id == 2
        setMeshID(3, 2, 61.0, 26.0, 47.0);
        setScaleMesh(3, 0.5);
        // draw cactus
        // game id = 4, cactus mesh id == 3
        setMeshID(4, 3, 63.0, 26.0, 47.0);
        setScaleMesh(4, 0.5);


    } else {
        //Set custom colors
        setUserColourRGBA(LIGHT_BROWN, 100, 100, 60, 1);
        setUserColourRGBA(DARK_BROWN, 50, 50, 30, 1);
        setUserColourRGBA(BEIGE, 120, 120, 98, 1);
        setUserColourRGBA(GREY, 100, 100, 100, 1);

        //Set textures
        setTexture(41, GRASS);
        setTexture(37, DIRT);
        setTexture(18, STONE_BRICK);
        setTexture(23, DIRTY_FLOOR);
        setTexture(52, ASPHALT);
        setTexture(20, SUN_MOON_BOX);
        setTexture(25, FLOWER_BOX);
        setTexture(50, TREE_BOX);
        setTexture(54, SNOW);

        //Prep the levels list
        levels = initializeList(printLevel, deleteLevel, compareLevels);

        //Generate outdoor level
        insertFront(levels, generateOutsideLevel());
        loadLevel((Level*)(levels->head->data), world);
    }


    /* starts the graphics processing loop */
    /* code after this will not run until the program exits */
    glutMainLoop();

    //Free memory
    freeList(levels);
    return 0;
}

