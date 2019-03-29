#ifndef  __Protocol_H
#define  __Protocol_H

//  Session server - Client protocol
enum SCProtocol
{
	SCPrototocolBase = 0,

							SC_CONNECTIONSUCCESS_ACK,
	SC_VERIFYACCOUNT_REQ,	SC_VERIFYACCOUNT_ACK,
	SC_CHATMSG_REQ,			SC_CHATMSG_ACK
};


//  Field server - Session server protocol
enum FSProtocol
{
	FSProtocolBase = 100,
							FS_CONNECTIONSUCCESS_ACK,
	FS_CHATMSG_REQ,			FS_CHATMSG_ACK
};

#endif