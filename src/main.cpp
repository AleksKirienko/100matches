#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <time.h>
#include <string>
#include <sstream>
#include <vector>
#include <cstdio>
#include <fstream>

enum class MenuState {
	WaitForPress,
	WaitForRelease
};


using namespace sf;
using namespace std;

void setColor(const Color& color);
int ts = 56;
Vector2i offset(15, 15);
int rec = 0;


struct Anti
{
	int x, y, kind, match, alpha;
	Anti(){ match = 0; alpha = 255; }
};
struct records
{
	char name[20];
	int record;
};

int checkmenu(int c)
{
	if (c == 1 || c == 2 || c == 3 || c == 4) return 1;
	else return 0;
}
int checkwindowup(int x, int y)
{
	if (y > 0) return 1;
	else return 0;
}
int checkwindowdown(int x, int y)
{
	if (y < (512 - 131)) return 1;
	else return 0;
}
int checkwindowright(int x, int y)
{
	if (x < (1024 - 131)) return 1;
	else return 0;
}
int checkwindowleft(int x, int y)
{
	if (x > 0) return 1;
	else return 0;
}
int main()
{
	Font font;
	font.loadFromFile("BuxtonSketch.ttf");
	srand(time(NULL));
	RenderWindow app(VideoMode(1024, 512), "Cthulhu!");
	app.setFramerateLimit(60);

	Texture t1, t2, t3, t4, t5, t6;
	t1.loadFromFile("images/back.png");
	t2.loadFromFile("images/cthulh.png");
	t3.loadFromFile("images/sprites.png");
	t4.loadFromFile("images/fire.png");
	t5.loadFromFile("images/heart.png");
	t6.loadFromFile("images/coins.png");

	Sprite Back(t1), Hero(t2), Enemy(t3), Fire(t4), Heart(t5), Coins(t6);

	Anti En;

	Music music, scream;
	music.openFromFile("epic.ogg");
	scream.openFromFile("scream.ogg");
	music.setVolume(10);
	music.play();
	music.setLoop(true);

	SoundBuffer Buffer1, Buffer2;
	Buffer1.loadFromFile("fire.ogg");
	Buffer2.loadFromFile("btoom.ogg");
	Sound gun(Buffer1);
	gun.setVolume(50);
	Sound death(Buffer2);
	En.x = 800;
	En.y = rand() % 400;
	En.kind = rand() % 3;
	En.match = En.kind + 1;

	int x = 100, y = 100, h = 200, alphaF = 0, xf = x, yf = y;
	int mark = 0, life = 3;
	const int N = 3;
	int xc[N];
	int yc[N];
	int c = 1;
	int i;

	const int M = 10;
	records tmp[M];
	ifstream fin("record.txt");
	for (i = 0; i < M; i++)
		fin >> tmp[i].record;
	fin.close();

	string str;
	Text text;
	text.setFont(font);
	text.setCharacterSize(20);

	Text txt[4];
	for (i = 0; i < 4; i++)
	{
		txt[i].setFont(font);
		txt[i].setCharacterSize(40);
		txt[i].setColor(Color::Yellow);
		txt[i].setPosition(35, 35 + 35*i);
	}

	txt[0].setString(string("1. New game"));
	txt[1].setString(string("\n2. Help"));
	txt[2].setString(string("\n3. Records"));
	txt[3].setString(string("\n4. Exit"));

	bool mar = 0;
	for (i = 0; i < N; i++)
	{
		xc[i] = rand() % 762 + 131;
		yc[i] = rand() % 250 + 131;
	}
	while (mar != 1)
	{
		for (i = 0; i < 4; i++)
		{
			txt[i].setColor(Color::Yellow);
		}
		txt[c - 1].setColor(Color::Blue);
		app.draw(Back);
		for (i = 0; i < 4;i++)
			app.draw(txt[i]);

		app.display();

		MenuState menuState = MenuState::WaitForPress;
		Event e;
		while (app.pollEvent(e)) 
		{
			if (e.type == Event::Closed) 
			{
				app.close();
			}
			switch (menuState) {
			case MenuState::WaitForPress:
				if (e.type != Event::KeyPressed) 
				{
					break;
				}
				if (e.key.code == Keyboard::Up && c > 1)
				{
					c--;
					menuState = MenuState::WaitForRelease;
				}
				else if (e.key.code == Keyboard::Down && c < 4) 
				{
					c++;
					menuState = MenuState::WaitForRelease;
				}
				else if (e.key.code == Keyboard::Return)
				{
					mar = 1;
					menuState = MenuState::WaitForRelease;
				}
				break;

			case MenuState::WaitForRelease:
				if (e.type == Event::KeyReleased && (e.key.code == Keyboard::Up || e.key.code == Keyboard::Down)) 
				{
					menuState = MenuState::WaitForPress;
				}
				break;
			}
		}
	}
	if (checkmenu(c))
	{
		if (c == 1)
		{
			while (app.isOpen())
			{
				Event e;
				Text txt, txt1;
				txt.setFont(font);
				txt.setCharacterSize(25);
				txt.setColor(Color::Blue);
				txt.setPosition(900, 30);
				txt.setString(string("Score: \n      ") + to_string(rec));

				txt1.setFont(font);
				txt1.setCharacterSize(40);
				txt1.setColor(Color::Red);
				txt1.setPosition(125, 35);
				txt1.setString(string("x") + to_string(life));


				while (app.pollEvent(e))
				{
					if (e.type == Event::Closed)
						app.close();
				}

				if (life <= 0) 
					break;

				if (Keyboard::isKeyPressed(Keyboard::Down) && checkwindowdown (x,y)) y += 3;
				if (Keyboard::isKeyPressed(Keyboard::Up) && checkwindowup(x, y)) y -= 3;
				if (Keyboard::isKeyPressed(Keyboard::Right) && checkwindowright(x, y)) x += 3;
				if (Keyboard::isKeyPressed(Keyboard::Left) && checkwindowleft(x, y)) x -= 3;
				if (Keyboard::isKeyPressed(Keyboard::Return))
				{
					xf = x + 5;
					yf = y;
					alphaF = 255;
					mark = 1;
					gun.play();

				}
				if (mark == 1) xf += 5;

				if (xf > 900)
				{
					mark = 0;
					alphaF = 0;
					xf = x;
					yf = y;
				}
				if (En.x > 30)
				{
					En.x -= 2;
				}
				else
				{
					En.x = 800;
					En.y = rand() % 400;
					En.kind = rand() % 3;
					En.match = En.kind + 1;
				}
				if (abs(En.x - xf) < 20 && abs(En.y - yf) < 65 && mark == 1)
				{
					En.match--;
					alphaF = 0;
					xf = x;
					yf = y;
					if (En.match == 0)
					{
						death.play();
						mark = 0;
						alphaF = 0;
						xf = x;
						yf = y;
						En.x = 800;
						En.y = rand() % 400;
						En.kind = rand() % 3;
						En.match = En.kind + 1;
						rec++;
					}
				}
				if (abs(En.x - x)<20 && abs(En.y - y) < 150)
				{
					life--;
					mark = 0;
					alphaF = 0;
					xf = x;
					yf = y;
					En.x = 800;
					En.y = rand() % 400;
					En.kind = rand() % 3;
					En.match = En.kind + 1;

				}
				for (i = 0; i < N; i++)
				{
					if (abs(x - xc[i]) < 30 && abs(y - yc[i]) < 30)
					{
						xc[i] = rand() % 762 + 131;
						yc[i] = rand() % 250 + 131;
						rec += 5;
					}
				}
				if (rec == 6 || rec == 66 || rec == 666 || rec == 13)
				{
					Texture t7;
					t7.loadFromFile("images/Hell.png");
					Sprite Hell(t7);
					music.stop();
					scream.setVolume(100);
					scream.play();
					scream.setLoop(true);
					for (int i=0;i<150;i++)
					{
						app.draw(Hell);
						app.display();
					}
					rec++;
					scream.stop();
					music.play();
				}
				Enemy.setTextureRect(IntRect(En.kind * 131, 0, 131, 131));
				Enemy.setColor(Color(255, 255, 255, En.alpha));
				Enemy.setPosition(En.x, En.y);

				Fire.setColor(Color(255, 255, 255, alphaF));
				Fire.setPosition(xf, yf);

				Hero.setPosition(x, y);

				Heart.setPosition(50, 30);

				app.draw(Back);
				for (i = 0; i < N; i++)
				{
					Coins.setPosition(xc[i], yc[i]);
					app.draw(Coins);
				}
				app.draw(Hero);
				app.draw(Enemy);
				app.draw(Fire);
				app.draw(txt);
				app.draw(Heart);
				app.draw(txt1);


				app.display();
			}
			app.draw(Back);
			int temp;
			if (rec <= tmp[M - 1].record) app.close();
			else
			{
				for (i = 0; i < M - 1; i++)
				{
					if (rec > tmp[i].record)
					{
						temp = i;
						break;
					}
				}
				for (i = M - 1; i > temp; i--)
					tmp[i].record = tmp[i - 1].record;
				tmp[temp].record = rec;
			}
			ofstream fout ("record.txt");
			for (int i = 0; i < M; i++)
				fout << tmp[i].record << " ";
			fout.close();
		}
		if (c == 2)
		{
			while (app.isOpen())
			{
				Event e;
				while (app.pollEvent(e))
				{
					if (e.type == Event::Closed || life <= 0)
						app.close();
				}
				app.draw(Back);
				text.setColor(Color::Red);
				text.setPosition(125, 35);
				text.setString(string("You control Cthulhu with the help of the arrows. \nCthulhu can move up, down, right, left, and also diagonally when you press 2 keys (for example, to the right and up). \nPress enter to shoot. \n Game rules : \n 1) The player gains points.Actions for which the account is increased : \n a) for every second of the game, 1 point is awarded  \n b) For one killed peasant is credited with 20 point \n The peasant dies for 1 shot for losing the peasant is taken away 1 life(heart) \n c) For one killed soldier is charged 50 points. \n The warrior dies for 3 shots.For loss to the warrior, the whole life(all the remaining hearts) is taken away. \n d) For a stone is not charged a single point.\n It can be jumped or destroyed in 10 strokes. \n When a pebble has come to you, the hero is deprived of all life(all hearts).\n 2) Terms of completion of the game : \n Loss of all lives(all hearts)."));
				app.draw(text);
				app.display();
			}
		}
		int out = 0;
		if (c == 3)
		{
			while (app.isOpen())
			{
				Event e;
				while (app.pollEvent(e))
				{
					if (e.type == Event::Closed || life <= 0)
						app.close();
				}
				app.draw(Back);
				ifstream fin("record.txt");
				text.setColor(Color::Red);
				for (i = 0; i < M; i++)
				{
					fin >> out;
					text.setPosition(35, i * 20);
					text.setString(to_string(out));
					app.draw(text);
				}
				app.display();
				fin.close();
			}

		}
		if (c == 4) app.close();
	}
	app.close();
	return 0;
}
