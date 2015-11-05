/*
 * debug.h
 *
 *  Created on: Mar 19, 2015
 *      Author: fazio
 */

#ifndef SRC_DEBUG_H_
#define SRC_DEBUG_H_

#define DEBUG_P
#define MEMWATCH
#define MW_STDIO

#ifdef  DEBUG_P
#define DEBUG_printf(format,...) printf(format,  ##__VA_ARGS__)
#else
#define DEBUG_printf(format,...)
#endif

#endif /* SRC_DEBUG_H_ */
