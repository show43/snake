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
/// <param name="x"></param>
/// <param name="y"></param>
void gotoxy(short x, short y)
{
	/* positions cursor to coodinate (x,y) */
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);	// �ܼ� Ŀ���� ��ġ�� ����
}

/// <summary>
/// ������ũ �׸��� �Լ�
/// �ܼ�â �󿡼� Ư�����ڴ� �ʺ� 2�� ���,
///              ���ĺ��� �ʺ� 1�� ���
/// �̷��� ������ ���� �Ӹ��� ��ġ�� ����(X)�� ����ġ�� �ʾƵ�
/// �������� ǥ���Ǵ� #�� ������ �ٷ� ���� �������鼭 ���̸� �Ծ��ٴ� �ν� ���̵�
/// ���� ��ü�� �縮���� �ǹǷ� ������ũ ���� ������ ���ĺ�/���ڷ� ǥ���� ��
/// </summary>
/// <param name="arr"></param>
/// <param name="leng"></param>
/// <returns></returns>
int redraw(int arr[BOUNDARY][2], int leng)
{
	// �ܼ�â �󿡼� Ư�����ڴ� �ʺ� 2�� ���,
	//              ���ĺ��� �ʺ� 1�� ���
	// �̷��� ������ ���� �Ӹ��� ��ġ�� ����(X)�� ����ġ�� �ʾƵ�
	// �������� ǥ���Ǵ� #�� ������ �ٷ� ���� �������鼭 ���̸� �Ծ��ٴ� �ν� ���̵�
	// ���� ��ü�� �縮���� �ǹǷ� ������ũ ���� ������ ���ĺ�/���ڷ� ǥ���� ��

	gotoxy(arr[0][0], arr[0][1]);
	printf("%s", "t");	// ����

	for (int i = 1; i < leng; i++)
	{
		gotoxy(arr[i][0], arr[i][1]);
		if (!(i == (leng - 1)))
			printf("%s", "#");	// ����
		else
			printf("%s", "O");	// �Ӹ�
	}
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
		*x = *x + move;
	else
		*y = *y + (move / 2);
}

/// <summary>
/// ���� �Ծ��� �� �۵�
/// (������ũ�� ���� ����,
/// ���ھ� ����)
/// </summary>
/// <param name="arr"></param>
/// <param name="leng">������ũ ���� ����</param>
/// <param name="fx"></param>
/// <param name="fy"></param>
/// <param name="score">����</param>
void foodgained(int arr[BOUNDARY][2], int *leng, int *fx, int *fy, int *score)
{
	//printf("x: %d, fx: %d, y: %d, fy: %d\n", arr[*leng - 1][0], *fx, arr[*leng - 1][1], *fy);
	// ������ũ �Ӹ��� ��ġ(x, y)�� food�� ��ġ(fx, fy)�� ���ٸ�
	// ������ ���� ������ �ν�
	if (arr[*leng - 1][0] == *fx && arr[*leng - 1][1] == *fy)	
	{
		Sleep(100);

		// ����(food) ��� (��ġ ���� �� ���)
		*fx = rand() % 5 + 6;
		*fy = rand() % 5 + 6;


		gotoxy(*fx, *fy);	// Ŀ���� x, y ��ġ�� ��ġ��Ű��
		printf("%s", "X");										// ����(X) ���
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
/// �浹
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
			return TRUE;	// �浹 ���� �� ��ȯ
		}

	}
	return FALSE;		// �浹 ���� �ʾ��� �� ��ȯ
}

/// <summary>
/// ��ġ �ʱ�ȭ (reinit -> init���� �̸� ����)
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

 

void CursorView(char show) //Ŀ�� �����
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

void DrawGameBoard() //�ڿ� ���׸���
{
	int x, y;
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
	CursorView(0);
	DrawGameBoard();


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

	init(arr, &fx, &fy, &leng, &score);
	while (1) {


		int delay = move == LEFT || move == RIGHT ? 10 : 15;
		Sleep(delay);	// �ӵ� ����
		t++;

		if (t == 10)
		{
			if (_kbhit()) {
				/*controls for the snake <ASDW> */
				curKeyVal = _getch();
				if (curKeyVal == 'w' && move != DOWN)	// ��
					move = UP;
				else if (curKeyVal == 's' && move != UP)	// ��w
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