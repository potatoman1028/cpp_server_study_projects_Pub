#include "stdafx.h"
#include "EventSelect.h"

DWORD WINAPI SelectThreadCallback(LPVOID parameter)
{
	CEventSelect *Owner = (CEventSelect*) parameter;
	Owner->SelectThreadCallback();

	return 0;
}

VOID CEventSelect::SelectThreadCallback(VOID)
{
	WSANETWORKEVENTS	NetworkEvents;
	DWORD				EventID = 0;
	DWORD				Result	= 0;
	HANDLE				ThreadEvents[2] = {mDestroyEventHandle, mSelectEventHandle};

	while (TRUE)
	{
		SetEvent(mStartupEventHandle);

		EventID = ::WaitForMultipleObjects(2, ThreadEvents, FALSE, INFINITE);
		switch (EventID)
		{
		case WAIT_OBJECT_0:
			return;

		case WAIT_OBJECT_0 + 1:
			EventID = WSAEnumNetworkEvents(mSocket, mSelectEventHandle, &NetworkEvents);

			if (EventID == SOCKET_ERROR)
				return;
			else
			{
				if (NetworkEvents.lNetworkEvents & FD_CONNECT)
				{
					OnIoConnected();

					if (NetworkEvents.iErrorCode[FD_CONNECT_BIT])
						return;
				}
				else if (NetworkEvents.lNetworkEvents & FD_WRITE)
				{
					//OnIoWrote();
				}
				else if (NetworkEvents.lNetworkEvents & FD_READ)
				{
					OnIoRead();
				}
				else if (NetworkEvents.lNetworkEvents & FD_CLOSE)
				{
					OnIoDisconnected();

					return;
				}
			}
			break;
		default:
			return;
		}
	}
}

CEventSelect::CEventSelect(VOID)
{
	mSelectEventHandle	= NULL;
	mDestroyEventHandle	= NULL;
	mSelectThreadHandle	= NULL;
	mStartupEventHandle	= NULL;

	mSocket				= NULL;
}

CEventSelect::~CEventSelect(VOID)
{
}

BOOL CEventSelect::Begin(SOCKET socket)
{
	if (!socket)
		return FALSE;

	if (mSocket)
		return FALSE;

	mSocket = socket;

	mSelectEventHandle = WSACreateEvent();
	if (mSelectEventHandle == WSA_INVALID_EVENT)
	{
		End();

		return FALSE;
	}

	mDestroyEventHandle = CreateEvent(0, FALSE, FALSE, 0);
	if (mDestroyEventHandle == NULL)
	{
		End();

		return FALSE;
	}

	mStartupEventHandle = CreateEvent(0, FALSE, FALSE, 0);
	if (mStartupEventHandle == NULL)
	{
		End();

		return FALSE;
	}

	DWORD Result = WSAEventSelect(socket, mSelectEventHandle, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
	if (Result == SOCKET_ERROR)
	{
		End();

		return FALSE;
	}

	DWORD SelectThreadID	= 0;
	mSelectThreadHandle		= CreateThread(NULL, 0, ::SelectThreadCallback, this, 0, &SelectThreadID);

	if (!mSelectThreadHandle)
	{
		End();

		return FALSE;
	}

	// Thread가 Wait될때까지 시간을 벌어준다.
	WaitForSingleObject(mStartupEventHandle, INFINITE);

	return TRUE;
}

BOOL CEventSelect::End(VOID)
{
	if (!mSocket)
		return FALSE;

	if (mSelectThreadHandle)
	{
		SetEvent(mDestroyEventHandle);

		WaitForSingleObject(mSelectThreadHandle, INFINITE);

		CloseHandle(mSelectThreadHandle);
	}

	if (mSelectEventHandle)
		CloseHandle(mSelectEventHandle);

	if (mDestroyEventHandle)
		CloseHandle(mDestroyEventHandle);

	if (mStartupEventHandle)
		CloseHandle(mStartupEventHandle);

	return TRUE;
}
