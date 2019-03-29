#include "stdafx.h"
#include "GameLoader.h"
#include "GameLoaderDlg.h"

#include <io.h>
#include <direct.h>
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGameLoaderDlg dialog

CGameLoaderDlg::CGameLoaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGameLoaderDlg::IDD, pParent),
	destroyDlg( false ), transBytesCurrent( 0 ), transBytesTotal( 0 ), currentWorkFileNo( 0 ),
	currentFileInfo( 0 ), totalFileCount( 0 ), updateLoader( 0 ),
	loadClient( true ),	receivedPatchInfo( false )
{
	//{{AFX_DATA_INIT(CGameLoaderDlg)
	m_strDestFile = _T("");
	m_strMessage = _T("");
	m_strRemainCount = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
}

void CGameLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGameLoaderDlg)
	DDX_Control(pDX, IDC_TOTALPROGRESS, m_ctrlTotalProgress);
	DDX_Control(pDX, IDC_CURRENTPROGRESS, m_ctrlCurrentProgress);
	DDX_Text(pDX, IDC_FILENAME, m_strDestFile);
	DDX_Text(pDX, IDC_MESSAGE, m_strMessage);
	DDX_Text(pDX, IDC_RECEIVECOUNT, m_strRemainCount);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGameLoaderDlg, CDialog)
	//{{AFX_MSG_MAP(CGameLoaderDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_NCHITTEST()
	ON_MESSAGE(WM_FTPCALLBACK, OnFTPCallback)
	ON_BN_CLICKED(IDC_CANCELPATCH, OnCancelPatch)
	ON_MESSAGE(WM_CONNECTUPDATESERVER, OnConnectUpdateServer)
	ON_MESSAGE(WM_CONNECTFTPSERVER, OnConnectFTPServer)
	ON_MESSAGE(WM_CLIENTSOCKET_CONNECT, OnSocketConnect)
	ON_MESSAGE(WM_CLIENTSOCKET_CLOSE,   OnSocketClose)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameLoaderDlg message handlers

BOOL CGameLoaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetWindowText( _T( "Game Loader" ) );

	//  버젼 정보를 읽어들인다.
	{
		CFile fileVersion;
		DWORD major = 0, minor = 0;

		if( fileVersion.Open( _T( "Data/Common.dat" ), CFile::modeRead ) == TRUE )
		{
			fileVersion.Read( &major, sizeof( DWORD ) );
			fileVersion.Read( &minor, sizeof( DWORD ) );

			fileVersion.Close();
		}

		clientVersion = MAKELONG( major, minor );
	}

	//  게임 실행 파일의 존재여부를 확인한다.
	{
		CFile fileClient;

		if( fileClient.Open( GAMECLIENTFILE, CFile::modeRead ) == false )
			clientVersion = 0;
		else
			fileClient.Close();
	}
	loaderVersion = LOADERVERSION;


	//  현재 디렉토리를 얻어온다.
	{
		TCHAR buffer[1024];

		::GetCurrentDirectory( 1024, buffer );
		localDir = buffer;
	}


	clientSocket.create( m_hWnd, packetParsing, this );

	m_strMessage = getResourceString( IDS_CONNECTTOUPDATESERVER );
	UpdateData( FALSE );

	PostMessage( WM_CONNECTUPDATESERVER );
	SetTimer( 0, 50, 0 );

	return TRUE;
}

void CGameLoaderDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CGameLoaderDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CGameLoaderDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CGameLoaderDlg::OnTimer(UINT nIDEvent) 
{
	clientSocket.selectEvent();

	CDialog::OnTimer(nIDEvent);
}


