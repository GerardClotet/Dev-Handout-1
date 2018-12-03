#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "j1Fonts.h"
#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes
enum EVENT
{
	NO_EVENT,
	MOUSE_IN,
	MOUSE_OUT,
	RIGHT_CLICK,
	LEFT_CLICK
};
enum UI_TYPE
{
	NONE,
	IMAGE,
	TEXT,
	ATLAS,
	BUTTON
};

class UI
{
public: 
	iPoint position;
	UI_TYPE type;
	UI(iPoint position, UI_TYPE type);
	~UI();
	virtual bool Update();
	
};

class Button : public UI
{
public:
	SDL_Rect button_rect;
	Button(iPoint position, UI_TYPE type, SDL_Rect button_rect);
	~Button();
	virtual bool Update();
	EVENT CheckMouse(const SDL_Rect rect, const iPoint position);
	EVENT m_Event;
};

class Image : public UI
{
public:
	SDL_Rect rect;
	Image(iPoint position, UI_TYPE type, SDL_Rect rect);
	~Image();
	virtual bool Update();

};

class Text : public UI
{
public:
	_TTF_Font * txt_font = nullptr;
	char* string = nullptr;

	virtual bool Update();
	Text(iPoint position, UI_TYPE type, _TTF_Font * txt_font, char* string);
	~Text();
};

class Atlas : public UI
{
public:
	SDL_Rect rect;
	SDL_Texture* sprite;
	virtual bool Update();

	Atlas(iPoint position, UI_TYPE type, SDL_Rect rect, SDL_Texture* sprite);
	~Atlas();

};
// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions

	const SDL_Texture* GetAtlas() const;

	UI* CreateImage(iPoint position, UI_TYPE type, SDL_Rect rect);
	UI* CreateText(iPoint position, UI_TYPE type, _TTF_Font* txt_font, char* string);
	UI* CreateBackground(iPoint position, UI_TYPE type, SDL_Rect rect, SDL_Texture *sprite);
	UI* CreateButton(iPoint position, UI_TYPE type, SDL_Rect rect);

	SDL_Texture* atlas = nullptr;
	
	
private:

	

	p2SString atlas_file_name;

	p2List<UI*> UI_elements;
};

#endif // __j1GUI_H__