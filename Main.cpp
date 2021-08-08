
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

#include "SFML/Graphics.hpp"
#include "SFML/OpenGL.hpp"
#include "HelpFile.h"
#include <WinUser.h>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <Glu.h>
#include <time.h>
#include <string>

#pragma comment(lib,"sfml-audio-d.lib")
#define GL_CLAMP_TO_EDGE 0x812F
using namespace sf;

float angleX, angleY;
void startWindowCreate();


class Player {
public:
	float x, y, z;
	float dx, dy, dz;
	int bufX, bufY, bufZ;
	float w, h, d;
	bool onGround;
	float speed;

	Player(float x0, float y0, float z0) {
		x = x0;	y = y0;	z = z0;
		dx = 0;	dy = 0;	dz = 0;
		w = 5;	h = 20;	d = 5;	speed = 5;
		onGround = false;
	}

	  void update(float time)
   {
     if (!onGround) dy-= 1.5 * time;
	 onGround=0;
	 
	 x+=dx*time;
     collision(dx,0,0);	  
     y+=dy*time;        
     collision(0,dy,0);
     z+=dz*time;
     collision(0,0,dz);
	 dx=dz=0; 
   }

	void collision(float Dx, float Dy, float Dz)
	{
		for (int X = (x - w) / size; X < (x + w) / size; X++)
			for (int Y = (y - h) / size; Y < (y + h) / size; Y++)
				for (int Z = (z - d) / size; Z < (z + d) / size; Z++)
					if (check(X, Y, Z)) {
						if (Dx > 0)  x = X * size - w;
						if (Dx < 0)  x = X * size + size + w;
						if (Dy > 0)  y = Y * size - h;
						if (Dy < 0) { y = Y * size + size + h; onGround = true; dy = 0; }
						if (Dz > 0)  z = Z * size - d;
						if (Dz < 0)  z = Z * size + size + d;
					}
	}
	void keyboard(float time) {

		if (sf::Keyboard::isKeyPressed(Keyboard::Space)) {
			if (y < -5) {
				x = 100;
				y = 500;
				z = 100;
			}
			if (onGround) {
				onGround = false; 
				dy = 12;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				dx = -sin(angleX / 180 * PI) * speed;
				//dy = tan(angleY / 180 * PI) * speed;
				dz = -cos(angleX / 180 * PI) * speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			dx = sin((angleX - 90) / 180 * PI) * speed;
			dz = cos((angleX - 90) / 180 * PI) * speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			dx = sin(angleX / 180 * PI) * speed;
			//dy = -tan(angleY / 180 * PI) * speed;
			dz = cos(angleX / 180 * PI) * speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			dx = sin((angleX + 90) / 180 * PI) * speed;
			dz = cos((angleX + 90) / 180 * PI) * speed;
		}
	}

};


GLuint LoadTexture(sf::String name) {
	Image image;
	image.loadFromFile(name);
	image.flipVertically();

	GLuint texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.getSize().x, image.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return texture;
}

void game() {
	Image im;
	im.loadFromFile("Res/heightmap.png");
	for (int x = 0; x < 256; x++) {
		for (int y = 0; y < 256; y++) {
			for (int z = 0; z < 256; z++) {
				if (x == 0 || x == 255) {
					mas[x][y][z] = 6;
				}
				if (z == 0 || z == 255) {
					mas[x][y][z] = 6;
				}
				if (y == 0) {
					mas[x][y][z] = 3;
				}

			}
		}

	}
	for (int x = 0; x < 256; x++) {
		for (int z = 0; z < 256; z++) {
			int c = im.getPixel(x, z).r / 10;
			int y = 0;
			for (int y = 0; y < c; y++) {
				if (mas[x][y][z] == NULL) {
					if ((y > c - 3))
						mas[x][y][z] = 1;
					else
						if ((y <= c - 3) && y != 0) {
							int temp = rand() % 200 + 1;
							if (temp == 1) {
								mas[x][y][z] = 7;
								mas[x + 1][y][z] = 7;
								mas[x + 1][y + 1][z] = 7;
								mas[x][y + 1][z] = 7;
							}
							else
								mas[x][y][z] = 2;
						}
				}
			}
		}
	}
	for (int x = 0; x < 256; x++) 
		for (int y = 0; y < 256; y++) 
			for (int z = 0; z < 256; z++) 
	if (mas[x][y][z] == 1 && mas[x][y + 3][z] == NULL && mas[x][y + 2][z] == NULL && mas[x][y + 1][z] == NULL) {
		int temp = rand() % 300 + 1;
		if (temp == 1) {
			mas[x][y + 1][z] = 4;
			mas[x][y + 2][z] = 4;
			mas[x][y + 3][z] = 4;
			mas[x][y + 4][z] = 4;
			mas[x][y + 5][z] = 5;
			mas[x - 1][y + 3][z + 1] = 5;
			mas[x + 1][y + 3][z + 1] = 5;
			mas[x - 1][y + 3][z - 1] = 5;
			mas[x + 1][y + 3][z - 1] = 5;
			mas[x - 1][y + 4][z] = 5;
			mas[x + 1][y + 4][z] = 5;
			mas[x - 1][y + 3][z] = 5;
			mas[x + 1][y + 3][z] = 5;
			mas[x][y + 4][z + 1] = 5;
			mas[x][y + 4][z - 1] = 5;
			mas[x][y + 3][z + 1] = 5;
			mas[x][y + 3][z - 1] = 5;
		}
	}



	sf::ContextSettings Settings;
	Settings.depthBits = 24;
	Settings.stencilBits = 8;
	Settings.antialiasingLevel = 2;
	RenderWindow window(sf::VideoMode(1920, 1080, 32), "Minecraft BSUIR edition", sf::Style::Close, Settings);
	window.setActive(true);

	//Texture t;
	//t.loadFromFile("Res/background.jpg");
	//Sprite background(t);

	GLuint box[6];
	box[0] = LoadTexture("Res/grassBox/side.jpg");
	box[1] = LoadTexture("Res/grassBox/side.jpg");
	box[2] = LoadTexture("Res/grassBox/side.jpg");
	box[3] = LoadTexture("Res/grassBox/side.jpg");
	box[4] = LoadTexture("Res/grassBox/bottom.jpg");
	box[5] = LoadTexture("Res/grassBox/top.jpg");

	GLuint oreBox[6];
	oreBox[0] = LoadTexture("Res/oreBox/IronOre.jpg");
	oreBox[1] = LoadTexture("Res/oreBox/IronOre.jpg");
	oreBox[2] = LoadTexture("Res/oreBox/IronOre.jpg");
	oreBox[3] = LoadTexture("Res/oreBox/IronOre.jpg");
	oreBox[4] = LoadTexture("Res/oreBox/IronOre.jpg");
	oreBox[5] = LoadTexture("Res/oreBox/IronOre.jpg");


	GLuint dirtBox[6];
	dirtBox[0] = LoadTexture("res/grassBox/bottom.jpg");
	dirtBox[1] = LoadTexture("res/grassBox/bottom.jpg");
	dirtBox[2] = LoadTexture("res/grassBox/bottom.jpg");
	dirtBox[3] = LoadTexture("res/grassBox/bottom.jpg");
	dirtBox[4] = LoadTexture("res/grassBox/bottom.jpg");
	dirtBox[5] = LoadTexture("res/grassBox/bottom.jpg");

	GLuint stoneBox[6];
	stoneBox[0] = LoadTexture("Res/stoneBox/stone.jpg");
	stoneBox[1] = LoadTexture("Res/stoneBox/stone.jpg");
	stoneBox[2] = LoadTexture("Res/stoneBox/stone.jpg");
	stoneBox[3] = LoadTexture("Res/stoneBox/stone.jpg");
	stoneBox[4] = LoadTexture("Res/stoneBox/stone.jpg");
	stoneBox[5] = LoadTexture("Res/stoneBox/stone.jpg");

	GLuint oakBox[6];
	oakBox[0] = LoadTexture("res/woodBox/oak.png");
	oakBox[1] = LoadTexture("res/woodBox/oak.png");
	oakBox[2] = LoadTexture("res/woodBox/oak.png");
	oakBox[3] = LoadTexture("res/woodBox/oak.png");
	oakBox[4] = LoadTexture("res/woodBox/oakTop.jpg");
	oakBox[5] = LoadTexture("res/woodBox/oakTop.jpg");


	GLuint bedrockBox[6];
	bedrockBox[0] = LoadTexture("Res/bedrockBox/bedrock.jpg");
	bedrockBox[1] = LoadTexture("Res/bedrockBox/bedrock.jpg");
	bedrockBox[2] = LoadTexture("Res/bedrockBox/bedrock.jpg");
	bedrockBox[3] = LoadTexture("Res/bedrockBox/bedrock.jpg");
	bedrockBox[4] = LoadTexture("Res/bedrockBox/bedrock.jpg");
	bedrockBox[5] = LoadTexture("Res/bedrockBox/bedrock.jpg");

	GLuint leavesBox[6];
	leavesBox[0] = LoadTexture("Res/Leaves.png");
	leavesBox[1] = LoadTexture("Res/Leaves.png");
	leavesBox[2] = LoadTexture("Res/Leaves.png");
	leavesBox[3] = LoadTexture("Res/Leaves.png");
	leavesBox[4] = LoadTexture("Res/Leaves.png");
	leavesBox[5] = LoadTexture("Res/Leaves.png");

	GLuint skyBox[6];
	skyBox[0] = LoadTexture("Res/skyBox3/skybox_front.bmp");
	skyBox[1] = LoadTexture("Res/skyBox3/skybox_back.bmp");
	skyBox[2] = LoadTexture("Res/skyBox3/skybox_left.bmp");
	skyBox[3] = LoadTexture("Res/skyBox3/skybox_right.bmp");
	skyBox[4] = LoadTexture("Res/skyBox3/skybox_bottom.bmp");
	skyBox[5] = LoadTexture("Res/skyBox3/skybox_top.bmp");

	Texture tex;
	tex.loadFromFile("Res/cursor.png");
	Sprite s(tex);
	s.setOrigin(8, 8);
	s.setPosition(960, 540);


	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearDepth(1.f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.f, 1.5f, 1.f, 2000.f);
	glEnable(GL_TEXTURE_2D);

	ShowCursor(false);

	Clock clock;

	bool mLeft = 0, mRight = 0;

	Player player(100, 500, 100);

	int inventoryData[4];
	for (int i = 0; i < 4; i++) {
		inventoryData[i] = 0;
	}
	int chosen = 0;

	while (window.isOpen()) {

		float time = clock.getElapsedTime().asMilliseconds();
		clock.restart();
		time = time / 50;
		if (time > 3) time = 3;
		

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.setActive(false);
				window.close();
				ShowCursor(true);
				startWindowCreate();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				window.setActive(false);
				window.close();
				ShowCursor(true);
				startWindowCreate();
			}
			if (event.type == Event::MouseButtonPressed) {
				if (event.key.code == Mouse::Right) mRight = 1;
				if (event.key.code == Mouse::Left) mLeft = 1;
			}
			if (event.type == sf::Event::MouseWheelMoved) {
				if (event.mouseWheel.delta > 0) {
					chosen--;
					if (chosen < 0) {
						chosen = 3;
					}
				}

				if (event.mouseWheel.delta < 0) {
					chosen++;
					if (chosen > 3) {
						chosen = 0;
					}
				}
			}
		}

