/*#include <raylib.h>
#include <raymath.h>

#include "imgui.h"
#include "rlImGui.h"

#include <iostream>
#include <vector>
#include <cstdint>

// cell

using Cell = uint32_t;

// grid

struct Grid
{
	int rows;
	int cols;
	int cellSize;

	std::vector<std::vector<Cell>> cells;

	inline void input()
	{
		Vector2 mousePos = GetMousePosition();
		if (IsMouseButtonDown(0))
		{
			for (int x = (int)mouseCell(mousePos).x - 2; x < (int)mouseCell(mousePos).x + 2; ++x)
			{
				for (int y = (int)mouseCell(mousePos).y - 2; y < (int)mouseCell(mousePos).y + 2; ++y)
				{
					if (inRows(x) && inCols(y) && cells[x][y] == 0) cells[x][y] = 1;
				}
			}
		}
	}
	
	inline void update()
	{
		std::vector<std::vector<Cell>> next (rows, std::vector<Cell> (cols, 0));

		for (int x = 0; x < rows; ++x)
		{
			for (int y = 0; y < cols; ++y)
			{
				int dir = ((rand() % 2) == 0) ? 1 : -1;

				switch (cells[x][y])
				{
				case 0:
					break;
				case 1:
					if (inCols(y+1) && cells[x][y+1] == 0)
					{
						next[x][y] = 0;
						next[x][y+1] = 1;
					}
					else if (inRows(x+1) && inCols(y+1) && inRows(x-1) && 
							 cells[x+1][y+1] == 0 && cells[x-1][y+1] == 0)
					{
						next[x][y] = 0;
						next[x+dir][y+dir] = 1;
					}
					else if (inCols(y+1) && inRows(x+1) && cells[x+1][y+1] == 0)
					{
						next[x][y] = 0;
						next[x+1][y+1] = 1;
					}
					else if (inCols(y+1) && inRows(x-1) && cells[x-1][y+1] == 0)
					{
						next[x][y] = 0;
						next[x-1][y+1] = 1;
					}
					else 
					{
						next[x][y] = 1;
					}
					break;
				case 2:
					if (inCols(y+1) && cells[x][y+1] == 0)
					{
						next[x][y] = 0;
						next[x][y+1] = 1;
					}
					for (int i = 0; i < 3; ++i)
					{
						if (inRows(x+i) && inCols(y+1) && inRows(x-i) && 
							 cells[x+i][y+1] == 0 && cells[x-i][y+1] == 0)
						else if()
			
					}
					else if (inRows(x+1) && inCols(y+1) && inRows(x-1) && 
							 cells[x+1][y+1] == 0 && cells[x-1][y+1] == 0)
					{
						next[x][y] = 0;
						next[x+dir][y+dir] = 1;
					}
					else if (inCols(y+1) && inRows(x+1) && cells[x+1][y+1] == 0)
					{
						next[x][y] = 0;
						next[x+1][y+1] = 1;
					}
					else if (inCols(y+1) && inRows(x-1) && cells[x-1][y+1] == 0)
					{
						next[x][y] = 0;
						next[x-1][y+1] = 1;
					}
					else 
					{
						next[x][y] = 1;
					}
					break;
				default:
					break;						
				}
			}
		}
		cells = next;
	}

	inline void draw()
	{
		for (int x = 0; x < rows; ++x)
		{
			for (int y = 0; y < cols; ++y)
			{
				switch (cells[x][y])
				{
				case 0:
					break;
				case 1:
					DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, WHITE);
					break;
				default:
					break;						
				}
			}
		}
	}

	inline Vector2 mouseCell(Vector2 mousePos)
	{
		return {mousePos.x / cellSize, mousePos.y / cellSize};
	}

	inline bool inRows(int x) {return x >= 0 && x < rows;}
	inline bool inCols(int y) {return y >= 0 && y < cols;}
};

Grid gridInit(int rows, int cols, int cellSize)
{
	Grid grid;

	grid.rows = rows;
	grid.cols = cols;
	grid.cellSize = cellSize;

	grid.cells = std::vector<std::vector<Cell>> (rows, std::vector<Cell> (cols, 0));

	return grid;
}

// main

int main(int argc, char* argv[])
{
	// initialization

	int screenWidth = 1920;
	int screenHeight = 1080;

	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
	InitWindow(screenWidth, screenHeight, "SandSim");
	SetTargetFPS(60);
	rlImGuiSetup(true);

	// setup grid

	const int cellSize = 4;
	const int rows = ceilf((float)screenWidth / (float)cellSize);
	const int cols = ceilf((float)screenHeight / (float)cellSize);

	Grid grid = gridInit(rows, cols, cellSize);
	grid.cells[10][0] = 1;
	//game loop

	while (!WindowShouldClose())
	{	
		grid.input();

		grid.update();

		BeginDrawing();
		ClearBackground(BLACK);

		grid.draw();

		rlImGuiBegin();

		rlImGuiEnd();

		EndDrawing();
	} 

    rlImGuiShutdown();
	CloseWindow();

	return 0;
}*/