#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <string.h>
#include "Menu.h"
#include <sstream>
#include <time.h>
#include <windows.h> 

using namespace sf;
using namespace std;
void ErrorMsg(string error, string title)
{
	MessageBoxA(NULL, error.c_str(), title.c_str(), MB_ICONERROR | MB_OK);
}
//void playMusic(const string& filename)
//{
//	// Load an ogg music file
//	
//	if (!music.openFromFile(filename))
//		return;
//	music.pause();
//}
bool intersects(const Sprite & rect1, const RectangleShape & rect2)
{
	FloatRect r1 = rect1.getGlobalBounds();
	FloatRect r2 = rect2.getGlobalBounds();
	return r1.intersects(r2);
}
bool intersects1(const Sprite & rect1, const Sprite & rect2)
{
	FloatRect r1 = rect1.getGlobalBounds();
	FloatRect r2 = rect2.getGlobalBounds();
	return r1.intersects(r2);
}
template <class T, class U>
Vector2<T> toVector2(const Vector2<U> &other)
{
	return Vector2<T>(
		static_cast<T>(other.x),
		static_cast<T>(other.y));
}

bool IsClickText(RenderWindow &window, Text &text)
{
	if (Mouse::getPosition(window).x >= text.getPosition().x
		&& Mouse::getPosition(window).x <= text.getPosition().x + text.getGlobalBounds().width
		&& Mouse::getPosition(window).y >= text.getPosition().y
		&& Mouse::getPosition(window).y <= text.getPosition().y + text.getGlobalBounds().height)
	{
		return true;
	}
	return false;
}

