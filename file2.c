#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>   
#include <Windows.h>

struct Point
{
    int x;
    int y;
};
typedef struct Point Point;

#define KEY_ESC 27
#define KEY_UP (256 + 72)
#define KEY_DOWN (256 + 80)
#define KEY_LEFT (256 + 75)
#define KEY_RIGHT (256 + 77)

void GotoXY(int x, int y);
void SetCursorInvisible();
int GetKey(void);
double GetElapsedTime(clock_t start_time);
void Initialize(void);      // 초기화, 우주선 위치, 지구 위치, 별 개수, 별 위치
void DrawSpace(void);       // 화면 그리기

int stage = 1;

Point spaceship;            // 우주선 위치
int direction;              // 현재 방향 (EAST, WEST, SOUTH, NORTH)
double speed;               // 현재 속도 0.1(초마다 이동), stage 증가(-0.01)

Point earth;                // 지구의 위치
int star_count = 10;        // 별의 개수
Point* stars = NULL;        // 별들의 위치

#define EAST 1
#define WEST 2
#define SOUTH 3
#define NORTH 4

int main(void)
{
    srand(time(NULL));
    system("mode CON COLS=80 LINES=30");
    SetCursorInvisible();

    while (1)
    {
        Initialize();
        DrawSpace();
        clock_t start_time = clock();
        int failure = 0;

        while (1)
        {
            if (_kbhit())  // 방향키 입력 처리
            {
                switch (GetKey())
                {
                case KEY_RIGHT:
                    direction = EAST;
                    break;

                case KEY_LEFT:
                    direction = WEST;
                    break;

                case KEY_UP:
                    direction = NORTH;
                    break;

                case KEY_DOWN:
                    direction = SOUTH;
                    break;
                }
            }

            if (GetElapsedTime(start_time) >= speed)  // 이동 처리
            {
                    // 현재 방향으로 한 칸 이동
                    GotoXY(spaceship.x, spaceship.y);
                    printf(" ");
                    if (direction == EAST)
                    {
                        if (spaceship.x != 79)
                            spaceship.x++;
                    }
                    else if (direction == WEST)
                    {
                        if (spaceship.x != 0)
                            spaceship.x--;
                    }
                    else if (direction == NORTH)
                    {
                        if (spaceship.y != 0)
                            spaceship.y--;
                    }
                    else if (direction == SOUTH)
                    {
                        if (spaceship.y != 24)
                            spaceship.y++;
                    }
                    GotoXY(spaceship.x, spaceship.y);
                    printf("@");

                    // 지구 도착 검사 => 성공 => while 문 탈출
                    if (earth.x == spaceship.x && earth.y == spaceship.y)
                    {
                        stage++;
                        break;
                    }
                    // 충돌 검사 => 충돌 => while 문 탈출
                    for (int i = 0; i < star_count; i++)
                    {
                        if (stars[i].x == spaceship.x && stars[i].y == spaceship.y)
                        {
                            failure = 1;
                            break;
                        }
                    }
                    start_time = clock();
                }

                // 성공으로 끝났다면 stage 1 증가 후 바깥쪽 while 문 계속 실행
                // 실패로 끝났다면 프로그램 종료
          

            if (failure == 1)
                break;
        }

        if (failure == 1)
            break;
    }
    GotoXY(0, 27);
}

void Initialize(void)
{
    spaceship.x = 0;
    spaceship.y = 0;
    direction = EAST;
    if (stage == 1)
        speed = 0.1;
    else
        speed = speed - 0.01;
    if (speed < 0.03)
        speed = 0.03;

    earth.x = rand() % (79 - 60 + 1) + 60;
    earth.y = rand() % (24 - 20 + 1) + 20;

    if (stage == 1)
        star_count = 10;
    else
    {
        star_count += 5;
        free(stars);
    }

    stars = malloc(sizeof(Point) * star_count);
    for (int i = 0; i < star_count; i++)
    {
        stars[i].x = rand() % 80;
        stars[i].y = rand() % 25;
    }
}

void DrawSpace(void)
{
    system("cls");
    GotoXY(spaceship.x, spaceship.y);
    printf("@");

    GotoXY(earth.x, earth.y);
    printf("$");

    for (int i = 0; i < star_count; i++)
    {
        GotoXY(stars[i].x, stars[i].y);
        printf("*");
    }
    GotoXY(0, 26);
    printf("LEVEL %d", stage);
}

void GotoXY(int x, int y)
{   // COORD 구조체 변수를 통해 이동할 위치 설정
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void SetCursorInvisible()
{
    CONSOLE_CURSOR_INFO ci = { 100, FALSE };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
}

int GetKey(void)
{
    int ch = _getch();

    if (ch == 0 || ch == 224)
        // 방향키의 경우 0 또는 224의 값이 먼저 입력됨
        ch = 256 + _getch();
    // 그 다음에 해당 방향키에 따라 72(Up), 
    // 80(Down), 75(Left), 77(Right) 값이 입력됨
    return ch;
}

double GetElapsedTime(clock_t start_time)
{
    clock_t current_time = clock();
    return ((double)(current_time - start_time) / CLOCKS_PER_SEC);
}
