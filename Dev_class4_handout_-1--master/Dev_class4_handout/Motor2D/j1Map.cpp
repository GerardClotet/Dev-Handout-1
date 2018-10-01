#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	// TODO 6: Iterate all tilesets and draw all their 
	// images in 0,0 (you should have only one tileset for now)
	p2List_item<SDL_Texture*>* item = tilesets_textures.start;

	while (item != NULL) {
		App->render->Blit(item->data, 0, 0);
		item = item->next;
	}
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// TODO 2: Make sure you clean up any memory allocated
	// from tilesets / map


	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	if(ret == true)
	{
		LOG("Success parsing map, XML file: %s", file_name);
		LoadMapData();
		// TODO 3: Create and call a private function to load and fill
		// all your map data
	}

	// TODO 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	LoadTileData();

	if(ret == true)
	{
		// TODO 5: LOG all the data loaded
		// iterate all tilesets and LOG everything
	}

	map_loaded = ret;

	return ret;
}

bool j1Map::LoadMapData() {
	bool ret = true;
	pugi::xml_node map_node = map_file.child("map");

	p2SString orientation(map_node.attribute("orientation").as_string());
	if (orientation == "orthogonal") info.orientation = map_orientation::orthogonal;
	else if (orientation == "isometric") info.orientation = map_orientation::isometric;


	p2SString render_order(map_node.attribute("renderorder").as_string());
	if (render_order == "right-down") info.render_order = map_render_order::right_down;
	else if (render_order == "left-down") info.render_order = map_render_order::left_down;
	else if (render_order == "right-up") info.render_order = map_render_order::right_up;
	else if (render_order == "left-up") info.render_order = map_render_order::left_up;

	info.width = map_node.attribute("width").as_uint();
	info.height = map_node.attribute("height").as_uint();
	info.tile_width = map_node.attribute("tilewidth").as_uint();
	info.tile_height = map_node.attribute("tileheight").as_uint();
	info.nextobjectid = map_node.attribute("nextobjectid").as_uint();

	LOG("width: %i \n height: %i \n tile_width: %i \n tile_height: %i", info.width, info.height, info.tile_width, info.tile_height);
	return ret;
}

bool j1Map::LoadTileData() {

	for (pugi::xml_node tileset = map_file.child("map").child("tileset"); tileset; tileset = tileset.next_sibling("tileset")) {
		LOG("Tileset ----");
		tileset_info info;
		info.firstgid = tileset.attribute("firstgid").as_uint();
		info.name = tileset.attribute("name").as_string();
		info.tile_width = tileset.attribute("tilewidth").as_uint();
		info.tile_height = tileset.attribute("tilheight").as_uint();
		info.spacing = tileset.attribute("spacing").as_uint();
		info.margin = tileset.attribute("margin").as_uint();
		info.image_path = tileset.child("image").attribute("source").as_string();
		tilesets.add(info);

		p2SString tmp("%s%s", folder.GetString(), info.image_path.GetString());
		SDL_Texture* texture = App->tex->Load(tmp.GetString());
		tilesets_textures.add(texture);

		LOG("name: %s firstgid: %i \n tile_width: %i tile_height: %i \n spacing: %i margin: %i", info.name, info.firstgid, info.tile_width, info.tile_height, info.spacing, info.margin);
	}
	return true;
}

