//ItemDrop.h
//author jslink

#ifndef ITEM_DROP_H
#define ITEM_DROP_H

#include "Object.h"
#include "EventCollision.h"

//item drops are powerups that drop when enemies are killed.

class ItemDrop : public df::Object{
private:
	void hit(const df::EventCollision *p_collision_event);

public:
	ItemDrop();
	int eventHandler(const df::Event *p_e);
};

#endif