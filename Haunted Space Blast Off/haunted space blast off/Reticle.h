//Reticle.h
//author jslink

#include "Object.h"

#define RETICLE_CHAR '+'

class Reticle : public df::Object{

public:
	Reticle();
	void draw(void);
	int eventHandler(const df::Event *p_e);
};