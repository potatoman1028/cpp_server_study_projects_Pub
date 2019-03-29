/*
1 ~ 20    : Ŭ���̾�Ʈ���� ������ ��û ��Ŷ�� ��ȣ
21 ~ 40   : Ŭ���̾�Ʈ���� ������ �˸� ��Ŷ�� ��ȣ
41 ~ 60   : NPC�������� ������ ���� ��Ŷ�� ��ȣ
61 ~ 80   : NPC�������� ������ �˸� ��Ŷ�� ��ȣ

101 ~ 130 : �������� Ŭ���̾�Ʈ�� ������ ���� ��Ŷ�� ��ȣ
131 ~ 160 : �������� Ŭ���̾�Ʈ�� ������ �˸� ��Ŷ�� ��ȣ
141 ~ 160 : �������� NPC������ ������ ��û ��Ŷ�� ��ȣ
161 ~ 180 : �������� NPC������ ������ �˸� ��Ŷ�� ��ȣ



__V__    :  ������ Ŭ���̾�Ʈ���� ������ ���� ��Ŷ
___Rq  :  Ŭ���̾�Ʈ���� ��û�ϴ� ��Ŷ
___Aq  :  �������� ������ ���� ��Ŷ
___Cn  :  Ŭ���̾�Ʈ���� ������ �˸� ��Ŷ
___Sn  :  �������� ������ �˸� ��Ŷ

NPC___Rq :  �������� NPC������ ��û�ϴ� ��Ŷ
NPC___Aq :  NPC�������� ������ �����ϴ� ��Ŷ
NPC___Cn :  �������� NPC������ ������ �˸� ��Ŷ
NPC___Sn :  NPC�������� ������ ������ �˸� ��Ŷ

���� ���� ��Ŷ�� ��� ��Ŷ�̸��� ___V__�� �ٴ´�.
*/

enum ePacket{

	LoginPlayer_Rq		= 1,	//�α� ��û
	MyPlayerInfo_Aq		= 101,	//�α� ��û�� �� �÷��̾� ����
	WorldPlayerInfo_VAq = 102,	//���� ������ �ִ� �÷��̾�� ����

	LoginPlayer_Sn		= 121,  //�α����� �÷��̾� ������ �ٸ� �÷��̾�鿡�� �˸�
	LogoutPlayer_Sn		= 122,	//�α� �ƿ��� �ߴٴ°��� �ٸ� �÷��̾�� �˸�
	MovePlayer_Cn		= 23,   //�÷��̾� �̵��� ������ �˸�
	MovePlayer_Sn		= 123,  //�÷��̾� �̵��� �ٸ� �÷��̾�� �˸�
	
	/////////////////////////////////////////////////////////////////
	//NPC���� ��Ŷ
	NPC_NpcInfo_VSn		= 61,  //NPC�������� NPC������ ���� ������ �˸�
    NPC_UpdateNpc_VSn		= 62,  //NPC�������� NPC�� ������Ʈ �Ǿ��ٴ� ���� ���� ������ �˸�
	NPC_AttackNpcToPlayer_Sn = 63,  //NPC�� �÷��̾ �����Ѵٴ� ���� ���� ������ �˸�

	NPC_LoginPlayer_Cn  = 164,   //�÷��̾ �α��ߴٴ� ���� NPC������ �˸�
	NPC_LogoutPlayer_Cn  = 165,   //�÷��̾ �α׾ƿ��ߴٴ� ���� NPC������ �˸�
	NPC_MovePlayer_Cn   = 166,   //�÷��̾ �������ٴ� ���� NPC������ �˸�
	NPC_DeadPlayer_Cn   = 167,	//�÷��̾ �׾��ٴ� ���� NPC������ �˸�

	KeepAlive_Cn		= 99,  //�÷��̾ ������ KeepAlive��Ŷ


};

#pragma pack(1)
//LoginPlayer_Rq	= 1,			//�α� ��û
struct LoginPlayerRq
{
	unsigned int	s_nLength;
	unsigned short	s_sType;
};

//MyPlayerInfo_Aq= 101,			//�α� ��û�� �� �÷��̾� ����
struct MyPlayerInfoAq
{
	unsigned int	s_nLength;
	unsigned short	s_sType;
	DWORD			s_dwPKey;	//�÷��̾� ���� ��ȣ
	char			s_szId[ MAX_ID_LENGTH ];  //���̵�
	char			s_szNickName[ MAX_NICKNAME_LENGTH ];  //����
	char			s_szName[ MAX_NAME_LENGTH ];  //�̸�
	DWORD			s_dwPos;	//�÷��̾� ��ġ
	BYTE			s_byLevel;	//����
	BYTE			s_byStr;	//���ݷ�
	BYTE			s_byDur;	//����
	DWORD			s_dwHp;		//�����
	DWORD			s_dwExp;	//����ġ
};
	
//WorldPlayerInfo_VAq = 102,		//���� ������ �ִ� �÷��̾�� ����
struct WorldPlayerInfoVAq
{
	unsigned int	s_nLength;
	unsigned short	s_sType;
	unsigned short	s_sPlayerCnt;	//���� ������ ���ӵǾ��ִ� �÷��̾� ���
	//���� ����
	DWORD			s_dwPKey;	//�÷��̾� ���� ��ȣ
	char			s_szId[ MAX_ID_LENGTH ];  //���̵�
	char			s_szNickName[ MAX_NICKNAME_LENGTH ];  //����
	char			s_szName[ MAX_NAME_LENGTH ];  //�̸�
	DWORD			s_dwPos;	//�÷��̾� ��ġ
	BYTE			s_byLevel;	//����
	BYTE			s_byStr;	//���ݷ�
	BYTE			s_byDur;	//����
	DWORD			s_dwHp;		//�����
	DWORD			s_dwExp;	//����ġ
};

