#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>
#include <queue>
class BreadthFirstSearch : public olc::PixelGameEngine
{
	struct Node
	{
		int x;
		int y;
	};

	Node *node = nullptr;
	Node* rootNode = nullptr;
	Node* endNode = nullptr;
	int gap = 30; // gap is gotten by dividing the screen width (or height since both are 600 pixels) by the number of rows or columns (both of which are 20)


public:
	BreadthFirstSearch()
	{
		sAppName = "Breadth-First search visualizer";
	}

public:
	bool OnUserCreate() override
	{
		Clear(olc::WHITE);
		node = new Node[400]; // This is a 20x20 grid so in total there are 400 members
		for (int x = 0; x < 20; x++)
		{
			for (int y = 0; y < 20; y++)
			{
				DrawLine(x, y * gap, 600, y * gap, olc::BLACK); // Draw rows
				DrawLine(x * gap, y, x * gap, 600, olc::BLACK); // Draw columns
				// Saving the coordinates for the nodes
				node[y * 20 + x].x = x;  
				node[y * 20 + x].y = y;
			}
		}
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Gets the coordinates for the mouse position.
		int mouseX = (GetMouseX() / gap) * gap;
		int mouseY = (GetMouseY() / gap) * gap;
		int nodeX = mouseX / gap;
		int nodeY = mouseY / gap;
		// Variables for storing the coordinates
		int startingRow;
		int startingColumn;
		int endRow{};
		int endColumn{};
		// Bool variables for checking the validity of the node
		bool reachedEnd = false;
		bool obstacle[20][20];
		bool visited[20][20];
		// This is used when searching for neighbours of a node (top, bottom, right, left)
		int directionRow[4] = { +1, -1, 0, 0 };
		int directionColumn[4] = { 0, 0, +1 ,-1 };
		// Queues for the rows and columns
		std::queue<int> rowQueue;
		std::queue<int> columnQueue;

		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				// Initialized the bool variables to false
				obstacle[i][j] = false;
				visited[i][j] = false;
			}
		}
		if (GetMouse(0).bReleased) // Start node
		{
			FillRect(mouseX + 1, mouseY + 1, gap - 1, gap - 1, olc::GREEN);
			startingRow = node[nodeY * 20 + nodeX].x;
			startingColumn = node[nodeY * 20 + nodeX].y;
			rootNode = &node[nodeY * 20 + nodeX];
		}
		if (GetMouse(1).bPressed) // End node
		{
			FillRect(mouseX + 1, mouseY + 1, gap - 1, gap - 1, olc::RED);
			endRow = node[nodeY * 20 + nodeX].x;
			endColumn = node[nodeY * 20 + nodeX].y;
			endNode = &node[nodeY * 20 + nodeX];
		}
		if (GetKey(olc::Key::CTRL).bHeld) // Placing obstacles
		{
			FillRect(mouseX + 1, mouseY + 1, gap - 1, gap - 1, olc::GREY);
			obstacle[nodeX][nodeY] = true; // Error happens here if the obstacle node is placed at a certain point on the grid (or rather, multiple)
		}
		if (GetKey(olc::Key::SHIFT).bHeld) // Clearing the grid
		{
			FillRect(mouseX + 1, mouseY + 1, gap - 1, gap - 1, olc::WHITE);
			if (obstacle[nodeX][nodeY] == true)
			{
				obstacle[nodeX][nodeY] = false;
			}
			if (&node[nodeY * 20 + nodeX] == rootNode)
			{
				FillRect(mouseX + 1, mouseY + 1, gap - 1, gap - 1, olc::WHITE);
				rootNode = nullptr;
			}
			if (&node[nodeY * 20 + nodeX] == endNode)
			{
				FillRect(mouseX + 1, mouseY + 1, gap - 1, gap - 1, olc::WHITE);
				endNode = nullptr;
			}
		}
		if (GetKey(olc::Key::SPACE).bPressed) // Main algorithm
		{
			int row;
			int column;
			int currentRow;
			int currentColumn;
			// Pushed the staring node into the queue
			rowQueue.push(startingRow);
			columnQueue.push(startingColumn);
			visited[startingRow][startingColumn] = true; // Error happens here if only the start and end node are placed or if the user begins to places the obstacle nodes

			while (!rowQueue.empty())
			{
				row = rowQueue.front();
				rowQueue.pop();
				column = columnQueue.front();
				columnQueue.pop();

				if (row == endRow && column == endColumn) // Check if the current node has the same coordinates as the end node
				{
					reachedEnd = true;
					FillRect(row + 1, column + 1, gap - 1, gap - 1, olc::BLUE);
					break;
				}
				for (int i = 0; i < 4; i++) // Neighbour checking
				{
					currentRow = row + directionRow[i];
					currentColumn = column + directionColumn[i];

					if (currentRow < 0 || currentColumn < 0)
					{
						continue;
					}
					if (currentRow >= 20 || currentColumn >= 20)
					{
						continue;
					}
					if (visited[currentRow][currentColumn] == true)
					{
						continue;
					}
					if (obstacle[currentRow][currentColumn] == true)
					{
						continue;
					}
					// Pushed the current node into the queue if all the statements were false
					rowQueue.push(currentRow);
					columnQueue.push(currentColumn);
					visited[currentRow][currentColumn] = true; 
					FillRect(currentRow + 1, currentColumn + 1, gap - 1, gap - 1, olc::BLUE);
				}
			}
		}
		return true;
	}
};


int main()
{
	BreadthFirstSearch demo;
	if (demo.Construct(600, 600, 1, 1))
		demo.Start();

	return 0;
}