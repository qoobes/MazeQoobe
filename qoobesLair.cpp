/*
This is a very simple maze generator I made while watching a youtube tutorial.
It's not much different but I found it interesting and didn't really have much to add to it so yeah
*/


#include <iostream>
#include <stack>
using namespace std;

#include "GameEngine.h"

class qoobesLair : public CustomGameEngine
{
public:
	qoobesLair()
	{
		m_sAppName = L"MAZE";
	}

private:
	int  m_nMazeWidth;
	int  m_nMazeHeight;
	int* m_maze;

	enum
	{
		CELL_PATH_N = 0x01,
		CELL_PATH_E = 0x02,
		CELL_PATH_S = 0x04,
		CELL_PATH_W = 0x08,
		CELL_VISITED = 0x10,
	};


	int  m_nVisitedCells;
	stack<pair<int, int>> m_stack;
	int  m_nPathWidth;


protected:
	virtual bool OnUserCreate()
	{
		m_nMazeWidth = 40;
		m_nMazeHeight = 25;
		m_maze = new int[m_nMazeWidth * m_nMazeHeight];
		memset(m_maze, 0x00, m_nMazeWidth * m_nMazeHeight * sizeof(int));
		m_nPathWidth = 3;
		int x = rand() % m_nMazeWidth;
		int y = rand() % m_nMazeHeight;
		m_stack.push(make_pair(x, y));
		m_maze[y * m_nMazeWidth + x] = CELL_VISITED;
		m_nVisitedCells = 1;

		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime)
	{
		// this_thread::sleep_for(10ms);

		auto offset = [&](int x, int y)
		{
			return (m_stack.top().second + y) * m_nMazeWidth + (m_stack.top().first + x);
		};

		if (m_nVisitedCells < m_nMazeWidth * m_nMazeHeight)
		{
			vector<int> neighbours;

			if (m_stack.top().second > 0 && (m_maze[offset(0, -1)] & CELL_VISITED) == 0)
				neighbours.push_back(0);
			if (m_stack.top().first < m_nMazeWidth - 1 && (m_maze[offset(1, 0)] & CELL_VISITED) == 0)
				neighbours.push_back(1);
			if (m_stack.top().second < m_nMazeHeight - 1 && (m_maze[offset(0, 1)] & CELL_VISITED) == 0)
				neighbours.push_back(2);
			if (m_stack.top().first > 0 && (m_maze[offset(-1, 0)] & CELL_VISITED) == 0)
				neighbours.push_back(3);

			if (!neighbours.empty())
			{
				int next_cell_dir = neighbours[rand() % neighbours.size()];

				switch (next_cell_dir)
				{
				case 0: 
					m_maze[offset(0, -1)] |= CELL_VISITED | CELL_PATH_S;
					m_maze[offset(0, 0)] |= CELL_PATH_N;
					m_stack.push(make_pair((m_stack.top().first + 0), (m_stack.top().second - 1)));
					break;

				case 1: 
					m_maze[offset(+1, 0)] |= CELL_VISITED | CELL_PATH_W;
					m_maze[offset(0, 0)] |= CELL_PATH_E;
					m_stack.push(make_pair((m_stack.top().first + 1), (m_stack.top().second + 0)));
					break;

				case 2: 
					m_maze[offset(0, +1)] |= CELL_VISITED | CELL_PATH_N;
					m_maze[offset(0, 0)] |= CELL_PATH_S;
					m_stack.push(make_pair((m_stack.top().first + 0), (m_stack.top().second + 1)));
					break;

				case 3:
					m_maze[offset(-1, 0)] |= CELL_VISITED | CELL_PATH_E;
					m_maze[offset(0, 0)] |= CELL_PATH_W;
					m_stack.push(make_pair((m_stack.top().first - 1), (m_stack.top().second + 0)));
					break;

				}

				m_nVisitedCells++;
			}
			else
			{
				m_stack.pop();
			}
		}


		// tHe drawening
		Fill(0, 0, ScreenWidth(), ScreenHeight(), L' '); // make it all 
		// I can not express how much i hate the amount of for loops here,
		// though i don't know a better way of doing it so that's just what it is
		for (int x = 0; x < m_nMazeWidth; x++)
		{
			for (int y = 0; y < m_nMazeHeight; y++)
			{
				for (int py = 0; py < m_nPathWidth; py++)
					for (int px = 0; px < m_nPathWidth; px++)
					{
						if (m_maze[y * m_nMazeWidth + x] & CELL_VISITED)
							Draw(x * (m_nPathWidth + 1) + px, y * (m_nPathWidth + 1) + py, PIXEL_SOLID, FG_WHITE); // Draw Cell
						else
							Draw(x * (m_nPathWidth + 1) + px, y * (m_nPathWidth + 1) + py, PIXEL_SOLID, FG_RED); // Draw Cell
					}

				for (int p = 0; p < m_nPathWidth; p++)
				{
					if (m_maze[y * m_nMazeWidth + x] & CELL_PATH_S)
						Draw(x * (m_nPathWidth + 1) + p, y * (m_nPathWidth + 1) + m_nPathWidth); // Draw South Passage

					if (m_maze[y * m_nMazeWidth + x] & CELL_PATH_E)
						Draw(x * (m_nPathWidth + 1) + m_nPathWidth, y * (m_nPathWidth + 1) + p); // Draw East Passage
				}
			}
		}

		// Make the top of the stacc spessl... with another for loop
		for (int py = 0; py < m_nPathWidth; py++)
			for (int px = 0; px < m_nPathWidth; px++)
				Draw(m_stack.top().first * (m_nPathWidth + 1) + px, m_stack.top().second * (m_nPathWidth + 1) + py, 0x2588, FG_BLUE); 


		return true;
	}
};


int main()
{
	

	// use the youtuber's stuff
	qoobesLair game;
	game.ConstructConsole(160, 100, 8, 8);
	game.Start();

	return 0;
}