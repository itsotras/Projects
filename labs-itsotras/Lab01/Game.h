#pragma once
#include "SDL2/SDL.h"
// TODO
class Game {
public:
	Game(); //Game Constructor
	bool Initialize(); //Initializes the game
	void Shutdown(); //Shuts down the game
	void RunLoop(); //Runs the game loop

private:
	//members
	SDL_Window* myWindow;//my window member
	SDL_Renderer* myRenderer;//my renderer member
	SDL_Point paddlePos;//point of the position of the paddle
	SDL_Point ballPos;//point of the position of the ball
	SDL_Point ballVel{
		300, 
		-200
	};
	float prevTime = 0.0f;
	float pDirection = 0.0f;//direction of the paddle, positive is up, negative is down
	const int wallThickness = 10;//constant wall thickness
	const int paddleH = 100;//paddle Height
	int windowH = 768; //height of the window
	int windowW = 1024; //width of the window
	bool isRunning;
	//functions
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

};