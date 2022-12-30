#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <vector>

const int screenWidth = 360;
const int screenHeight = 360;

enum nodeType
{
	PATH,
	WALL,
	TELEPORTER
};

struct sNode
{
	int x, y;
	std::vector<sNode> neighbours;
	std::vector<sNode> pathNeighbours;
	nodeType type;
};

class Game : public olc::PixelGameEngine
{
public:
	Game()
	{
		sAppName = "Pacman";
	}

private:
	const static int mapWidth = 16;
	const static int mapHeight = 16;
	sNode nodeMap[mapHeight][mapWidth];

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		// Inialize Map
		int bitMap[mapHeight][mapWidth] = {
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
			{1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
			{1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
			{1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1},
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		};
		for (int row = 0; row < mapHeight; row++)
		{
			for (int col = 0; col < mapWidth; col++)
			{
				nodeMap[row][col].x = col;
				nodeMap[row][col].y = row;
				switch (bitMap[row][col])
				{
				case 1:
					nodeMap[row][col].type = WALL;
					break;
				case 2:
					nodeMap[row][col].type = TELEPORTER;
					break;
				default:
					nodeMap[row][col].type = PATH;
				}
			}
		}
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame

		// Render
		int cellWidth = (screenWidth) / (mapWidth - 3);
		int cellHeight = (screenHeight) / (mapHeight - 3);

		for (int row = 0; row < mapHeight; row++)
		{
			for (int col = 0; col < mapWidth; col++)
			{
				// coordinates of top left corner of a cell
				int cellX = col * cellWidth - cellWidth;
				int cellY = row * cellHeight - cellHeight;
				switch (nodeMap[row][col].type)
				{
				case WALL:
					DrawRect(cellX, cellY, cellWidth, cellHeight, olc::BLUE);
					break;
				case PATH:
					FillCircle(cellX + cellWidth / 2, cellY + cellHeight / 2, 1);
				}
			}
		}

		return true;
	}
};

int main()
{
	Game demo;
	demo.if (demo.Construct(screenWidth, screenHeight, 2, 2))
		demo.Start();

	return 0;
}