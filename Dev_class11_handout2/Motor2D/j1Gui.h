#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "j1Fonts.h"
#include "p2DynArray.h"
#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes
enum EVENT
{
	NO_EVENT,
	MOUSE_IN,
	MOUSE_OUT,
	RIGHT_CLICK,
	LEFT_CLICK,
	RELEASE_CLICK
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
	iPoint screen_pos;
	iPoint local_pos;

	iPoint mouse_pos;
	UI_TYPE UI_type;
	EVENT m_Event;
	SDL_Rect screen_rect;
	SDL_Rect local_rect;
	SDL_Rect sprite_rect;
	bool parentboxCond;
	bool dragUIelem = false;
	bool interactable = false;

	bool clicked = false;
	UI(iPoint position, UI_TYPE type, UI* parent);
	UI(iPoint position, UI_TYPE type, SDL_Rect rect, UI* parent);
	//UI(iPoint position, UI_TYPE type, SDL_Rect rect, UI* parent);
	~UI();
	virtual bool Update();
	
	void SetScreenPos(iPoint position);
	iPoint GetScreenPos();

	void SetLocalPos(iPoint position);
	iPoint GetLocalPos();

	SDL_Rect GetScreenRect();
	SDL_Rect GetLocalRect();

	bool DragUI();

	EVENT CheckMouse(const SDL_Rect rect, const iPoint position);
	bool PriorityBox(int x, int y); //children rect is inside parent rect
public:
		UI* parent = nullptr;
	p2DynArray<UI*> children;
};

class Button : public UI
{
public:
	//SDL_Rect sprite_rect;
	SDL_Texture* sprite;
	Button(iPoint position, UI_TYPE type, SDL_Rect button_rect,SDL_Texture* sprite, UI* parent);
	~Button();
	virtual bool Update();
	
	
};

//class Image : public UI
//{
//public:
//	SDL_Rect rect;
//	Image(iPoint position, UI_TYPE type, SDL_Rect rect,UI* parent);
//	~Image();
//	virtual bool Update();
//
//};

class Label : public UI
{
public:
	_TTF_Font * txt_font = nullptr;
	std::string text="";
	char* string = nullptr;
	SDL_Texture* label_tex;
	//SDL_Rect sprite_rect= { 0,0,0,0 };
	virtual bool Update();
	void SetLabel(char* txt)
	{
		text = txt;
	}
	Label(iPoint position, UI_TYPE type /*_TTF_Font * txt_font*/, char* string,UI* parent);
	~Label();

	void CleanUp();
};

class Image : public UI
{
public:
	//SDL_Rect changerect;
	SDL_Texture* sprite;
	virtual bool Update();

	Image(iPoint position, UI_TYPE type, SDL_Rect rect, SDL_Texture* sprite,UI*parent);
	~Image();

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

	//UI* CreateImage(iPoint position, UI_TYPE type, SDL_Rect rect, UI* parent);
	UI* CreateLabel(iPoint position, UI_TYPE type, /*_TTF_Font* txt_font,*/ char* string, UI* parent);
	UI* CreateImage(iPoint position, UI_TYPE type, SDL_Rect rect, SDL_Texture *sprite, UI* parent);
	UI* CreateButton(iPoint position, UI_TYPE type, SDL_Rect rect,SDL_Texture* sprite,UI* parent);

	SDL_Texture* atlas = nullptr;
	
	void ElementPriority(int x, int y);
	UI*current_interac_elem = nullptr;
private:

	
	p2SString font_path;
	p2SString atlas_file_name;

	p2List<UI*> UI_elements;
};

#endif // __j1GUI_H__