		player.keyboard(time);
		player.update(time);

		POINT mousexy;
		GetCursorPos(&mousexy);
		int xt = window.getPosition().x + 960;
		int yt = window.getPosition().y + 540;

		angleX += (xt - mousexy.x) / 5;
		angleY += (yt - mousexy.y) / 5;

		if (angleY < -89.0) { angleY = -89.0; }
		if (angleY > 89.0) { angleY = 89.0; }
		SetCursorPos(xt, yt);

		if (mRight || mLeft) {
			float x = player.x;
			float y = player.y + player.h / 2;
			float z = player.z;

			int X, Y, Z;
			int oldX, oldY, oldZ;
			oldX = oldY = oldZ = 0;
			int dist = 0;
			while (dist < 80) {
				dist++;
				x += -sin(angleX / 180 * PI);   X = x / size;
				y += tan(angleY / 180 * PI);    Y = y / size;
				z += -cos(angleX / 180 * PI);   Z = z / size;

				if (check(X, Y, Z)) {
					if (mLeft && mas[X][Y][Z] != 3 && X != 0 && Z != 0) {
						if (mas[X][Y][Z] == 1 && inventoryData[0] < 64) {
							inventoryData[0]++;
						}
						if (mas[X][Y][Z] == 2 && inventoryData[1] < 64) {
							inventoryData[1]++;
						}
						if (mas[X][Y][Z] == 4 && inventoryData[2] < 64) {
							inventoryData[2]++;
						}
						if (mas[X][Y][Z] == 7 && inventoryData[3] < 64) {
							inventoryData[3]++;
						}
						mas[X][Y][Z] = 0;
						break;
					}
					else if (mRight) {
						if (chosen == 0 && inventoryData[0] > 0) {
							mas[oldX][oldY][oldZ] = 1;
							inventoryData[0]--;
						}
						else if (chosen == 1 && inventoryData[1] > 0) {
							mas[oldX][oldY][oldZ] = 2;
							inventoryData[1]--;
						}
						else if (chosen == 2 && inventoryData[2] > 0) {
							mas[oldX][oldY][oldZ] = 4;
							inventoryData[2]--;
						}
						else if (chosen == 3 && inventoryData[3] > 0) {
							mas[oldX][oldY][oldZ] = 7;
							inventoryData[3]--;
						}
						break;
					}
				}
				oldX = X;
				oldY = Y;
				oldZ = Z;
			}
		}

