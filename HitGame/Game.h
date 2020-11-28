#pragma once
#include <iostream>
#include<ctime>
#include<vector>
#include<sstream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

/*
	Class that acts as the game engine.
	Wrapper class.
*/

class Game
{
private:
	//Variables
	sf::RenderWindow * window;
	sf::VideoMode videomode;
	sf::Event event;

	//Game logic
	bool endGame;
	unsigned points;
	int health;
	float enemySpwanTimer;
	float enemySpwanTimerMax;
	int maxEnemies;
	bool mouseHeld;



	// Mouse positions
	sf::Vector2i mousePositionWindow;
	sf::Vector2f mousePositionView;
	
	// Resources
	sf::Font font;
	// Text
	sf::Text uiText;
	// Game Objects
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;
	void initVariables();
	void initWindow();
	void initEnemies();
	void initFont();
	void initText();
	void setUniquePosition();


public:
	// Constructor 
	Game();
	// Destructor
	virtual ~Game();
	
	//Accessors

	const bool isrunning() const;
	const bool getEndGame() const;
	// Functions
	void pollEvents();

	// Update Function
	void update();
	void updateMousePositions();
	void updateEnemies();
	void updateText();

	// Render Function
	void render();
	void renderText(sf::RenderTarget& target);
	void renderEnemies(sf::RenderTarget& target);
	void spwanEnemy();

};

