#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

const int WINDOW_SIZE = 800;
const int WINDOW_CENTER = WINDOW_SIZE / 2;
const int TOP_BOUNDARY = 0;
const int BOTTOM_BOUNDARY = WINDOW_SIZE;
const int LEFT_BOUNDARY = 0;
const int RIGHT_BOUNDARY = 800;

// Directional
const int UP = -1;
const int DOWN = 1;
const int LEFT = -1;
const int RIGHT = 1;

const int PADDLE_HEIGHT = 100;
const int PADDLE_WIDTH = 20;
const float PADDLE_TIP_HEIGHT = 40 / PADDLE_HEIGHT;

const int BALL_HEIGHT = 20;
const int BALL_WIDTH = 20;
const float BALL_SPEED = 5.00;
const float PADDLE_SPEED = 10.00;

typedef struct Vector2D
{
    float x;
    float y;
} Vector2D;

typedef struct Ball
{
    Vector2D position;
    Vector2D direction;
} Ball;

typedef struct Paddle
{
    Vector2D postion;
    Vector2D direction;
    int keycode_up;
    int keycode_down;

} Paddle;

typedef struct Rect
{
    float h;
    float w;
    float x;
    float y;

} Rect;

bool is_rect_colliding(Rect rect1, Rect rect2);

