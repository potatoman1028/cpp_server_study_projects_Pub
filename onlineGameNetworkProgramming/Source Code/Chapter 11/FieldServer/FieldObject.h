#ifndef  __FieldObject_H
#define  __FieldObject_H

#include <cpp/lang/String.h>
#include <cpp/util/List.h>

/*
32 Bits					0    1    2    3    4    5    6    7
						0000 0000 0000 0000 0000 0000 0000 0000
Event Type(128)			---- ---
Field ID(32)		            - ----
Class Type(64)			               ---- --
Extends(16384)			                      -- ---- ---- ----
*/

enum EventType
{
	EventType_None				= 0x00,			//  0000 0000
};

enum ClassType
{
	//  EventType_None
	ClassType_PC				= 0x01,			//  0000 0001
	ClassType_NPC				= 0x02,			//  0000 0010
	ClassType_Monster			= 0x04,			//  0000 0100
	ClassType_Character			= 0x07,			//  0000 0111

	ClassType_Item				= 0x08,			//  0000 1000
};

class FieldObject : public Object
{
	friend class Field;

public:
			FieldObject( Field* field );
	virtual ~FieldObject();

	virtual void			onCreate();
	virtual	void			onDestroy();

	virtual	void			createObjectID( EventType et, ClassType ct, DWORD extends );
	virtual	bool			process();

	static	ClassType		getObjectClassType( DWORD id );

			EventType		getObjectEventType();
			int				getObjectFieldServerID();
			ClassType		getObjectClassType();

			DWORD			getObjectID(){ return objectID; }
			String			getName(){ return name; }

			POINT			getPosition(){ return pos; }
			void			setPosition( POINT pt );
			void			setPosition( int x, int y );

protected:
			String			name;
			Field*			parentField;

private:
			DWORD			objectID;
			POINT			pos;
};

#endif