#include "stdafx.h"
#include "Service.h"

CService *g_pOwner = NULL;

VOID WINAPI RunCallback(DWORD dwArgCnt, LPWSTR *lpwpArg)
{
	g_pOwner->RunCallback(dwArgCnt, lpwpArg);
}

VOID WINAPI CtrlHandlerCallback(DWORD dwOpCode)
{
	g_pOwner->CtrlHandlerCallback(dwOpCode);
}

CService::CService(VOID)
{
	ZeroMemory(m_wszSvcName, sizeof(m_wszSvcName));

	g_pOwner = this;
}

CService::~CService(VOID)
{
}

VOID CService::RunCallback(DWORD dwArgCnt, LPWSTR *lpwpArg)
{
	DWORD dwStatus;

	m_oSvcStatus.dwServiceType = SERVICE_WIN32;
	m_oSvcStatus.dwCurrentState = SERVICE_START_PENDING;
	m_oSvcStatus.dwControlsAccepted	= SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;
	m_oSvcStatus.dwWin32ExitCode = 0;
	m_oSvcStatus.dwServiceSpecificExitCode = 0;
	m_oSvcStatus.dwCheckPoint = 0;
	m_oSvcStatus.dwWaitHint	= 0;

	m_hSvcStatus = RegisterServiceCtrlHandler(m_wszSvcName, ::CtrlHandlerCallback);

	if (m_hSvcStatus == (SERVICE_STATUS_HANDLE) 0)
		return;

	dwStatus = NO_ERROR;

	m_oSvcStatus.dwCurrentState	= SERVICE_RUNNING;
	m_oSvcStatus.dwCheckPoint = 0;
	m_oSvcStatus.dwWaitHint = 0;

	if (!SetServiceStatus(m_hSvcStatus, &m_oSvcStatus))
		return;
}

VOID CService::CtrlHandlerCallback(DWORD dwOpCode)
{
	switch (dwOpCode)
	{
	case SERVICE_CONTROL_PAUSE:
		m_oSvcStatus.dwCurrentState = SERVICE_PAUSED;
		break;
	case SERVICE_CONTROL_CONTINUE:
		m_oSvcStatus.dwCurrentState = SERVICE_RUNNING;
		break;
	case SERVICE_CONTROL_STOP:
		OnStopped();
		m_oSvcStatus.dwWin32ExitCode	= 0;
		m_oSvcStatus.dwCurrentState	= SERVICE_STOPPED;
		m_oSvcStatus.dwCheckPoint		= 0;
		m_oSvcStatus.dwWaitHint		= 0;
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	}

	if (!SetServiceStatus(m_hSvcStatus, &m_oSvcStatus))
		return;
}

BOOL CService::Install(LPCWSTR lpcwSvcName)
{
	WCHAR wszSvcFileName[MAX_PATH] = {0,};
	SC_HANDLE hSvcCtrlMgr	= NULL;
	SC_HANDLE hSvc = NULL;

	if (!lpcwSvcName)
		return FALSE;

	wszSvcFileName[0]		= '"';
	GetModuleFileName(NULL, wszSvcFileName + 1, MAX_PATH - 1);
	wcscat(wszSvcFileName, L"\"");

	hSvcCtrlMgr	= OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if (!hSvcCtrlMgr)
		return FALSE;

	hSvc			= CreateService(hSvcCtrlMgr,
		lpcwSvcName,
		lpcwSvcName,
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL,
		wszSvcFileName,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL);

	if (!hSvc)
	{
		CloseServiceHandle(hSvcCtrlMgr);
		return FALSE;
	}

	CloseServiceHandle(hSvc);
	CloseServiceHandle(hSvcCtrlMgr);

	return TRUE;
}

BOOL CService::Uninstall(LPCWSTR lpcwSvcName)
{
	SC_HANDLE hSvcCtrlMgr = NULL;
	SC_HANDLE hSvc = NULL;

	if (!lpcwSvcName)
		return FALSE;

	hSvcCtrlMgr	= OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if (!hSvcCtrlMgr)
		return FALSE;

	hSvc = OpenService(hSvcCtrlMgr, lpcwSvcName, DELETE);
	if (!hSvc)
	{
		CloseServiceHandle(hSvcCtrlMgr);
		return FALSE;
	}

	if (!DeleteService(hSvc))
	{
		CloseServiceHandle(hSvc);
		CloseServiceHandle(hSvcCtrlMgr);

		return FALSE;
	}

	CloseServiceHandle(hSvc);
	CloseServiceHandle(hSvcCtrlMgr);

	return TRUE;
}

BOOL CService::Begin(LPWSTR lpwSvcName)
{
	if (!lpwSvcName)
		return FALSE;

	SERVICE_TABLE_ENTRY DispatchTable[] = 
	{
		{lpwSvcName,	::RunCallback},
		{NULL,			NULL}
	};

	wcscpy(m_wszSvcName, lpwSvcName);

	OnStarted();

	if (!StartServiceCtrlDispatcher(DispatchTable))
	{
		wprintf(L"## Debug console mode ##\n");
		getchar();
	}

	return TRUE;
}

BOOL CService::End(VOID)
{
	OnStopped();

	return TRUE;
}