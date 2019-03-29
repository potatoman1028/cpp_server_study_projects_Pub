#include "stdafx.h"
#include "RunawayAction.h"

RunawayAction::RunawayAction( Character* parent )
	: CharacterState( parent )
{
}

RunawayAction::~RunawayAction()
{
}

void RunawayAction::move()
{
}

void RunawayAction::attack()
{
}

void RunawayAction::process( Character* ch )
{
}
