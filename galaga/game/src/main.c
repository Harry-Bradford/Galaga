#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_STARS 200
#define STAR_YELLOW CLITERAL(Color){ 255, 250, 144, 255 }
#define STAR_RED CLITERAL(Color){ 255, 59, 59, 255 }
#define STAR_BLUE CLITERAL(Color){ 118, 130, 255, 255 }
#define STAR_GREEN CLITERAL(Color){ 118, 255, 118, 255 }

typedef struct Star {
    Vector2 position;
    Color color;
    float blinkTime;
    bool blink;
} Star;

int main(void)
{
    const int screenWidth = 768;
    const int screenHeight = 1024;
    
    InitWindow(screenWidth, screenHeight, "Galaga by Harry Bradford");

    // Set our game to run at 60 frames-per-second
    SetTargetFPS(60);
    
    bool gameOver = false;
    
    const int starRadius = 2;
    const int starSpeed = 4;
    Star stars[MAX_STARS];
    Color colors[] = {STAR_RED, STAR_GREEN, STAR_BLUE, STAR_YELLOW};
    stars[0].blinkTime = 0;
    float lightPeriod = 0.6;
    float blinkPeriod = 0.2;
    
    // Generate starting locations, colours, & blink settings for each star.
    for (int i = 0; i < MAX_STARS; i++)
    {
        stars[i].position = (Vector2){GetRandomValue(10, screenWidth-10), GetRandomValue(10, screenHeight-10)};
        stars[i].color = colors[GetRandomValue(0, 3)];
        stars[i].blink = false;
        
        // Each star has a different blink timing
        if (i > 0)
        {
            stars[i].blinkTime = stars[i-1].blinkTime + 0.01;
        }
    }
    
    //Texture2D shipTexture = LoadTexture("assets/");
    
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {   
        float timeElapsed = GetTime();
                
        BeginDrawing();
        
        for (int i = 0; i < MAX_STARS; i++)
        {
            
            // Draw the stars with prev generated locations/colours
            DrawCircleV(stars[i].position, starRadius, stars[i].color);
            
            if (!gameOver)
            {
                // Stars in the background move downwards
                stars[i].position.y += starSpeed;
                
                // If stars reach bottom of screen, start back at the top with new position and colour (as if loading a new star)
                if (stars[i].position.y >= screenHeight)
                {
                    stars[i].position.y = 0;
                    stars[i].position.x = GetRandomValue(0, screenWidth);
                    stars[i].color = colors[GetRandomValue(0, 3)];
                }
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
        
        ClearBackground(BLACK);

        EndDrawing();
    }
    
    CloseWindow();        // Close window and OpenGL context

    return 0;
}
