/*
* Copyright 2015 Cloud Posse All rights reserved.
* Released under the GNU license
*
* original author : Cloud Posse
* original author link: https://github.com/gupta-shantanu/ascii-snake
* original author email : https://slack.cloudposse.com
*/
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
/// �ܼ�â �󿡼� Ŀ���� ��ġ ����
/// </summary>
/// <param name="x">x ��ǥ</param>
/// <param name="y">y ��ǥ</param>
void gotoxy(short x, short y)
{
	/* positions cursor to coodinate (x,y) */
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);	// �ܼ� Ŀ���� ��ġ�� ����
}

/// <summary>
/// Ŀ�� ���� ����
/// </summary>
/// <param name="color_number"></param>
void textcolor(int color_number)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
}

/// <summary>
/// Ŀ�� �����
/// </summary>
/// <param name="show">TRUE: ���̱�, FALSE: ��Ȱ��ȭ</param>
void cursorview(BOOL show) 
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO consoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	consoleCursor.bVisible = show;
	consoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &consoleCursor);
}

/// <summary>
/// ������ũ �׸��� �Լ�
/// �ܼ�â �󿡼� Ư�����ڴ� �ʺ� 2�� ���,
///              ���ĺ��� �ʺ� 1�� ���
/// �̷��� ������ ���� �Ӹ��� ��ġ�� ����(X)�� ����ġ�� �ʾƵ�
/// �������� ǥ���Ǵ� #�� ������ �ٷ� ���� �������鼭 ���̸� �Ծ��ٴ� �ν� ���̵�
/// ���� ��ü�� �縮���� �ǹǷ� ������ũ ���� ������ ���ĺ�/���ڷ� ǥ���� ��
/// </summary>
/// <param name="arr">������ũ ����(�Ӹ�,���� ����) ��ġ �迭</param>
/// <param name="leng">������ũ ���� ����</param>
/// <returns></returns>
void redraw(int arr[BOUNDARY][2], int leng)
{
	// �ܼ�â �󿡼� Ư�����ڴ� �ʺ� 2�� ���,
	//              ���ĺ��� �ʺ� 1�� ���
	// �̷��� ������ ���� �Ӹ��� ��ġ�� ����(X)�� ����ġ�� �ʾƵ�
	// �������� ǥ���Ǵ� #�� ������ �ٷ� ���� �������鼭 ���̸� �Ծ��ٴ� �ν� ���̵�
	// ���� ��ü�� �縮���� �ǹǷ� ������ũ ���� ������ ���ĺ�/���ڷ� ǥ���� ��

	gotoxy(arr[0][0], arr[0][1]);
	textcolor(10);
	printf("%s", "��");	// ����

	for (int i = 1; i < leng; i++)
	{
		gotoxy(arr[i][0], arr[i][1]);
		if (!(i == (leng - 1)))
			printf("%s", "��");	// ����
		else
			printf("%s", "��");	// �Ӹ�
	}
	textcolor(15);
}



/// <summary>
/// ��ġ ���� (x, y)
/// </summary>
/// <param name="x">x��ǥ ���� �ּ�</param>
/// <param name="y">y��ǥ ���� �ּ�</param>
/// <param name="move">�̵� ���� (-1 ����, 1: ������ ��0�̸� �ȵ�)</param> // => �̵� ������ define�Ͽ� �ҽ��ڵ带 �д� ����� ���⿡ ���� ����� ������ �� �ֵ��� �ϱ�
void movechange(int *x, int *y, int move)
{
	if (move == LEFT || move == RIGHT)
		*x = *x + move * 2;
	else
		*y = *y + (move / 2);
}

/// <summary>
/// ���� �Ծ��� �� �۵�
/// (������ũ�� ���� ����,
/// ���ھ� ����)
/// </summary>
/// <param name="arr">������ũ ����(�Ӹ�,���� ����) ��ġ �迭</param>
/// <param name="leng">������ũ ���� ����</param>
/// <param name="fx">food x ��ǥ</param>
/// <param name="fy">food y ��ǥ</param>
/// <param name="score">����</param>
void foodgained(int arr[BOUNDARY][2], int *leng, int *fx, int *fy, int *score)
{
	//printf("x: %d, fx: %d, y: %d, fy: %d\n", arr[*leng - 1][0], *fx, arr[*leng - 1][1], *fy);
	// ������ũ �Ӹ��� ��ġ(x, y)�� food�� ��ġ(fx, fy)�� ���ٸ�
	// ������ ���� ������ �ν�
	if (arr[*leng - 1][0] == *fx && arr[*leng - 1][1] == *fy)
	{ 

		// ����(food) ��� (��ġ ���� �� ���) 

		*fx = (rand() % (GBOARD_WIDTH) * 2) + GBOARD_ORIGIN_X + 2;
		*fy = (rand() % (GBOARD_HEIGHT - 1)) + GBOARD_ORIGIN_Y + 1;
		gotoxy(*fx, *fy);
		textcolor(12);
		printf("%s", "��");
		textcolor(15);
		(*score)++;

		// ���ھ� ��� (��ġ ����(50, 0) �� ���)
		gotoxy(50, 0);
		printf("Score: %d", *score);
		arr[*leng][0] = arr[*leng - 1][0];
		arr[*leng][1] = arr[*leng - 1][1];
		(*leng)++;
	}
}

