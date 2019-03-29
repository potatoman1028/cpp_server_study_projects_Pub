#include "stdafx.h"
#include "MoveAction.h"
#include "Character.h"

MoveAction::MoveAction( Character* parent )
	: CharacterState( parent )
{
	destPos.x = rand() % 700;
	destPos.y = rand() % 500;
}

MoveAction::~MoveAction()
{
}

void MoveAction::move()
{
}

void MoveAction::attack()
{
}

void MoveAction::process( Character* ch )
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

	rect.SetRect( parentObj->getPosition().x - 5,
				parentObj->getPosition().y - 5,
				parentObj->getPosition().x + 5,
				parentObj->getPosition().y + 5 );

	if( rect.PtInRect( destPos ) == TRUE )
	{
		destPos.x = rand() % 700;
		destPos.y = rand() % 500;

		parentObj->setTransition( Character::EVENT_STOPWALK );

		return;
	}

	parentObj->moveTo( destPos.x, destPos.y );
}