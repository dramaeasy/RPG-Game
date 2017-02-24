#include "level.h"

std::vector<bool> Level::isWalkable;
int Level::width;
int Level::height;
math::vec3 Level::playerLoc(0.0f, 0.0f, 0.0f);
std::vector<Entity *> Level::entities;

Level::Level(int levelNumber, SpriteSheet * tileSprites)
{
	indexOfCurrScene = 0;
	loadLevel(levelNumber, tileSprites);
}

Level::~Level()
{

}

void Level::loadLevel(int levelNumber, SpriteSheet * tileSprites)
{

	if (levelNumber == 0)
	{
		//Load level data from file
		
		//TmxFile level1File("levels/levelTEST2.tmx");
		TmxFile level1File("levels/DemoLevel.tmx");
		level1File.loadTileLayersData();

		TileLayer * backgroundLayer = new TileLayer(new Shader("shaders/basic.vert", "shaders/basic.frag"), tileSprites, level1File.getLayersData()[0], level1File.getAnimationData());
		TileLayer * objectLayer = new TileLayer(new Shader("shaders/basic.vert", "shaders/basic.frag") , tileSprites, level1File.getLayersData()[1], level1File.getAnimationData());
		TileLayer * collisionLayer = new TileLayer(new Shader("shaders/basic.vert", "shaders/basic.frag"), tileSprites, level1File.getLayersData()[2], level1File.getAnimationData());
		Layer * playerLayer = new Layer(new BatchRenderer2D(), new Shader("shaders/basic.vert", "shaders/basic.frag"), math::mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
		Layer * hudLayer = new Layer(new BatchRenderer2D(), new Shader("shaders/basic.vert", "shaders/basic.frag"), math::mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));

		scenes.push_back(new Scene(backgroundLayer, objectLayer, collisionLayer, playerLayer, hudLayer));

		indexOfCurrScene = 0;

		isWalkable = TileLayer::isWalkable;
		Level::width = backgroundLayer->getWidth();
		Level::height = backgroundLayer->getHeight();

		for (int i = 0; i < objectLayer->getRenderables().size(); i++)
		{
			//math::vec3 objectCords(Level::screenToWorldCoords(objectLayer->getRenderables()[i]->getPosition()));
			math::vec3 objectCords(objectLayer->getRenderables()[i]->getPosition());
			//Chest object
			if (objectLayer->getRenderables()[i]->getTID() == 188)
			{
				Chest * chest = new Chest();
				chest->addComponent(new Transform(objectCords));
				chest->addComponent(new Collectible());
				Level::entities.push_back(chest);
			}
			//Key object
			else if (objectLayer->getRenderables()[i]->getTID() == 199)
			{
				Key * key = new Key();
				key->addComponent(new Transform(objectCords));
				key->addComponent(new RectangleAppearance((float)TileLayer::sizeOfTile, (float)TileLayer::sizeOfTile));
				key->addComponent(new Collectible());
				key->addComponent(new SpriteComponent((Sprite*)objectLayer->getRenderables()[i]));
				Level::entities.push_back(key);
			}
			else if (objectLayer->getRenderables()[i]->getTID() == 1875)//Disapearing Tile object
			{
				DisapearingTile * disapearingTile = new DisapearingTile();
				disapearingTile->addComponent(new Transform(objectCords));
				disapearingTile->addComponent(new SpriteComponent((Sprite*)objectLayer->getRenderables()[i]));
				disapearingTile->addComponent(new TimerComponent(30.0f));
				Level::entities.push_back(disapearingTile);
			}
			else if (objectLayer->getRenderables()[i]->getTID() == 3121) //Slime Enemy
			{
				Slime * slime = new Slime();
				slime->addComponent(new Transform(objectCords));
				slime->addComponent(new RectangleAppearance((float)TileLayer::sizeOfTile, (float)TileLayer::sizeOfTile));
				slime->addComponent(new SpriteComponent((Sprite*) objectLayer->getRenderables()[i]));
				slime->addComponent(new Attack(1.0f, 2.0, true, true));
				slime->addComponent(new Collision());
				slime->addComponent(new AIComponent(true, 0.04f));
				slime->addComponent(new CircleAppearance(5.0f));
				slime->addComponent(new HealthComponent(100.0f));
				Level::entities.push_back(slime);
			}
		}

	}
	else if (levelNumber == 1)
	{
		//load level 1;
	}

}

//Location: (44.1, 54.3, 0) -> (88.2, 108.6, 0)
math::vec3 Level::worldToScreenCoords(math::vec3 loc)
{
	return math::vec3((loc.x * (float)TileLayer::sizeOfTile), (loc.y * (float)TileLayer::sizeOfTile), loc.z);
}

// Location: (88.2, 108.6, 0) -> (44.1, 54.3, 0)
math::vec3 Level::screenToWorldCoords(math::vec3 loc)
{
	return math::vec3((loc.x / (float)TileLayer::sizeOfTile), (loc.y / (float)TileLayer::sizeOfTile), loc.z);
}

// Location: (88.32, 108.65, 0) -> (44, 54, 0)
math::vec3 Level::screenToTileCoords(math::vec3 loc)
{
	return math::vec3(((int)loc.x), ((int)loc.y), loc.z);
}