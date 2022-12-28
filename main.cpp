#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <vector>

enum nodeType
{
	PATH,
	WALL,
	TELEPORTER
};

struct sNode
{
	int x, y;
	std::vector<*sNode> neighbours;
	std::vector<*sNode> pathNeighbours;
	nodeType type;
};

class Game : public olc::PixelGameEngine
{
public:
	Game()
	{
		sAppName = "Game";
	}

private:
	int mapWidth = 16;
	int mapHeight = 16;
	sNode nodeMap[mapHeight][mapWidth];

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		// Inialize Map
		int bitMap[mapHeight][mapWidth] = {
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
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

		return true;
	}
};

int main()
{
	Game demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();

	return 0;
}