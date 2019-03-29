#pragma once
#pragma warning( disable:4786 )
#pragma warning( disable:4251 )
#pragma warning( disable:4311 )
#pragma warning( disable:4244 )
#pragma warning( disable:4996 )

#include <iostream>

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "mswsock.lib")  
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <Mswsock.h>

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <ws2spi.h>
#include <winbase.h>
#include <process.h>
#include <vector>
#include <map>
#include <queue>
#include <list>
#include <fstream>
#include <math.h>

using namespace std;

#include "CommonDef.h"
#include "Monitor.h"
#include "Singleton.h"
#include "Thread.h"
#include "Queue.h"
#include "Log.h"
#include "Connection.h"
#include "IocpServer.h"
#include "VBuffer.h"
