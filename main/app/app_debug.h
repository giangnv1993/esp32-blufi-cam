/** @file: vsm_debug.h
 *  @desc: this is a C header file
 *  @author: v.manhbt1@vinsmart.net.vn
 *  HISTORY:
 *  Ver   Who       Date                Changes
 *  ----- --------- ------------------  ----------------------------------------
 *  0.0   ManhBT1                       Create
 *  0.1   DuyNH8   Nov 29, 2019         Modify comment
 *
 * *************************************************
    * Copyright (c) 2019 VinSmart R&M, JSC.
    * All Rights Reserved.
    * VinSmart R&M Confidential and Proprietary.
*/

#ifndef DEBUG_H_
#define DEBUG_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define DEBUG_ENABLE						1




void debug_nothing(const char* sz, ...);



#if DEBUG_ENABLE
#define APP_DEBUG(f, ...)								\
	do{ 												\
		int count = 0; 	                                \
		int i = 0;								        \
		for( i = 0; i < strlen((char *)f); i++) 		\
		{ 												\
			if( (f[i] == '\r') || (f[i] == '\n') ) 		\
			{ 											\
				printf("%c", f[i]); 					\
				count++; 								\
			} 											\
			else 										\
			{ 											\
				break; 									\
			} 											\
		} 												\
		printf("---- %s: %d:  ",  __FUNCTION__, __LINE__ ); 	\
		printf(&f[count], ##__VA_ARGS__); 				\
	}while(0)

#else
#define VSM_DEBUG(...)
#endif




#define APP_DEBUG_RAW(s, args...)                	printf(s, ##args)





#endif /* DEBUG_H_ */
