#pragma once

class TickThread : public Thread
{
public:
	TickThread(void);
	~TickThread(void);

	virtual void	OnProcess();

};
