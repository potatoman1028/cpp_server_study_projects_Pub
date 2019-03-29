#pragma once
#include "stdafx.h"
#include <locale>
#include <iostream>
#include <sstream>
#include <time.h>

class ProgramValidation
{
	void checkSMTP()
	{
		SOCKET smtpSocket;
		if (!connectSMTP(&smtpSocket)) {
			SLog(L"! The smtp server is not loaded.");
			exit(1);
		}
	}

	class ProgramExpire
	{
		tick_t serverBirthTick_;

		void checkExpire()
		{
			tick_t expireTick = serverBirthTick_ + DAY_TO_TICK(30);
			tick_t now = CLOCK.systemTick();
			if (!isInRange(serverBirthTick_, now, expireTick)) {
				sendMail("serverAlert@server.com",
					"serverProgramer@server.com",
					"[EXPIRE] ���� ��ȿ�� üũ ����",
					"IP�ּ�, port ��ȣ��...");
				exit(1);
			}
		}

		void setBirthTick()
		{
			//���ڿ� �ð��� tick_tȭ �ϱ�
			//https://msdn.microsoft.com/en-us/library/ta5wcy3s.aspx
			locale loc;
			basic_stringstream<char> birthDate;
			ios_base::iostate st = 0;
			struct tm t;
			memset(&t, 0, sizeof(struct tm));

			birthDate << __DATE__;		//������ ��¥
			birthDate.imbue(loc);
			basic_istream<char>::_Iter i = use_facet
				<time_get <char> >
				(loc).get_date(basic_istream<char>::_Iter(birthDate.rdbuf()),
				basic_istream<char>::_Iter(0), birthDate, st, &t);

			if (st & ios_base::failbit) {
				cout << "time_get::get_time(" << birthDate.rdbuf()->str() << ") FAILED on char: " << *i << endl;
				ASSERT(FALSE);
			}
			array<WCHAR, SIZE_64> dataTime;
			snwprintf(dataTime, L"%4d-%2d-%2d %d:%d:%d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, 0, 0, 0);
			serverBirthTick_ = CLOCK.strToTick(dataTime.data());
		}

	public:
		ProgramExpire()
		{
			this->setBirthTick();
			this->checkExpire();
		}
	};
	

public:
	ProgramValidation()
	{
		ProgramExpire checkExpire;
		this->checkSMTP();
	}
};

static ProgramValidation programValidation;