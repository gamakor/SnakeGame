#include <cstdlib>
#include <stdlib.h>
#include <deque>
#include <iostream>
#include <raymath.h>

#include "raylib.h"
using namespace std;

#define SCREEN_WIDTH (750)
#define SCREEN_HEIGHT (750)

Color green {173,204,96,255};
Color darkGreen{43,51,24,255};

int cellSize = 30;
int cellCount = 25;
int offset = 75;


double lastUpdateTime = 0;

bool ElementInDequeu(Vector2 food, deque<Vector2> snakeBody) {

    for (int i = 0; i < snakeBody.size(); i++) {
        if (Vector2Equals(Vector2{food.x,food.y},snakeBody[i])) {
           return true;
        }
    }
    return false;
}


bool eventTriggered(double interval) {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Snake {
public:
    deque<Vector2> body = {Vector2{6,9},Vector2{5,9},Vector2{4,9}};
    Vector2 direction = {1,0};
    bool addSegment = false;

    void Draw() {
        for (int i = 0; i < body.size(); i++) {
            float x = body[i].x;
            float y = body[i].y;

         //   DrawRectangle(x*cellSize,y*cellSize,cellSize,cellSize,darkGreen);
            Rectangle segment = Rectangle{offset+x * cellSize, offset+y * cellSize, static_cast<float>(cellSize), static_cast<float
                >(cellSize)};
           DrawRectangleRounded(segment,.5f,6.f,darkGreen);
        }
    }

    void Update() {
        if (!addSegment) {
            body.pop_back();
            body.push_front(Vector2Add(body[0],direction));
        } else {
            body.push_front(Vector2Add(body[0],direction));
        }
        addSegment = false;
    }
    void Reset() {
        body ={Vector2{6,9},Vector2{5,9},Vector2{4,9}};
        direction = {Vector2{1,0}};

    }


};

class Food {
public:
    Vector2 position ;
    Texture2D texture;
    Food(deque<Vector2> body) {
        Image image= LoadImage(ASSETS_PATH"food.png");
        texture =  LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos(body);
    }

    ~Food() {
        UnloadTexture(texture);
    }

 void Draw() {
        DrawTexture(texture, offset+position.x * cellSize ,offset+position.y * cellSize,WHITE);
    }
    Vector2 GenerateRandomCell() {
        float x = GetRandomValue(0, cellCount -1);
        float y = GetRandomValue(0, cellCount -1);

        return Vector2{x,y};
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakeBody) {
        Vector2 position = GenerateRandomCell();
       while (ElementInDequeu(position,snakeBody)) {
            position = GenerateRandomCell();
       }

        return position;
    }
};

class Game {
public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool isRunning = true;
    int score;
    Sound eatSound;
    Sound wallSound;

    Game() {
        InitAudioDevice();
        eatSound = LoadSound(ASSETS_PATH"eat.mp3");
        wallSound = LoadSound(ASSETS_PATH"wall.mp3");
    }

    void Draw() {
        food.Draw();
        snake.Draw();

    }

    void Update() {
        if (isRunning) {
            snake.Update();
            CheckCollisionsWithFood();
            CheckCollisionsWithEdges();
            CheckCollisisonsWithTail(snake.body);

        }
    }

    void CheckCollisionsWithFood() {
        if (Vector2Equals(snake.body[0],food.position)) {
           food.position =  food.GenerateRandomPos(snake.body);
            snake.addSegment =true;
            score++;
            PlaySound(eatSound);
        }
    }

    void CheckCollisionsWithEdges() {
        if (snake.body[0].x== cellCount || snake.body[0].x == -1) {
            PlaySound(wallSound);
            GameOver();

        }

        if (snake.body[0].y == cellCount || snake.body[0].y == -1) {
            PlaySound(wallSound);
            GameOver();

        }
    }

    void CheckCollisisonsWithTail(deque<Vector2> snakeBody) {
        for (int i = 1; i < snakeBody.size(); i++) {
            if (Vector2Equals(snakeBody[0],snakeBody[i])) {
                GameOver();
            }
        }
    }

    void GameOver() {
        cout << "GAME OVER" << endl;
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.body);
        isRunning = false;
        score = 0;
    }
};


int main(void)
{
    InitWindow(2*offset +cellSize*cellCount, 2*offset +cellCount*cellSize, "Window title");
    SetTargetFPS(60);

    Game game = Game();

    while (!WindowShouldClose())
    {


        BeginDrawing();
        if (eventTriggered(.2)) {
            game.Update();

        }

        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) {

                game.snake.direction = {0,-1};
            game.isRunning = true;

        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
            game.snake.direction = {0,1};
            game.isRunning = true;

        }
        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) {
            game.snake.direction = {-1,0};
            game.isRunning = true;

        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
            game.snake.direction = {1,0};
            game.isRunning = true;

        }

        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{(float)offset-5,(float)offset-5,(float)cellSize*cellCount+10,(float)cellSize*cellCount+10},5,darkGreen);
        DrawText("Retro Snake",offset - 5,20,40,darkGreen);
        DrawText(TextFormat("%i",game.score),offset - 5,offset+cellSize*cellCount+10,40,darkGreen);
        game.Draw();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
