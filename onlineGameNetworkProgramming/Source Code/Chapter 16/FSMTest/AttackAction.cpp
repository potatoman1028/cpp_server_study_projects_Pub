#include "stdafx.h"
#include "AttackAction.h"
#include "Character.h"

AttackAction::AttackAction( Character* parent )
	: CharacterState( parent )
{
}

AttackAction::~AttackAction()
{
}

void AttackAction::move()
{
}

void AttackAction::attack()
{
}

void AttackAction::process( Character* ch )
{
	CRect rect;

	rect.SetRect( parentObj->getPosition().x - parentObj->getRadius() * 7,
				parentObj->getPosition().y - parentObj->getRadius() * 7,
				parentObj->getPosition().x + parentObj->getRadius() * 7,
				parentObj->getPosition().y + parentObj->getRadius() * 7 );

	if( rect.PtInRect( ch->getPosition() ) == FALSE )
	{
		parentObj->setTransition( Character::EVENT_LOSTTARGET );

		return;
	}

	parentObj->moveTo( ch->getPosition().x, ch->getPosition().y );
}