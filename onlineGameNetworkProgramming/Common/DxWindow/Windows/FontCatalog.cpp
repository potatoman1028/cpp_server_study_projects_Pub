#include <Common/DxWindow/stdafx.h>
#include <Common/DxWindow/Windows/FontCatalog.h>

FontCatalog*		FontCatalog::selfInstance = 0;

HFONT		FontCatalog::handleFont[Font_Count];

FontCatalog::FontCatalog()
{
	handleFont[0] = ::CreateFont( -11, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0,
						DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH, _T( "µ¸¿òÃ¼" ) );
	handleFont[1] = ::CreateFont( -11, 0, 0, 0, FW_NORMAL, TRUE, FALSE, 0,
						DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH, _T( "µ¸¿òÃ¼" ) );
	handleFont[2] = ::CreateFont( -11, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0,
						DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH, _T( "µ¸¿òÃ¼" ) );
	handleFont[3] = ::CreateFont( -11, 0, 0, 0, FW_BOLD, TRUE, FALSE, 0,
						DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH, _T( "µ¸¿òÃ¼" ) );
	handleFont[4] = ::CreateFont( -13, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0,
						DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH, _T( "µ¸¿òÃ¼" ) );
	handleFont[5] = ::CreateFont( -13, 0, 0, 0, FW_NORMAL, TRUE, FALSE, 0,
						DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH, _T( "µ¸¿òÃ¼" ) );
	handleFont[6] = ::CreateFont( -13, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0,
						DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH, _T( "µ¸¿òÃ¼" ) );
	handleFont[7] = ::CreateFont( -13, 0, 0, 0, FW_BOLD, TRUE, FALSE, 0,
						DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH, _T( "µ¸¿òÃ¼" ) );
	handleFont[8] = ::CreateFont( -16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0,
						DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH, _T( "µ¸¿òÃ¼" ) );
	handleFont[9] = ::CreateFont( -16, 0, 0, 0, FW_NORMAL, TRUE, FALSE, 0,
						DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH, _T( "µ¸¿òÃ¼" ) );
	handleFont[10] = ::CreateFont( -16, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0,
						DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH, _T( "µ¸¿òÃ¼" ) );
	handleFont[11] = ::CreateFont( -16, 0, 0, 0, FW_BOLD, TRUE, FALSE, 0,
						DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH, _T( "µ¸¿òÃ¼" ) );
}

FontCatalog::~FontCatalog()
{
	int i;

	for( i = 0 ; i < Font_Count ; i++ )
		::DeleteObject( handleFont[i] );
}

FontCatalog* FontCatalog::getInstance()
{
	if( selfInstance == 0 )
		selfInstance = new FontCatalog();

	return selfInstance;
}

void FontCatalog::releaseInstance()
{
	if( selfInstance == 0 )
		return;

	delete selfInstance;
	selfInstance = 0;
}