// G711u.cpp: implementation of the G711u class.    
//    
//////////////////////////////////////////////////////////////////////    

#include "g711u.h"    
#define IN    
#define OUT    

int g711u_Encode(IN const char *src,OUT char *dest,IN int srclen,OUT int *dstlen)    
{       
        int i,tmp;    
        *dstlen=0;    
        signed short *ps=(signed short*)src;    
        tmp = srclen/2;    
        for(i=0; i<tmp; i++)    
        {    
                dest[i]=linear2ulaw(ps[i]);    
                (*dstlen)++;    
        }    
        return 1;    
}    

int g711u_Decode(IN const char *src,OUT char *dest,IN int srclen,OUT int *dstlen)    
{    
        int i;    
        *dstlen=0;    
        signed short *pd=(signed short*)dest;    
        for(i=0; i<srclen; i++)    
        {       
                pd[i]=(signed short)ulaw2linear(src[i]);    
                (*dstlen)+=2;    
        }    
        return 1;    
}  