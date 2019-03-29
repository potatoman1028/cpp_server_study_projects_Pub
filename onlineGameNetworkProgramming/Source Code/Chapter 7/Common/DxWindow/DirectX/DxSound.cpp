#include <AfxWin.h>

#include <Common/DxWindow/stdafx.h>
#include <Common/DxWindow/DirectX/DxSound.h>

DxSound::DxSound()
	: directSound( 0 ), ds3dListener( 0 ), handleWindow( 0 ), handleInstance( 0 )
{
}

DxSound::~DxSound()
{
	release();
}

HRESULT DxSound::initialize( HWND hWnd )
{
	HRESULT hResult;

	handleWindow   = hWnd;
	handleInstance = ::AfxGetInstanceHandle();		//  AfxGetApp()->m_hInstance

	{
		hResult = ::DirectSoundCreate( 0, &directSound, 0 );
		if( FAILED( hResult ) )
			return hResult;

		hResult = directSound->SetCooperativeLevel( handleWindow, DSSCL_PRIORITY );
		if( FAILED( hResult ) )
			return hResult;
	}

	initSoundBuffer();
	setListener();

	return DS_OK;
}

void DxSound::initSoundBuffer()
{
	int i;

	for( i = 0 ; i < MAX_SOUNDBUFFER ; i++ )
	{
		pcmSound[i].buffer		= 0;
		pcmSound[i].playStatus	= PS_None;
		pcmSound[i].playMode	= PM_IgnoreSound;
	}
}

void DxSound::release()
{
	if( directSound != 0 )
	{
		directSound->Release();
		directSound = 0;
	}
}

void DxSound::setListener()
{
	LPDIRECTSOUNDBUFFER primary;
	DSBUFFERDESC dsdc;

	::ZeroMemory( &dsdc, sizeof( dsdc ) );
	dsdc.dwSize  = sizeof( dsdc );
	dsdc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;

	if( FAILED( directSound->CreateSoundBuffer( &dsdc, &primary, 0 ) ) )
		return;

	if( FAILED( primary->QueryInterface( IID_IDirectSound3DListener, ( void** )&ds3dListener ) ) )
		return;
}

bool DxSound::loadWav( int index, LPCTSTR fileName )
{
	if( fileName == 0 )
		return false;

	pcmSound[index].buffer		= loadWavFile( fileName );
	pcmSound[index].playStatus	= PS_Loaded;
	pcmSound[index].playMode	= PM_ReplaySound;

	return true;
}

