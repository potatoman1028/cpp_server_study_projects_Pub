#ifndef  __Field_H
#define  __Field_H

#pragma warning (disable: 4786) // for set (microsoft compilers only)

#include "FieldObjectManager.h"

class Field : public Object
{
public:
			Field( DWORD id );
	virtual ~Field();

			DWORD			getFieldID(){ return fieldID; }

			void			enter( FieldObject* obj );
			void			leave( FieldObject* obj );

			void			processField();
			void			broadcast( cpp::net::Packet& packet );

private:
			FieldObjectManager		manager;
			DWORD					fieldID;
};

#endif