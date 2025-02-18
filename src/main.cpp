#include <cstdlib>
#include <stdlib.h>

#include "raylib.h"

#define SCREEN_WIDTH (750)
#define SCREEN_HEIGHT (750)

Color green {173,204,96,255};
Color darkGreen{43,51,24,255};

int cellSize = 30;
int cellCount = 25;

class Food {
public:
    Vector2 position ;
    Texture2D texture;
    Food() {
        Image image= LoadImage(ASSETS_PATH"food.png");
        texture =  LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos();
    }

    ~Food() {
        UnloadTexture(texture);
    }

 void Draw() {
        DrawTexture(texture, position.x * cellSize ,position.y * cellSize,WHITE);
    }

    Vector2 GenerateRandomPos() {
        float x = GetRandomValue(0, cellCount -1);
        float y = GetRandomValue(0, cellCount -1);

        return Vector2 {x,y};
    }
};


int main(void)
{
    InitWindow(cellSize*cellCount, cellCount*cellSize, "Window title");
    SetTargetFPS(60);
    Food food =Food();


    while (!WindowShouldClose())
    {
        BeginDrawing();
        food.Draw();
        ClearBackground(green);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
