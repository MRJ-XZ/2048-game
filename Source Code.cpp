#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <ctime>
#include <cmath>
using namespace sf;
int array[4][4] = { 0 };
int current_score;
bool game_over = false;
void modify_circles(CircleShape* [][4] , Texture* []);
bool check(RenderWindow &, Text &, Text &, Font &);
void validate_new_score(int , int);
int main()
{
	ContextSettings setting;
	setting.antialiasingLevel = 8;
	RenderWindow window(VideoMode(400, 500), "2048", Style::Default, setting);
	window.setKeyRepeatEnabled(false);
	window.setFramerateLimit(10);
	window.setVerticalSyncEnabled(true);

// region load
	Texture* control_pic[5];
	for (int i = 0; i < 5; i++)
	{
		control_pic[i] = new Texture;
	}
	control_pic[0]->loadFromFile("textures\\controls.png");
	control_pic[1]->loadFromFile("textures\\left.png");
	control_pic[2]->loadFromFile("textures\\right.png");
	control_pic[3]->loadFromFile("textures\\up.png");
	control_pic[4]->loadFromFile("textures\\down.png");
	//Array index is representing the ( power - 1 ) of 2 .
	//e.g. textures[7] -> texture of 256 .
	Texture* textures[11];  
	for (int i = 0; i < 11; i++)
	{
		textures[i] = new Texture;
		std::string address = "textures\\";
		address += std::to_string((int)pow(2 , i + 1));
		address += ".png";
		textures[i]->loadFromFile(address);
	}
// endregion 

// region initializing
	srand((int)time(0));
	array[rand() % 4][rand() % 4] = 2;
	array[rand() % 4][rand() % 4 + 2] = 2;
	current_score = 0;
	
	Font font;
	font.loadFromFile("fonts\\aerial.ttf");
	
	Text title, score, bonus , controls;
	bonus.setFont(font);
	bonus.setString("GOLDEN BONUS . SCORE + 20");
	bonus.setCharacterSize(16);
	bonus.setPosition(Vector2f(0, 240));
	bonus.setFillColor(Color(227, 213, 28));
	bonus.setStyle(Text::Style::Bold);
	
	title.setFont(font);
	title.setString("SCORE :");
	title.setCharacterSize(18);
	title.setPosition(Vector2f(0, 440));
	title.setFillColor(Color::White);
	title.setStyle(Text::Style::Bold);
	
	score.setFont(font);
	score.setPosition(Vector2f(120, 440));
	score.setCharacterSize(18);
	score.setFillColor(Color(73, 186, 138));
	score.setStyle(Text::Style::Bold);
		
	controls.setFont(font);
	controls.setString("CONTROLS");
	controls.setPosition(Vector2f(240, 410));
	controls.setCharacterSize(14);
	controls.setFillColor(Color(73, 186, 138));
	controls.setStyle(Text::Style::Bold);
	
	RectangleShape* controls_pic = new RectangleShape(Vector2f(50, 50));
	controls_pic->setPosition(Vector2f(270, 435));

	CircleShape* circle[4][4];
	for (int i = 0; i < 4 ; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			circle[i][j] = new CircleShape(50.f, 100U);
			circle[i][j]->setPosition(Vector2f(i * 100, j * 100));
		}		
	}
// endregion

// region music
	//SoundBuffer buffer;
	//buffer.loadFromFile("music\\");
	//Sound sound;
	//sound.setBuffer(buffer);
	//sound.play();
	//sound.setLoop(true); 
