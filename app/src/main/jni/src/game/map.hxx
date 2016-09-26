/**
 * @file map.hxx
 * @desc Map header.
 */
#ifndef GAME_MAP_HXX
#define GAME_MAP_HXX

class Movable;

/**
 * A map class.
 */
class Map
{
public:
	/**
	 * Destructs the map.
	 */
	~Map();

	/**
	 * Constructs the map.
	 */
	Map();

	/**
	 * Loads map data.
	 */
	bool load(const char *fileName);

	int getWidth() const { return width; }
	int getHeight() const { return height; }
	int* getImageData() { return imageData; }

private:
	int width, height;
	char *mapData;
	int *imageData;

	Map(const Map&);
	Map& operator=(const Map&);
};

#endif // GAME_MAP_HXX

