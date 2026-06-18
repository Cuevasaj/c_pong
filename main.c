#include <raylib.h>

const int WINDOW_SIZE = 800;
const int WINDOW_CENTER = WINDOW_SIZE / 2;
const int TOP_BOUNDARY = 0;
const int LEFT_BOUNDARY = 0;
const int RIGHT_BOUNDARY = 800;

const int PADDLE_HEIGHT = 100;
const int PADDLE_WIDTH = 20;
const float PADDLE_TIP_HEIGHT = 30 / PADDLE_HEIGHT;

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

    Ball ball;
    ball.position.x = WINDOW_CENTER;
    ball.position.y = WINDOW_CENTER;
    ball.direction.x = GetRandomValue(1, 2);
    ball.direction.y = 0;

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

        BeginDrawing();

        ClearBackground(BLACK);

        DrawText(TextFormat("%d", player_one_score), 200, 20, 50, WHITE);
        DrawText(TextFormat("%d", player_two_score), 600, 20, 50, WHITE);

        DrawRectangle(ball.position.x, ball.position.y, BALL_WIDTH, BALL_HEIGHT, WHITE);

        DrawRectangle(leftPaddle.postion.x, leftPaddle.postion.y, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);

        DrawRectangle(rightPaddle.postion.x, rightPaddle.postion.y, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);

        EndDrawing();

        Vector2D BallVelocity;
        BallVelocity.x = ball.direction.x * BALL_SPEED;
        BallVelocity.y = ball.direction.y * BALL_SPEED;
        ball.position.x = ball.position.x + BallVelocity.x;
        ball.position.y = ball.position.y + BallVelocity.y;

        // Left Paddle
        if (IsKeyDown(leftPaddle.keycode_down))
        {
            leftPaddle.postion.y += PADDLE_SPEED;

            if (leftPaddle.postion.y + PADDLE_HEIGHT >= WINDOW_SIZE)
            {
                leftPaddle.postion.y = WINDOW_SIZE - PADDLE_HEIGHT;
            }
        }

        if (IsKeyDown(leftPaddle.keycode_up))
        {
            leftPaddle.postion.y -= PADDLE_SPEED;

            if (leftPaddle.postion.y <= TOP_BOUNDARY)
            {
                leftPaddle.postion.y = TOP_BOUNDARY;
            }
        }

        // Right Paddle
        if (IsKeyDown(rightPaddle.keycode_down))
        {
            rightPaddle.postion.y += PADDLE_SPEED;

            if (rightPaddle.postion.y + PADDLE_HEIGHT >= WINDOW_SIZE)
            {
                rightPaddle.postion.y = WINDOW_SIZE - PADDLE_HEIGHT;
            }
        }

        if (IsKeyDown(rightPaddle.keycode_up))
        {
            rightPaddle.postion.y -= PADDLE_SPEED;

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
        rect_left_paddle.h = PADDLE_HEIGHT;
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
            ball.direction.x *= -1;
            ball.direction.y *= -1;

            Rect paddle_top_tip;
            paddle_top_tip.h = PADDLE_TIP_HEIGHT;
            paddle_top_tip.w = PADDLE_WIDTH;
            paddle_top_tip.x = rect_left_paddle.x;
            paddle_top_tip.y = rect_left_paddle.y;

            if (is_rect_colliding(rect_ball, paddle_top_tip))
            {
                ball.direction.x = 1;
                ball.direction.y = -1;
            }
        }

        if (is_rect_colliding(rect_ball, rect_right_paddle))
        {
            ball.direction.x *= -1;
            ball.direction.y *= -1;

            Rect paddle_top_tip;
            paddle_top_tip.h = PADDLE_TIP_HEIGHT;
            paddle_top_tip.w = PADDLE_WIDTH;
            paddle_top_tip.x = rect_right_paddle.x;
            paddle_top_tip.y = rect_right_paddle.y;

            if (is_rect_colliding(rect_ball, paddle_top_tip))
            {
                ball.direction.x = -1;
                ball.direction.y = -1;
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
        }

        if (ball.position.x >= RIGHT_BOUNDARY)
        {
            ball.position.x = WINDOW_CENTER;
            ball.position.y = WINDOW_CENTER;
            ball.direction.x = 1;
            ball.direction.y = 0;
            player_one_score++;
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
