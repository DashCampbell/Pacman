#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <vector>

const int screenWidth = 390;
const int screenHeight = 390;

enum nodeType
{
	PATH_FULL,
	PATH_EMPTY,
	WALL,
	TELEPORTER
};

struct sNode
{
	int x, y;
	std::vector<sNode *> neighbours;
	std::vector<sNode> pathNeighbours;
	nodeType type;
};
struct PacMan
{
	float x, y;
	bool bAlive;
};

class Game : public olc::PixelGameEngine
{
public:
	Game()
	{
		sAppName = "Pacman";
	}

private:
	// Number of Tiles accros a map
	const static int mapWidth = 16;
	const static int mapHeight = 16;
	// Size of a Map Tile
	const static int cellWidth = (screenWidth) / (mapWidth - 2);
	const static int cellHeight = (screenHeight) / (mapHeight - 2);

	static constexpr float playerRadius = (cellWidth - 6) / 2;

	int nPoints, nTotalPoints;
	sNode nodeMap[mapHeight][mapWidth];
	PacMan player;

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		nPoints = 0;
		nTotalPoints = 0;
		// Inialize Map
		int bitMap[mapHeight][mapWidth] = {
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1},
			{1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
			{1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
			{1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1},
			{1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1},
			{1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
			{1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
			{1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1},
			{1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1},
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
					nodeMap[row][col].type = PATH_FULL;
					nTotalPoints++;
				}
			}
		}
		// Inialize Player Position
		player.x = 8.5f;
		player.y = 8.5f;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame

		// Update Player Position
		float velocity = 4 * fElapsedTime;
		if (GetKey(olc::W).bHeld)
			player.y -= velocity;
		else if (GetKey(olc::S).bHeld)
			player.y += velocity;
		else if (GetKey(olc::A).bHeld)
			player.x -= velocity;
		else if (GetKey(olc::D).bHeld)
			player.x += velocity;

		// Resolve Player Collision
		// Radius of Player Scaled to Node Map Size is 0.5f
		// Left Side
		if (nodeMap[(int)player.y][(int)(player.x - 0.5f)].type == WALL)
			player.x = (int)player.x + 0.5f;
		// Right Side
		if (nodeMap[(int)player.y][(int)(player.x + 0.5f)].type == WALL)
			player.x = (int)player.x + 0.5f;
		// Top Side
		if (nodeMap[(int)(player.y - 0.5f)][(int)player.x].type == WALL)
			player.y = (int)player.y + 0.5f;
		// Bottom Side
		if (nodeMap[(int)(player.y + 0.5f)][(int)player.x].type == WALL)
			player.y = (int)player.y + 0.5f;

		// Eat Coins
		if (nodeMap[(int)player.y][(int)player.x].type == PATH_FULL)
		{
			nodeMap[(int)player.y][(int)player.x].type = PATH_EMPTY;
			nPoints++;
		}

		// Render
		Clear(olc::BLACK);
		// Render Map
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
					// Render Relevant Edges of Cell
					// Left Edge
					if (col >= 0 && nodeMap[row][col - 1].type != WALL)
						DrawLine(cellX, cellY, cellX, cellY + cellHeight, olc::BLUE);
					// Right Edge
					if (col < (mapWidth - 1) && nodeMap[row][col + 1].type != WALL)
						DrawLine(cellX + cellWidth, cellY, cellX + cellWidth, cellY + cellHeight, olc::BLUE);
					// Top Edge
					if (row >= 0 && nodeMap[row - 1][col].type != WALL)
						DrawLine(cellX, cellY, cellX + cellWidth, cellY, olc::BLUE);
					// Bottom Edge
					if (row < (mapHeight - 1) && nodeMap[row + 1][col].type != WALL)
						DrawLine(cellX, cellY + cellHeight, cellX + cellWidth, cellY + cellHeight, olc::BLUE);
					break;
				case PATH_FULL:
					FillCircle(cellX + cellWidth / 2, cellY + cellHeight / 2, 1);
				}
			}
		}

		// Render Player
		FillCircle(player.x * cellWidth - cellWidth, player.y * cellHeight - cellHeight, playerRadius, olc::YELLOW);

		// Display Player Coins
		DrawString(10, 10, "Coins: " + std::to_string(nPoints) + "/" + std::to_string(nTotalPoints));

		return true;
	}
};

int main()
{
	Game demo;
	if (demo.Construct(screenWidth, screenHeight, 2, 2))
		demo.Start();

	return 0;
}