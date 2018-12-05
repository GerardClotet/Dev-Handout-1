#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("atlas.png");
	
	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	for (uint i = 0; i < UI_elements.count(); ++i)
	{
		delete UI_elements[i];
		UI_elements[i] = nullptr;
	}
	App->tex->UnLoad(atlas);
	App->font->CleanUp();
	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------
UI::UI(iPoint position, UI_TYPE type) :position(position), type(type){}
bool UI::Update()
{
	return true;
}
UI::~UI(){}

Image::Image(iPoint position, UI_TYPE type, SDL_Rect rect) : UI(position, type), rect(rect) {}
bool Image::Update()
{

	App->render->Blit((SDL_Texture*)(App->gui->GetAtlas()), this->position.x, this->position.y, &this->rect);
	return true;
}
Image::~Image(){}


Text::Text(iPoint position, UI_TYPE type, _TTF_Font* font, char* string) :UI(position, type), txt_font(font), string(string) {}
bool Text::Update()
{
	App->render->Blit(App->font->Print(this->string, { 255,255,255,255 }, this->txt_font),this->position.x, this->position.y);
	return true;
}
Text::~Text(){}

Atlas::Atlas(iPoint position, UI_TYPE type, SDL_Rect rect, SDL_Texture* sprite) :UI(position, type), rect(rect), sprite(sprite){}
bool Atlas::Update()
{
	App->render->Blit(this->sprite, this->position.x, this->position.y, &this->rect);
	return true;
}
Atlas::~Atlas(){}

Button::Button(iPoint position, UI_TYPE type, SDL_Rect rect) : UI(position, type), button_rect(rect) {}
bool Button::Update()
{
	
	CheckMouse(this->button_rect, this->position);
	return true;
}
Button::~Button() {}

UI*j1Gui::CreateImage(iPoint position, UI_TYPE type, SDL_Rect rect)

{
	UI* image_element = nullptr;
	image_element = new Image(position, type, rect);
	UI_elements.add(image_element);
	return image_element;
}

UI*j1Gui::CreateText(iPoint position, UI_TYPE type, _TTF_Font* txt_font, char* string)
{
	UI* text_element = nullptr;
	text_element = new Text(position, type, (txt_font) ? txt_font:App->font->default, string);
	UI_elements.add(text_element);
	LOG("text created");

	return text_element;

}


UI*j1Gui::CreateBackground(iPoint position, UI_TYPE type, SDL_Rect rect, SDL_Texture* sprite)
{
	UI* background_element = nullptr;
	//background_element = new Atlas(position, type, rect, sprite);

	background_element = new Atlas(position, type, rect, sprite);
	UI_elements.add(background_element);
	//LOG("background created");

	return background_element;
}

UI*j1Gui::CreateButton(iPoint position, UI_TYPE type, SDL_Rect rect )
{
	UI* button_element = nullptr;
	button_element = new Button(position, type, rect);
	UI_elements.add(button_element);
	return button_element;
}

EVENT Button::CheckMouse(const SDL_Rect rect_button, const iPoint position)
{
	m_Event = NO_EVENT;
	int mouse_X, mouse_Y;
	App->input->GetMousePosition(mouse_X, mouse_Y);

	if ((mouse_X < position.x + rect_button.w && mouse_X > position.x) && mouse_Y < position.y + rect_button.h && mouse_Y > position.y)
	{
		m_Event = MOUSE_IN;

		LOG("mouse in");
		if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			LOG("right click");
			m_Event = RIGHT_CLICK;
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			LOG("left click");
			m_Event = LEFT_CLICK;
		}
	}
	else
	{
		m_Event = MOUSE_OUT;
	}
	
	return m_Event;
}