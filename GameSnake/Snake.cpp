#include <iostream>
#include <fstream>
#include <ctime>
#include <conio.h>
#include <stdio.h>
#include <Windows.h>
#include <string>
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
	TOADO* dot;
	int n;
	int level;
	TrangThai tt;
	int thoiGianSleep;
};

struct MOI
{
	TOADO td;
};

ifstream& operator>>(ifstream& in, SNAKE& s) {
	in >> s.level;
	in >> s.thoiGianSleep;
	in >> s.n;
	char c = 'R';
	in >> c;
	if (c == 'L') {
		s.tt = LEFT;
	} else if(c == 'U') {
		s.tt = UP;
	}
	else if (c == 'D') {
		s.tt = DOWN;
	}
	else {
		s.tt = RIGHT;
	}
	s.dot = new TOADO[s.n];
	for (int i = 0; i < s.n; i++) {
		in >> s.dot[i].x >> s.dot[i].y;
	}

	return in;
}

ofstream& operator<<(ofstream& out, SNAKE& s) {
	out << s.level << endl;
	out << s.thoiGianSleep << endl;
	out << s.n << endl;
	if (s.tt == LEFT) {
		out << 'L' << endl;
	} else if (s.tt == RIGHT) {
		out << 'R' << endl;
	}
	else if (s.tt == UP) {
		out << 'U' << endl;
	}
	else {
		out << 'D' << endl;
	}
	for (int i = 0; i < s.n; i++) {
		out << s.dot[i].x << endl;
		out << s.dot[i].y << endl;
	}

	return out;
}

char dot[4][7] = { {'1', '4', '1', '2', '4', '9', '2'},
				 {'1', '6', '1', '2', '1', '1', '2'},
				 {'1', '6', '1', '2', '2', '5', '8'},
			     {'1', '7', '1', '2', '4', '0', '5'} };

void KhoiTao(SNAKE& snake, MOI& moi, int level, int thoiGanSleep) {
	snake.n = 1;
	snake.level = level;
	snake.dot = new TOADO[snake.n];

	snake.dot[0].x = 1;
	snake.dot[0].y = 1;

	snake.tt = RIGHT;

	snake.thoiGianSleep = thoiGanSleep;

	moi.td.x = 10;
	moi.td.y = 10;
}

