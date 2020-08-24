#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <conio.h>

using namespace std;

char tmp_map[21][60];

char map[21][60] = {
"+#########################################+",
"#                                         #",
"#  ###########                            #",
"#            #                            #",
"#            #######                      #",
"#                                         #",
"#                                         #",
"#                #############            #",
"#                                         #",
"#                                         #",
"#    ##########                           #",
"#             #                           #",
"#             #                           #",
"#             #        ############       #",
"#                                #        #",
"#                                ####     #",
"#                                         #",
"#                                         #",
"#                                         #",
"+#########################################+"
};

void ShowMap()
{
	for (int i = 0; i < 21; i++) {
		printf("%s\n", map[i]);
	}
}

void gotoxy(short x, short y)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { x, y };

	SetConsoleCursorPosition(hStdout, position); 
}

/*class entity {
public:
	entity(int x, int y) {
		this->x = x;
		this->y = y;
	}

	void move_x(int p) {
		if (map[y][x + p] == ' ') x += p;
	}

	void move_y(int p) {
		if (map[y + p][x] == ' ') y += p;
	}

	void move(int p, int q) {
		x += p;
		y += q;
	}

	int get_x() { return x; }
	int get_y() { return y; }

	void draw(char p) {
		map[x][y] = p;
		gotoxy(x, y); printf("%c", p);
	}

private:
	int x;
	int y 
};*/

struct walk {
	long walk_count;
	long x;
	long y;
	long back;
};

struct target {
	long x;
	long y;
};

vector<target> walk_queue;

vector<walk> BFSArray;

void AddArray(int x, int y, int wc, int back) {
	if (tmp_map[y][x] == ' ' || tmp_map[y][x] == '.') {
		tmp_map[y][x] = '#';
		walk tmp;
		tmp.x = x;
		tmp.y = y;
		tmp.walk_count = wc;
		tmp.back = back;
		BFSArray.push_back(tmp);
	}
}

void FindPath(int sx, int sy, int x, int y) {
	memcpy(tmp_map, map, sizeof(map));
	BFSArray.clear();
	walk tmp;
	tmp.x = sx;
	tmp.y = sy;
	tmp.walk_count = 0;
	tmp.back = -1;
	BFSArray.push_back(tmp);

	int i = 0;
	while (i < BFSArray.size()) {
		if (BFSArray[i].x == x && BFSArray[i].y == y) {
			walk_queue.clear();
			target tmp2;
			while (BFSArray[i].walk_count != 0) {
				tmp2.x = BFSArray[i].x;
				tmp2.y = BFSArray[i].y;
				walk_queue.push_back(tmp2);

				i = BFSArray[i].back;
			}

			break;
		}

		AddArray(BFSArray[i].x + 1, BFSArray[i].y, BFSArray[i].walk_count + 1, i);
		AddArray(BFSArray[i].x - 1, BFSArray[i].y, BFSArray[i].walk_count + 1, i);
		AddArray(BFSArray[i].x, BFSArray[i].y + 1, BFSArray[i].walk_count + 1, i);
		AddArray(BFSArray[i].x, BFSArray[i].y - 1, BFSArray[i].walk_count + 1, i);

		/*
			AddArray( BFSArray[i].x+1, BFSArray[i].y+1, BFSArray[i].walk_count+1, i );
			AddArray( BFSArray[i].x-1, BFSArray[i].y+1, BFSArray[i].walk_count+1, i );
			AddArray( BFSArray[i].x+1, BFSArray[i].y+1, BFSArray[i].walk_count+1, i );
			AddArray( BFSArray[i].x+1, BFSArray[i].y-1, BFSArray[i].walk_count+1, i );

			AddArray( BFSArray[i].x+1, BFSArray[i].y-1, BFSArray[i].walk_count+1, i );
			AddArray( BFSArray[i].x-1, BFSArray[i].y-1, BFSArray[i].walk_count+1, i );
			AddArray( BFSArray[i].x-1, BFSArray[i].y+1, BFSArray[i].walk_count+1, i );
			AddArray( BFSArray[i].x-1, BFSArray[i].y-1, BFSArray[i].walk_count+1, i );
		*/
		i++;
	}

	BFSArray.clear();
}


int main()
{
	bool running = true;
	int x = 14; // hero column
	int y = 16; // hero row
	int old_x;
	int old_y;
	int a = 1000000000;
	int ex = 4;//enemy column
	int ey = 3;//enemy row
	int pts = 0;

	system("COLOR 37");
	printf("Hello World\n\n");
	printf("H -> Hard\nN -> Normal\nE -> Easy\n\nInput : ");

	char diffi;
	int speedmod = 3;

	cin >> diffi;

	if (diffi == 'N') { speedmod = 2; }if (diffi == 'H') { speedmod = 1; }if (diffi == 'n') { speedmod = 2; }
	else if (diffi == 'h') { speedmod = 1; }

	system("cls");
	ShowMap();

	gotoxy(x, y); cout << "s";

	int frame = 0;

	FindPath(ex, ey, x, y);

	while (running) {
		gotoxy(x, y); printf(" ");

		old_x = x;
		old_y = y;

		if (GetAsyncKeyState(VK_UP)) {
			if (map[y - 1][x] == '.') { y--; pts += 10; }
			else
				if (map[y - 1][x] == ' ') y--;
			if (map[y - 1][x] == '|') y--;
			if (map[y - 1][x] == '_') y--;
		}
		if (GetAsyncKeyState(VK_DOWN)) {
			if (map[y + 1][x] == '.') { y++; pts += 10; }
			else
				if (map[y + 1][x] == ' ') y++;
			if (map[y + 1][x] == '|') y++;
			if (map[y + 1][x] == '_') y++;
		}
		if (GetAsyncKeyState(VK_LEFT)) {
			if (map[y][x - 1] == '.') { x--; pts += 10; }
			else
				if (map[y][x - 1] == ' ') x--;
			if (map[y][x - 1] == '|') x--;
			if (map[y][x - 1] == '_') x--;
		}
		if (GetAsyncKeyState(VK_RIGHT)) {
			if (map[y][x + 1] == '.') { x++; pts += 10; }
			else
				if (map[y][x + 1] == ' ') x++;
			if (map[y][x + 1] == '|') x++;
			if (map[y][x + 1] == '_') x++;
		}

		if (old_x != x || old_y != y) {
			FindPath(ex, ey, x, y);
		}

		gotoxy(x, y); printf("o");

		map[ey][ex] = '.';
		gotoxy(ex, ey); printf(".");

		if (frame%speedmod == 0 && walk_queue.size() != 0) {
			ex = walk_queue.back().x;
			ey = walk_queue.back().y;
			walk_queue.pop_back();
		}
		gotoxy(ex, ey); printf("@");

		if (ex == x && ey == y) {
			break;
		}


		gotoxy(32, 18);
		gotoxy(45, 1); cout << pts;
		Sleep(100);
		frame++;
	}
	if (pts < 1000000)
	{
		system("cls");
		printf("You Lose and your points: %i", pts);
		cin.get();
		cin.get();
		cin.get();
		cin.get();
		cin.get();
		return 0;
	}
	else if (pts > 1000000)
	{
		system("cls");
		printf("You Won and your Points: %i", pts);
		cin.get();
		cin.get();
		cin.get();
		cin.get();
		cin.get();
		return 0;
	}
}
