// G711.cpp: implementation of the CG711 class.    
//    
//////////////////////////////////////////////////////////////////////    


#include "g711a.h"    
#define IN    
#define OUT    



int g711a_Encode(IN const char *src,OUT char *dest,IN int srclen,OUT int *dstlen)    
{    
        int i,tmp;    
        unsigned short *ps=(unsigned short*)src;        
        *dstlen=0;    
        tmp = srclen/2;    
        for(i=0; i<tmp; i++)    
        {    
                dest[i]=linear2alaw(ps[i]);    
                (*dstlen)++;    
        }    
        return 1;    
}    

int g711a_Decode(IN const char *src,OUT char *dest,IN int srclen,OUT int *dstlen)    
{    
        int i;    
        unsigned short *pd=(unsigned short*)dest;    
        *dstlen=0;    
        for(i=0; i<srclen; i++)    
        {       
                pd[i]=(unsigned short)alaw2linear(src[i]);    
                (*dstlen)+=2;    
        }    
        return 1;    
}  