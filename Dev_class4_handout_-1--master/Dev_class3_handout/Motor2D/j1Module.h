// ----------------------------------------------------
// j1Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __j1MODULE_H__
#define __j1MODULE_H__

#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"

class j1App;

class j1Module
{
public:

	j1Module() : active(false)
	{}

	void Init()
	{
		active = true;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	// TODO 3: Create new virtual methods to Load and Save
	virtual bool Load(const pugi::xml_node&) { //Els seus membres de dades no es poden modificar, ni directament (encara que s�hagin declarant com a p�blics) ni a trav�s de membres funci� que els pretenguin modificar.
		return true;
	}

	virtual bool Save(pugi::xml_node&)const { //no pugui modificar cap dels membres de dades de la classe ni tampoc a activar un altre membre funci� que no hagi estat declarat com a constant.
		return true;
	}
public:

	p2SString	name;
	bool		active;

};

#endif // __j1MODULE_H__