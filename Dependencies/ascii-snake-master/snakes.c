#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include<stdlib.h>
//#include "graphics.h"


#define GBOARD_WIDTH 50
#define GBOARD_HEIGHT 25
#define GBOARD_ORIGIN_X 2
#define GBOARD_ORIGIN_Y 2

#define BOUNDARY 100
#define UP -2
#define LEFT -1
#define DOWN 2
#define RIGHT 1

/// <summary>
/// 콘솔창 상에서 커서의 위치 지정
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void gotoxy(short x, short y)
{
	/* positions cursor to coodinate (x,y) */
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);	// 콘솔 커서의 위치를 조정
}

/// <summary>
/// 스네이크 그리는 함수
/// 콘솔창 상에서 특수문자는 너비 2를 사용,
///              알파벳은 너비 1을 사용
/// 이러한 이유로 실제 머리의 위치가 먹이(X)를 지나치지 않아도
/// 몸통으로 표현되던 #이 먹이의 바로 옆을 지나가면서 먹이를 먹었다는 인식 없이도
/// 먹이 자체가 사리지게 되므로 스네이크 문자 구성시 알파벳/숫자로 표현할 것
/// </summary>
/// <param name="arr"></param>
/// <param name="leng"></param>
/// <returns></returns>
int redraw(int arr[BOUNDARY][2], int leng)
{
	// 콘솔창 상에서 특수문자는 너비 2를 사용,
	//              알파벳은 너비 1을 사용
	// 이러한 이유로 실제 머리의 위치가 먹이(X)를 지나치지 않아도
	// 몸통으로 표현되던 #이 먹이의 바로 옆을 지나가면서 먹이를 먹었다는 인식 없이도
	// 먹이 자체가 사리지게 되므로 스네이크 문자 구성시 알파벳/숫자로 표현할 것

	gotoxy(arr[0][0], arr[0][1]);
	printf("%s", "t");	// 꼬리

	for (int i = 1; i < leng; i++)
	{
		gotoxy(arr[i][0], arr[i][1]);
		if (!(i == (leng - 1)))
			printf("%s", "#");	// 몸통
		else
			printf("%s", "O");	// 머리
	}
}



/// <summary>
/// 위치 변경 (x, y)
/// </summary>
/// <param name="x">x좌표 변수 주소</param>
/// <param name="y">y좌표 변수 주소</param>
/// <param name="move">이동 방향 (-1 왼쪽, 1: 오른쪽 ※0이면 안됨)</param> // => 이동 방향을 define하여 소스코드를 읽는 사람이 방향에 대해 제대로 인지할 수 있도록 하기
void movechange(int *x, int *y, int move)
{
	if (move == LEFT || move == RIGHT)
		*x = *x + move;
	else
		*y = *y + (move / 2);
}

/// <summary>
/// 음식 먹었을 때 작동
/// (스네이크의 길이 증가,
/// 스코어 증가)
/// </summary>
/// <param name="arr"></param>
/// <param name="leng">스네이크 몸통 길이</param>
/// <param name="fx"></param>
/// <param name="fy"></param>
/// <param name="score">점수</param>
void foodgained(int arr[BOUNDARY][2], int *leng, int *fx, int *fy, int *score)
{
	//printf("x: %d, fx: %d, y: %d, fy: %d\n", arr[*leng - 1][0], *fx, arr[*leng - 1][1], *fy);
	// 스네이크 머리의 위치(x, y)가 food의 위치(fx, fy)와 같다면
	// 음식을 먹은 것으로 인식
	if (arr[*leng - 1][0] == *fx && arr[*leng - 1][1] == *fy)	
	{
		Sleep(100);

		// 음식(food) 출력 (위치 지정 후 출력)
		*fx = rand() % 5 + 6;
		*fy = rand() % 5 + 6;


		gotoxy(*fx, *fy);	// 커서를 x, y 위치에 위치시키고
		printf("%s", "X");										// 음식(X) 출력
		(*score)++;

		// 스코어 출력 (위치 지정(50, 0) 후 출력)
		gotoxy(50, 0);
		printf("Score: %d", *score);
		arr[*leng][0] = arr[*leng - 1][0];
		arr[*leng][1] = arr[*leng - 1][1];
		(*leng)++;
	}
}

/// <summary>
/// (기존 이름 repaint) => maintLastDirection 마지막으로 입력된 방향으로 스네이크가 전진
/// </summary>
/// <param name="arr"></param>
/// <param name="move"></param>
/// <param name="leng"></param>
void maintLastDirection(int arr[BOUNDARY][2], int move, int *leng)
{
	int i = 0;
	gotoxy(arr[0][0], arr[0][1]);
	printf("%s", " ");
	for (i = 0; i < *leng - 1; i++) {
		arr[i][0] = arr[i + 1][0];
		arr[i][1] = arr[i + 1][1];
	}
	arr[*leng - 1][0] = arr[*leng - 2][0];
	arr[*leng - 1][1] = arr[*leng - 2][1];
	movechange(&arr[*leng - 1][0], &arr[*leng - 1][1], move);
}

