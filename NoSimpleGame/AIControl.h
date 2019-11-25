#pragma once
#include "Entity.h"

class AIControl
{
public:
	void default_update(Entity* _e, float _d);
	virtual void update(Entity* _e, float _d);

	AIControl();
	~AIControl();
};