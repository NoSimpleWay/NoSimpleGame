#pragma once
#include "AIControl.h"

class AIControlPlayer : public AIControl
{
public:
	virtual void acceleration_control(Entity* _e, float _d);

	AIControlPlayer();
	~AIControlPlayer();
};