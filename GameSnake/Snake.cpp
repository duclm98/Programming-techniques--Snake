#include <iostream>
#include <ctime>
#include <conio.h>
#include <stdio.h>
#include <Windows.h>
using namespace std;

#define consoleWidth 58
#define consoleHeight 20
enum TrangThai{UP,DOWN,LEFT,RIGHT};


void gotoxy(int x, int y) //Di chuyen den toa do (x,y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}

void textcolor(int x) //To mau
{
	HANDLE mau;
	mau = GetStdHandle
	(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(mau, x);
}

void resizeConsole(int width, int height) //Dieu chinh do dai rong man hinh console
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}

struct TOADO
{
	int x, y;
};

struct SNAKE
{
	TOADO dot[31];
	int n;
	TrangThai tt;
};

struct MOI
{
	TOADO td;
};

void KhoiTao(SNAKE&snake, MOI&moi) {
	snake.n = 1;
	snake.dot[0].x = 1;
	snake.dot[0].y = 1;
	snake.tt = RIGHT;
	moi.td.x = 10;
	moi.td.y = 10;
}

void HienThi(SNAKE snake,MOI moi)
{
	system("cls");
	//Hien thi cai khung
	textcolor(4);
	for (int i = 0; i < consoleHeight; i++)
	{
		gotoxy(0, i);
		putchar(178);
		gotoxy(consoleWidth, i);
		putchar(178);
	}
	for (int i = 0; i <= consoleWidth; i++)
	{
		gotoxy(i, 0);
		putchar(178);
		gotoxy(i,consoleHeight);
		putchar(178);
	}
	//Hien thi diem so, lever
	gotoxy(1, consoleHeight + 1);
	printf("So diem: %d", snake.n - 1);
	gotoxy(consoleWidth + 5, 2);
	//Hien thi moi
	textcolor(2);
	gotoxy(moi.td.x, moi.td.y);
	putchar('*');
	//Hien thi con ran
	textcolor(5);
	gotoxy(snake.dot[0].x, snake.dot[0].y);
	putchar(15);
	for (int i = 1; i < snake.n; i++)
	{
		gotoxy(snake.dot[i].x, snake.dot[i].y);
		putchar('*');
	}
}
void DieuKhienDiChuyen(SNAKE&snake)
{
	for (int i = snake.n - 1; i > 0; i--)
	{
		snake.dot[i] = snake.dot[i - 1];
	}
	if (kbhit())//Neu co phim nhan vao
	{
		int key = _getch();
		if (key == 'a' || key == 'A')
			snake.tt = LEFT;
		else if (key == 'd' || key == 'D')
			snake.tt = RIGHT;
		else if (key == 'w' || key == 'W')
			snake.tt = UP;
		else if (key == 's' || key == 'S')
			snake.tt = DOWN;
	}
	if (snake.tt == UP)
		snake.dot[0].y--;
	if (snake.tt == DOWN)
		snake.dot[0].y++;
	if (snake.tt == LEFT)
		snake.dot[0].x--;
	if (snake.tt == RIGHT)
		snake.dot[0].x++;
}

int XuLi(SNAKE&snake, MOI&moi,int&thoigiansleep) // -1 la thua, 0 la dang choi
{
	if (snake.dot[0].x == moi.td.x && snake.dot[0].y == moi.td.y)
	{
		//An thuc an va tang chieu dai con ran
		for (int i = snake.n; i > 0; i--)
		{
			snake.dot[i] = snake.dot[i - 1];
		}
		snake.n++;
		if (snake.tt == UP)
			snake.dot[0].y--;
		if (snake.tt == DOWN)
			snake.dot[0].y++;
		if (snake.tt == LEFT)
			snake.dot[0].x--;
		if (snake.tt == RIGHT)
			snake.dot[0].x++;
		//Tao thuc an moi ngau nhien
		moi.td.x = rand() % consoleWidth;
		moi.td.y = rand() % consoleHeight;
		if (thoigiansleep > 10)
			thoigiansleep -= 10;
	}
	//Neu cham vao thanh xung quanh thi in ra -1
	if (snake.dot[0].x <= 0 || snake.dot[0].x >= consoleWidth || snake.dot[0].y <= 0 || snake.dot[0].y >= consoleHeight)
		return -1;
	//Neu cham vao duoi thi in ra -1
	for (int i = 1; i < snake.n; i++)
	{
		if (snake.dot[0].x == snake.dot[i].x && snake.dot[0].y == snake.dot[i].y)
			return -1;
	}
	return 0;
}
void game()
{
	resizeConsole(510, 600);
	srand(time(NULL));//Khoi tao ra bo sinh so ngau nhien (khong can thiet)
	SNAKE snake;
	MOI moi;
	KhoiTao(snake, moi);
	int thoihiansleep = 400;
	int xuli = 0;
	void menu();
	while (1)
	{
		//Hien thi
		HienThi(snake, moi);
		//Dieu khien
		DieuKhienDiChuyen(snake);
		//Xu li
		xuli = XuLi(snake, moi, thoihiansleep);
		//Ket thuc
		if (xuli == -1)
		{
			gotoxy(25, 5);
			printf("GAME OVER!!!\n");
			gotoxy(20, 6);
			printf("Nhan ENTER de tro lai\n");
			while (_getch() != 13);
			break;
		}
		Sleep(thoihiansleep);
	};
}
void main()
{
	resizeConsole(510, 600);
	int chon;
	bool flag = true;
	do
	{
		system("cls");
		textcolor(2);

		cout << " =========================================================" << endl;
		cout << "||           #####                                       ||" << endl;
		cout << "||          #     # #    #   ##   #    # ######          ||" << endl;
		cout << "||          #       ##   #  #  #  #   #  #               ||" << endl;
		cout << "||           #####  # #  # #    # ####   #####           ||" << endl;
		cout << "||                # #  # # ###### #  #   #               ||" << endl;
		cout << "||          #     # #   ## #    # #   #  #               ||" << endl;
		cout << "||           #####  #    # #    # #    # ######          ||" << endl;
		cout << "||           =================================           ||" << endl;
		cout << "||                    1. Tro choi moi                    ||" << endl;
		cout << "||                    2. Huong dan                       ||" << endl;
		cout << "||                    3. Luu                             ||" << endl;
		cout << "||                    4. Thoat                           ||" << endl;
		cout << "||                      Moi chon                         ||" << endl;
		cout << " =========================================================" << endl;

		scanf("%d", &chon);
		switch (chon)
		{
		case 1:
			game();
			break;
		case 2:
			cout << "Nhan A, D, W, S de dieu khien ran san moi" << endl;
			system("pause");
			break;
		case 3:
			
			break;
		default:
			flag = false;
			break;
		}
	} while (flag);
}