bool IsClickPic(RenderWindow &window, Sprite &sprite)
{
	if (Mouse::getPosition(window).x >= sprite.getPosition().x
		&& Mouse::getPosition(window).x <= sprite.getPosition().x + sprite.getGlobalBounds().width
		&& Mouse::getPosition(window).y >= sprite.getPosition().y
		&& Mouse::getPosition(window).y <= sprite.getPosition().y + sprite.getGlobalBounds().height)
	{
		return true;
	}
	return false;
}
bool startGame()
{
	const int  width = 640;
	const  int height = 480;
	float height1 = static_cast<float>(height);
	float borderSize = 21.0f;
	float moveDistance = 5.0f;
	RenderWindow window(sf::VideoMode(width, height), "AM7 WORK!",Style::Close);
	Menu menu(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
	RectangleShape top;
	RectangleShape left;
	RectangleShape right;
	RectangleShape bottom;
	Texture pTexture, pTexture1, pTexture2, pTexture3;
	Sprite PlayerImage, PlayerImage2, DoorImage, SoundImage;
	Clock clock;
	Font font;
	Music music2;
	if (!music2.openFromFile("enter_the_east.wav"))
		return EXIT_FAILURE;
	Music musicIntro;
	if (!musicIntro.openFromFile("MainMusic.ogg"))
		return EXIT_FAILURE;
	music2.setLoop(true);
	musicIntro.setLoop(true);
	enum Direction { Down, Left, Right, Up };
	enum Direction1 { S, A, D, W };
	enum states { INTRO, PLAYING, P1WON, P1LOST };

	float counter = 0, switchcharacter = 50, characterspeed = 50;
	bool Update = true;
	Vector2i source(1, Down);
	Vector2i source1(1, S);
	int p1Score = 0, p2Score = 0;
	if (!(font.loadFromFile("font1.ttf")))
		//if (!(font.loadFromFile("pic/font1.ttf")))
		cout << "Error to load font !" << endl;

	if (!pTexture.loadFromFile("Player5.png"))
		//if (!pTexture.loadFromFile("pic/Player5.png"))
		cout << "Error to load image !" << endl;

	if (!pTexture1.loadFromFile("Player4.png"))
		//if (!pTexture1.loadFromFile("pic/Player4.png"))
		cout << "Error to load image !" << endl;
	if (!pTexture2.loadFromFile("door.png", IntRect(10, 0, 32, 32)))
		//if (!pTexture2.loadFromFile("pic/door.png", IntRect(10, 0, 32, 32)))
		cout << "Error to load image !" << endl;
	if (!pTexture3.loadFromFile("sound.png"))
		//if (!pTexture2.loadFromFile("pic/door.png", IntRect(10, 0, 32, 32)))
		cout << "Error to load image !" << endl;
	PlayerImage.setTexture(pTexture);
	PlayerImage2.setTexture(pTexture1);
	DoorImage.setTexture(pTexture2);
	SoundImage.setTexture(pTexture3);
	window.setKeyRepeatEnabled(false);

	top.setPosition(borderSize, 0);
	top.setSize(Vector2f(width - 2 * borderSize, borderSize));
	top.setFillColor(Color(100, 100, 100));
	top.setOutlineColor(Color::Blue);
	top.setOutlineThickness(3);

	left.setPosition(0, 0);
	left.setSize(Vector2f(borderSize, height1));
	left.setFillColor(Color(100, 100, 100));
	left.setOutlineColor(Color::Blue);
	left.setOutlineThickness(3);

	right.setPosition(width - borderSize, 0);
	right.setSize(Vector2f(borderSize, height1));
	right.setFillColor(Color(100, 100, 100));
	right.setOutlineColor(Color::Blue);
	right.setOutlineThickness(3);

	bottom.setPosition(borderSize, height1 - borderSize);
	bottom.setSize(Vector2f(width - 2 * borderSize, borderSize));
	bottom.setFillColor(Color(100, 100, 100));
	bottom.setOutlineColor(Color::Blue);
	bottom.setOutlineThickness(3);

	Text text("  Police & Theif  ", font);
	text.setCharacterSize(20);
	text.setPosition(240, 1);
	text.setFillColor(sf::Color::Red);

	PlayerImage.setPosition(610, 425);
	PlayerImage2.setPosition(28, 425);
	DoorImage.setPosition(width / 2 - 22, 27);
	SoundImage.setPosition(50, -5);

	Text title("Police | Theif Game", font, 50);
	title.setPosition(width / 2 - title.getGlobalBounds().width / 2, 25);
	title.setFillColor(Color::Blue);

	Text start("Press Any Key To Start", font, 30);
	start.setPosition(width / 2 - start.getGlobalBounds().width / 2, 400);
	start.setFillColor(Color::Red);

	Text score("0      0", font, 50);
	score.setPosition(width / 2 - score.getGlobalBounds().width / 2, 20);
	score.setFillColor(Color(0, 100, 50));

	Text won("Police Catch The Thief \n Nice Work Policeman!\n\n\n\n Enter TAB To Play Again", font, 20);
	won.setPosition(width / 2 - won.getGlobalBounds().width / 2, height / 2 - won.getGlobalBounds().height / 2);
	won.setFillColor(Color::Green);

	Text lost("Ooh!!\n Thief Go Into the door\n Try Again Policeman\n The Good Luck!\n\n\n\n Enter TAB To Play Again", font, 20);
	lost.setFillColor(Color::Red);
	lost.setPosition(width / 2 - lost.getGlobalBounds().width / 2, height / 2 - lost.getGlobalBounds().height / 2);
	int gameState = INTRO;
	//playMusic("enter_the_east.wav");
	musicIntro.play();

	while (window.isOpen())
	{
		window.clear(Color::White);

		switch (gameState)
		{

		case INTRO:
			window.draw(title);
			menu.draw(window);
			break;
		case PLAYING:
			musicIntro.stop();
			window.draw(top);
			window.draw(bottom);
			window.draw(left);
			window.draw(right);
			window.draw(PlayerImage);
			window.draw(PlayerImage2);
			window.draw(text);
			window.draw(DoorImage);
			window.draw(score);
			window.draw(SoundImage);
			break;
		case P1WON:
			window.draw(won);
			break;
		case P1LOST:
			window.draw(lost);
			break;
		}
		window.display();
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
				case Keyboard::Tab:
					return true;
					break;
				case Keyboard::Escape:
					return false;
					break;
				case sf::Keyboard::Up:
					menu.MoveUp();
					break;
				case sf::Keyboard::Down:
					menu.MoveDown();
					break;
				case sf::Keyboard::Return:
					if ((menu.GetPressedItem() == 0) && (gameState == INTRO))
					{
						gameState = PLAYING;
					}
					else if (menu.GetPressedItem() == 1) //play
					{
						if (gameState == INTRO && musicIntro.getStatus() == musicIntro.Playing)
						{
							ErrorMsg("Really!! Music Is playing !", "Error Title");
							break;
						}
						else if (gameState == INTRO && musicIntro.getStatus() == musicIntro.Stopped)
							musicIntro.play();

					}
					else if (menu.GetPressedItem() == 2) //stop
					{
						if (gameState == INTRO)
							musicIntro.stop();
						else
							music2.stop();
					}
					else if (menu.GetPressedItem() == 3)
						window.close();
					break;
				}
				/////mousssssseeeeeeeee

			case Event::MouseButtonReleased:
				if (event.key.code == Mouse::Left && IsClickText(window, menu.menu[0]) && (gameState == INTRO))
				{
					gameState = PLAYING;
					if (music2.getStatus() == music2.Stopped)
						music2.play();
					break;
				}
				else if (event.key.code == Mouse::Left && IsClickText(window, menu.menu[1]))
				{
					if (gameState == INTRO && musicIntro.getStatus() == musicIntro.Playing)
					{
						ErrorMsg("Really!! Music Is playing !", "Error Title");
						break;
					}
					else if (gameState == INTRO && musicIntro.getStatus() == musicIntro.Stopped)
						musicIntro.play();
					break;
				}
				else if (event.key.code == Mouse::Left && IsClickText(window, menu.menu[2]))
				{
					if (gameState == INTRO && musicIntro.getStatus() == musicIntro.Stopped) //stop
						ErrorMsg("Really!! Music Is stopped !", "Error Title");
					else
						musicIntro.stop();
					break;
				}
				else if (event.key.code == Mouse::Left && IsClickPic(window, SoundImage))
				{
					if (gameState == PLAYING && musicIntro.getStatus() == musicIntro.Playing)
						music2.stop();
					else if (gameState == PLAYING && musicIntro.getStatus() == musicIntro.Stopped)
						music2.play();
					break;
				}
				else if (event.key.code == Mouse::Left && IsClickText(window, menu.menu[3]))
					window.close();

				break;
			case Event::MouseMoved:
				cout << "x : " << Mouse::getPosition().x << "y : " << Mouse::getPosition().y << endl;
				for (int i = 0; i < 4; i++)
				{
					if (IsClickText(window, menu.menu[i]))
						menu.menu[i].setFillColor(Color::Red);
					else
					{
						menu.menu[i].setFillColor(Color::Black);
					}
				}
				break;
			case  Event::Closed: // áãÇ íÏæÓ Úáí x íÞÝá
				window.close();
				break;
			}
		}

		//if (gameState == PLAYING)
		//{
		//	music2.play();
		//}
		if (Mouse::isButtonPressed(Mouse::Left)) // áÌÚá ÍÑßå ÇáÑÌá ÊÚãá
			Update = true;

		else if (Mouse::isButtonPressed(Mouse::Right)) // áÌÚá ÍÑßå ÇáÑÌá ÊÞÝ
			Update = false;

		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			source.y = Up;
			PlayerImage.move(0, static_cast<float>(-moveDistance / 50.0));
		}
		else if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			source.y = Down;
			PlayerImage.move(0, static_cast<float>(moveDistance / 50.0));
		}
		else if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			source.y = Left;
			PlayerImage.move(static_cast<float>(-moveDistance / 50.0), 0);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			source.y = Right;
			PlayerImage.move(static_cast<float>(moveDistance / 50.0), 0);
		}
		////////////////////////////////////////
		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			source1.y = W;
			PlayerImage2.move(0, static_cast<float>(-moveDistance / 50.0));
		}
		else if (Keyboard::isKeyPressed(Keyboard::S))
		{
			source1.y = S;
			PlayerImage2.move(0, static_cast<float>(moveDistance / 50.0));
		}
		else if (Keyboard::isKeyPressed(Keyboard::A))
		{
			source1.y = A;
			PlayerImage2.move(static_cast<float>(-moveDistance / 50.0), 0);
		}
		else if (Keyboard::isKeyPressed(Keyboard::D))
		{
			source1.y = D;
			PlayerImage2.move(static_cast<float>(moveDistance / 50.0), 0);
		}
		///////////////////////////////////////////////

		counter = (Update) ? counter + characterspeed * clock.restart().asSeconds() : 0;
		if (counter >= switchcharacter)
		{
			counter = 0;
			source.x++;
			source1.x++;
			if ((static_cast<int>(source.x * 32) >= static_cast<float>(pTexture.getSize().x)) && (static_cast<int>(source1.x * 32) >= static_cast<float>(pTexture1.getSize().x)))
			{
				source.x = 0;
				source1.x = 0;
			}
		}
		if (intersects(PlayerImage, top))
		{
			FloatRect t = top.getGlobalBounds();
			FloatRect b = PlayerImage.getGlobalBounds();
			PlayerImage.move(0, 2);
		}
		else if (intersects(PlayerImage, bottom))
		{
			FloatRect bot = bottom.getGlobalBounds();
			FloatRect b = PlayerImage.getGlobalBounds();
			PlayerImage.move(0, -2);
		}
		else if (intersects(PlayerImage, right))
		{

			FloatRect p = right.getGlobalBounds();
			FloatRect b = PlayerImage.getGlobalBounds();
			PlayerImage.move(-2, 0);

		}
		else if (intersects(PlayerImage, left))
		{

			FloatRect p = right.getGlobalBounds();
			FloatRect b = PlayerImage.getGlobalBounds();
			PlayerImage.move(2, 0);
		}
		///////////////////////////////////////////////////
		if (intersects(PlayerImage2, top))
		{
			FloatRect t = top.getGlobalBounds();
			FloatRect b = PlayerImage2.getGlobalBounds();
			PlayerImage2.move(0, 2);
		}
		else if (intersects(PlayerImage2, bottom))
		{
			FloatRect bot = bottom.getGlobalBounds();
			FloatRect b = PlayerImage2.getGlobalBounds();
			PlayerImage2.move(0, -2);
		}
		else if (intersects(PlayerImage2, right))
		{
			FloatRect p = right.getGlobalBounds();
			FloatRect b = PlayerImage2.getGlobalBounds();
			PlayerImage2.move(-2, 0);
		}
		else if (intersects(PlayerImage2, left))
		{
			FloatRect p = right.getGlobalBounds();
			FloatRect b = PlayerImage2.getGlobalBounds();
			PlayerImage2.move(2, 0);
		}
		//////////////////////////////////////////////////
		////////////////////////// score
		if (intersects1(PlayerImage, PlayerImage2))   ///player 1 = police   /// player2 = theif
		{
			p1Score++;
			stringstream str;
			str << p1Score << "      " << p2Score;
			score.setString(str.str());
			score.setPosition(width / 2 - score.getGlobalBounds().width / 2, 20);
			PlayerImage.setPosition(610, 425);
			PlayerImage2.setPosition(28, 425);

		}
		else if (intersects1(PlayerImage2, DoorImage))
		{
			p2Score++;
			stringstream str;
			str << p1Score << "      " << p2Score;
			score.setString(str.str());
			score.setPosition(width / 2 - score.getGlobalBounds().width / 2, 20);
			PlayerImage.setPosition(610, 425);
			PlayerImage2.setPosition(28, 425);

		}
		if (p1Score == 11)
		{
			gameState = P1WON;
		}
		else if (p2Score == 11)
		{
			gameState = P1LOST;
		}

		////////////////////////////////////////////
		PlayerImage.setTextureRect(IntRect(source.x * 32, source.y * 32, 32, 32)); // ÊÍÑíß ÇáÕæÑÉ
		PlayerImage2.setTextureRect(IntRect(source1.x * 32, source1.y * 32, 32, 32)); // ÊÍÑíß ÇáÕæÑÉ
		//DoorImage.setTextureRect(IntRect(source1.x * 32, source1.y * 32, 32, 32)); // ÊÍÑíß ÇáÕæÑÉ		

	}
	return false;
}
void gameRunning()
{
	if (startGame())
	{
		gameRunning();
	}
}
int main()
{
	gameRunning();

	return EXIT_SUCCESS;
}
