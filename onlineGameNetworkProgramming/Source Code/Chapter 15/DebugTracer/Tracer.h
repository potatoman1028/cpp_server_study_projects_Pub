#ifndef  __Tracer_H
#define  __Tracer_H

#include <Windows.h>
#include <stdio.h>

class ITracer
{
public:
			ITracer(){}
	virtual	~ITracer(){}

	virtual	bool		open( LPCTSTR out ) = 0;
	virtual	void		output( LPCTSTR str ) = 0;
	virtual	void		close() = 0;
};

class TracerConsole : public ITracer
{
public:
			TracerConsole();
	virtual	~TracerConsole();

	virtual	bool		open( LPCTSTR out );
	virtual	void		output( LPCTSTR str );
	virtual	void		close();

private:
			HANDLE		handleConsole;
};


class TracerFile : public ITracer
{
public:
			TracerFile();
	virtual	~TracerFile();

	virtual	bool		open( LPCTSTR out );
	virtual	void		output( LPCTSTR str );
	virtual	void		close();

private:
			FILE*		fp;
};

class TracerTrace : public ITracer
{
public:
			TracerTrace();
	virtual	~TracerTrace();

	virtual	bool		open( LPCTSTR out );
	virtual	void		output( LPCTSTR str );
	virtual	void		close();
};

#endif
