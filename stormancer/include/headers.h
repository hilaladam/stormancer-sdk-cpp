#pragma once

//For static libs
#define _NO_ASYNCRTIMP
#define _NO_PPLXIMP
#define _RAKNET_LIB

//Name    Version  _MSC_VER
//VS 6        6.0      1200
//VS 2002     7.0      1300
//VS 2003     7.1      1310
//VS 2005     8.0      1400
//VS 2008     9.0      1500
//VS 2010    10.0      1600
//VS 2012    11.0      1700
//VS 2013    12.0      1800
//VS 2015    13.0      1900


#ifndef _STORMANCERSDKCPP
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "Bcrypt.lib")
#pragma comment(lib, "Crypt32.lib")
#endif
// DLL IMPORT / EXPORT
#ifdef _NO_ASYNCRTIMP
#define STORMANCER_DLL_API
#else
#ifdef STORMANCER_DLL_EXPORT
#define STORMANCER_DLL_API __declspec(dllexport) 
#else
#define STORMANCER_DLL_API __declspec(dllimport) 
#endif
#endif


// DEFINES


#define CPPREST_FORCE_PPLX 1
#define STORMANCER_LOG_CLIENT
//#define STORMANCER_LOG_PACKETS
//#define STORMANCER_LOG_RAKNET_PACKETS
//#define STORMANCER_LOG_RPC

#if defined(UE_EDITOR) || defined(UE_GAME)
#include "AllowWindowsPlatformTypes.h"
#endif

// standard libs
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <cstdint>
#include <cstdio>
#include <string>
#include <typeinfo>
#include <vector>
#include <list>
#include <iomanip>
#include <thread>
#include <regex>
#include <numeric>
#include <list>
#include <forward_list>

// custom libs

// cpprestsdk
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <pplx/pplxtasks.h>

// rxcpp
#include <rx.hpp>

// raknet
#include <PacketPriority.h>
#include <RakPeerInterface.h>
#include <BitStream.h>
#include <RakNetTypes.h>
#include <DS_Map.h>
#include <NatPunchthroughClient.h>

// msgpack
#include <msgpack.hpp>

// custom types
#include "typedef.h"

#if defined(UE_EDITOR) || defined(UE_GAME)
#include "HideWindowsPlatformTypes.h"
#endif