void HienThiCaiCong() {
	for (int i = consoleHeight - 5; i < consoleHeight; i++) {
		gotoxy(consoleWidth - 1, i);
		putchar('*');
		gotoxy(consoleWidth - 2, i);
		putchar('*');
	}
	for (int i = consoleHeight - 5; i < consoleHeight; i++) {
		if (i != consoleHeight -3) {
			gotoxy(consoleWidth - 3, i);
			putchar('*');
			gotoxy(consoleWidth - 4, i);
			putchar('*');
		}
	}
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

	//Hien thi diem so, level
	gotoxy(1, consoleHeight + 1);
	printf("So diem: %d", snake.n - 1);
	gotoxy(15, consoleHeight + 1);
	cout << "Level: " << snake.level;
	gotoxy(30, consoleHeight + 1);
	cout << "Nhan P de tam dung";

	// Hien thi cai cong
	if (snake.n == 7) {
		HienThiCaiCong();
	}

	//Hien thi moi
	textcolor(2);
	gotoxy(moi.td.x, moi.td.y);
	putchar('*');

	//Hien thi con ran
	textcolor(5);
	for (int i = 0; i < snake.n; i++)
	{
		gotoxy(snake.dot[i].x, snake.dot[i].y);
		putchar(dot[snake.level][i]);
	}
}
int DieuKhienDiChuyen(SNAKE&snake)
{
	for (int i = snake.n - 1; i > 0; i--)
	{
		snake.dot[i] = snake.dot[i - 1];
	}
	if (kbhit())//Neu co phim nhan vao
	{
		int key = _getch();
		if (key == 'P' || key == 'p') { // Nhấn P tức là bắt sự kiện pause, sẽ trả về -1
			return	-1;
		} else if ((key == 'a' || key == 'A') && snake.tt != RIGHT)
			snake.tt = LEFT;
		else if ((key == 'd' || key == 'D') && snake.tt != LEFT)
			snake.tt = RIGHT;
		else if ((key == 'w' || key == 'W') && snake.tt != DOWN)
			snake.tt = UP;
		else if ((key == 's' || key == 'S') && snake.tt != UP)
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

	return 0;
}

int XuLi(SNAKE& snake, MOI& moi) // -1 la thua, 0 la dang choi
{
	// Khi rắn đạt độ dài bằng 7 thì xử lý qua màn
	if (snake.n == 7) {
		// Nếu đuôi chạm vào cổng thì qua màn
		if (snake.dot[6].x == consoleWidth - 2 && snake.dot[6].y == consoleHeight - 3) {
			KhoiTao(snake, moi, snake.level + 1, snake.thoiGianSleep - 70);
		}

		//Neu cham vao thanh xung quanh thi in ra -1
		if (snake.dot[0].x <= 0 || snake.dot[0].x >= consoleWidth || snake.dot[0].y <= 0 || snake.dot[0].y >= consoleHeight) {
			if (snake.dot[0].y != consoleHeight - 3) {
				return -1;
			}
		}

		//Neu cham vao duoi thi in ra -1
		for (int i = 1; i < snake.n; i++)
		{
			if (snake.dot[0].x == snake.dot[i].x && snake.dot[0].y == snake.dot[i].y)
				return -1;
		}

		// Neu cham vao cai cong
		for (int i = consoleHeight - 5; i < consoleHeight; i++) {
			if (i != consoleHeight - 3) {
				for (int j = consoleWidth - 4; j < consoleWidth; j++) {
					if (snake.dot[0].x == j && snake.dot[0].y == i)
						return -1;
				}
			}
		}

		return 0;
	}
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
		if (snake.n < 7) {
			moi.td.x = rand() % ((consoleWidth - 1) - 1 + 1) + 1; // Phat sinh 1 so ngau nhien trong khoang 1 den consoleWidth -1
			moi.td.y = rand() % ((consoleHeight - 1) - 1 + 1) + 1; // Phat sinh 1 so ngau nhien trong khoang 1 den consoleHeight -1
		}
		else
		{
			moi.td.x = 0;
			moi.td.y = 0;
		}

		if (snake.thoiGianSleep > 5)
			snake.thoiGianSleep -= 5;
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

void LuuGame(ofstream& output, SNAKE snake) {
	output << snake;
}

void LoadTuFile(ifstream& input, SNAKE& snake) {
	input >> snake;
}

void game(SNAKE& snake, MOI& moi)
{
	resizeConsole(510, 600);
	srand(time(NULL));//Khoi tao ra bo sinh so ngau nhien (khong can thiet)
	int xuli = 0, dieuKhien = 0;
	void menu();
	while (1)
	{
		//Hien thi
		HienThi(snake, moi);

		//Dieu khien
		dieuKhien = DieuKhienDiChuyen(snake);
		if (dieuKhien == -1) { // Nếu = -1 tức là đã nhận sự kiện pause
			gotoxy(25, 5);
			cout << "TAM DUNG" << endl;
			gotoxy(20, 6);
			cout << "Nhan enter de tiep tuc" << endl;
			gotoxy(20, 7);
			cout << "Nhan S de luu va thoat" << endl;
			gotoxy(20, 8);
			cout << "Nhan Q de thoat (khong luu)" << endl;
			int key = _getch();
			if (key == 'Q' || key == 'q') {
				break;
			}
			else if (key == 'S' || key == 's') {
				ofstream output;
				string filename;
				gotoxy(20, 9);
				cout << "Nhap ten file: ";
				cin >> filename;
				output.open(filename.append(".txt"));
				LuuGame(output, snake);
				output.close();
				break;
			}
		}
		//Xu li
		xuli = XuLi(snake, moi);
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
		Sleep(snake.thoiGianSleep);
	};
}
void main()
{
	resizeConsole(510, 600);
	SNAKE snake;
	MOI moi;
	string filename;
	ifstream input;
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
		cout << "||                    3. Tai game                        ||" << endl;
		cout << "||                    4. Thoat                           ||" << endl;
		cout << "||                      Moi chon                         ||" << endl;
		cout << " =========================================================" << endl;

		scanf("%d", &chon);
		switch (chon)
		{
		case 1:
			KhoiTao(snake, moi, 0, 200);
			game(snake,moi);
			break;
		case 2:
			cout << "Nhan A, D, W, S de dieu khien ran san moi" << endl;
			system("pause");
			break;
		case 3:
			cout << "Nhap ten game da luu: ";
			cin >> filename;
			try
			{
				input.open(filename.append(".txt"));
				LoadTuFile(input, snake);
				input.close();
				moi.td.x = 10;
				moi.td.y = 10;
				game(snake, moi);
			}
			catch (const std::exception&)
			{
				cout << "Game khong ton tai!!!" << endl;
			}
			break;
		default:
			flag = false;
			break;
		}
	} while (flag);
}