/// <summary>
/// (���� �̸� repaint) => maintLastDirection ���������� �Էµ� �������� ������ũ�� ����
/// </summary>
/// <param name="arr">������ũ ����(�Ӹ�,���� ����) ��ġ �迭</param>
/// <param name="move">�̵��ϴ� ����</param>
/// <param name="leng">������ũ ���� ����</param>
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
/// �浹
/// </summary>
/// <param name="arr">������ũ ����(�Ӹ�,���� ����) ��ġ �迭</param>
/// <param name="leng">������ũ ���� ����</param>
/// <param name="move">�̵��ϴ� ����</param>
/// <returns></returns>
BOOL collision(int arr[BOUNDARY][2], int leng, int *move)
{
	BOOL collcheck_x = (arr[leng-1][0] >= GBOARD_WIDTH * 2 + GBOARD_ORIGIN_X +2&& *move == RIGHT) || (arr[leng - 1][0] <= GBOARD_ORIGIN_X+1 && *move == LEFT);
	BOOL collcheck_y = (arr[leng - 1][1] >= GBOARD_HEIGHT + GBOARD_ORIGIN_Y&& *move == DOWN) || (arr[leng - 1][1] <= GBOARD_ORIGIN_Y  && *move == UP);

	if (collcheck_x || collcheck_y)
	{ 
		return TRUE;
	}

	int i, j;
	for (i = 0; i < leng - 2; i++)
	{
		if (arr[i][0] == arr[leng - 1][0] && arr[i][1] == arr[leng - 1][1])
		{
			return TRUE;	// �浹 ���� �� ��ȯ
		}

	}
	return FALSE;		// �浹 ���� �ʾ��� �� ��ȯ
}

/// <summary>
/// ��ġ �ʱ�ȭ (reinit -> init���� �̸� ����)
/// </summary>
/// <param name="arr">������ũ ����(�Ӹ�,���� ����) ��ġ �迭</param>
/// <param name="fx">food x ��ǥ</param>
/// <param name="fy">food y ��ǥ</param>
/// <param name="leng">������ũ ���� ����</param>
/// <param name="score">����</param>
void init(int arr[BOUNDARY][2], int *fx, int *fy, int *leng, int *score)
{ 
	*fx = (rand() % (GBOARD_WIDTH) * 2) + GBOARD_ORIGIN_X + 2;
	*fy = (rand() % (GBOARD_HEIGHT - 1)) + GBOARD_ORIGIN_Y + 1;
	gotoxy(*fx, *fy);
	textcolor(12);
	printf("%s", "��");
	textcolor(15);
	int r = 0;
	*leng = 3;
	for (r = 0; r < *leng; r++)
	{
		arr[r][0] = GBOARD_ORIGIN_X + 4;
		arr[r][1] = GBOARD_ORIGIN_Y + 4;
	}
	*score = 0;
}

/// <summary>
/// cmdâ���� ��ũ�ѹ� ����
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
/// �ڿ� ��� �׸���
/// </summary>
void drawgameboard()
{
	int x, y;
	 
	for (x = 0; x < GBOARD_WIDTH +4; x++)
	{ 
		for (int y = 0; y < GBOARD_HEIGHT+4; y++)
		{

			gotoxy(x*2,  y);
			printf("  ");
		}
	} 
	for (y = 0; y <= GBOARD_HEIGHT; y++)
	{
		gotoxy(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);
		if (y == GBOARD_HEIGHT)
			printf("��");
		else
			printf("��");
		gotoxy(GBOARD_WIDTH * 2 + GBOARD_ORIGIN_X + 2, GBOARD_ORIGIN_Y + y);
		if (y == GBOARD_HEIGHT)
			printf("��");
		else
			printf("��");
	} 

	gotoxy(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y);
	printf("��");
	gotoxy(GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + 2, GBOARD_ORIGIN_Y);
	printf("��");
	for (x = 0; x < GBOARD_WIDTH; x++)
	{

		gotoxy(GBOARD_ORIGIN_X + 2 + (2 * x), GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
		printf("��");

		gotoxy(GBOARD_ORIGIN_X + 2 + (2 * x), GBOARD_ORIGIN_Y);
		printf("��");
	} 
} 



/// <summary>
/// ���� �Լ�
/// </summary>
/// <returns></returns>
int main()
{
	while (1)
	{
		cursorview(FALSE);
		drawgameboard(); 

		// �ܼ�â ��ũ�ѹ� ����
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
		int score = 0;	// ����
		int check_gameover = 1;
		init(arr, &fx, &fy, &leng, &score);
		while (check_gameover) {  
			Sleep(8);	// �ӵ� ����
			t++;

			if (t == 10)
			{
				if (_kbhit()) {
					/*controls for the snake <ASDW> */
					curKeyVal = _getch();
					if (curKeyVal == 'w' && move != DOWN)	// ��
						move = UP;
					else if (curKeyVal == 's' && move != UP)	// ��
						move = DOWN;
					else if (curKeyVal == 'd' && move != LEFT)	// ��
						move = RIGHT;
					else if (curKeyVal == 'a' && move != RIGHT)		// ��
						move = LEFT;
				}
				maintLastDirection(arr, move, &leng);
				redraw(arr, leng);
				foodgained(arr, &leng, &fx, &fy, &score);

				if (collision(arr, leng, &move)) {

					gotoxy(GBOARD_WIDTH - GBOARD_ORIGIN_X - 1, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2);
					printf("  GAME OVER ", score);
					gotoxy(GBOARD_WIDTH - GBOARD_ORIGIN_X - 3, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2 + 1);
					printf(" Your Score : %d ", score);
					gotoxy(GBOARD_WIDTH - GBOARD_ORIGIN_X - 14, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2 + 5);
					printf("�ƹ�Ű�� ������ ������ �ٽ� ���۵˴ϴ�.");
					_getch(); 
					check_gameover = 0;
				}

				t = 0;
			}
		}
		while (_kbhit())//���� ���� �� �ƹ�Ű �Է� �� ���� �ٽ� ����
		{
		}
	}
	return 0;
}