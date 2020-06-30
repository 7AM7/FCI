#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <algorithm>
#include <sstream>
#include "Menu.h"
#include <windows.h> 
#include <string>
#include <string.h>
#include <time.h>
using namespace sf;

void ErrorMsg(string error, string title)
{
	MessageBoxA(NULL, error.c_str(), title.c_str(), MB_ICONERROR | MB_OK);
}

bool intersects(const CircleShape & rect1, const RectangleShape & rect2)
{
	FloatRect r1 = rect1.getGlobalBounds();
	FloatRect r2 = rect2.getGlobalBounds();
	return r1.intersects(r2);
}

bool intersects1(const RectangleShape & rect1, const RectangleShape & rect2)
{
	FloatRect r1 = rect1.getGlobalBounds();
	FloatRect r2 = rect2.getGlobalBounds();
	return r1.intersects(r2);
}
int clamp(const int x, const int a, const int b)
{
	return std::min(std::max(a, x), b);
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

	const int width = 655;
	const int height = 495;
	const int borderSize = 12;
	const int margin = 50;
	const int moveDistance = 5;

	RenderWindow window(VideoMode(width, height), "AM7 WORK!", Style::Close);
	Menu menu(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
	Font font;


	Music music2, musicIntro;
	Texture  pTexture;
	if (!music2.openFromFile("pack/enter_the_east.ogg"))
		return EXIT_FAILURE;
	if (!musicIntro.openFromFile("pack/MainMusic.ogg"))
		return EXIT_FAILURE;
	music2.setLoop(true);
	musicIntro.setLoop(true);

	if (!font.loadFromFile("pack/font1.ttf"))
		return EXIT_FAILURE;
	if (!pTexture.loadFromFile("pack/sound.png"))
		return EXIT_FAILURE;
	RectangleShape top;
	RectangleShape left;
	RectangleShape right;
	RectangleShape bottom;
	Sprite  SoundImage;
	SoundImage.setTexture(pTexture);
	SoundImage.setPosition(50, -6);
	// setting up all items
	top.setPosition(0, 0);
	top.setSize(Vector2f(width, borderSize + 6));

	left.setPosition(-borderSize, 0);
	left.setSize(Vector2f(borderSize, height));

	right.setPosition(width, 0);
	right.setSize(Vector2f(borderSize, height));

	bottom.setPosition(0, height - borderSize);
	bottom.setSize(Vector2f(width, borderSize));

	top.setFillColor(Color(100, 100, 100));
	top.setOutlineColor(Color::Blue);
	top.setOutlineThickness(3);

	left.setFillColor(Color(100, 100, 100));
	left.setOutlineColor(Color::Blue);
	left.setOutlineThickness(3);

	right.setFillColor(Color(100, 100, 100));
	right.setOutlineColor(Color::Blue);
	right.setOutlineThickness(3);

	bottom.setFillColor(Color(100, 100, 100));
	bottom.setOutlineColor(Color::Blue);
	bottom.setOutlineThickness(3);

	CircleShape ball(10.0f);
	ball.setPosition(width / 2, height / 2);
	//ball.setSize(Vector2f(20, 20));
	ball.setFillColor(Color::Yellow);
	ball.setOutlineColor(Color::Red);
	ball.setOutlineThickness(2);

	Vector2<float> ballSpeed(0.1, 0.1);

	RectangleShape player1;
	player1.setSize(Vector2f(borderSize, 90));
	player1.setPosition(margin - borderSize, height / 2 - 25);
	player1.setFillColor(Color(0, 122, 245));
	player1.setOutlineColor(Color::Red);
	player1.setOutlineThickness(3);

	RectangleShape player2;
	player2.setSize(Vector2f(borderSize, 90));
	player2.setPosition(width - margin, height / 2 - 25);
	player2.setFillColor(Color(0, 122, 245));
	player2.setOutlineColor(Color::Red);
	player2.setOutlineThickness(3);

	RectangleShape middleLine;
	middleLine.setFillColor(Color(100, 100, 100, 30));
	middleLine.setOutlineColor(Color(0, 0, 100, 30));
	middleLine.setOutlineThickness(2);
	middleLine.setPosition(width / 2, 0);
	middleLine.setSize(Vector2f(0, height));

	Text title("Pong Game", font, 50);
	title.setPosition(240, 1);
	title.setColor(Color::Blue);

	Text won("You have won this game.\n\n Congratulations !\n\n\n\n Enter TAB To Play Again", font, 20);
	won.setPosition(width / 2 - won.getGlobalBounds().width / 2, height / 2 - won.getGlobalBounds().height / 2);
	won.setColor(Color::Green);

	Text lost("You have lost this game, \n better luck next time!\n\n\n\n Enter TAB To Play Again", font, 20);
	lost.setPosition(width / 2 - lost.getGlobalBounds().width / 2, height / 2 - lost.getGlobalBounds().height / 2);
	lost.setColor(Color::Red);

	Text score("0   0", font, 50);
	score.setPosition(width / 2 - score.getGlobalBounds().width / 2, 40);
	score.setColor(Color(0, 0, 100, 50));

	unsigned int p1Score = 0, p2Score = 0;

	enum states { INTRO, PLAYING, P1WON, P1LOST };

	int gameState = INTRO;
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
			window.draw(middleLine);
			window.draw(left);
			window.draw(right);
			window.draw(player1);
			window.draw(player2);
			window.draw(ball);
			window.draw(score);
			window.draw(top);
			window.draw(bottom);
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
			case Event::KeyReleased:
				switch (event.key.code)
				{
				case Keyboard::Tab:
					return true;
					break;
				case Keyboard::Escape:
					return false;
					break;
				case Keyboard::Up:
					menu.MoveUp();
					break;
				case Keyboard::Down:
					menu.MoveDown();
					break;
				case Keyboard::Return:
					if ((menu.GetPressedItem() == 0) && (gameState == INTRO))
					{
						gameState = PLAYING;
						if (music2.getStatus() == music2.Stopped)
							music2.play();
						break;
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
						break;

					}
					else if (menu.GetPressedItem() == 2) //stop
					{
						if (gameState == INTRO && musicIntro.getStatus() == musicIntro.Stopped) //stop
							ErrorMsg("Really!! Music Is stopped !", "Error Title");
						else
							musicIntro.stop();
						break;
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
				else if (event.key.code == Mouse::Left && IsClickPic(window, SoundImage) )
				{
					if (gameState == PLAYING && music2.getStatus() == music2.Playing)
					{
						music2.stop();
						//	break;
					}
					else
						music2.play();

				}
				else if (event.key.code == Mouse::Left && IsClickText(window, menu.menu[3]))
					window.close();

				break;
			case Event::MouseMoved:
				//cout << "x : " << Mouse::getPosition().x << "y : " << Mouse::getPosition().y << endl;
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
			case  Event::Closed: // لما يدوس علي x يقفل
				window.close();
				break;
			}
		}

		//if (gameState != PLAYING)
		//	continue;

		// auto move player2 pad
		if (ball.getPosition().y < player2.getPosition().y)
			player2.move(0, -moveDistance / 40.0);
		else if (ball.getPosition().y + ball.getGlobalBounds().height > player2.getPosition().y + player2.getSize().y)
			player2.move(0, moveDistance / 40.0);
		// move player 1 pad
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			player1.move(0, -moveDistance / 50.0);
		}
		else
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			player1.move(0, moveDistance / 50.0);
		}
		// block players pad inside the play area
		if (intersects1(player1, bottom) || intersects1(player1, top))
		{
			FloatRect t = top.getGlobalBounds();
			FloatRect b = bottom.getGlobalBounds();
			Vector2f p = player1.getPosition();
			p.y = clamp(p.y, t.top + t.height + 5, b.top - player1.getSize().y - 5);
			player1.setPosition(p);
		}
		if (intersects1(player2, bottom) || intersects1(player2, top))
		{
			FloatRect t = top.getGlobalBounds();
			FloatRect b = bottom.getGlobalBounds();
			Vector2f p = player2.getPosition();
			p.y = clamp(p.y, t.top + t.height + 5, b.top - player2.getSize().y - 5);
			player2.setPosition(p);
		}
		// ball collides with top and bottom
		if (intersects(ball, top))
		{
			FloatRect t = top.getGlobalBounds();
			FloatRect b = ball.getGlobalBounds();
			ballSpeed.y = -ballSpeed.y;
			int u = t.top + t.height - b.top;
			ball.move(0, 2 * u);
		}
		if (intersects(ball, bottom))
		{
			FloatRect bot = bottom.getGlobalBounds();
			FloatRect b = ball.getGlobalBounds();
			ballSpeed.y = -ballSpeed.y;
			int u = bot.top - b.height - b.top;
			ball.move(0, 2 * u);
		}
		// ball collides with player1 and player2
		if (intersects(ball, player1))
		{
			FloatRect p = player1.getGlobalBounds();
			FloatRect b = ball.getGlobalBounds();
			ballSpeed.x = -ballSpeed.x;
			ballSpeed.y = (b.top + b.height / 2 - p.top - p.height / 2) / 100;
			int u = p.left + p.width - b.left;
			b.left = p.left + p.width + u;
			ball.setPosition(b.left, b.top);
			//increase ball speed by 1%
			ballSpeed.x = ballSpeed.x*1.01;
			ballSpeed.y = ballSpeed.y*1.01;
		}
		if (intersects(ball, player2))
		{
			FloatRect p = player2.getGlobalBounds();
			FloatRect b = ball.getGlobalBounds();
			ballSpeed.x = -ballSpeed.x;
			ballSpeed.y = (b.top + b.height / 2 - p.top - p.height / 2) / 100;
			int u = b.left + b.width - p.left;
			b.left = p.left - b.width - u;
			ball.setPosition(b.left, b.top);
			//increase ball speed by 1%
			ballSpeed.x = ballSpeed.x*1.01;
			ballSpeed.y = ballSpeed.y*1.01;
		}

		// check for scoring
		if (intersects(ball, left))
		{
			p2Score++;
			std::stringstream str;
			str << p1Score << "   " << p2Score;
			score.setString(str.str());
			score.setPosition(width / 2 - score.getGlobalBounds().width / 2, 40);
			FloatRect p = player2.getGlobalBounds();
			FloatRect b = ball.getGlobalBounds();
			ball.setPosition(p.left - b.width - 5, height / 2);
			ballSpeed.x = -0.1;
			ballSpeed.y = 0.1;

		}
		if (intersects(ball, right))
		{
			p1Score++;
			std::stringstream str;
			str << p1Score << "   " << p2Score;
			score.setString(str.str());
			score.setPosition(width / 2 - score.getGlobalBounds().width / 2, 40);
			FloatRect p = player1.getGlobalBounds();
			FloatRect b = ball.getGlobalBounds();
			ball.setPosition(p.left + p.width + 5, height / 2);
			ballSpeed.x = 0.1;
			ballSpeed.y = 0.1;
		}
		// detect if game is over
		if (p1Score >= 11 && p1Score >= p2Score + 2)
			gameState = P1WON;
		if (p2Score >= 11 && p2Score >= p1Score + 2)
			gameState = P1LOST;
		ball.move(ballSpeed.x, ballSpeed.y);
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
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
	gameRunning();

	return EXIT_SUCCESS;
}
