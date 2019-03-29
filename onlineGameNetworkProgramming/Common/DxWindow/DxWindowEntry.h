#ifndef  __DxWindowEntry_H
#define  __DxWindowEntry_H

class DxWindowEntry
{
public:
	static	DxWindowEntry*		getInstance();
	static	void				releaseInstance();

			bool				startUp();
			void				close();

private:
			DxWindowEntry();
	virtual	~DxWindowEntry();

	static	DxWindowEntry*		selfInstance;
};

#endif