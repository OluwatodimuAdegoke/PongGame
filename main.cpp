#include <iostream>
#include <raylib.h>

using namespace std;
int player_score = 0;
int cpu_score = 0;
const int overflow = 150;
Color Green = Color{38, 185, 154, 255};
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
    Ball(float x , float y, int speed_x, int speed_y, int radius){
        this->x = x;
        this->y = y;
        this->speed_x = speed_x;
        this->speed_y = speed_y;
        this->radius = radius;
    }
    void Draw(){
        DrawCircle(x, y, radius, BLACK);
    }
    void Update(){
        x += speed_x;
        y += speed_y;
        if(x+radius >= GetScreenWidth() - overflow || x - radius <= 0){
            changeSpeedX();
        }
        if(y+radius >= GetScreenHeight()){
            cpu_score++;
            ResetBall();
        } 
        if( y - radius <= 0){
            player_score++;
            ResetBall();
        }

    }

    void ResetBall(){
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;

        int speed_choices[2] = {-1,1};
        speed_x *= speed_choices[GetRandomValue(0,1)];
        speed_y *= speed_choices[GetRandomValue(0,1)];
    }
};

class Paddle{
    protected:
    void LimitMovement(){
        if(x <= 0){
            x = x + speed;
        }
        if(x + width >= GetScreenWidth() - 150){
            x = x - speed;
        }
    }
    public:
    float x,y;
    float width, height;
    int speed;

    Paddle(float x, float y, float width, float height, int speed){
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->speed = speed;
    }
    void Draw(){
        DrawRectangle(x, y, width, height, WHITE);
    }

    void Update(){
        if(IsKeyDown(KEY_LEFT) ){
            x = x - speed;
        }
        if(IsKeyDown(KEY_RIGHT)){
            x = x + speed;
        }
        LimitMovement();
    }
};

class CpuPaddle : public Paddle{
    public:
    CpuPaddle(float x, float y, float width, float height, int speed): Paddle(x, y, width, height, speed){

    }
    void Update(int ball_x){
        if(x + width/2 > ball_x){
            x = x - speed;
        }
        if(x + width/2 <= ball_x){
            x = x + speed;
        }
        LimitMovement();
    }
};

int main () {
    cout << "Starting the game" << endl;
    const int screen_width = 700, screen_height = 800;
    InitWindow(screen_width, screen_height, "Pong");
    SetTargetFPS(60);
    int play_w = screen_width - overflow;

    Paddle player = Paddle( play_w/2 - 120/2, screen_height - 35, 120, 25, 12);
    CpuPaddle cpu = CpuPaddle( play_w/2 - 120/2, 10 , 120, 25, 7);
    Ball ball = Ball(screen_width/2 - overflow/2, screen_height/2, 7, 7, 10);

    // player.width = 25;
    // player.height = 120;
    // player.x = screen_width - player.width - 10;
    // player.y = screen_height/2 - player.height/2;
    // player.speed = 6;
    // cpu.width = 25;
    // cpu.height = 120;
    // cpu.x = 10;
    // cpu.y = screen_height/2 - player.height/2;
    // cpu.speed = 6;


    while(!WindowShouldClose()){
        BeginDrawing();
        
        //Update the ball's position before drawing
        ball.Update();
        player.Update();
        cpu.Update(ball.getX());

        if(CheckCollisionCircleRec(Vector2{ball.getX(), ball.getY()}, ball.getRadius(), Rectangle{player.x, player.y, player.width, player.height})){
            ball.changeSpeedY();
        }
        if(CheckCollisionCircleRec(Vector2{ball.getX(), ball.getY()}, ball.getRadius(), Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})){
            ball.changeSpeedY();
        }

        // Clear the background every loop
        ClearBackground(GRAY);
        DrawRectangle(screen_width - overflow, 0, 10, GetScreenHeight(), WHITE);
        ball.Draw();

        cpu.Draw();
        player.Draw();

        DrawText(TextFormat("%i",cpu_score), screen_width - 90, 200, 80, WHITE);
        DrawText(TextFormat("%i",player_score),screen_width - 90, screen_height - 300, 80, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}