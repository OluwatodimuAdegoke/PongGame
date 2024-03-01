#include <iostream>
#include <raylib.h>

using namespace std;
int player_score = 0;
int cpu_score = 0;

class Ball{
    private:
    float x, y;
    int speed_x, speed_y;
    int radius;
    public:

    float getY(){
        return y;
    }
    float getX(){
        return x;
    }
    int getRadius(){
        return radius;
    }
    void changeSpeedX(){
        speed_x *= -1;
    }
    void changeSpeedY(){
        speed_y *= -1;
    }
    Ball(float x1 , float y1, int speed_x1, int speed_y1, int radius1){
        x = x1;
        y = y1;
        speed_x = speed_x1;
        speed_y = speed_y1;
        radius = radius1;
    }
    void Draw(){
        DrawCircle(x, y, radius, YELLOW);
    }
    void Update(){
        x += speed_x;
        y += speed_y;
        if(y+radius >= GetScreenHeight() || y - radius <= 0){
            changeSpeedY();
        }
        if(x+radius >= GetScreenWidth()){
            cpu_score++;
        } 
        if( x -radius <= 0){
            player_score++;
        }

    }
};

class Paddle{
    protected:
    void LimitMovement(){
        if(y <= 0){
            y = y + speed;
        }
        if(y + height >= GetScreenHeight()){
            y = y - speed;
        }
    }
    public:
    float x,y;
    float width, height;
    int speed;

    void Draw(){
        DrawRectangle(x, y, width, height, WHITE);
    }

    void Update(){
        if(IsKeyDown(KEY_UP) ){
            y = y - speed;
        }
        if(IsKeyDown(KEY_DOWN)){
            y = y + speed;
        }
        LimitMovement();
    }
};

class CpuPaddle : public Paddle{
    public:
    void Update(int ball_y){
        if(y + height/2 > ball_y){
            y = y - speed;
        }
        if(y + height/2 <= ball_y){
            y = y + speed;
        }
        LimitMovement();
    }
};

int main () {
    cout << "Starting the game" << endl;
    const int screen_width = 1280, screen_height = 800;
    InitWindow(screen_width, screen_height, "Pong Game!");
    SetTargetFPS(60);

    Paddle player;
    CpuPaddle cpu;
    Ball ball = Ball(screen_width/2 , screen_height/2, 7, 7, 20);

    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height/2 - player.height/2;
    player.speed = 6;
    cpu.width = 25;
    cpu.height = 120;
    cpu.x = 10;
    cpu.y = screen_height/2 - player.height/2;
    cpu.speed = 6;


    while(!WindowShouldClose()){
        BeginDrawing();
        
        //Update the ball's position before drawing
        ball.Update();
        player.Update();
        cpu.Update(ball.getY());

        if(CheckCollisionCircleRec(Vector2{ball.getX(), ball.getY()}, ball.getRadius(), Rectangle{player.x, player.y, player.width, player.height})){
            ball.changeSpeedX();
        }
        if(CheckCollisionCircleRec(Vector2{ball.getX(), ball.getY()}, ball.getRadius(), Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})){
            ball.changeSpeedX();
        }

        // Clear the background every loop
        ClearBackground(BLACK);
        DrawLine(screen_width/2, 0 , screen_width/2, screen_height, WHITE);
        ball.Draw();

        cpu.Draw();
        player.Draw();
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}