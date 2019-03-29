#pragma once

enum _EQUIP_INDEX
{
	E_HEAD		= 0,
	E_CHEST,
	E_PANTS,
	E_GLOVES,
	E_SOCKS,
	E_SHOES,
	E_ACCESSORY,
	E_SKILL1,
	E_SKILL2,
	E_SKILL3,
	E_SSHOOT1,
	E_SSHOOT2,
	E_SSHOOT3,
	E_END
};

class CCharacter
{
public:
	CCharacter(VOID);
	virtual ~CCharacter(VOID);

private:
	TCHAR			mName[32];

	INT64			mIndex;
	INT				mJob;
	INT				mType;
	INT64			mExp;

	INT				mEquips[E_END];

public:
	VOID			Initialize(VOID);

	inline VOID		SetIndex(INT64 index){mIndex = index;}
	inline INT64	GetIndex(VOID){return mIndex;}
	inline VOID		SetJob(INT job){mJob	= job;}
	inline INT		GetJob(VOID){return mJob;}
	inline VOID		SetType(INT type){mType	= type;}
	inline INT		GetType(VOID){return mType;}
	inline VOID		SetExp(INT64 exp){mExp	= exp;}
	inline INT64	GetExp(VOID){return mExp;}
	inline VOID		SetEquip(USHORT equipLocation, INT type){mEquips[equipLocation] = type;}
	inline INT		GetEquip(USHORT equipLocation){return mEquips[equipLocation];}
	inline BOOL		SetName(LPTSTR name){if (!name)return FALSE;_tcsncpy(mName, name, 32);return TRUE;}
	inline LPTSTR	GetName(VOID){return mName;}
};
