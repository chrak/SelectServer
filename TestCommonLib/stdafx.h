// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 특정 포함 파일이 들어 있는
// 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#undef FD_SETSIZE
#define FD_SETSIZE 1024
#define __TBB_malloc_proxy

#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_  //< winsock2.h 와 msado15.tlh(ws2def.h) 의 재정의 방지
#endif

#define ALLOCATE_MALLOC(type_, size_) static_cast<type_*>(malloc(size_))
#define ALLOCATE_NEW(type_, ...) new type_(##__VA_ARGS__)
#define SAFE_FREE(a_) if (a_ != NULL) { free(a_);\
                        a_ = NULL; }
#define SAFE_DELETE(a_) if (a_ != NULL) { delete a_;\
						a_ = NULL; }

#ifdef _DEBUG
#pragma comment(lib, "tbb_debug.lib")
#ifdef __TBB_malloc_proxy
// new를 통해 tbb_malloc 할당
#pragma comment (lib, "tbbmalloc_proxy_debug.lib") 
#pragma comment (linker, "/include:__TBB_malloc_proxy") 
#include "tbb/tbbmalloc_proxy.h"
#endif
#else
#pragma comment(lib, "tbb.lib")
#ifdef __TBB_malloc_proxy
#pragma comment (lib, "tbbmalloc_proxy.lib") 
#pragma comment (linker, "/include:__TBB_malloc_proxy") 
#include "tbb/tbbmalloc_proxy.h"
#endif
#endif


// 여기서 프로그램에 필요한 추가 헤더를 참조합니다.