void CGameLoaderDlg::OnClose() 
{
	if( destroyDlg == false )
		return;

	KillTimer( 0 );

	if( currentFileInfo != 0 )
	{
		delete currentFileInfo;
		currentFileInfo = 0;
	}

	while( listFile.GetCount() > 0 )
		delete listFile.RemoveHead();
	while( listDownloadFile.GetCount() > 0 )
		delete listDownloadFile.RemoveHead();

	if( loadClient == true )
	{
		//  게임 클라이언트를 실행한다.
		CString				commandLine;
		STARTUPINFO			sui;
		PROCESS_INFORMATION	pi;

		if( updateLoader == true )	
			commandLine = GAMECLIENTFILE;
		else						 	
			commandLine = GAMECLIENTFILE + CString( _T( " " ) ) + GAMECLIENT_ARGUMENT;

		memset( &sui, 0, sizeof( STARTUPINFO ) );
		sui.cb = sizeof( STARTUPINFO );

		::CreateProcess( 0, commandLine.GetBuffer( 0 ), 0, 0, TRUE, 0, 0, localDir, &sui, &pi );
	}

	CDialog::OnClose();
}

UINT CGameLoaderDlg::OnNcHitTest(CPoint point) 
{
	return HTCAPTION;
}

void CGameLoaderDlg::removeDir( LPCTSTR dir )
{
	CFileFind fileFind;
	CFileStatus status;
	BOOL find;

	if( ::SetCurrentDirectory( dir ) == FALSE )
		return;

	find = fileFind.FindFile();
	while( find == TRUE )
	{
		find = fileFind.FindNextFile();

		killFileAttr( fileFind.GetFilePath() );

		if( fileFind.IsDirectory() == TRUE && fileFind.IsDots() == FALSE )
			removeDir( fileFind.GetFilePath() );
		else if( fileFind.IsDots() == FALSE )
			CFile::Remove( fileFind.GetFilePath() );
	}

	fileFind.Close();

	::SetCurrentDirectory( _T( ".." ) );
	::RemoveDirectory( dir );
}

bool CGameLoaderDlg::isEnoughDiskSpace( DWORD requireSpace )
{
	ULARGE_INTEGER caller, totalBytes;

	::GetDiskFreeSpaceEx( localDir, &caller, &totalBytes, 0 );

	if( caller.QuadPart <= requireSpace )
		return false;

	return true;
}

DWORD CGameLoaderDlg::addFileToList( CString fileName, CString localFile )
{
	FTPFileFind fileFind( &ftpClient );
	FILEINFO* info = new FILEINFO;
	CFtpFileFind* file;
	DWORD size;

	splitFullPath( fileName, &info->directory, &info->remoteFile );
	ftpClient.setCurrentDir( _T( "/" ) );
	ftpClient.setCurrentDir( info->directory );

	file = fileFind.find( info->remoteFile );

	info->localFile = localFile;

	if( file != 0 )
	{
		info->fileSize = file->GetLength();
		size = info->fileSize;

		listFile.AddTail( info );
	}
	else
		delete info;

	return size;
}

void CGameLoaderDlg::createFullPath( CString fullPath )
{
	int offset;

	fullPath.TrimRight( _T( "/" ) );

	offset = fullPath.Find( _T( "/" ), 0 );
	while( ( offset = fullPath.Find( _T( "/" ), offset + 1 ) ) >= 0 )
		CreateDirectory( fullPath.Left( offset ), 0 );

	CreateDirectory( fullPath, 0 );
}

bool CGameLoaderDlg::killFileAttr( CString fileName )
{
	CFileStatus status;

	try
	{
		if( CFile::GetStatus( fileName, status ) == TRUE )
		{
			status.m_attribute = CFile::normal;

			CFile::SetStatus( fileName, status );
		}
	}
	catch( CFileException* exception )
	{
		exception->Delete();

		return false;
	}

	return true;
}


void CGameLoaderDlg::OnConnectUpdateServer(WPARAM wParam, LPARAM lParam)
{
	clientSocket.close();
	clientSocket.create( m_hWnd, packetParsing, this );

	clientSocket.connectTo( "127.0.0.1", 11010 );
}


//-----------------------------------------------------------------------------------------------
//-------------------------------------   Network routines  -------------------------------------

