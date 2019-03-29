// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.
//

#pragma once

#define DEFAULT_PORT 1820

#include "../LowLib/Global.h"

#include "../LowLib/CriticalSection.h"
#include "../LowLib/MultiThreadSync.h"
#include "../LowLib/MemoryPool.h"
#include "../LowLib/CircularQueue.h"
#include "../LowLib/NetworkSession.h"
#include "../LowLib/PacketSession.h"
#include "../LowLib/EventSelect.h"
#include "../LowLib/Stream.h"
#include "../LowLib/MiniDump.h"
#include "../LowLib/IniFile.h"
#include "../LowLib/ClientSession.h"

#include "../Server/Protocol.h"
#include "../Server/PT_Structure.h"
#include "../Server/PT_ReadPacket.h"
#include "../Server/PT_WritePacket.h"
