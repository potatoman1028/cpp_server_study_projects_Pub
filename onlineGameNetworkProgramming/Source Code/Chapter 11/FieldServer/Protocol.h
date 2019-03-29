#ifndef  __Protocol_H
#define  __Protocol_H

//  Field server - Session server protocol
enum FSProtocol
{
	FSProtocolBase = 100,
							FS_CONNECTIONSUCCESS_ACK,
	FS_CHATMSG_REQ,			FS_CHATMSG_ACK,
	FS_ENTERFIELD_REQ,		FS_ENTERFIELD_ACK,
	FS_LEAVEFIELD_REQ,		FS_LEAVEFIELD_ACK,
	FS_CHARACTERINFO_REQ,	FS_CHARACTERINFO_ACK
};

#endif