		mLeft = 0;
		mRight = 0;
		window.pushGLStates();
		//window.draw(background);
		window.popGLStates();

		glClear(GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(player.x, player.y + (player.h / 2), player.z, player.x - sin(angleX / 180 * PI),
			player.y + tan(angleY / 180 * PI) + (player.h / 2), player.z - cos(angleX / 180 * PI), 0, 1, 0);


		sf::Font font;
		font.loadFromFile("Res/Font.ttf");
		sf::Text text("Inventory", font, 40);
		text.setFillColor(Color::Red);



		Text inventory[4];
		inventory[0].setFont(font);
		inventory[0].setString("Grass - " + std::to_string(inventoryData[0]));
		inventory[0].setCharacterSize(30);
		if (chosen == 0) {
			inventory[0].setFillColor(Color::Red);
		}
		else {
			inventory[0].setFillColor(Color::Black);
		}
		inventory[0].setPosition(20, 80);

		inventory[1].setFont(font);
		inventory[1].setString("Stone - " + std::to_string(inventoryData[1]));
		inventory[1].setCharacterSize(30);
		if (chosen == 1) {
			inventory[1].setFillColor(Color::Red);
		}
		else {
			inventory[1].setFillColor(Color::Black);
		}
		inventory[1].setPosition(20, 120);

		inventory[2].setFont(font);
		inventory[2].setString("Wood - " + std::to_string(inventoryData[2]));
		inventory[2].setCharacterSize(30);
		if (chosen == 2) {
			inventory[2].setFillColor(Color::Red);
		}
		else {
			inventory[2].setFillColor(Color::Black);
		}
		inventory[2].setPosition(20, 160);

		inventory[3].setFont(font);
		inventory[3].setString("Iron Ore - " + std::to_string(inventoryData[3]));
		inventory[3].setCharacterSize(30);
		if (chosen == 3) {
			inventory[3].setFillColor(Color::Red);
		}
		else {
			inventory[3].setFillColor(Color::Black);
		}
		inventory[3].setPosition(20, 200);

		int R = 15;

		int X = player.x / size;
		int Y = player.y / size;
		int Z = player.z / size;


		for (int x = X - R; x < X + R; x++)
			for (int y = 0; y < Y + R; y++)
				for (int z = Z - R; z < Z + R; z++)
				{
					if (!check(x, y, z)) continue;
					if(x > 0 && z > 0)
					if (mas[x + 1][y][z] == NULL || mas[x - 1][y][z] == NULL || mas[x][y + 1][z] == NULL || mas[x][y - 1][z] == NULL|| mas[x][y][z + 1] == NULL|| mas[x][y][z - 1] == NULL)

						if (mas[x][y][z] == 7) {
							glTranslatef(size * x + size / 2, size * y + size / 2, size * z + size / 2);
							createBox(oreBox, size / 2);
							glTranslatef(-size * x - size / 2, -size * y - size / 2, -size * z - size / 2);
						}
						else
						if (mas[x][y][z] == 5) {
							glTranslatef(size * x + size / 2, size * y + size / 2, size * z + size / 2);
							createBox(leavesBox, size / 2);
							glTranslatef(-size * x - size / 2, -size * y - size / 2, -size * z - size / 2);
						}
						else
							if (mas[x][y][z] == 4) {
								glTranslatef(size * x + size / 2, size * y + size / 2, size * z + size / 2);
								createBox(oakBox, size / 2);
								glTranslatef(-size * x - size / 2, -size * y - size / 2, -size * z - size / 2);
							}
							else
								if (mas[x][y][z] == 3) {
									glTranslatef(size * x + size / 2, size * y + size / 2, size * z + size / 2);
									createBox(bedrockBox, size / 2);
									glTranslatef(-size * x - size / 2, -size * y - size / 2, -size * z - size / 2);
								}
								else									
									if (mas[x][y][z] == 2) {
										glTranslatef(size * x + size / 2, size * y + size / 2, size * z + size / 2);
										createBox(stoneBox, size / 2);
										glTranslatef(-size * x - size / 2, -size * y - size / 2, -size * z - size / 2);
									}
	
									else 				
									if (mas[x][y + 1][z] != 1 && mas[x][y][z] != 6 && mas[x][y][z] != 5) {
										glTranslatef(size * x + size / 2, size * y + size / 2, size * z + size / 2);
										createBox(box, size / 2);
										glTranslatef(-size * x - size / 2, -size * y - size / 2, -size * z - size / 2);
									}
									
									else if (mas[x][y][z] == 1){// || mas[x - 1][y][z] == NULL) {
										glTranslatef(size * x + size / 2, size * y + size / 2, size * z + size / 2);

										createBox(dirtBox, size / 2);

										glTranslatef(-size * x - size / 2, -size * y - size / 2, -size * z - size / 2);
									}
									
				}

		glTranslatef(player.x, player.y, player.z);
		createBox(skyBox, 1000);
		glTranslatef(-player.x, -player.y, -player.z);

		window.pushGLStates();
		window.draw(s);
		window.popGLStates();

		window.pushGLStates();
		window.draw(text);
		window.popGLStates();

		window.pushGLStates();
		window.draw(inventory[0]);
		window.draw(inventory[1]);
		window.draw(inventory[2]);
		window.draw(inventory[3]);
		window.popGLStates();


		window.display();
	}
}

