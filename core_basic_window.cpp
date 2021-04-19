#include "raylib.h"
#include <deque>
#include <random>
#include <chrono>
#include <iostream>

int randint(int a, int b){
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return (int)(a + r);
}

bool inRange(int low, int high, int x) // https://www.geeksforgeeks.org/how-to-check-whether-a-number-is-in-the-rangea-b-using-one-comparison/
{
    return ((x-high)*(x-low) <= 0);
}

bool isColliding(std::deque<Vector2> snakePos, Vector2 colliderPos){
    snakePos.pop_front();
    for (auto i : snakePos){
        if (i.x == colliderPos.x && i.y == colliderPos.y) return true;
    }
    return false;
}

int main(int argc, char* argv[])
{
    srand (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    for (int i = 0; i < 100; i++) rand();
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800 + 16*2;
    int screenHeight = 448 + 16*2;

    // Play field is 23x11
    Vector2 foodPos = {(float)randint(0, 21), (float)randint(0, 10)};
    int snakeDir = KEY_W;
    std::deque<Vector2> snakePos;
    snakePos.push_back( {11, 10} );
    bool alternator = false;
    int tmp, tmp2;

    InitWindow(screenWidth, screenHeight, "Raysnake");

    Texture2D wallTex = LoadTexture("resources/wall.png");
    Texture2D bodyTex = LoadTexture("resources/snake_body.png");
    Texture2D grassTex = LoadTexture("resources/grass.png");
    Texture2D appleTex = LoadTexture("resources/apple.png");

    SetTargetFPS(4);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        tmp2 = GetKeyPressed();
        if (tmp2 != 0) tmp = tmp2;


        if (alternator){
            Vector2 tmpPos = snakePos[0];

            switch (tmp){
                case KEY_UP:
                    tmpPos.y--;
                    snakeDir = tmp;
                    break;

                case KEY_DOWN:
                    tmpPos.y++;
                    snakeDir = tmp;
                    break;

                case KEY_LEFT:
                    tmpPos.x--;
                    snakeDir = tmp;
                    break;

                case KEY_RIGHT:
                    tmpPos.x++;
                    snakeDir = tmp;
                    break;

                case 0:
                default:
                    switch (snakeDir){
                        case KEY_UP:
                            tmpPos.y--;
                            break;

                        case KEY_DOWN:
                            tmpPos.y++;
                            break;

                        case KEY_LEFT:
                            tmpPos.x--;
                            break;

                        case KEY_RIGHT:
                            tmpPos.x++;
                            break;
                    }
                    break;
            }

            snakePos.push_front(tmpPos);
            if (tmpPos.x == foodPos.x && tmpPos.y == foodPos.y){
                foodPos = {(float)randint(0, 21), (float)randint(0, 10)};
                while (isColliding(snakePos, foodPos)) {
                    foodPos = {(float)randint(0, 21), (float)randint(0, 10)};
                }
            } else {
                snakePos.pop_back();
            }

            if (!(inRange(0, 21, tmpPos.x) && inRange(0, 10, tmpPos.y)) || isColliding(snakePos, tmpPos)) {
                snakePos.clear();
                snakePos.push_back( {11, 10} );
            }
        }

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        DrawFPS(8, 8);
        for (int x = 0; x < screenWidth/32-2; x++){
            for (int y = 0; y < screenHeight/32-2; y++){
                DrawTextureRec(wallTex, {0, 0, 31, 31}, {(float)((x*32)+32), (float)((y*32)+32)}, WHITE);
            }
        }
        for (int x = 0; x < screenWidth/32-4; x++){
            for (int y = 0; y < screenHeight/32-4; y++){
                DrawTextureRec(grassTex, {0, 0, 31, 31}, {(float)((x*32)+64), (float)((y*32)+64)}, WHITE);
            }
        }

        for (auto i : snakePos){
            DrawTextureRec(bodyTex, {0, 0, 31, 31}, {(float)((i.x*32)+64), (float)((i.y*32)+64)}, WHITE);
            std::cout << "drew snake at (" << i.x << ", " << i.y << ")";
        }

        DrawTextureRec(appleTex, {0, 0, 31, 31}, {(float)((foodPos.x*32)+64), (float)((foodPos.y*32)+64)}, WHITE);

        EndDrawing();

        //----------------------------------------------------------------------------------

        alternator = !alternator;
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------   
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}