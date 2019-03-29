#ifndef  __DxSound_H
#define  __DxSound_H

#include <Windows.h>
#include <MMSystem.h>
#include <DSound.h>

#define  MAX_SOUNDBUFFER		32

class DxSound
{
public:
	enum PlayMode
	{
		PM_ReplaySound = 0,
		PM_IgnoreSound
	};

	enum PlayStatus
	{
		PS_None = 0,
		PS_Loaded,
		PS_Playing,
		PS_Pause,
		PS_Stopped
	};

private:
	typedef struct
	{
		IDirectSoundBuffer*		buffer;
		PlayStatus				playStatus;
		PlayMode				playMode;
	} PcmSound;

public:
			DxSound();
	virtual	~DxSound();

			HRESULT		initialize( HWND hWnd );
			void		release();

			bool		loadWav( int index, LPCTSTR fileName );

			bool		playWav( int index, bool loop = true );
			void		stop( int index );
			void		pause( int index );

			bool		isPlaying( int index );
			PlayStatus	getStatus( int index ){ return pcmSound[index].playStatus; }

			bool		setVolume( int index, DWORD volume );

private:
			void		initSoundBuffer();

			void		setListener();
			LPDIRECTSOUNDBUFFER		loadWavFile( LPCTSTR fileName, DWORD flag = PM_ReplaySound );



			PcmSound					pcmSound[MAX_SOUNDBUFFER];

			IDirectSound*				directSound;
			IDirectSound3DListener*		ds3dListener;

			HWND						handleWindow;
			HINSTANCE					handleInstance;
};

#endif
