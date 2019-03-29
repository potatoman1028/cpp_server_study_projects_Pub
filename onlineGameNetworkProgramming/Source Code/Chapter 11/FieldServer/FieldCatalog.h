#ifndef  __FieldCatalog_H
#define  __FieldCatalog_H

#include <cpp/util/List.h>

#include "Field.h"

class FieldCatalog : public Object
{
public:
	static	FieldCatalog*		getInstance();
	static	void				releaseInstance();

			bool				createField( DWORD fieldID );
			void				processFields();
			Field*				findField( DWORD fieldID );

private:
			FieldCatalog();
	virtual	~FieldCatalog();

	static	FieldCatalog*		selfInstance;

			cpp::util::List		listField;
};

#endif