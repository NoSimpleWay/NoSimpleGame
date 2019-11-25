#pragma once
#include "AIControl.h"

class AIControlMovableEnemy: public AIControl
{
public:
	virtual void update(Entity* _e, float _d);

	AIControlMovableEnemy();
	~AIControlMovableEnemy();
};