#pragma once

#ifndef WIN32
    #define eMU_CORE_DECLSPEC
#elif eMU_CORE_EXPORTS
    #define eMU_CORE_DECLSPEC __declspec(dllexport)
#else
    #define eMU_CORE_DECLSPEC __declspec(dllimport)
#endif
