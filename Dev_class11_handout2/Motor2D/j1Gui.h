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
	iPoint screen_pos;
	iPoint local_pos;
	UI_TYPE UI_type;
	EVENT m_Event;
	SDL_Rect global_rect;
	SDL_Rect local_rect;
	bool parentboxCond;
	UI(iPoint position, UI_TYPE type, UI* parent);
	~UI();
	virtual bool Update();
	
	void SetScreenPos(iPoint position);
	iPoint GetScreenPos();
	void SetLocalPos(iPoint position);
	iPoint GetLocalPos();
	SDL_Rect GetScreenRect();
	SDL_Rect GetLocalRect();

	bool PriorityBox(int x, int y); //children rect is inside parent rect
protected:
		UI* parent = nullptr;
	p2DynArray<UI*> children;
};

class Button : public UI
{
public:
	SDL_Rect button_rect;
	SDL_Texture* sprite;
	Button(iPoint position, UI_TYPE type, SDL_Rect button_rect,SDL_Texture* sprite, UI* parent);
	~Button();
	virtual bool Update();
	EVENT CheckMouse(const SDL_Rect rect, const iPoint position);
	
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
	SDL_Rect label_rect= { 0,0,0,0 };
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
	SDL_Rect changerect;
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
	UI* CreateBackground(iPoint position, UI_TYPE type, SDL_Rect rect, SDL_Texture *sprite, UI* parent);
	UI* CreateButton(iPoint position, UI_TYPE type, SDL_Rect rect,SDL_Texture* sprite,UI* parent);

	SDL_Texture* atlas = nullptr;
	
	
private:

	
	p2SString font_path;
	p2SString atlas_file_name;

	p2List<UI*> UI_elements;
};

#endif // __j1GUI_H__