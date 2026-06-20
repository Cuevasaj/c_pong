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

// Button
const int BUTTON_HEIGHT = 100;
const int BUTTON_WIDTH = 200;

// Score
const int WINNING_SCORE = 2;

typedef struct Ball
{
    Vector2 position;
    Vector2 direction;
} Ball;

typedef struct Paddle
{
    Vector2 postion;
    Vector2 direction;
    int keycode_up;
    int keycode_down;

} Paddle;

bool is_rect_colliding(Rectangle rect1, Rectangle rect2);
Rectangle make_rectangle(float h, float w, float x, float y);
Vector2 make_vector2(float x, float y);
Paddle make_paddle(Vector2 postion, Vector2 direction, int keycode_up, int keycode_down);
int main_menu();
int draw_button(char *title, float h, float w, float x, float y);

int main(void)
{
    InitWindow(WINDOW_SIZE, WINDOW_SIZE, "Pong");
    SetTargetFPS(60);

start:
    int should_quit = main_menu();
    if (should_quit)
    {
        CloseWindow();
        return 0;
    }

    Ball ball;
    ball.position = make_vector2(WINDOW_CENTER, WINDOW_CENTER);

    int random_direction = GetRandomValue(1, 2);
    ball.direction = make_vector2(random_direction, 0);

    float ball_speed = BALL_SPEED; // Ball speed mod;
    Vector2 ballVelocity = make_vector2(ball.direction.x * ball_speed, ball.direction.y * ball_speed);

    Paddle leftPaddle = make_paddle(make_vector2(20, WINDOW_CENTER - 50), make_vector2(0, 0), KEY_W, KEY_S);
    Paddle rightPaddle = make_paddle(make_vector2(WINDOW_SIZE - 40, WINDOW_CENTER - 50), make_vector2(0, 0), KEY_UP, KEY_DOWN);

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

        Rectangle rect_ball = make_rectangle(BALL_HEIGHT, BALL_WIDTH, ball.position.x, ball.position.y);

        Rectangle rect_left_paddle = make_rectangle(PADDLE_HEIGHT - PADDLE_TIP_HEIGHT, PADDLE_WIDTH, leftPaddle.postion.x, leftPaddle.postion.y);

        Rectangle rect_right_paddle = make_rectangle(PADDLE_HEIGHT, PADDLE_WIDTH, rightPaddle.postion.x, rightPaddle.postion.y);

        // Ball and paddle collision
        if (is_rect_colliding(rect_ball, rect_left_paddle))
        {
            ball.direction.x = RIGHT;
            ball.direction.y *= -1;

            Rectangle paddle_top_tip = make_rectangle(PADDLE_HEIGHT - PADDLE_TIP_HEIGHT, PADDLE_WIDTH, rect_left_paddle.x, rect_left_paddle.y);

            Rectangle paddle_bottom_tip = make_rectangle(PADDLE_HEIGHT - PADDLE_TIP_HEIGHT, PADDLE_WIDTH, rect_left_paddle.x, rect_left_paddle.y + PADDLE_HEIGHT - PADDLE_TIP_HEIGHT);

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

            Rectangle paddle_top_tip = make_rectangle(PADDLE_TIP_HEIGHT, PADDLE_WIDTH, rect_right_paddle.x, rect_right_paddle.y);
            Rectangle paddle_bottom_tip = make_rectangle(PADDLE_TIP_HEIGHT, PADDLE_WIDTH, rect_right_paddle.x, rect_right_paddle.y + PADDLE_HEIGHT - PADDLE_TIP_HEIGHT);

            // ball hits right paddle top
            if (is_rect_colliding(rect_ball, paddle_top_tip))
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
        if (player_one_score >= WINNING_SCORE)
        {
            break;
        }

        if (player_two_score >= WINNING_SCORE)
        {
            break;
        }
    }

    BeginDrawing();
    ClearBackground(BLACK);
    DrawText(TextFormat("%d", player_one_score), 200, 20, 50, WHITE);
    DrawText(TextFormat("%d", player_two_score), 600, 20, 50, WHITE);

    if (player_one_score > player_two_score)
    {
        DrawText("Player One has won the game", WINDOW_CENTER - 140, WINDOW_CENTER, 20, PURPLE);
    }
    else
    {
        DrawText("Player two has won the game", WINDOW_CENTER - 140, WINDOW_CENTER, 20, PURPLE);
    }

    EndDrawing();
    WaitTime(3.0);

    goto start;

    CloseWindow();

    return 0;
}

// Functions
bool is_rect_colliding(Rectangle a, Rectangle b)
{

    return a.x <= b.x + b.width &&
           a.x + a.width >= b.x &&
           a.y <= b.y + b.height &&
           a.y + a.height >= b.y;
}

Rectangle make_rectangle(float h, float w, float x, float y)
{

    Rectangle rect;
    rect.height = h;
    rect.width = w;
    rect.x = x;
    rect.y = y;

    return rect;
}

Vector2 make_vector2(float x, float y)
{

    Vector2 vec;
    vec.x = x;
    vec.y = y;

    return vec;
}
Paddle make_paddle(Vector2 pos, Vector2 dir, int keycode_up, int keycode_down)
{

    Paddle paddle;
    paddle.postion = pos;
    paddle.direction = dir;
    paddle.keycode_up = keycode_up;
    paddle.keycode_down = keycode_down;
    return paddle;
}

int draw_button(char *title, float h, float w, float x, float y)
{
    Rectangle rect = make_rectangle(h, w, x, y);
    int was_pressed = GuiButton(rect, title);
    return was_pressed;
}

// returns 1 if should quit
int main_menu()
{
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("LEXI PONG", WINDOW_CENTER - 135, 200, 50, WHITE);

        int start_game = draw_button("Start Game", BUTTON_HEIGHT, BUTTON_WIDTH, WINDOW_CENTER - BUTTON_WIDTH / 2, WINDOW_CENTER - BUTTON_HEIGHT);
        int end_game = draw_button("Quit", BUTTON_HEIGHT, BUTTON_WIDTH, WINDOW_CENTER - BUTTON_WIDTH / 2, WINDOW_CENTER + 20);

        if (end_game)
        {
            return 1;
        }

        if (start_game)
        {
            return 0;
        }

        EndDrawing();
    }
};
