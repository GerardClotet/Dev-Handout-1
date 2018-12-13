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
	font_path = "fonts/open_sans/OpenSans-Semibold.ttf";
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
bool j1Gui::CleanUp() // esta creant elements tot el rato per aixo hi ha memory leak
{
	LOG("Freeing GUI");
	
	for (uint i = 0; i < UI_elements.count(); ++i)
	{
		LOG("UI elements %i", UI_elements);
		delete UI_elements[i];
		UI_elements[i] = nullptr;
		LOG("Element deleted");
		
	}
	App->tex->UnLoad(atlas);
	App->font->CleanUp();
	 
	
	/*for (p2List_item<UI*>* item = UI_elements.start; item != nullptr; item = item->next) {
		RELEASE(item->data);
	} */
	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------
UI::UI(iPoint position, UI_TYPE type, UI* parent) :screen_pos(position), UI_type(type),parent(parent)
{
	UI_type = type;
	local_pos = position;

	if (parent != nullptr)
	{
		this->parent = parent;
		parent->children.PushBack(this);
		screen_pos = { parent->screen_pos.x + local_pos.x, parent->screen_pos.y + local_pos.y };
	}

	else screen_pos = position;
}
bool UI::Update()
{
	
	return true;
}
UI::~UI(){}

//Image::Image(iPoint position, UI_TYPE type, SDL_Rect rect,UI* parent) : UI(position, type,parent), rect(rect) {}
//bool Image::Update()
//{
//	LOG("image updated");
//	App->render->Blit((SDL_Texture*)(App->gui->GetAtlas()), this->screen_pos.x, this->screen_pos.y, &this->rect);
//	return true;
//}
//Image::~Image(){}


Label::Label(iPoint position, UI_TYPE type, /* _TTF_Font* font*/ char* string,UI* parent) :UI(position, type,parent),/* txt_font(font),*/ string(string) 
{
}
bool Label::Update()
{
	
		SDL_DestroyTexture(label_tex);
		label_tex = App->font->Print(text.c_str());
		SDL_QueryTexture(label_tex, NULL, NULL, &label_rect.w, &label_rect.h);
		App->render->Blit(label_tex, this->screen_pos.x, this->screen_pos.y, &label_rect, 0.0f);
	
	//App->render->Blit(App->font->Print(this->string, { 255,255,255,255 }, this->txt_font), this->position.x, this->position.y, 0, 0.0f);
	return true;
}
void Label::CleanUp()
{
	label_tex = nullptr;
	delete &label_rect;
	
}
Label::~Label(){}

Image::Image(iPoint position, UI_TYPE type, SDL_Rect rect, SDL_Texture* sprite,UI* parent) :UI(position, type,parent), changerect(rect), sprite(sprite){}
bool Image::Update()
{
	
	
	App->render->Blit(this->sprite, this->screen_pos.x, this->screen_pos.y, &this->changerect,0.0f);
	return true;
}
Image::~Image(){}

Button::Button(iPoint position, UI_TYPE type, SDL_Rect rect,SDL_Texture* sprite, UI* parent) : UI(position, type,parent), button_rect(rect), sprite(sprite) {}
bool Button::Update()
{
	App->render->Blit(this->sprite,this->screen_pos.x, this->screen_pos.y, &this->button_rect,0.0f);
	CheckMouse(this->button_rect, this->screen_pos);
	return true;
}
Button::~Button() {}

//UI*j1Gui::CreateImage(iPoint position, UI_TYPE type, SDL_Rect rect, UI* parent)
//
//{
//	UI* image_element = nullptr;
//	image_element = new Image(position, type, rect, parent);
//	UI_elements.add(image_element);
//	return image_element;
//}

UI*j1Gui::CreateLabel(iPoint position, UI_TYPE type,/* _TTF_Font* txt_font,*/ char* string, UI* parent)
{
	UI* text_element = nullptr;
	//txt_font = App->font->Load(font_path.GetString());

	text_element = new Label(position, type,/* txt_font,*/ string,parent);
	UI_elements.add(text_element);
	LOG("text created");

	return text_element;

}


UI*j1Gui::CreateBackground(iPoint position, UI_TYPE type, SDL_Rect rect, SDL_Texture* sprite, UI* parent)
{
	UI* background_element = nullptr;

	background_element = new Image(position, type, rect, sprite, parent);
	UI_elements.add(background_element);
	//LOG("background created");

	return background_element;
}

UI*j1Gui::CreateButton(iPoint position, UI_TYPE type, SDL_Rect rect, SDL_Texture* sprite, UI* parent )
{
	UI* button_element = nullptr;
	button_element = new Button(position, type, rect,sprite,parent);
	LOG("button created");
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

void UI::SetScreenPos(iPoint position)
{
	screen_pos = position;
}
iPoint UI::GetScreenPos()
{
	return screen_pos;
}

void UI::SetLocalPos(iPoint position)
{
	local_pos = position;
}
iPoint UI::GetLocalPos()
{
	return local_pos;
}

SDL_Rect UI::GetScreenRect()
{
	return global_rect;
}

SDL_Rect UI::GetLocalRect()
 {
	return local_rect;
}

bool UI::PriorityBox(int x, int y)
{
	if (x > global_rect.x && x < global_rect.x + global_rect.w && y > global_rect.y && y < global_rect.y + global_rect.h)
	{
		parentboxCond = true;
	}
	else parentboxCond = false;
	return parentboxCond;
}