void startWindowCreate() {
	RenderWindow startWindow(VideoMode(640, 480), "MainMenu", sf::Style::None | sf::Style::Close);

	Texture t;
	t.loadFromFile("Res/menuBackground.png");
	Sprite background(t);

	Texture logo;
	logo.loadFromFile("Res/Logo.png");
	Sprite logoSprite(logo);
	logoSprite.setPosition(10, -100);
	logoSprite.scale(0.5f, 0.5f);

	Texture buttonTex, pressedButtonTex;
	buttonTex.loadFromFile("Res/greyButton.jpg");
	pressedButtonTex.loadFromFile("Res/pushedButton.jpg");
	startWindow.setActive(true);

	RectangleShape startB(Vector2f(213, 50));
	startB.setPosition(213, 200);
	startB.setTexture(&buttonTex);
	startB.setOutlineColor(sf::Color::Black);
	startB.setOutlineThickness(2);

	RectangleShape exitB(Vector2f(213, 50));
	exitB.setPosition(213, 280);
	exitB.setTexture(&buttonTex);
	exitB.setOutlineColor(sf::Color::Black);
	exitB.setOutlineThickness(2);

	Font font;
	font.loadFromFile("Res/Font.ttf");

	Font newFont;
	newFont.loadFromFile("Res/vyaz.ttf");

	Text underHeader("BSUIR edition", newFont, 40);
	underHeader.setFillColor(Color(248, 24, 148));
	underHeader.setPosition(190, 150);

	Text startText("Start", font, 20);
	startText.setFillColor(Color(255,255,255));
	startText.setPosition(285, 213);

	Text backStartText("Start", font, 20);
	backStartText.setFillColor(Color(0, 0, 0));
	backStartText.setPosition(286, 214);

	Text exitText("Exit", font, 20);
	exitText.setFillColor(Color(255, 255, 255));
	exitText.setPosition(295, 293);

	Text backExitText("Exit", font, 20);
	backExitText.setFillColor(Color(0, 0, 0));
	backExitText.setPosition(296, 294);

	

	while (startWindow.isOpen()) {
		Vector2i mousePos = Mouse::getPosition(startWindow);

		Event newEvent;
		while (startWindow.pollEvent(newEvent)) {
			if (newEvent.type == sf::Event::Closed)
				startWindow.close();

			if (newEvent.type == sf::Event::MouseButtonPressed) {
				if (Mouse::isButtonPressed(Mouse::Left)) {
					if (startB.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
						startB.setTexture(&pressedButtonTex);
					}
					else if (exitB.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
						exitB.setTexture(&pressedButtonTex);
					}
				}
			}

			if (newEvent.type == sf::Event::MouseButtonReleased) {
				if (newEvent.key.code == Mouse::Left) {
					if (startB.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
						startB.setTexture(&buttonTex);
						startWindow.close();
						game();
					}
					else if (exitB.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
						exitB.setTexture(&buttonTex);
						startWindow.close();
					}
				}
			}

			if (newEvent.type == sf::Event::MouseMoved) {
				sf::Cursor cursor;
				if (startB.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
					cursor.loadFromSystem(sf::Cursor::Hand);
					startWindow.setMouseCursor(cursor);
				}else
					if (exitB.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
						cursor.loadFromSystem(sf::Cursor::Hand);
						startWindow.setMouseCursor(cursor);
					}
					else {
						cursor.loadFromSystem(sf::Cursor::Arrow);
						startWindow.setMouseCursor(cursor);
					}
			}
		}
		startWindow.clear();



		
		startWindow.draw(background);
		startWindow.draw(logoSprite);
		startWindow.draw(underHeader);
		startWindow.draw(startB);
		startWindow.draw(backStartText);
		startWindow.draw(startText);
		startWindow.draw(exitB);
		startWindow.draw(backExitText);
		startWindow.draw(exitText);
		startWindow.display();
	}
}

int main() {
	startWindowCreate();
	return 0;
}



