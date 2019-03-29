#pragma once
#include "stdafx.h"

// �͹̳� ������, Ÿ ������ �����ؼ� ������ �ۼ��ſ� ����Ѵ�.
typedef enum TERMINAL_STATUS {
	TERMINAL_STOP,
	TERMINAL_READY,
};

class Server;
class Terminal
{
protected:
	Server					*server_;
	wstr_t					name_;
	TERMINAL_STATUS			status_;

	char					ip_[16];
	int						port_;

	TerminalSession			session_;

	Thread					*processThread_;

public:
	Terminal(Server *server, wstr_t name);
	virtual ~Terminal();
	TERMINAL_STATUS &status();

	void		initialize(xmlNode_t *config);
	void		sendPacket(Packet *packet);
	const char *ip();
	int			port();

private:
	void		connectProcess();
	void		run();
};
