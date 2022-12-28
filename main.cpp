#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

enum nodeType {
	PATH,
	WALL,
	TELEPORTER
}

struct sNode
{
	int x, y;
	vector<*sNode> neighbours;
	nodeType type;
};

class Game : public olc::PixelGameEngine
{
public:
	Game()
	{
		sAppName = "Game";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
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