// endregion 
	
	Event event;
	while (window.isOpen() && !game_over)
	{
		controls_pic->setTexture(control_pic[0]);
		window.draw(*controls_pic);
		window.display();
		// region enents		
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Left)
				{
					controls_pic->setTexture(control_pic[1]);
					for (int j = 0; j < 4; j++)
					{
						for (int i = 0; i < 9; i++)
						{
							if (array[i % 3][j] != 0)
							{
								if (array[i % 3 + 1][ j] == array[i % 3 ][ j])
								{
									array[i % 3 ][ j ] = 2 * array[i % 3 ][ j];
									array[i % 3 + 1 ][ j] = 0;
									validate_new_score(i % 3 , j );
									if (array[i % 3 ][j ] == 4096)
									{
										window.draw(bonus);
										window.display();
										sleep(milliseconds(1000));
										window.clear(Color::Black);
										array[i % 3][j] = 0;
									}

								}
							}
							else
							{
								array[i % 3][j] = array[i % 3 + 1][j];
								array[i % 3 + 1][j] = 0;
							}
						}
					}	
					game_over = check(window, title, score, font);
				}
				if (event.key.code == Keyboard::Right)
				{
					controls_pic->setTexture(control_pic[2]);
					for (int j = 0; j < 4; j++)
					{
						for (int i = 8; i >= 0; i--)
						{
							if (array[i % 3 + 1 ][ j] != 0)
							{
								if (array[i % 3 ][j] == array[i % 3 + 1][j])
								{
									array[i % 3 + 1][j] = 2 * array[i % 3 + 1][j];
									array[i % 3 ][j] = 0;
									validate_new_score(i % 3 + 1 , j);
									if (array[i % 3 + 1][j] == 4096)
									{
										window.draw(bonus);
										window.display();
										sleep(milliseconds(1000));
										window.clear(Color::Black);
										array[i % 3 + 1][j] = 0;
									}

								}
							}
							else
							{
								array[i % 3 + 1][j] = array[i % 3][j];
								array[i % 3 ][j] = 0;
							}
						}
					}
					game_over = check(window, title, score, font);
				}
				if (event.key.code == Keyboard::Up)
				{
					controls_pic->setTexture(control_pic[3]);
					for (int j = 0; j < 4; j++)
					{
						for (int i = 0; i < 9; i++)
						{
							if (array[j][i % 3] != 0)
							{
								if (array[j][i % 3 + 1] == array[j][i % 3])
								{
									array[j][i % 3] = 2 * array[j][i % 3];
									array[j][i % 3 + 1] = 0;
									validate_new_score(j , i % 3);
									if (array[j][i % 3] == 4096)
									{
										window.draw(bonus);
										window.display();
										sleep(milliseconds(1000));
										window.clear(Color::Black);
										array[j][i % 3] = 0;
									}
								}
							}
							else
							{
								array[j][i % 3] = array[j][i % 3 + 1];
								array[j][i % 3 + 1] = 0;
							}
						}
					}
					game_over = check(window, title, score, font);
				}
				if (event.key.code == Keyboard::Down)
				{
					controls_pic->setTexture(control_pic[4]);
					for (int j = 0; j < 4; j++)
					{
						for (int i = 8; i > 0; i--)
						{
							if (array[j][i % 3 + 1] != 0)
							{
								if (array[j][i % 3] == array[j][i % 3 + 1])
								{
									array[j][i % 3 + 1] = 2 * array[j][i % 3 + 1];
									array[j][i % 3] = 0;
									validate_new_score(j , i % 3 + 1);
									if (array[j][i % 3 + 1] == 4096)
									{
										window.draw(bonus);
										window.display();
										sleep(milliseconds(1000));
										window.clear(Color::Black);
										array[j][i % 3 + 1] = 0;
									}
								}
							}
							else
							{
								array[j][i % 3 + 1] = array[j][i % 3];
								array[j][i % 3] = 0;
							}
						}
					}
					game_over = check(window, title, score, font);
				}

			}
			// endregion
		}
		std::string print_score = std::to_string(current_score);
		score.setString(print_score);
		modify_circles(circle , textures);
		window.clear();
		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				window.draw(*circle[i][j]);
			}
		}
		
		window.draw(title);
		window.draw(score);
		window.draw(controls);
		window.draw(*controls_pic);
		window.display();		
	}	
	window.clear(Color(10, 10, 10, 100));
	while (window.isOpen())
	{		
		if (window.pollEvent(event))
		{
			if (event.key.code == Keyboard::Return || event.type == Event::Closed)
			{
				window.close();
			}
		}
		title.setFont(font);
		title.setString("GAME OVER . YOUR SCORE :\n\nPRESS (Enter) TO EXIT");
		title.setCharacterSize(16);
		title.setPosition(Vector2f(0, 240));
		title.setFillColor(Color(209, 208, 163));
		title.setStyle(Text::Style::Bold);
		score.setPosition(Vector2f(190, 260));
		window.draw(score);
		window.draw(title);
		window.display();
	}
}
void modify_circles(CircleShape* circle[4][4] , Texture* textures[])
{
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			switch (array[i][j])
			{
			case 0:
				circle[i][j]->setTexture(nullptr);
				circle[i][j]->setFillColor(Color(105, 105, 105));
				break;
			case 2:
				circle[i][j]->setTexture(nullptr);
				circle[i][j]->setFillColor(Color(255, 255, 255));
				circle[i][j]->setTexture(textures[0]);
				break;
			case 4:
				circle[i][j]->setTexture(nullptr);
				circle[i][j]->setFillColor(Color(255, 255, 255));
				circle[i][j]->setTexture(textures[1]);
				break;
			case 8:
				circle[i][j]->setTexture(nullptr);
				circle[i][j]->setFillColor(Color(255, 255, 255));
				circle[i][j]->setTexture(textures[2]);
				break;
			case 16:
				circle[i][j]->setTexture(nullptr);
				circle[i][j]->setFillColor(Color(255, 255, 255));
				circle[i][j]->setTexture(textures[3]);
				break;
			case 32:
				circle[i][j]->setTexture(nullptr);
				circle[i][j]->setFillColor(Color(255, 255, 255));
				circle[i][j]->setTexture(textures[4]);
				break;
			case 64:
				circle[i][j]->setTexture(nullptr);
				circle[i][j]->setFillColor(Color(255, 255, 255));
				circle[i][j]->setTexture(textures[5]);
				break;
			case 128:
				circle[i][j]->setTexture(nullptr);
				circle[i][j]->setFillColor(Color(255, 255, 255));
				circle[i][j]->setTexture(textures[6]);
				break;
			case 256:
				circle[i][j]->setTexture(nullptr);
				circle[i][j]->setFillColor(Color(255, 255, 255));
				circle[i][j]->setTexture(textures[7]);
				break;
			case 512:
				circle[i][j]->setTexture(nullptr);
				circle[i][j]->setFillColor(Color(255, 255, 255));
				circle[i][j]->setTexture(textures[8]);
				break;
			case 1024:
				circle[i][j]->setTexture(nullptr);
				circle[i][j]->setFillColor(Color(255, 255, 255));
				circle[i][j]->setTexture(textures[9]);
				break;
			case 2048:
				circle[i][j]->setTexture(nullptr);
				circle[i][j]->setFillColor(Color(255, 255, 255));
				circle[i][j]->setTexture(textures[10]);
				break;
			}
		}
	}
}
bool check(RenderWindow &window, Text &title, Text &score, Font &font)
{
	bool is_full = true;
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (array[i][j] == 0)
			{
				is_full = false;
				break;
			}
		}
	}	
	if (is_full)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (array[i][j] == array[i][j + 1])
				{
					return false;
					break;
				}
			}
		}
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (array[i][j] == array[i + 1][j])
				{
					return false;
					break;
				}
			}
		}
		return true;		
	}
	else
	{
		int x = rand() % 4;
		int y = rand() % 4;
		while (array[x][y] != 0)
		{
			x = rand() % 4;
			y = rand() % 4;
		}
		array[x][y] = 2;
		return false;
	}
}
void validate_new_score(int i , int j)
{
	switch (array[i][j])
	{
	case 4:
		current_score++;
		break;
	case 8:
		current_score += 2;
		break;
	case 16:
		current_score += 3;
		break;
	case 32:
		current_score += 4;
		break;
	case 64:
		current_score += 5;
		break;
	case 128:
		current_score += 6;
		break;
	case 256:
		current_score += 7;
		break;
	case 512:
		current_score += 8;
		break;
	case 1024:
		current_score += 9;
		break;
	case 2048:
		current_score += 10;
		break;
	case 4096:
		current_score += 20;
		break;
	default:
		break;
	}
}