void CGameLoaderDlg::packetParsing( Packet& packet, ClientSocket* parent, void* arg )
{
	CGameLoaderDlg* dlg = ( CGameLoaderDlg* )arg;

	switch( packet.id() )
	{
	case  UC_CONNECTIONSUCCESS_ACK :	dlg->onUCConnectionSuccessAck( packet );	break;
	case  UC_CLIENTVERSION_ACK :		dlg->onUCClientVersionAck( packet );		break;
	case  UC_LOADERVERSION_ACK :		dlg->onUCLoaderVersionAck( packet );		break;
	case  UC_UPDATECLIENTLIST_ACK :		dlg->onUCUpdateClientListAck( packet );		break;
	case  UC_UPDATELOADERLIST_ACK :		dlg->onUCUpdateLoaderListAck( packet );		break;
	}
}

int CGameLoaderDlg::sendNetMessage( Packet packet )
{
	return clientSocket.sendPacket( packet );
}

void CGameLoaderDlg::OnSocketConnect( WPARAM wParam, LPARAM lParam )
{
	if( wParam == 0 )
	{
		//  연결 성공
	}
	else
	{
		::AfxMessageBox( getResourceString( IDS_UPDATESERVERCONNECTIONFAILED ) );

		loadClient = false;
		destroyDlg = true;
		PostMessage( WM_CLOSE );
	}
}

void CGameLoaderDlg::OnSocketClose( WPARAM wParam, LPARAM lParam )
{
	if( receivedPatchInfo == false )
	{
		m_strMessage = getResourceString( IDS_CLOSEDFROMUPDATESERVER );
		UpdateData( FALSE );
	}
}

void CGameLoaderDlg::onUCConnectionSuccessAck( Packet& packet )
{
	sendLoaderVersion();
}




//-----------------------------------------------------------------------------------------------
//-------------------------------   Loader version check routines  -------------------------------

void CGameLoaderDlg::sendLoaderVersion()
{
	Packet packet;

	packet.id( UC_LOADERVERSION_REQ ) << loaderVersion;
	sendNetMessage( packet );
}

void CGameLoaderDlg::sendClientVersion()
{
	Packet packet;

	packet.id( UC_CLIENTVERSION_REQ ) << clientVersion;
	sendNetMessage( packet );
}

void CGameLoaderDlg::requestUpdateList()
{
	if( updateLoader == true )		requestUpdateLoaderList();
	else if( updateClient == true )	requestUpdateClientList();
}

void CGameLoaderDlg::requestUpdateLoaderList()
{
	Packet sendPacket;

	sendPacket.id( UC_UPDATELOADERLIST_REQ ) << loaderVersion;
	sendNetMessage( sendPacket );
}

void CGameLoaderDlg::requestUpdateClientList()
{
	Packet sendPacket;

	sendPacket.id( UC_UPDATECLIENTLIST_REQ ) << clientVersion;
	sendNetMessage( sendPacket );
}

void CGameLoaderDlg::onUCLoaderVersionAck( Packet& packet )
{
	packet >> updateLoader;

	if( updateLoader == true )
	{
		packet >> FTPAddress >> FTPAccount >> FTPPasswd;

		PostMessage( WM_CONNECTFTPSERVER );
	}
	else
	{
		sendClientVersion();
	}
}

void CGameLoaderDlg::onUCUpdateLoaderListAck( Packet& packet )
{
	CString fileName, localFile;
	DWORD i, dwTotalSize = 0;

	clientSocket.close();

	packet >> totalFileCount;
	for( i = 0 ; i < totalFileCount ; i++ )
	{
		packet >> fileName >> localFile;

		dwTotalSize += addFileToList( fileName, localFile );
	}

	m_ctrlTotalProgress.SetRange32( 0, dwTotalSize );
	m_strRemainCount.Format( _T( "0 / %2d" ), listFile.GetCount() );

	if( isEnoughDiskSpace( ( dwTotalSize * 3 ) ) == false )
	{
		::AfxMessageBox( getResourceString( IDS_NOTENOUGHDISK ) );

		destroyDlg = true;
		PostMessage( WM_CLOSE );
		return;
	}

	//  다운로드를 시작한다.
	getNextFile();
}