int main(void)
{

    InitWindow(WINDOW_SIZE, WINDOW_SIZE, "Pong");

    SetTargetFPS(60);

    Rectangle start_box;
    start_box.height = 100;
    start_box.width = 200;
    start_box.x = WINDOW_CENTER - start_box.width / 2;
    start_box.y = WINDOW_CENTER - start_box.height;

    Rectangle quit_box;
    quit_box.height = 100;
    quit_box.width = 200;
    quit_box.x = WINDOW_CENTER - quit_box.width / 2;
    quit_box.y = WINDOW_CENTER + 20;

    while (true)
    {
        BeginDrawing();
        DrawText("LEXI PONG", WINDOW_CENTER - 135, 200, 50, WHITE);

        int start_game = GuiButton(start_box, "Start Game");
        int end_game = GuiButton(quit_box, "Quit");

        if (end_game)
        {
            CloseWindow();
            return 0;
        }

        if (start_game)
        {

            break;
        }
        EndDrawing();
    }

    Ball ball;
    ball.position.x = WINDOW_CENTER;
    ball.position.y = WINDOW_CENTER;
    ball.direction.x = GetRandomValue(1, 2);
    ball.direction.y = 0;

    float ball_speed = BALL_SPEED; // Ball speed mod;

    Vector2D ballVelocity;
    ballVelocity.x = ball.direction.x * ball_speed;
    ballVelocity.y = ball.direction.y * ball_speed;

    Paddle leftPaddle;
    leftPaddle.postion.x = 20;
    leftPaddle.postion.y = (WINDOW_CENTER)-50;
    leftPaddle.keycode_up = KEY_W;
    leftPaddle.keycode_down = KEY_S;
    leftPaddle.direction.x = 0;
    leftPaddle.direction.y = 0;

    Paddle rightPaddle;
    rightPaddle.postion.x = WINDOW_SIZE - 40;
    rightPaddle.postion.y = (WINDOW_CENTER)-50;
    rightPaddle.keycode_up = KEY_UP;
    rightPaddle.keycode_down = KEY_DOWN;
    rightPaddle.direction.x = 0;
    rightPaddle.direction.y = 0;

    int player_one_score = 0;
    int player_two_score = 0;

    if (ball.direction.x == 1)
    {
        ball.direction.x = -1;
    }
    if (ball.direction.x == 2)
    {
        ball.direction.x = 1;
    }

    while (!WindowShouldClose())
    {

        //
        bool left_moving_up = false;
        bool left_moving_down = false;
        bool right_moving_up = false;
        bool right_moving_down = false;

        BeginDrawing();

        ClearBackground(BLACK);

        DrawText(TextFormat("%d", player_one_score), 200, 20, 50, WHITE);
        DrawText(TextFormat("%d", player_two_score), 600, 20, 50, WHITE);

        DrawRectangle(ball.position.x, ball.position.y, BALL_WIDTH, BALL_HEIGHT, WHITE);

        DrawRectangle(leftPaddle.postion.x, leftPaddle.postion.y, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);

        DrawRectangle(rightPaddle.postion.x, rightPaddle.postion.y, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);

        EndDrawing();

        // calculating call velocity

        ball.position.x = ball.position.x + ballVelocity.x;
        ball.position.y = ball.position.y + ballVelocity.y;

        // Left Paddle
        if (IsKeyDown(leftPaddle.keycode_down))
        {
            leftPaddle.postion.y += PADDLE_SPEED;
            left_moving_down = true;

            if (leftPaddle.postion.y + PADDLE_HEIGHT >= WINDOW_SIZE)
            {
                leftPaddle.postion.y = WINDOW_SIZE - PADDLE_HEIGHT;
            }
        }

        if (IsKeyDown(leftPaddle.keycode_up))
        {
            leftPaddle.postion.y -= PADDLE_SPEED;
            left_moving_up = true;

            if (leftPaddle.postion.y <= TOP_BOUNDARY)
            {
                leftPaddle.postion.y = TOP_BOUNDARY;
            }
        }

        // Right Paddle
        if (IsKeyDown(rightPaddle.keycode_down))
        {
            rightPaddle.postion.y += PADDLE_SPEED;
            right_moving_down = true;

            if (rightPaddle.postion.y + PADDLE_HEIGHT >= WINDOW_SIZE)
            {
                rightPaddle.postion.y = WINDOW_SIZE - PADDLE_HEIGHT;
            }
        }

        if (IsKeyDown(rightPaddle.keycode_up))
        {
            rightPaddle.postion.y -= PADDLE_SPEED;
            right_moving_up = true;

            if (rightPaddle.postion.y <= TOP_BOUNDARY)
            {
                rightPaddle.postion.y = TOP_BOUNDARY;
            }
        }

        Rect rect_ball;
        rect_ball.h = BALL_HEIGHT;
        rect_ball.w = BALL_WIDTH;
        rect_ball.x = ball.position.x;
        rect_ball.y = ball.position.y;

        Rect rect_left_paddle;
        rect_left_paddle.h = PADDLE_HEIGHT - PADDLE_TIP_HEIGHT;
        rect_left_paddle.w = PADDLE_WIDTH;
        rect_left_paddle.x = leftPaddle.postion.x;
        rect_left_paddle.y = leftPaddle.postion.y;

        Rect rect_right_paddle;
        rect_right_paddle.h = PADDLE_HEIGHT;
        rect_right_paddle.w = PADDLE_WIDTH;
        rect_right_paddle.x = rightPaddle.postion.x;
        rect_right_paddle.y = rightPaddle.postion.y;

        // Ball and paddle collision
        if (is_rect_colliding(rect_ball, rect_left_paddle))
        {
            ball.direction.x = RIGHT;
            ball.direction.y *= -1;

            Rect paddle_top_tip;
            paddle_top_tip.h = PADDLE_HEIGHT - PADDLE_TIP_HEIGHT;
            paddle_top_tip.w = PADDLE_WIDTH;
            paddle_top_tip.x = rect_left_paddle.x;
            paddle_top_tip.y = rect_left_paddle.y;

            Rect paddle_bottom_tip;
            paddle_bottom_tip.h = PADDLE_HEIGHT - PADDLE_TIP_HEIGHT;
            paddle_bottom_tip.w = PADDLE_WIDTH;
            paddle_bottom_tip.x = rect_left_paddle.x;
            paddle_bottom_tip.y = rect_left_paddle.y + PADDLE_HEIGHT - PADDLE_TIP_HEIGHT;
            // ball hits left paddle top
            if (is_rect_colliding(rect_ball, paddle_top_tip))
            {
                ball.direction.x = RIGHT;
                ball.direction.y = UP;
            }
            // ball hits left paddle bottom
            if (is_rect_colliding(rect_ball, paddle_bottom_tip))
            {
                ball.direction.x = RIGHT;
                ball.direction.y = DOWN;
            }

            ballVelocity.x = ball.direction.x * ball_speed;
            ballVelocity.y = ball.direction.y * ball_speed;

            if (left_moving_up)
            {
                ballVelocity.y += PADDLE_SPEED * UP * 0.5;
            }
            else if (left_moving_down)
            {
                ballVelocity.y += PADDLE_SPEED * DOWN * 0.5;
            }
        }

        if (is_rect_colliding(rect_ball, rect_right_paddle))
        {
            ball.direction.x = LEFT;
            ball.direction.y *= -1;
            ball_speed += 0.5;

            Rect paddle_top_tip;
            paddle_top_tip.h = PADDLE_TIP_HEIGHT;
            paddle_top_tip.w = PADDLE_WIDTH;
            paddle_top_tip.x = rect_right_paddle.x;
            paddle_top_tip.y = rect_right_paddle.y;

            Rect paddle_bottom_tip;
            paddle_bottom_tip.h = PADDLE_TIP_HEIGHT;
            paddle_bottom_tip.w = PADDLE_WIDTH;
            paddle_bottom_tip.x = rect_right_paddle.x;
            paddle_bottom_tip.y = paddle_bottom_tip.y + PADDLE_HEIGHT - PADDLE_TIP_HEIGHT;

            // ball hits right paddle top
            if (is_rect_colliding(rect_ball, paddle_bottom_tip))
            {
                ball.direction.x = LEFT;
                ball.direction.y = UP;
                ball_speed += 0.5;
            }
            // ball hits right paddle bottom
            if (is_rect_colliding(rect_ball, paddle_bottom_tip))
            {
                ball.direction.x = LEFT;
                ball.direction.y = DOWN;
            }
            ballVelocity.x = ball.direction.x * ball_speed;
            ballVelocity.y = ball.direction.y * ball_speed;

            if (right_moving_up)
            {
                ballVelocity.y += PADDLE_SPEED * UP * 0.5;
            }
            else if (right_moving_down)
            {
                ballVelocity.y += PADDLE_SPEED * DOWN * 0.5;
            }
        }

        // Boundary condition

        if (ball.position.x <= LEFT_BOUNDARY)
        {
            ball.position.x = WINDOW_CENTER;
            ball.position.y = WINDOW_CENTER;
            ball.direction.x = -1;
            ball.direction.y = 0;
            player_two_score++;
            ball_speed = BALL_SPEED;

            ballVelocity.x = ball.direction.x * ball_speed;
            ballVelocity.y = ball.direction.y * ball_speed;
        }

        if (ball.position.x >= RIGHT_BOUNDARY)
        {
            ball.position.x = WINDOW_CENTER;
            ball.position.y = WINDOW_CENTER;
            ball.direction.x = 1;
            ball.direction.y = 0;
            player_one_score++;
            ball_speed = BALL_SPEED;

            ballVelocity.x = ball.direction.x * ball_speed;
            ballVelocity.y = ball.direction.y * ball_speed;
        }

        if (ball.position.y <= TOP_BOUNDARY)
        {
            ball.position.y = TOP_BOUNDARY;
            ball.direction.y = DOWN;

            ballVelocity.x = ball.direction.x * ball_speed;
            ballVelocity.y = ball.direction.y * ball_speed;
        }

        if (ball.position.y + BALL_HEIGHT >= BOTTOM_BOUNDARY)
        {
            ball.position.y = BOTTOM_BOUNDARY - BALL_HEIGHT;
            ball.direction.y = UP;

            ballVelocity.x = ball.direction.x * ball_speed;
            ballVelocity.y = ball.direction.y * ball_speed;
        }

        // Player wins
        if (player_one_score >= 10)
        {
            DrawText("Player One has won the game", WINDOW_CENTER + 20, WINDOW_CENTER, 20, PURPLE);
        }

        if (player_two_score >= 10)
        {
            DrawText("Player two has won the game", WINDOW_CENTER + 20, WINDOW_CENTER, 20, PURPLE);
        }
    }

    CloseWindow();

    return 0;
}

bool is_rect_colliding(Rect a, Rect b)
{

    return a.x <= b.x + b.w &&
           a.x + a.w >= b.x &&
           a.y <= b.y + b.h &&
           a.y + a.h >= b.y;
}
