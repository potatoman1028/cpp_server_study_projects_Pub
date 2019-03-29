#pragma once

class CService
{
public:
	CService(VOID);
	virtual ~CService(VOID);

private:
	SERVICE_STATUS			mServiceStatus;
	SERVICE_STATUS_HANDLE	mServiceStatusHandle;

	TCHAR					mServiceName[MAX_PATH];

protected:
	virtual VOID			OnStarted(VOID)		= 0;
	virtual VOID			OnStopped(VOID)		= 0;

public:
	BOOL					Install(LPCTSTR serviceName);
	BOOL					Uninstall(LPCTSTR serviceName);

	BOOL					Begin(LPTSTR serviceName);
	BOOL					End(VOID);

	VOID					RunCallback(DWORD argumentCount, LPTSTR *arguments);
	VOID					CtrlHandlerCallback(DWORD opCode);
};
