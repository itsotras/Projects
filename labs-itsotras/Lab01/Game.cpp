//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"


Game::Game() {
	myWindow = nullptr;//initialize myWindow as nullptr
	myRenderer = nullptr;//initialize myRenderer as nullptr
	isRunning = true;//sets the isRunning to true
	paddlePos.x = 30;
	paddlePos.y = windowH / 2;
	ballPos.x = windowW / 2;
	ballPos.y = windowH / 2;
}

bool Game::Initialize() {
	int i = SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO); //initializes SDL with the audio and video subsystems
	if (i != 0) {//if this initialization doesn't work, return false
		return false;
	}
	myWindow = SDL_CreateWindow("Pong", 50, 50, windowW, windowH, 0);//create a window and assign it to myWindow member
	if (myWindow == nullptr) {//if the window is still NULL return false
		return false;
	}
	myRenderer = SDL_CreateRenderer(myWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);//create a renderer using mywindow and the accelerated and vsync flags
	if (myRenderer == nullptr) {//if the renderer is NULL, return false
		return false;
	}
	return true;
}

void Game::RunLoop() {//runs a loop of the game
	while (isRunning) {
		ProcessInput();//processes input
		UpdateGame();//updates the game
		GenerateOutput();//generates the output
	}
}

void Game::Shutdown() {//shuts down the game
	SDL_DestroyRenderer(myRenderer);//destroys the renderer
	SDL_DestroyWindow(myWindow);//destroys the window
	SDL_Quit();//quits SDL
}

void Game::ProcessInput() {

	SDL_Event new_event;
	while (SDL_PollEvent(&new_event)) {//poll for new events
		switch (new_event.type) {//switch for the event types

		case SDL_QUIT: //if you click the 'x', set isRunning to false
			isRunning = false;
		}
	}
	const Uint8* mystate = SDL_GetKeyboardState(NULL);//get the state of the keyboard

	if (mystate[SDL_SCANCODE_ESCAPE]) {//if the escape key is pressed, set isRunning to false
		isRunning = false;
	}
	pDirection = 0;//reset the p direction each time

	if (mystate[SDL_SCANCODE_W]) {//if the w key is pressed
		pDirection -= 1;//move up
	}

	if (mystate[SDL_SCANCODE_S]) {//if the s key is pressed
		pDirection += 1;//move down
	}
}
void Game::UpdateGame() {
	//calculate deltaTime
	int currTime = SDL_GetTicks();
	float deltaTime;
	while (currTime - prevTime < 16) {//loop until the difference of milliseconds is at least 16
		currTime = SDL_GetTicks();//try again
	}
	deltaTime = currTime - prevTime;//calculate deltaTime
	if (deltaTime > 33) {//set max before using deltaTime
		deltaTime = 33;
	}
	deltaTime /= 1000.0f; //convert to milliseconds
	prevTime = (float)currTime;//set prevTime to the currentTime
	
	if (pDirection != 0) {//if the paddle isn't still
		paddlePos.y += (int)(300.0f * pDirection * deltaTime); //change the paddle position based on the direction of the paddle created in the processInput function
		
		if (paddlePos.y < paddleH/2 +wallThickness) {//if the position of the paddle is crossing the top wall
			paddlePos.y = paddleH / 2 + wallThickness;
		}
		if (paddlePos.y > (windowH - paddleH / 2 - wallThickness)) {//if the position of the paddle is crossing the bottom wall
			paddlePos.y = windowH - paddleH / 2 - wallThickness;
		}
	}
	if (ballPos.x +wallThickness/2 > windowW - wallThickness) {//if the ball hits the right wall, reverse its x velocity
		ballPos.x = windowW - wallThickness - wallThickness / 2;//move the ball so it doesn't get stuck in the wall
		ballVel.x *= -1;//flip its x velocity
	}
	if (ballPos.y - wallThickness / 2 < wallThickness) {//if the ball hits the top wall, reverse its y velocity
		ballPos.y = wallThickness + wallThickness / 2;//move the ball so it doesn't get stuck in the wall
		ballVel.y *= -1;//flip its y velocity
	}
	else if (ballPos.y + wallThickness / 2 > windowH - wallThickness) {//if the ball hits the bottom wall, reverse its y velocity
		ballPos.y = windowH - wallThickness - wallThickness / 2;//move the ball so it doesn't get stuck in the wall
		ballVel.y *= -1;//flip its y velocity
	}
	if (ballPos.x - wallThickness/2 < paddlePos.x + wallThickness/2) {//if the ball is in the x where it might hit the paddle
		if ((ballPos.y - wallThickness/2 < paddlePos.y +paddleH/2) && (ballPos.y + wallThickness / 2 > paddlePos.y - paddleH/2)) {
			//if the ball is within the range of the paddle
			ballPos.x = paddlePos.x+wallThickness;//move the ball so it doesn't get stuck in the paddle
			ballVel.x *= -1;//flip its x velocity
		}
	}
	if (ballPos.x - wallThickness / 2 < paddlePos.x - wallThickness / 2) {//if the ball is missed by the player
		isRunning = false;//end the game
	}
	ballPos.x += (int)(ballVel.x * deltaTime);//update the x and y positions
	ballPos.y += (int)(ballVel.y * deltaTime);
}

void Game::GenerateOutput() {

	SDL_SetRenderDrawColor(myRenderer, 0, 0, 255, 255);//draw the background blue

	SDL_RenderClear(myRenderer);//clear the renderer

	//draw the game objects

	SDL_SetRenderDrawColor(myRenderer, 255, 255, 255, 255);//set color to white

	//draw walls

	SDL_Rect topRect;//declare and set topRect attributes
	topRect.x = 0;
	topRect.y = 0;
	topRect.w = windowW;
	topRect.h = wallThickness;

	SDL_Rect rightRect;//declare and set rightRect attributes
	rightRect.x = windowW - wallThickness;
	rightRect.y = 0;
	rightRect.w = wallThickness;
	rightRect.h = windowH;

	SDL_Rect botRect;//declare and set botRect attributes
	botRect.x = 0;
	botRect.y = windowH-wallThickness;
	botRect.w = windowW;
	botRect.h = wallThickness;

	SDL_RenderFillRect(myRenderer, &topRect);//render the top rectangle
	SDL_RenderFillRect(myRenderer, &rightRect);//render the right rectangle
	SDL_RenderFillRect(myRenderer, &botRect);//render the bottom rectangle

	//draw paddle
	SDL_Rect paddle;//declare the paddle and its attributes
	paddle.x = paddlePos.x - wallThickness/2;
	paddle.y = paddlePos.y - paddleH/2;
	paddle.w = wallThickness;
	paddle.h = paddleH;

	//draw ball
	SDL_Rect ball;//declare the paddle and its attributes
	ball.x = ballPos.x - wallThickness / 2;
	ball.y = ballPos.y - wallThickness / 2;
	ball.w = wallThickness;
	ball.h = wallThickness;

	SDL_RenderFillRect(myRenderer, &paddle);//render the paddle
	SDL_RenderFillRect(myRenderer, &ball);//render the ball

	SDL_RenderPresent(myRenderer);//present the renderer
}

