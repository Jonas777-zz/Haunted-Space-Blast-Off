//Points.cpp
//author jslink
//author jmcgowanmartin

#include "Points.h"
#include "EventStep.h"
#include "WorldManager.h"

Points::Points(){
	setLocation(df::TOP_RIGHT);
	setViewString(POINTS_STRING);
	setColor(df::WHITE);

	//update score every second, so register step
	registerInterest(df::STEP_EVENT);
}

//event handler
int Points::eventHandler(const df::Event *p_e){
	//parent handles event if score updates
	if (df::ViewObject::eventHandler(p_e)){
		return 1;
	}

	//if step, increment score every second (30 steps)
	if (p_e->getType() == df::STEP_EVENT){
		if (static_cast <const df::EventStep *> (p_e)->getStepCount() % 30 == 0){
			//check if player is alive
			df::ObjectList ol = df::WorldManager::getInstance().getAllObjects();
			df::ObjectListIterator iter(&ol);
			bool found = false;
			while (!iter.isDone()){
				if (iter.currentObject()->getType() == "Player"){
					found = true;
				}
				iter.next();
			}

			//if player died, dont increment score
			if (found){
				setValue(getValue() + 1);
			}
			return 1;
		}
	}
	return 0;
}