#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

// TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
// ----------------------------------------------------

enum class map_orientation {
	orthogonal,
	isometric,
	error

};
// TODO 1: Create a struct needed to hold the information to Map node

enum class map_render_order
{
	none,
	right_down,
	right_up,
	left_up,
	left_down
};

struct tileset_info {
	uint firstgid = 0;
	p2SString name = "";
	p2SString image_path = "";
	uint tile_width = 0;
	uint tile_height = 0;
	uint spacing = 0;
	uint margin = 0;
};
struct map_info {
	map_orientation orientation = map_orientation::error;
	map_render_order render_order = map_render_order::none;
	uint width = 0;
	uint height = 0;
	uint tile_width = 0;
	uint tile_height = 0;
	uint nextobjectid = 0;

	p2SString filepath = "";

	
};
// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

private:
	bool LoadMapData();
	bool LoadTileData();
public:

	// TODO 1: Add your struct for map info as public for now
	map_info info;
	p2List<tileset_info> tilesets;
	p2List<SDL_Texture*> tilesets_textures;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__