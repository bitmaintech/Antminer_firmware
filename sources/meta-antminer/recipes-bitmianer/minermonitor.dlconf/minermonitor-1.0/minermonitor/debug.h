#ifndef  __DEBUG_H_
#define  __DEBUG_H_
#define DEBUG_S
#define DEBUG_P

#ifdef  DEBUG_S
#define DEBUG_SHOW(format,...) printf("File: "__FILE__", Line: %05d: "format"/n", __LINE__, ##__VA_ARGS__)
#else
#define DEBUG_SHOW(format,...)
#endif


#ifdef  DEBUG_P
#define DEBUG_printf(format,...) printf(format,  ##__VA_ARGS__)
#else
#define DEBUG_printf(format,...)
#endif

#endif


