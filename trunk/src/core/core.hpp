#ifndef eMU_CORE_CORE_HPP
#define eMU_CORE_CORE_HPP

#ifndef WIN32
    #define eMU_CORE_DECLSPEC
#elif eMU_CORE_EXPORTS
    #define eMU_CORE_DECLSPEC __declspec(dllexport)
#else
    #define eMU_CORE_DECLSPEC __declspec(dllimport)
#endif

#endif