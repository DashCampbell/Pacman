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
	sNode *parent;
	nodeType type;

	// Distance from starting node
	int gCost;
	// Distance to end node
	int hCost;

	int fCost() const
	{
		return gCost + hCost;
	}

	inline bool operator==(const sNode &rhs)
	{
		return this->x == rhs.x && this->y == rhs.y;
	}
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
	// Screen Size of a Map Tile
	const static int cellWidth = (screenWidth) / (mapWidth - 2);
	const static int cellHeight = (screenHeight) / (mapHeight - 2);

	// Screen Size of Player Radius
	static constexpr float playerRadius = (cellWidth - 6) / 2;

	int nPoints, nTotalPoints;
	sNode nodeMap[mapHeight][mapWidth];
	PacMan player;

	// test path
	std::list<sNode> path;

public:
	// Pathfinding Algorithm
	void pathFindingAStar(sNode nStart, sNode nEnd)
	{
		auto searchList = [](std::list<sNode *> &list, sNode *node)
		{
			for (auto &n : list)
			{
				if (n == node)
					return true;
			}
			return false;
		};
		// Get Distance between nodes
		auto getDistance = [](sNode &s, sNode &e)
		{
			return std::abs(s.x - e.x) + std::abs(s.y - e.y);
		};
		// Reset Map Values

		std::list<sNode *> openSet;
		std::list<sNode *> closedSet;
		sNode *currentNode;

		openSet.push_back(&nStart);

		// Prevent Infinite Loop
		const int maxIterations = mapHeight * mapWidth;
		int iteration = 0;
		while (!openSet.empty())
		{
			iteration++;
			if (iteration > maxIterations)
			{
				return;
			}
			// Get current node with the lowest f cost
			openSet.sort([](const sNode *lhs, const sNode *rhs)
						 { return (lhs->fCost() < rhs->fCost()) || (lhs->fCost() == rhs->fCost() && lhs->hCost < rhs->hCost); });
			currentNode = openSet.front();
			closedSet.push_back(openSet.front());
			openSet.pop_front();

			// If the current node equals the End Node, then path is found
			if (*currentNode == nEnd)
			{
				std::list<sNode> node_path;
				sNode cNode = *currentNode;

				while (!(cNode == nStart))
				{
					node_path.push_back(cNode);
					cNode = *cNode.parent;
				}
				node_path.reverse();
				path = node_path;
				std::cout << "Path Found" << std::endl;
				return;
			}

			// Loop through node neighbours
			for (auto &neighbour : currentNode->neighbours)
			{
				if (searchList(closedSet, neighbour))
					continue;

				int newMovementCostToNeighbour = currentNode->gCost + 1;
				if (newMovementCostToNeighbour < neighbour->gCost || !searchList(openSet, neighbour))
				{
					neighbour->gCost = newMovementCostToNeighbour;
					neighbour->hCost = getDistance(*neighbour, nEnd);
					neighbour->parent = currentNode;

					if (!searchList(openSet, neighbour))
						openSet.push_back(neighbour);
				}
			}
		}
	}

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
		// Add Node Neighbours
		for (int row = 1; row < mapHeight - 1; row++)
		{
			for (int col = 1; col < mapWidth - 1; col++)
			{
				for (int i = -1; i < 2; i += 2)
				{
					if (nodeMap[row + i][col].type == PATH_FULL)
						nodeMap[row][col].neighbours.push_back(&nodeMap[row + i][col]);
					if (nodeMap[row][col + i].type == PATH_FULL)
						nodeMap[row][col].neighbours.push_back(&nodeMap[row][col + i]);
				}
			}
		}
		std::cout << nodeMap[2][8].gCost << std::endl;

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

		// Update Path Finding
		sNode p;
		p.x = (int)player.x;
		p.y = (int)player.y;
		sNode nEnd;
		nEnd.x = 10;
		nEnd.y = 14;

		// Render
		Clear(olc::BLACK);
		pathFindingAStar(p, nEnd);
		DrawString(10, 30, std::to_string(path.size())), olc::WHITE;

		// Render Map
		for (int row = 0; row < mapHeight; row++)
		{
			for (int col = 0; col < mapWidth; col++)
			{
				// screen coordinates of top left corner of a cell
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
		FillCircle(nEnd.x * cellWidth - cellWidth, nEnd.y * cellHeight - cellHeight, playerRadius, olc::CYAN);

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