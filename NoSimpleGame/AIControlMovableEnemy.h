#pragma once
#include "AIControl.h"

class AIControlMovableEnemy: public AIControl
{
public:
	virtual void acceleration_control(Entity* _e, float _d);

	AIControlMovableEnemy();
	~AIControlMovableEnemy();
};