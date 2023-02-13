//
//  Main.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"

int main(int argc, char** argv)
{
	// TODO
	Game newGame;
	if (newGame.Initialize()) {
		newGame.RunLoop();
	}

	newGame.Shutdown();
	return 0;
}
