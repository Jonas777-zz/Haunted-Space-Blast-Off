//Plant.h
//author jslink

#ifndef PLANT_H
#define PLANT_H

#include "Object.h"

#define PLANT_CHAR '~'
#define PLANT_CHAR_2 '^'

class Plant : public df::Object{

private:
	int plant_type; //determines which plant char is drawn

public:
	Plant();
	void draw(void);
	int eventHandler(const df::Event *p_e);
	void setPlantType(int new_plant_type);
};

#endif