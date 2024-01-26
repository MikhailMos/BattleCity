#include "AIComponent.h"

#include "GameObjects/Tank.h"

AIComponent::AIComponent(Tank* pParentTank)
	: pParentTank_(pParentTank)
{
}

void AIComponent::Update(const double delta)
{
	pParentTank_->Fire();
}
