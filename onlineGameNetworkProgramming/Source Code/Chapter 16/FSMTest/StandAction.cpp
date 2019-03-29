#include "stdafx.h"
#include "StandAction.h"
#include "Character.h"

StandAction::StandAction( Character* parent )
	: CharacterState( parent )
{
}

StandAction::~StandAction()
{
}

void StandAction::move()
{
}

void StandAction::attack()
{
}

void StandAction::process( Character* ch )
{
	CRect rect;

	rect.SetRect( parentObj->getPosition().x - parentObj->getRadius() * 7,
				parentObj->getPosition().y - parentObj->getRadius() * 7,
				parentObj->getPosition().x + parentObj->getRadius() * 7,
				parentObj->getPosition().y + parentObj->getRadius() * 7 );

	if( rect.PtInRect( ch->getPosition() ) == TRUE )
	{
		parentObj->setTransition( Character::EVENT_FINDTARGET );

		return;
	}

	if( rand() % 100 < 5 )
	{
		parentObj->setTransition( Character::EVENT_PATROL );
		return;
	}
}
