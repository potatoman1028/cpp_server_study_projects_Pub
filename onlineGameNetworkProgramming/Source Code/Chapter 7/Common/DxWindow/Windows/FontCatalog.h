#ifndef  __FontCatalog_H
#define  __FontCatalog_H

enum FontType
{
	Font_Small_Normal = 0,
	Font_Small_Italic,
	Font_Small_Bold,
	Font_Small_ItalicBold,

	Font_Medium_Normal,
	Font_Medium_Italic,
	Font_Medium_Bold,
	Font_Medium_ItalicBold,

	Font_Large_Normal,
	Font_Large_Italic,
	Font_Large_Bold,
	Font_Large_ItalicBold,

	Font_Count
};

class FontCatalog
{
public:
	static	FontCatalog*		getInstance();
	static	void				releaseInstance();

	static	HFONT				handleFont[Font_Count];

private:
			FontCatalog();
	virtual	~FontCatalog();

	static	FontCatalog*		selfInstance;
};

#endif