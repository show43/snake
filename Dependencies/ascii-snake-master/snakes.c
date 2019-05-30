#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include<stdlib.h>
//#include "graphics.h"

#define BOUNDARY 100

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
/// </summary>
/// <param name="arr"></param>
/// <param name="le"></param>
/// <returns></returns>
int redraw(int arr[BOUNDARY][2], int le)
{
	gotoxy(arr[0][0], arr[0][1]);
	printf("%s", "+");	// 꼬리

	for (int i = 1; i < le; i++)
	{
		gotoxy(arr[i][0], arr[i][1]);
		if (!(i == (le - 1)))
			printf("%s", "#");	// 몸통
		else
			printf("%s", "○");	// 머리
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
	if (move == 1 || move == -1)
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
/// <param name="le"></param>
/// <param name="fx"></param>
/// <param name="fy"></param>
/// <param name="s">점수</param>
void foodgained(int arr[BOUNDARY][2], int *le, int *fx, int *fy, int *s)
{
	if (arr[*le - 1][0] == *fx&&arr[*le - 1][1] == *fy)
	{
		gotoxy(*fx = rand() % 30 + 6, *fy = rand() % 20 + 6);	// 커서를 x, y 위치에 위치시키고
		printf("%s", "X");										// 음식(X) 출력
		(*s)++;
		gotoxy(50, 0);
		printf("Score: %d", *s);
		arr[*le][0] = arr[*le - 1][0];
		arr[*le][1] = arr[*le - 1][1];
		(*le)++;
	}
}

/// <summary>
/// (기존 이름 repaint) => maintLastDirection 마지막으로 입력된 방향으로 스네이크가 전진
/// </summary>
/// <param name="arr"></param>
/// <param name="move"></param>
/// <param name="le"></param>
void maintLastDirection(int arr[BOUNDARY][2], int move, int *le)
{
	int i = 0;
	gotoxy(arr[0][0], arr[0][1]);
	printf("%s", " ");
	for (i = 0; i < *le - 1; i++) {
		arr[i][0] = arr[i + 1][0];
		arr[i][1] = arr[i + 1][1];
	}
	arr[*le - 1][0] = arr[*le - 2][0];
	arr[*le - 1][1] = arr[*le - 2][1];
	movechange(&arr[*le - 1][0], &arr[*le - 1][1], move);
}

/// <summary>
/// 충돌 (반환값을 불값으로 변경하기)
/// </summary>
/// <param name="arr"></param>
/// <param name="le"></param>
/// <returns></returns>
short collision(int arr[BOUNDARY][2], int le)
{
	int i, j;
	for (i = 0; i < le - 2; i++)
	{
		if (arr[i][0] == arr[le - 1][0] && arr[le - 1][1] == arr[i][1])
		{
			return 1;	// 충돌 했을 때 반환
		}

	}
	return 0;		// 충돌 하지 않았을 때 반환
}

/// <summary>
/// 위치 초기화 (reinit -> init으로 이름 변경)
/// </summary>
/// <param name="arr"></param>
/// <param name="fx"></param>
/// <param name="fy"></param>
/// <param name="le"></param>
/// <param name="s"></param>
void init(int arr[BOUNDARY][2], int *fx, int *fy, int *le, int *s)
{
	gotoxy(*fx = rand() % 30 + 1, *fy = rand() % 30 + 1);
	printf("%s ", "X");
	int r = 0;
	*le = 3;
	for (r = 0; r < *le; r++)
	{
		arr[r][0] = r + 1;
		arr[r][1] = 0;
	}
	*s = 0;
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




/// <summary>
/// 메인 함수
/// </summary>
/// <returns></returns>
int main()
{
	// 콘솔창 스크롤바 제거
	remove_scrollbar();

	// textcolor(RED);
	int arr[BOUNDARY][2];
	int le = 0;
	int move = 1;
	int r;
	int t = 0;
	int fx;
	int fy;
	int over = 0;
	int s = 0;	// 점수

	init(arr, &fx, &fy, &le, &s);
	while (1) {
		if (_kbhit()) {
			/*controls for the snake <ASDW> */
			r = _getch();
			if (r == 'w' && move != 2)	// 상
				move = -2;
			else if (r == 's' && move != -2)	// 하
				move = 2;
			else if (r == 'd' && move != -1)	// 좌
				move = 1;
			else if (r == 'a' && move != 1)		// 우
				move = -1;
		}
		Sleep(10);	// 속도 조절
		t++;
		if (t == 10)
		{
			maintLastDirection(arr, move, &le);
			redraw(arr, le);
			foodgained(arr, &le, &fx, &fy, &s);
			if (collision(arr, le)) {
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