//-----------------------------------------------------------------------------------------------
//-------------------------------   Client version check routines  ------------------------------
void CGameLoaderDlg::onUCClientVersionAck( Packet& packet )
{
	packet >> updateClient;

	if( updateClient == true )
	{
		packet >> FTPAddress >> FTPAccount >> FTPPasswd;

		PostMessage( WM_CONNECTFTPSERVER );
	}
	else
	{
		destroyDlg = true;
		loadClient = true;
		PostMessage( WM_CLOSE );
	}
}

void CGameLoaderDlg::onUCUpdateClientListAck( Packet& packet )
{
	CString fileName, localFile;
	DWORD i;
	static DWORD totalSize = 0, receiveCount = 0;
	int command;

	packet >> command;
	if( command == 0 )		//  수신 받을 정보의 개수
	{
		m_strMessage = getResourceString( IDS_RECEIVEPATCHINFO );

		packet >> totalFileCount;

		m_ctrlCurrentProgress.SetRange32( 0, totalFileCount );
		m_ctrlTotalProgress.SetRange32( 0, totalFileCount );
		m_strRemainCount.Format( _T( "0 / %2d" ), totalFileCount );

		UpdateData( FALSE );

		return;
	}
	if( command == 1 )		//  패치 파일의 정보
	{
		DWORD size;

		packet >> size;
		for( i = 0 ; i < size ; i++ )
		{
			packet >> fileName >> localFile;

			totalSize += addFileToList( fileName, localFile );

			m_ctrlCurrentProgress.SetPos( receiveCount );
			m_ctrlTotalProgress.SetPos( receiveCount );
			m_strRemainCount.Format( _T( "%2d / %2d" ), receiveCount, totalFileCount );
			UpdateData( FALSE );

			receiveCount++;
		}

		return;
	}

	receivedPatchInfo = true;
	m_ctrlCurrentProgress.SetPos( 0 );
	m_ctrlTotalProgress.SetPos( 0 );
	m_ctrlTotalProgress.SetRange32( 0, totalSize );
	m_strRemainCount.Format( _T( "0 / %2d" ), listFile.GetCount() );

	if( isEnoughDiskSpace( ( totalSize * 3 ) ) == false )
	{
		::AfxMessageBox( getResourceString( IDS_NOTENOUGHDISK ) );

		loadClient = false;
		destroyDlg = true;
		PostMessage( WM_CLOSE );

		return;
	}

	//  다운로드를 시작한다.
	getNextFile();
}




//-----------------------------------------------------------------------------------------------
//-------------------------------   Patch file download routines  -------------------------------

void CGameLoaderDlg::splitFullPath( CString fullPath, CString* directory, CString* fileName )
{
	bool bFind = false;
	int nOffset;

	nOffset = fullPath.GetLength() - 1;
	while( nOffset >= 0 )
	{
		if( fullPath.GetAt( nOffset ) == '/' )
		{
			bFind = true;
			break;
		}

		nOffset--;
	}

	if( bFind == true )
	{
		*directory = fullPath.Left( nOffset );
		*fileName  = fullPath.Right( fullPath.GetLength() - nOffset - 1 );
	}
	else
	{
		*directory = "";
		*fileName = fullPath;
	}
}

void CGameLoaderDlg::OnConnectFTPServer( WPARAM wParam, LPARAM lParam )
{
	m_strMessage = getResourceString( IDS_CONNECTTOFTPSERVER );
	UpdateData( FALSE );

	if( connectToFTPServer( FTPAddress, FTPAccount, FTPPasswd ) == true )
	{
	}
	else
	{
		::AfxMessageBox( getResourceString( IDS_FTPCONNECTIONFAILED ) );

		loadClient = false;
		destroyDlg = true;
		PostMessage( WM_CLOSE );
	}
}

