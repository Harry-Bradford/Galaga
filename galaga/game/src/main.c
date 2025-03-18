#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>

// Some defines
#define SCREEN_WIDTH 768
#define SCREEN_HEIGHT 1024

#define MAX_STARS 200
#define STAR_YELLOW CLITERAL(Color){ 255, 250, 144, 255 }
#define STAR_RED CLITERAL(Color){ 255, 59, 59, 255 }
#define STAR_BLUE CLITERAL(Color){ 118, 130, 255, 255 }
#define STAR_GREEN CLITERAL(Color){ 118, 255, 118, 255 }

#define SHIP_START_POS 900

// Structs
typedef struct Star {
    Vector2 position;
    Color color;
    float blinkTime;
    bool blink;
} Star;

// Global variables declaration
const int starRadius = 2;
const int starSpeed = 4;
const float lightPeriod = 0.6;
const float blinkPeriod = 0.2;

Star stars[MAX_STARS];
Color colors[] = {STAR_RED, STAR_GREEN, STAR_BLUE, STAR_YELLOW};

int shipX = SCREEN_WIDTH/2;
int shipY = SHIP_START_POS;
int shipVelocity;
const float shipAcceleration = 2;

static bool gameOver = false;
static bool pause = false;

// prototypes
static void InitStars(void);
static void DrawStars(void);


int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Galaga by Harry Bradford");

    // Set our game to run at 60 frames-per-second
    SetTargetFPS(60);
    
    InitStars();

    Texture2D whiteShip = LoadTexture("resources/ship-white/white-ship1.png");

    Rectangle textureRect = {0, 0, whiteShip.width, whiteShip.height};
    Vector2 origin = {whiteShip.width/2, whiteShip.height/2};
    
    // Detect window close button or ESC key
    while (!WindowShouldClose())
    {   
        float dt = GetFrameTime();

        BeginDrawing();
        
        ClearBackground(BLACK);

        DrawStars();

        if (IsKeyDown(KEY_LEFT))
        {
            shipVelocity -= shipAcceleration * dt;
        }
        else if (IsKeyDown(KEY_RIGHT))
        {
            shipVelocity += shipAcceleration * dt;
        }
        else
        {
            shipVelocity = 0;
        }

        shipX -= shipVelocity;

        Rectangle shipRectDest = {shipX, shipY, whiteShip.width, whiteShip.height};

        DrawTexturePro(whiteShip, textureRect, shipRectDest, origin, 0, WHITE);

        EndDrawing();
    }

    UnloadTexture(whiteShip);
    
    // Close window and OpenGL context
    CloseWindow();

    return 0;
}


// Generate starting locations, colours, & blink settings for each star.
void InitStars(void)
{
    stars[0].blinkTime = 0;
    
    for (int i = 0; i < MAX_STARS; i++)
    {
        stars[i].position = (Vector2){GetRandomValue(10, SCREEN_WIDTH-10), GetRandomValue(10, SCREEN_HEIGHT-10)};
        stars[i].color = colors[GetRandomValue(0, 3)];
        stars[i].blink = false;
        
        // Each star has a different blink timing
        if (i > 0)
        {
            stars[i].blinkTime = stars[i-1].blinkTime + 0.01;
        }
    }
}


void DrawStars()
{
    float timeElapsed = GetTime();

    for (int i = 0; i < MAX_STARS; i++)
    {
        
        // Draw the stars with prev generated locations/colours
        DrawCircleV(stars[i].position, starRadius, stars[i].color);
        
        // Stars in the background move downwards
        stars[i].position.y += starSpeed;
        
        // If stars reach bottom of screen, start back at the top with new position and colour
        if (stars[i].position.y >= SCREEN_HEIGHT)
        {
            stars[i].position.y = 0;
            stars[i].position.x = GetRandomValue(0, SCREEN_WIDTH);
            stars[i].color = colors[GetRandomValue(0, 3)];
        }
        
        // If 0.6 seconds has passed since star started shining, make star blink
        if (timeElapsed >= stars[i].blinkTime + lightPeriod)
        {
            stars[i].blinkTime = timeElapsed;
            stars[i].blink = true;
            stars[i].color.a = 0;
        }

        // If 0.2 seconds has passed since star started to blink, make star shine
        else if (stars[i].blink && timeElapsed >= stars[i].blinkTime + blinkPeriod)
        {
            stars[i].blink = false;
            stars[i].color.a = 255;
        }
    }
}