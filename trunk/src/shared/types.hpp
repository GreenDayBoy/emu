#ifndef SHARED_TYPES_HPP
#define SHARED_TYPES_HPP

typedef unsigned char uint8;
typedef signed char int8;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned int uint32;
typedef signed int int32;
typedef signed long long int int64;
typedef unsigned long long int uint64;

#ifdef UNIT_TESTING
    #define MOCKABLE virtual
#else
    #define MOCKABLE
#endif // UNIT_TESTING

#endif //SHARED_TYPES_HPP