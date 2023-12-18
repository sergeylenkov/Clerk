#ifndef Defines_h
#define Defines_h

constexpr auto APP_NAME = "Clerk";
constexpr auto APP_VERSION = "0.5.3";

#ifdef _DEBUG
	#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
	#define DBG_NEW new
#endif

#endif