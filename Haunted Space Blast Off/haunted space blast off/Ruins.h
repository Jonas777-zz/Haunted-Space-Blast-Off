//
// Ruins.h
//
//author jmcgowanmartin

#ifndef RUINS_H
#define RUINS_H

#include "Object.h"

class Ruins : public df::Object{

private:
	int ruins_type; //determines which ruins sprite is drawn

public:
	Ruins();
	//void draw(void);
	int eventHandler(const df::Event *p_e);
	void setRuinsType(int new_ruins_type);
};

#endif