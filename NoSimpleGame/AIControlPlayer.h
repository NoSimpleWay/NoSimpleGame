#pragma once
#include "AIControl.h"

class AIControlPlayer : public AIControl
{
public:
	virtual void update(Entity* _e, float _d);

	AIControlPlayer();
	~AIControlPlayer();
};