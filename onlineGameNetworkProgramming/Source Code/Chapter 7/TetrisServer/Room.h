#ifndef  __Room_H
#define  __Room_H

#include <cpp/lang/String.h>
#include <cpp/util/List.h>
#include <cpp/net/Packet.h>

using namespace cpp::net;

class User;
class Room : public Object
{
public:
	virtual	bool		enter( User* user ) = 0;
	virtual	void		leave( User* user ) = 0;
	virtual	void		sendUserList( User* user );

			int			getRoomNo(){ return roomNo; }
			String		getTitle(){ return title; }
			void		getUserCount( int* cur, int* max );

			bool		addUser( User* user );
			void		deleteUser( User* user );

			void		broadcast( Packet& packet, User* except = 0, bool ttcp = false );

protected:
			Room( int no, String& t, int lc );
	virtual	~Room();

			int			roomNo;
			String		title;
			int			limiteCount;

			User*		master;
			cpp::util::List		listUser;
};

#endif