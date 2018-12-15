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

void j1Gui::ElementPriority(int x, int y)
{
	
	//for (p2List_item<UI*>* ui_item = UI_elements.start; ui_item != NULL; ui_item = ui_item->next)
	//{
	//	UI* current_elem = ui_item->data;
	//	int children_num = current_elem->children.Count();
	//	if (current_elem->PriorityBox(x, y))
	//	{
	//		if (children_num > 0)
	//		{
	//			for (int i = 0; i < children_num; i++)
	//			{
	//				if (current_elem->children[i]->PriorityBox(x,y))
	//				{
	//					if (current_elem->children[i]->interactable) //potser ha de ser una funció
	//					{
	//						current_interac_elem = current_elem->children[i];
	//					}

	//					//if (App->in)
	//					//{

	//					//}
	//				}
	//			}
	//		}
	//	}
	//}
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

UI::UI(iPoint position, UI_TYPE type, SDL_Rect rect, UI* parent) : screen_pos(position), UI_type(type), sprite_rect(rect)
{}
bool UI::Update()//aqui
{
	int x, y;
	App->input->GetMousePosition(x, y);
	if (dragUIelem)
	{
		if (parent != nullptr)
		{
			local_pos = { x - parent->screen_pos.x, y - parent->screen_pos.y };
		
		}
		else local_pos = { x,y };
	}

	if (parent != nullptr)
	{
		screen_pos = { parent->GetScreenPos().x + local_pos.x, parent->GetScreenPos().y + local_pos.y };
	}
	else
	{
		screen_pos = local_pos;
	}

	local_rect.x = local_pos.x;
	local_rect.y = local_pos.y;

	screen_rect.x = screen_pos.x;
	screen_rect.y = screen_pos.y;
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
		SDL_QueryTexture(label_tex, NULL, NULL, &sprite_rect.w, &sprite_rect.h);
		App->render->Blit(label_tex, this->screen_pos.x, this->screen_pos.y, &sprite_rect, 0.0f);
	
	//App->render->Blit(App->font->Print(this->string, { 255,255,255,255 }, this->txt_font), this->position.x, this->position.y, 0, 0.0f);
	return true;
}
void Label::CleanUp()
{
	label_tex = nullptr;
	delete &sprite_rect;
	
}
Label::~Label(){}

Image::Image(iPoint position, UI_TYPE type, SDL_Rect rect, SDL_Texture* sprite,UI* parent) :UI(position, type,rect,parent),  sprite(sprite)
{
	local_rect = { position.x, position.y,rect.w,rect.h };
	screen_rect = { screen_pos.x, screen_pos.x,screen_rect.w,screen_rect.h };
	this->sprite_rect = rect;
}
bool Image::Update()
{
	
	
	App->render->Blit(this->sprite, this->screen_pos.x, this->screen_pos.y, &this->sprite_rect,0.0f);
	return true;
}
Image::~Image(){}

Button::Button(iPoint position, UI_TYPE type, SDL_Rect rect,SDL_Texture* sprite, UI* parent) : UI(position, type,rect,parent),  sprite(sprite) 
{
	local_rect = { position.x,position.y, rect.w,rect.h };
	local_pos = { position.x,position.y };
	this->sprite_rect = rect;
}
bool Button::Update()
{
	
	App->render->Blit(this->sprite,this->local_pos.x, this->local_pos.y, &this->sprite_rect,0.0f);
	CheckMouse(this->sprite_rect, this->screen_pos);
	return true;
}
Button::~Button() {}


UI*j1Gui::CreateLabel(iPoint position, UI_TYPE type,/* _TTF_Font* txt_font,*/ char* string, UI* parent)
{
	UI* text_element = nullptr;
	//txt_font = App->font->Load(font_path.GetString());

	text_element = new Label(position, type,/* txt_font,*/ string,parent);
	UI_elements.add(text_element);
	LOG("text created");

	return text_element;

}


UI*j1Gui::CreateImage(iPoint position, UI_TYPE type, SDL_Rect rect, SDL_Texture* sprite, UI* parent)
{
	UI* image_element = nullptr;

	image_element = new Image(position, type, rect, sprite, parent);
	UI_elements.add(image_element);
	

	return image_element;
}

UI*j1Gui::CreateButton(iPoint position, UI_TYPE type, SDL_Rect rect, SDL_Texture* sprite, UI* parent )
{
	UI* button_element = nullptr;
	button_element = new Button(position, type, rect,sprite,parent);
	LOG("button created");
	UI_elements.add(button_element);
	return button_element;
}

EVENT UI::CheckMouse(const SDL_Rect rect_button, const iPoint position)
{
	m_Event = NO_EVENT;
	LOG("rect.x: %i", rect_button.x);
	LOG("rect.y: %i", rect_button.y);
	LOG("rect.w: %i", rect_button.w);
	LOG("rect.h: %i", rect_button.h);

	
	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);
	uint right_click, left_click;
	right_click = App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT);
	left_click = App->input->GetMouseButtonDown(SDL_BUTTON_LEFT);

	if ((mouse_pos.x < position.x + rect_button.w && mouse_pos.x > position.x) && mouse_pos.y < position.y + rect_button.h && mouse_pos.y > position.y)
	{
		m_Event = MOUSE_IN;

		LOG("mouse in %i", right_click);
		if (right_click == KEY_REPEAT)
		{
			m_Event = RIGHT_CLICK;
			LOG("right click");
			clicked = true;
			
			
		}
		else if (left_click == KEY_REPEAT)
		{
			LOG("left click");
			m_Event = LEFT_CLICK;
			clicked = true;
		}

		if (!RIGHT_CLICK && !LEFT_CLICK)
		{
			LOG("released");
			//m_Event = RELEASE_CLICK;
			clicked = false;
		}
		
		/*else if ((right_click || left_click) == KEY_UP)
		{

			m_Event = RELEASE_CLICK;
		}*/
		if (interactable == true && m_Event == (LEFT_CLICK || RIGHT_CLICK) && clicked == true)
		{
			dragUIelem = true;
			DragUI();
		}
		
	}
	else if (!RIGHT_CLICK && !LEFT_CLICK)
	{
		dragUIelem = false;
	}
	else
	{
		m_Event = MOUSE_OUT;
	}
	
	return m_Event;
}

bool UI::DragUI()
{
	if (dragUIelem)
	{
		if (local_pos != mouse_pos)
		{
			iPoint spaceVec = { mouse_pos.x - local_pos.x, mouse_pos.y - local_pos.y };
			int normal = sqrt(abs(spaceVec.x) + abs(spaceVec.y));
			iPoint approach = { spaceVec.x / normal, spaceVec.y / normal };
			local_pos += approach;
		}

		else if (local_pos == mouse_pos)
		{
			dragUIelem = false;
		}
	}

	return true;
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
	return screen_rect;
}

SDL_Rect UI::GetLocalRect()
 {
	return local_rect;
}

bool UI::PriorityBox(int x, int y)
{
	
	
	return (x > screen_rect.x && x < screen_rect.x + screen_rect.w && y > screen_rect.y && y < screen_rect.y + screen_rect.h);
}