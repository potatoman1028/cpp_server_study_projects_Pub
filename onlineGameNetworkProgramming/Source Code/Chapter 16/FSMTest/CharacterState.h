#ifndef  __CharacterState_H
#define  __CharacterState_H

class Character;

class CharacterState
{
public:
			CharacterState( Character* parent );
	virtual ~CharacterState();

	virtual	void		move() = 0;
	virtual	void		attack() = 0;
	virtual	void		process( Character* ch ) = 0;

protected:
			Character*		parentObj;
};

#endif