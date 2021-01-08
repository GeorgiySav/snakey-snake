/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"



Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	Sleep(100);
	gfx.EndFrame();
}

//checks for updates on the screen
void Game::UpdateModel()
{
	//only performs these if the snake is still alive
	if (gameOver == false)
	{
		//checks if the snake has hit a boundary
		CheckEdge();

		//checks if the snake has hit itself
		CheckCollision();

		//checks for keyboard inputs
		Input();

		//checks if the snake ate the fruit
		Ate();

		//changes the velocity
		headx += vx;
		heady += vy;
	}
}

//draws the frame
void Game::ComposeFrame()
{
	//draws boundaries
	DrawFrame();

	//draws the snake's head
	DrawBox(headx, heady, 0, 255, 0);

	//draws the snake's tail
	DrawTail();

	//draws the fruit
	DrawBox(fruitx, fruity, 255, 0, 0);
	
}

//checks for collisions
void Game::CheckCollision()
{
	//loops through all tail segments, and if the head's position is equal to a snakes position, then the velocity is reset and its game over
	for (int i = 1; i < nTail; i++)
	{
		if (headx == TailX[i] && heady == TailY[i])
		{
			vy, vx = 0;
			gameOver = true;
		}
			
	}
}

//draws the snake's tail
void Game::DrawTail()
{
	//second tail segment's value is set to the first segment's coords
	int prevX = TailX[0];
	int prevY = TailY[0];
	//declare third tail segment
	int prev2X, prev2Y;
	//first tail segment is set to the head's coords
	TailX[0] = headx;
	TailY[0] = heady;

	//loops through the entire tail list and moves the coords along
	for (int i = 1; i < nTail; i++)
	{
		prev2X = TailX[i];
		prev2Y = TailY[i];
		TailX[i] = prevX;
		TailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}

	//draws the tail
	for (int i = 0; i < nTail; i++)
	{
		//if the segment position is even, then it will be green
		if (i % 2 == 0)
		{
			DrawBox(TailX[i], TailY[i], 0, 255, 0);
		}
		//otherwise, it will be purple
		else
		{
			DrawBox(TailX[i], TailY[i], 255, 0, 255);
		}
			
		
	}
}

//checks if a fruit has been eaten
void Game::Ate()
{
	//compares the fruits and head's coords, and if they are equal, the tail grows, and the fruit is given a new random position
	if (headx == fruitx && heady == fruity)
	{
		bool can = false;
		do
		{
			//finds random coords
			fruitx = 30 + (14 * (rand() % 54 + 1)) - 14;
			fruity = 30 + (14 * (rand() % 54 + 1)) - 14;

			//checks if the snake is in that position
			for (int i = 0; i < nTail; i++)
			{
				//if it isn't, then it can print, otherwise, it will generates new coord
				if (fruitx != TailX[i] && fruity != TailY[i])
				{
					can = true;
				}
			}

		} while (can == false);
		

		nTail++;
	}

}

//draws the boundaries
void Game::DrawFrame()
{
	for (int i = 0; i < Graphics::ScreenWidth; i++)
	{
		for (int j = 0; j < Graphics::ScreenHeight; j++)
		{
			//only if the x value is 22 or less, a pixel will be drawn
			if (i >= 0 && i <= 22)
			{
				gfx.PutPixel(i, j, 255, 255, 255);
			}
			//only if the x value is 779 and above, a pixel with be drawn
			if (i >= Graphics::ScreenWidth - 22 && i <= Graphics::ScreenWidth)
			{
				gfx.PutPixel(i, j, 255, 255, 255);
			}

			//same logic applies here
			if (j >= 0 && j <= 22)
			{
				gfx.PutPixel(i, j, 255, 255, 255);
			}
			if (j >= Graphics::ScreenHeight - 22 && j <= Graphics::ScreenHeight)
			{
				gfx.PutPixel(i, j, 255, 255, 255);
			}
		}
	}
}

//draws a box
void Game::DrawBox(int x, int y, int r, int g, int b)
{
	//15 x 15 box
	for (int i = x - 7; i < x + 7; i++)
	{
		for (int j = y - 7; j < y + 7; j++)
		{
			if (i == x - 7 || i == x + 7 || j == y - 7 || j == y + 7)
			{
				gfx.PutPixel(i, j, 0, 0, 0);
			}
			else
			{
				gfx.PutPixel(i, j, r, g, b);
			}
		}
	}
}

//checks if the snake hit the edge
void Game::CheckEdge()
{
	//compares if the head's coords is within the white frames
	if (headx >= Graphics::ScreenWidth - 22 || headx <= 22 || heady >= Graphics::ScreenHeight - 22 || heady <= 22)
	{
		//if it is, velocity will be set to 0, and its game over
		vy = 0;
		vx = 0;
		gameOver = true;
	}
}

//checks for inputs
void Game::Input()
{
	//if the Up arrow key is pressed
	if (wnd.kbd.KeyIsPressed(VK_UP))
	{
		// checks if Up key has already been pressed so that velocity won't increase
		//also checks if the direction the snake is moving in down, so that the snake won't move back on its seld
		if (!pressedUp && dir != "DOWN")
		{
			vy = 0;
			vy -= 14;
			//resets x so that the snake won't be moving diagonaly
			vx = 0;
			pressedUp = true;
			dir = "UP";
		}
	}
	else
	{
		//resets the other presses so that they can be pressed again
		pressedRight, pressedLeft, pressedDown = false;
	}

	//same logic applies for the reset of the subroutine
	if (wnd.kbd.KeyIsPressed(VK_DOWN))
	{
		if (!pressedDown && dir != "UP")
		{
			vy = 0;
			vy += 14;
			vx = 0;
			pressedDown = true;
			dir = "DOWN";
		}
	}
	else
	{
		pressedRight, pressedLeft, pressedUp = false;
	}


	if (wnd.kbd.KeyIsPressed(VK_RIGHT))
	{
		if (!pressedRight && dir != "LEFT")
		{
			vx = 0;
			vx += 14;
			vy = 0;
			pressedRight = true;
			dir = "RIGHT";
		}
	}
	else
	{
		pressedDown, pressedUp, pressedLeft = false;
	}


	if (wnd.kbd.KeyIsPressed(VK_LEFT))
	{
		if (!pressedLeft && dir != "RIGHT")
		{
			vx = 0;
			vx -= 14;
			vy = 0;
			pressedLeft = true;
			dir = "LEFT";
		}
	}
	else
	{
		pressedDown, pressedUp, pressedRight = false;
	}
}
