// G711u.h: interface for the G711u class.  
//  
//////////////////////////////////////////////////////////////////////  
#ifndef	G711U_H__  
#define	G711U_H__  

#include "g711_cq.h"  

int g711u_Encode( const char *src, char *dest, int srclen, int *dstlen);  

int g711u_Decode( const char *src, char *dest, int srclen, int *dstlen);  

#endif // 