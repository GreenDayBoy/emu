#ifndef eMUCORE_CORE_HPP
#define eMUCORE_CORE_HPP

#ifndef WIN32
    #define eMUCORE_DECLSPEC
#elif eMUCORE_EXPORTS
    #define eMUCORE_DECLSPEC __declspec(dllexport)
#else
    #define eMUCORE_DECLSPEC __declspec(dllimport)
#endif //WIN32

#endif //eMUCORE_CORE_HPP