//LoginPlayer_Sn		= 151,  //�α����� ����� ������ �ٸ� ����ڵ鿡�� �˸���.
struct LoginPlayerSn
{
	unsigned int	s_nLength;
	unsigned short	s_sType;

	DWORD			s_dwPKey;	//�÷��̾� ���� ��ȣ
	char			s_szId[ MAX_ID_LENGTH ];  //���̵�
	char			s_szNickName[ MAX_NICKNAME_LENGTH ];  //����
	char			s_szName[ MAX_NAME_LENGTH ];  //�̸�
	DWORD			s_dwPos;	//�÷��̾� ��ġ
	BYTE			s_byLevel;	//����
	BYTE			s_byStr;	//���ݷ�
	BYTE			s_byDur;	//����
	DWORD			s_dwHp;		//�����
	DWORD			s_dwExp;	//����ġ
};
//LogoutPlayer_Sn		= 152,	//�α� �ƿ��� �ߴٴ°��� �ٸ� ����ڿ��� �˸���.
struct LogoutPlayerSn
{
	unsigned int	s_nLength;
	unsigned short	s_sType;
	DWORD			s_dwPKey;	//�÷��̾� ���� ��ȣ
};
//MovePlayer_Cn		= 53,   //�÷��̾� �̵��� ������ �˸�
struct MovePlayerCn
{
	unsigned int	s_nLength;
	unsigned short	s_sType;
	DWORD			s_dwCPos;	//���� ��ġ
	DWORD			s_dwTPos;	//�̵��� ��ġ
};
//MovePlayer_Sn		= 153,  //�÷��̾� �̵��� �ٸ� �÷��̾�� �˸�
struct MovePlayerSn
{
	unsigned int	s_nLength;
	unsigned short	s_sType;
	DWORD			s_dwPKey;	//�̵��� �÷��̾� ���� Ű
	DWORD			s_dwCPos;	//���� ��ġ
	DWORD			s_dwTPos;	//�̵��� ��ġ
};	
//NPC_NpcInfo_VSn		= 61,  //NPC�������� NPC������ ���� ������ �˸�
struct NPCNpcInfoVSn
{
	unsigned int	s_nLength;
	unsigned short	s_sType;
	unsigned short  s_sNpcCnt;	//NPC����
	//���� ����
	DWORD			s_dwNpcKey; //NPCŰ
	DWORD			s_dwNpcPos; //NPC��ġ
	DWORD			s_dwNpcType; //NPC����
	char*			s_szNpcName; //NPC�̸�
};
//NPC_UpdateNpc_VSn		= 62,  //NPC�������� NPC�� ������Ʈ �Ǿ��ٴ� ���� ���� ������ �˸�
struct NPCUpdateNpcVSn
{
	unsigned int	s_nLength;
	unsigned short	s_sType;
	unsigned short  s_sNpcCnt;	//NPC����
	//���� ����
	DWORD			s_dwNpcKey; //NPCŰ
	DWORD			s_dwNpcPos; //NPC��ġ
	DWORD			s_dwNpcStatus; //NPC����
};
//NPC_AttackNpcToPlayer_Sn = 63,  //NPC���°� ��ȭ�ߴٴ°��� ���� ������ �˸�
struct NPCAttackNpcToPlayerSn
{
	unsigned int	s_nLength;
	unsigned short	s_sType;
	DWORD			s_dwNpcKey; //�����ϴ� NPCŰ
	DWORD			s_dwPKey;   //���ݴ��ϴ� �÷��̾� Ű
};

//NPC_LoginPlayer_Cn  = 164,   //�÷��̾ �α��ߴٴ� ���� NPC������ �˸�
struct NPCLoginPlayerCn
{
	unsigned int	s_nLength;
	unsigned short	s_sType;
	DWORD			s_dwPKey;
	DWORD			s_dwPos;
};
//NPC_LogoutPlayer_Cn  = 165,   //�÷��̾ �α��ߴٴ� ���� NPC������ �˸�
struct NPCLogoutPlayerCn
{
	unsigned int	s_nLength;
	unsigned short	s_sType;
	DWORD			s_dwPKey;
};
//NPC_MovePlayer_Cn   = 166,   //�÷��̾ �������ٴ� ���� NPC������ �˸�
struct NPCMovePlayerCn
{
	unsigned int	s_nLength;
	unsigned short	s_sType;
	DWORD			s_dwPKey;
	DWORD			s_dwPos;
};
//NPC_DeadPlayer_Cn   = 167,	//�÷��̾ �׾��ٴ� ���� NPC������ �˸�
struct NPCDeadPlayerCn
{
	unsigned int	s_nLength;
	unsigned short	s_sType;
	DWORD			s_dwPKey;
};
//KeepAlive_Cn		= 99,  //�÷��̾ ������ KeepAlive��Ŷ
struct KeepAliveCn
{
	unsigned int	s_nLength;
	unsigned short	s_sType;
};

//NPC_NPCINFO_Sn		= 161,  //NPC�������� NPC������ ���� ������ ������.

#pragma pack()