/// <summary>
/// 충돌
/// </summary>
/// <param name="arr"></param>
/// <param name="leng"></param>
/// <returns></returns>
BOOL collision(int arr[BOUNDARY][2], int leng, int *move)
{
	BOOL collcheck_x = (arr[0][0] > GBOARD_WIDTH * 2 + GBOARD_ORIGIN_X - 1 && *move == RIGHT) || (arr[0][0] <= GBOARD_ORIGIN_X + 3 && *move == LEFT);
	BOOL collcheck_y = (arr[0][1] > GBOARD_HEIGHT + GBOARD_ORIGIN_Y-3&& *move == DOWN) || (arr[0][1] <= GBOARD_ORIGIN_Y + 2 && *move == UP);

	if (collcheck_x || collcheck_y)
	{ 
		return TRUE;
	}

	int i, j;
	for (i = 0; i < leng - 2; i++)
	{
		if (arr[i][0] == arr[leng - 1][0] && arr[leng - 1][1] == arr[i][1])
		{
			return TRUE;	// 충돌 했을 때 반환
		}

	}
	return FALSE;		// 충돌 하지 않았을 때 반환
}

/// <summary>
/// 위치 초기화 (reinit -> init으로 이름 변경)
/// </summary>
/// <param name="arr"></param>
/// <param name="fx"></param>
/// <param name="fy"></param>
/// <param name="le"></param>
/// <param name="score"></param>
void init(int arr[BOUNDARY][2], int *fx, int *fy, int *leng, int *score)
{
	*fx = rand() % 30 + 1;
	*fy = rand() % 30 + 1; 
	gotoxy(*fx, *fy);
	printf("%s ", "X");
	int r = 0;
	*leng = 3;
	for (r = 0; r < *leng; r++)
	{
		arr[r][0] = GBOARD_ORIGIN_X + 4;
		arr[r][1] = GBOARD_ORIGIN_Y+4;
	}
	*score = 0;
}

/// <summary>
/// cmd창에서 스크롤바 제거
/// </summary>
void remove_scrollbar()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(handle, &info);
	COORD new_size =
	{
		info.srWindow.Right - info.srWindow.Left + 1,
		info.srWindow.Bottom - info.srWindow.Top + 1
	};
	SetConsoleScreenBufferSize(handle, new_size);
}

 

void CursorView(char show) //커서 숨기기
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

void DrawGameBoard() //뒤에 배경그리기
{
	int x, y;
	for (y = 0; y <= GBOARD_HEIGHT; y++)
	{
		gotoxy(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);
		if (y == GBOARD_HEIGHT)
			printf("└");
		else
			printf("│");
		gotoxy(GBOARD_WIDTH * 2 + GBOARD_ORIGIN_X + 2, GBOARD_ORIGIN_Y + y);
		if (y == GBOARD_HEIGHT)
			printf("┘");
		else
			printf("│");
	} 

	gotoxy(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y);
	printf("┌");
	gotoxy(GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + 2, GBOARD_ORIGIN_Y);
	printf("┐");
	for (x = 0; x < GBOARD_WIDTH; x++)
	{

		gotoxy(GBOARD_ORIGIN_X + 2 + (2 * x), GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
		printf("─");

		gotoxy(GBOARD_ORIGIN_X + 2 + (2 * x), GBOARD_ORIGIN_Y);
		printf("─");
	}
} 



/// <summary>
/// 메인 함수
/// </summary>
/// <returns></returns>
int main()
{
	CursorView(0);
	DrawGameBoard();


	// 콘솔창 스크롤바 제거
	remove_scrollbar();

	// textcolor(RED);
	int arr[BOUNDARY][2];
	int leng = 0;
	int move = 1;
	int curKeyVal;
	int t = 0;
	int fx;
	int fy;
	int over = 0;
	int score = 0;	// 점수

	init(arr, &fx, &fy, &leng, &score);
	while (1) {


		int delay = move == LEFT || move == RIGHT ? 10 : 15;
		Sleep(delay);	// 속도 조절
		t++;

		if (t == 10)
		{
			if (_kbhit()) {
				/*controls for the snake <ASDW> */
				curKeyVal = _getch();
				if (curKeyVal == 'w' && move != DOWN)	// 상
					move = UP;
				else if (curKeyVal == 's' && move != UP)	// 하w
					move = DOWN;
				else if (curKeyVal == 'd' && move != LEFT)	// 우
					move = RIGHT;
				else if (curKeyVal == 'a' && move != RIGHT)		// 좌
					move = LEFT;
			}
			maintLastDirection(arr, move, &leng);
			redraw(arr, leng);
			foodgained(arr, &leng, &fx, &fy, &score);

			if (collision(arr, leng, &move)) {

				_cprintf("Game Over-- X ---");
				_getch();
				_cprintf("Restart the game.....");
				_getch();

				break;
			}

			t = 0;
		}
	}

	return 0;
}