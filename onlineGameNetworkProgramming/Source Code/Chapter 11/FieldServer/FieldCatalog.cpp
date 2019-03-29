#include "FieldCatalog.h"

FieldCatalog*		FieldCatalog::selfInstance = 0;

FieldCatalog::FieldCatalog()
{
}

FieldCatalog::~FieldCatalog()
{
	while( listField.size() > 0 )
		delete listField.popHead();
}

FieldCatalog* FieldCatalog::getInstance()
{
	if( selfInstance == 0 )
		selfInstance = new FieldCatalog();

	return selfInstance;
}

void FieldCatalog::releaseInstance()
{
	if( selfInstance == 0 )
		return;

	delete selfInstance;
	selfInstance = 0;
}

bool FieldCatalog::createField( DWORD fieldID )
{
	Field* field;
	cpp::util::List::Position pos;

	pos = listField.headPosition();
	while( listField.isEnd( pos ) == false )
	{
		field = ( Field* )listField.getItem( pos++ );

		if( field->getFieldID() == fieldID )
			return false;
	}

	listField.pushTail( new Field( fieldID ) );

	return true;
}

Field* FieldCatalog::findField( DWORD fieldID )
{
	Field* field;
	cpp::util::List::Position pos;

	pos = listField.headPosition();
	while( listField.isEnd( pos ) == false )
	{
		field = ( Field* )listField.getItem( pos++ );

		if( field->getFieldID() == fieldID )
			return field;
	}

	return 0;
}

void FieldCatalog::processFields()
{
	Field* field;
	cpp::util::List::Position pos;

	pos = listField.headPosition();
	while( listField.isEnd( pos ) == false )
	{
		field = ( Field* )listField.getItem( pos++ );
		field->processField();
	}
}