bool CGameLoaderDlg::connectToFTPServer( CString address, CString account, CString password )
{
	transBytesCurrent = 0;
	transBytesTotal   = 0;
	currentWorkFileNo = 0;

	GetDlgItem( IDC_CANCELPATCH )->EnableWindow( TRUE );

	ftpClient.close();

	if( ftpClient.connect( address, account, password ) == false )
		return false;

	m_strMessage = getResourceString( IDS_SUCCESSCONNECT );

	UpdateData( FALSE );

	requestUpdateList();

	return TRUE;
}

bool CGameLoaderDlg::getNextFile()
{
	if( currentFileInfo != 0 )
	{
		listDownloadFile.AddTail( currentFileInfo );
		currentFileInfo = 0;
	}

	if( listFile.GetCount() == 0 )
		return false;

	//  패치 목록에서 하나의 정보를 가져온다.
	//  그리고 목록에서는 정보를 제거한다.
	currentFileInfo = listFile.RemoveHead();

	m_strMessage = getResourceString( IDS_DOWNLOADING );
	m_strDestFile = currentFileInfo->remoteFile;
	m_strRemainCount.Format( _T( "%2d / %2d" ), ++currentWorkFileNo, totalFileCount );
	UpdateData( FALSE );

	m_ctrlCurrentProgress.SetRange32( 0, currentFileInfo->fileSize );
	m_ctrlCurrentProgress.SetPos( 0 );
	transBytesCurrent = 0;


	//  다운로드 받을 파일이 있는 디렉토리로 이동한다.
	ftpClient.setCurrentDir( _T( "/" ) );
	ftpClient.setCurrentDir( currentFileInfo->directory );

	CString path, fileName;

	//  패치 정보를 디렉토리와 파일 명으로 분류한다.
	splitFullPath( currentFileInfo->localFile, &path, &fileName );

	if( path.GetLength() != 0 )
		path = localDir + _T( "/" ) + path;
	else
		path = localDir;

	createFullPath( path );


	//  만일 로컬에 동일한 파일이 존재한다면 Read Only 등 파일 속성을
	//  모두 제거한 후 다운로드 작업을 수행한다.
	killFileAttr( ( localDir + currentFileInfo->localFile ) );
	if( ftpClient.getFile( m_strDestFile, ( path + _T( "/" ) + fileName ), m_hWnd ) == false )
	{
		::AfxMessageBox( getResourceString( IDS_FTPGETFILEFAILED ) );

		loadClient = false;
		destroyDlg = true;
		PostMessage( WM_CLOSE );
	}

	return true;
}

void CGameLoaderDlg::OnFTPCallback( WPARAM wParam, LPARAM lParam )
{
	DWORD dwTransBytes = lParam;

	switch( wParam )
	{
	case  FTPClient::CALLBACK_LOCALFILEOPENERROR :
		//  로컬 디렉토리에 파일을 생성할 수 없다.
		break;

	case  FTPClient::CALLBACK_TRANSFERRED:
		//  현재 다운로드 작업이 진행중이다.
		{
			transBytesCurrent += lParam;
			transBytesTotal   += lParam;

			m_ctrlCurrentProgress.SetPos( transBytesCurrent );
			m_ctrlTotalProgress  .SetPos( transBytesTotal );
		}
		break;

	case  FTPClient::CALLBACK_CANCEL:
		//  사용자의 요청에 의해 작업이 취소되었다.
		destroyDlg = true;
		PostMessage( WM_CLOSE );
		break;

	case  FTPClient::CALLBACK_COMPLETE:
		//  파일 하나의 다운로드 작업이 완료되었다.
		if( getNextFile() == FALSE )
		{
			loadClient = true;
			destroyDlg = true;
			PostMessage( WM_CLOSE );
		}
		break;
	}
}

void CGameLoaderDlg::OnCancelPatch() 
{
	if( ::AfxMessageBox( getResourceString( IDS_CANCEL ), MB_YESNO ) == IDYES )
	{
		loadClient = false;
		destroyDlg = true;
		PostMessage( WM_CLOSE );
	}
}
