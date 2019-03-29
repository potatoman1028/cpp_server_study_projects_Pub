#ifndef  __FieldObjectManager_H
#define  __FieldObjectManager_H

#pragma warning (disable: 4786) // for set (microsoft compilers only)

#include <cpp/net/Packet.h>
#include <cpp/util/List.h>

#include "FieldObject.h"

class FieldObjectManager : public Object
{
public:
			FieldObjectManager();
	virtual	~FieldObjectManager();

			bool			addObject( FieldObject* object );
			void			deleteObject( FieldObject* object );
			FieldObject*	getObject( DWORD objID );

			void			broadcast( cpp::net::Packet& packet );

			void			processObjects();
			void			destroyObject( FieldObject* obj );

private:
			cpp::util::List			listObject;
			cpp::util::List			listPCObject;
};

#endif