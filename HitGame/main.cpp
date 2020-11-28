#include "Game.h"

int main() {

	std::srand(static_cast<unsigned>(time(NULL)));
	//Init Game 
	Game game;

	// Game Loop
	while (game.isrunning() && !game.getEndGame())
	{


		// Update
		game.update();

		// Render 
		game.render();


	}

	return 0;
}