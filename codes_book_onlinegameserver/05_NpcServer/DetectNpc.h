#pragma once
#include "Npc.h"

class DetectNpc : public Npc
{
public:
	DetectNpc(void);
	virtual ~DetectNpc(void);

	virtual void	OnProcess();    //ó�� �Լ�
	virtual void	Init();         //�ʱ�ȭ �Լ� 
	void			InitDetect();   //�ʱ�ȭ �Լ�
	void			DoDetect();     //Ž�� �Լ�
	void			DoDisComport(); //���� �����϶� �Ҹ��� �Լ� 
private:
	
	BYTE	m_byDisComportIndex;	//���� ���� , ������ 5�̻��̸� ȭ������ �ٲ�
};
