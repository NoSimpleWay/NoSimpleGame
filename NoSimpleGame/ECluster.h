#pragma once
#include <vector>
#include "Entity.h"

class ECluster
{
public:
	std::vector<Entity*> entity_list;

	ECluster();
	~ECluster();
};