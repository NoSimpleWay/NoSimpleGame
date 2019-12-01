#pragma once
#include "Entity.h"
#include "EWindow.h"

class AIControl
{
public:
	void default_update(Entity* _e, float _d);
	virtual void update(Entity* _e, float _d);

	virtual void acceleration_control(Entity* _e, float _d);



	AIControl();
	~AIControl();
};