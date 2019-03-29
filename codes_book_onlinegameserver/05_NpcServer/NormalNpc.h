#pragma once
#include "Npc.h"

class NormalNpc : public Npc
{
public:
	NormalNpc(void);
	virtual ~NormalNpc(void);

	virtual void	OnProcess();
	virtual void	Init();
};