LPDIRECTSOUNDBUFFER DxSound::loadWavFile( LPCTSTR fileName, DWORD flag )
{
	LPDIRECTSOUNDBUFFER lpDSTemp;
	int ret;
	WORD wExtraAlloc;
	UINT i, DataIn;
	HMMIO hmmioIn;
	MMIOINFO mmioinfoIn;
	MMCKINFO ckInfo, ckInfoRIFF;
	LPWAVEFORMATEX lpWaveFormat=0;
	PCMWAVEFORMAT pcmWaveFormat;
	LPBYTE lpData;
	LPVOID lpWrapPtr;
	DWORD dwBSize, dwWavSize, dwDataSize, dwWrapBSize;
	DSBUFFERDESC dsbd;

	hmmioIn = mmioOpen( ( char* )fileName, 0, MMIO_ALLOCBUF | MMIO_READ );
	if( !hmmioIn )
		return 0;

    ckInfoRIFF.fccType = mmioFOURCC( 'W', 'A', 'V', 'E' );
    if( mmioDescend( hmmioIn, &ckInfoRIFF, 0, MMIO_FINDRIFF ) != 0 )
		return 0;

	ckInfo.ckid = mmioFOURCC( 'f', 'm', 't', ' ' );
	if( mmioDescend( hmmioIn, &ckInfo,&ckInfoRIFF, MMIO_FINDCHUNK ) != 0 )
		return 0;

	ret = mmioRead( hmmioIn, ( HPSTR )&pcmWaveFormat, sizeof( pcmWaveFormat ) );
	if( ret != sizeof( pcmWaveFormat ) )
		return 0;

	if( pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM )
		wExtraAlloc = 0;
	else
	{
		ret = mmioRead( hmmioIn, ( LPSTR )&wExtraAlloc, sizeof( wExtraAlloc ) );
		if( ret != sizeof( wExtraAlloc ) )
			return 0;
	}
	
	lpWaveFormat = ( LPWAVEFORMATEX )malloc( sizeof( WAVEFORMATEX) + wExtraAlloc );
	::memcpy( lpWaveFormat, &pcmWaveFormat, sizeof( pcmWaveFormat ) );
	lpWaveFormat->cbSize = wExtraAlloc;
	
	if( wExtraAlloc != 0 )
	{
		ret = mmioRead( hmmioIn, ( LPSTR )( ( ( BYTE* )&( lpWaveFormat->cbSize ) ) + sizeof( wExtraAlloc ) ), wExtraAlloc );
		if( ret != sizeof( wExtraAlloc ) )
			return 0;
	}

	if( mmioAscend( hmmioIn, &ckInfo, 0 ) != MMSYSERR_NOERROR )
		return 0;
    if( mmioSeek( hmmioIn, ckInfoRIFF.dwDataOffset + sizeof( FOURCC ), SEEK_SET ) == -1 )
		return 0;
	
	MMCKINFO pckIn;

    pckIn.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
    if( mmioDescend( hmmioIn, &pckIn, &ckInfoRIFF, MMIO_FINDCHUNK ) != MMSYSERR_NOERROR )
		return 0;
	
	dwDataSize = pckIn.cksize;
	lpData = 0;


	// create soundbuffer
    ::memset( &dsbd, 0, sizeof( DSBUFFERDESC ) );
	dsbd.dwSize			= sizeof( DSBUFFERDESC );
	dsbd.dwFlags		= DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME | DSBCAPS_MUTE3DATMAXDISTANCE | flag;
	dsbd.dwBufferBytes	= dwDataSize;
	dwWavSize			= lpWaveFormat->cbSize + sizeof( WAVEFORMATEX );
	
	dsbd.lpwfxFormat = ( LPWAVEFORMATEX )malloc( dwWavSize );
	::memcpy( dsbd.lpwfxFormat, lpWaveFormat, dwWavSize );
	free( lpWaveFormat );

	if( directSound->CreateSoundBuffer( &dsbd, &lpDSTemp, 0 ) != DS_OK )
		return 0;

	free( dsbd.lpwfxFormat );

	ret = lpDSTemp->Lock( 0, dwDataSize, ( void** )&lpData, &dwBSize, &lpWrapPtr, &dwWrapBSize, 0 );
	if( ret != DS_OK )
		return 0;

	dwDataSize = dwBSize;	

	if( mmioGetInfo( hmmioIn, &mmioinfoIn, 0 ) != MMSYSERR_NOERROR )
		return 0;
	
	DataIn = dwDataSize;
	if( DataIn > pckIn.cksize )
		DataIn = pckIn.cksize;
	pckIn.cksize -= DataIn;

	for( i = 0 ; i < DataIn ; i++ )
	{
		if( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
		{
			ret = mmioAdvance( hmmioIn, &mmioinfoIn, MMIO_READ );
			if( ret != MMSYSERR_NOERROR )
				return 0;
		}

		*( ( BYTE* )lpData + i ) = *( ( BYTE* )mmioinfoIn.pchNext++ );
	}

	if( mmioSetInfo( hmmioIn, &mmioinfoIn, 0 ) != MMSYSERR_NOERROR )
		return 0;

	lpDSTemp->Unlock( lpData, dwBSize, lpWrapPtr, dwWrapBSize );

	return lpDSTemp;
}

bool DxSound::setVolume( int index,DWORD volume )
{
	if( directSound == 0 )
		return false;

	if( volume > 100 )
		volume = 100;

	if( pcmSound[index].playStatus == PS_Playing ||
		pcmSound[index].playStatus == PS_Loaded )
		pcmSound[index].buffer->SetVolume( -30 * ( 100 - volume ) );

	return true;
}

bool DxSound::playWav( int index, bool loop )
{
	if( pcmSound[index].playMode == PM_ReplaySound )
	{
		if( pcmSound[index].playStatus == PS_None ||
			pcmSound[index].buffer == 0 )
			return false;

		pcmSound[index].buffer->SetCurrentPosition( 0 );
		pcmSound[index].buffer->Play( 0, 0, ( loop == true ? DSBPLAY_LOOPING : 0 ) );
		pcmSound[index].playStatus = PS_Playing;

		return true;
	}

	if( pcmSound[index].playStatus == PS_Loaded || pcmSound[index].playStatus == PS_Stopped ||
		pcmSound[index].playStatus == PS_Pause )
	{
		pcmSound[index].buffer->Play( 0, 0, ( loop == true ? DSBPLAY_LOOPING : 0 ) );
		pcmSound[index].playStatus = PS_Playing;
	}

	return true;
}

void DxSound::stop( int index )
{
	if( ( pcmSound[index].playStatus == PS_Playing || pcmSound[index].playStatus == PS_Loaded ||
		  pcmSound[index].playStatus == PS_Pause ) && pcmSound[index].buffer != 0 )
	{
		pcmSound[index].buffer->SetCurrentPosition( 0 );
		pcmSound[index].buffer->Stop();
		pcmSound[index].playStatus = PS_Stopped;
	}
}

void DxSound::pause( int index )
{
	if( pcmSound[index].playStatus == PS_Playing || pcmSound[index].buffer == 0 )
	{
		pcmSound[index].buffer->Stop();
		pcmSound[index].playStatus = PS_Pause;
	}
}

bool DxSound::isPlaying( int index )
{
	HRESULT hResult;
	DWORD flag = 0;

	if( pcmSound[index].buffer == 0 )
		return false;

	hResult = pcmSound[index].buffer->GetStatus( &flag );
	if( pcmSound[index].playStatus == PS_Playing )
	{
		if( hResult == DSBSTATUS_LOOPING || hResult == DSBSTATUS_PLAYING )
			return true;
	}

	return false;
}