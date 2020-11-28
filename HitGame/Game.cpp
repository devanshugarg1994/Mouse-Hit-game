#include "Game.h"

void Game::initVariables()
{
	this->window = nullptr;
	//Game Logic
	this->endGame = false;
	this->points = 0;
	this->enemySpwanTimer = 0.f;
	this->enemySpwanTimerMax = 20.f;
	this->maxEnemies = 10;
	this->mouseHeld = false;
	this->health = 10;
}

void Game::initWindow()
{
	this->videomode.width = 1280;
	this->videomode.height = 720;

	this->window = new sf::RenderWindow(this->videomode, "SFML First Game", sf::Style::Default);
	this->window->setFramerateLimit(144);
}

void Game::initEnemies()
{
	this->enemy.setSize(sf::Vector2f(100.f, 100.f));
	this->enemy.setFillColor(sf::Color::Cyan);

}

void Game::initFont()
{
	if (!this->font.loadFromFile("Fonts/Dosis_Light.ttf")) {
		std::cout << "Error!::GAME::INITFONTS::Failed to load" << std::endl;
	}
}

void Game::initText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(20);
	this->uiText.setFillColor(sf::Color::White);
	this->uiText.setString("NONE");


}



Game::Game() {
	this->initVariables();
	this->initWindow();
	this->initEnemies();
	this->initFont();
	this->initText();
}

Game::~Game() {
	delete this->window;
}

const bool Game::isrunning() const
{
	return this->window->isOpen();
}

const bool Game::getEndGame() const
{
	return this->endGame;
}



void Game::pollEvents()
{
	// Event polling
	while (this->window->pollEvent(this->event))
	{
		switch (this->event.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->event.key.code == sf::Keyboard::Escape)
				this->window->close();	
			break;
		default:
			break;
		}
	}
}

void Game::updateMousePositions()
{
	/* 
	* @return void
		Mouse position is updated realtive to window
	*/
	this->mousePositionWindow = sf::Mouse::getPosition(*this->window);
	this->mousePositionView = this->window->mapPixelToCoords(this->mousePositionWindow);
}

void Game::renderEnemies(sf::RenderTarget& target)
{

	for (auto &e : this->enemies) {
		target.draw(e);
	}

}

void Game::updateEnemies()
{
	/*
	* @ return void
		 Updates the enemy spwan timer and spwans enemies
		 when the total amount of enemies is smaller than the maximum.
		 Moves the enemy downwards
	*/

	//Updating the timer for enemy spwaning
	if (this->enemies.size() < this->maxEnemies)
	{
		if (this->enemySpwanTimer >= this->enemySpwanTimerMax) {
			// Spawn the enemy and reset the timer
			this->spwanEnemy();
			this->enemySpwanTimer = 0.f;
		}
		else {
			this->enemySpwanTimer += 1.f;
		}
	}

	for (int i = 0; i < this->enemies.size(); i++) {
		// move enemy downwrad
		this->enemies[i].move(0.f, 1.f);
		
		/*
			-Deleting the enemy when it go out of the window
		*/
		if (this->enemies[i].getPosition().y > this->window->getSize().y)
		{
			this->enemies.erase(this->enemies.begin() + i);
			this->health -= 1;
			if (this->health <= 0) {
				this->endGame = true;
			}
			std::cout << "health:" << this->health << std::endl;
		}

		/*
			-Deleting the enemy on the mouse click.

		*/
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (this->mouseHeld == false) {
				this->mouseHeld = true;
				bool deleted = false;
				for (size_t i = 0; i < this->enemies.size() && deleted == false; i++) {
					if (this->enemies[i].getGlobalBounds().contains(this->mousePositionView)) {
						deleted = true;
						if (this->enemies[i].getFillColor() == sf::Color::Magenta) {
							this->points += 10;

						}
						else if (this->enemies[i].getFillColor() == sf::Color::Blue) {
							this->points += 8;

						}	
						else if (this->enemies[i].getFillColor() == sf::Color::Cyan) {
							this->points += 5;

						}	
						else if (this->enemies[i].getFillColor() == sf::Color::Red) {
							this->points += 3;

						}	
						else if (this->enemies[i].getFillColor() == sf::Color::Green) {
							this->points += 1;

						}
						this->enemies.erase(this->enemies.begin() + i);
						std::cout << "Points:" << this->points << std::endl;
					}
				}
			}
		}
		else
		{
			this->mouseHeld = false;

		} 
	}
}

void Game::updateText()
{
	std::stringstream ss;
	ss << "Points: " << this->points << std::endl << "Health: " << this->health;
	this->uiText.setString(ss.str());

}

 
void Game::spwanEnemy()
{
	/*
		@return void
		Spawns enemies and sets their colors and positions
		Sets a random position.
		Sets a random color.
		Adds enemy to the vector. 
	*/

	int type = rand() % 5;
	switch (type)
	{
	case 0:
		this->enemy.setFillColor(sf::Color::Magenta);
		this->enemy.setSize(sf::Vector2f(50.f, 50.f));
		break;
	case 1:
		this->enemy.setFillColor(sf::Color::Blue);
		this->enemy.setSize(sf::Vector2f(70.f, 70.f));
		break;
	case 2:
		this->enemy.setFillColor(sf::Color::Cyan);
		this->enemy.setSize(sf::Vector2f(80.f, 80.f));
	case 3:
		this->enemy.setFillColor(sf::Color::Red);
		this->enemy.setSize(sf::Vector2f(90.f, 90.f));
		break;

	case 4:
		this->enemy.setFillColor(sf::Color::Green);
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		break;
	default:
		this->enemy.setFillColor(sf::Color::Green);
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		break;
	}
	this->setUniquePosition();

	this->enemies.push_back(this->enemy);

}
void Game::setUniquePosition()
{
	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
		0.f
	);

	// Checking Collision of two block
	// if they collide then we genrate new random co-ordinate
	for (auto&e : this->enemies) {
		if ((e.getPosition().x + e.getSize().x > this->enemy.getPosition().x) && (e.getPosition().x < this->enemy.getPosition().x + this->enemy.getSize().x) &&
			(e.getPosition().y + e.getSize().y > this->enemy.getPosition().y) && (e.getPosition().y < this->enemy.getPosition().y + this->enemy.getSize().y)) {
			this->setUniquePosition();
		}

	}
}


// Functions
void Game::update()
{
	// Event polling
	this->pollEvents();
	if (this->endGame == false) {
		//Mouse postion relative to window
		this->updateMousePositions();
		this->updateEnemies();
		this->updateText();
	}
}

void Game::render() {
	/*
		- clear window
		- render objects
		- display frame in windows
	*/
	this->window->clear();
	this->renderEnemies(*this->window);
	this->renderText(*this->window);
	this->window->display();
}

void Game::renderText(sf::RenderTarget& target)
{
	target.draw(this->uiText);
}


