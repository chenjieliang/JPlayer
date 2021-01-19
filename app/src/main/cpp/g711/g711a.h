// G711.h: interface for the CG711 class.  
//  
//////////////////////////////////////////////////////////////////////  

#ifndef G711A_H__  
#define G711A_H__  

#include "g711_cq.h"  
////	unsigned long FRAME_SIZE; //  
////	unsigned long PACKET_SIZE; //  
//	unsigned char	sample_char; //  
//	int			in_coding; //  
//	int			in_size; //  
//	int			out_coding; //  
//	int			out_size; //  
//	unsigned		*in_buf; //  
//	int			(*enc_routine)(int sample,int in_coding,struct g72x_state *state_ptr); //  
//	int			enc_bits; //  
//	int			(*dec_routine)(int code,int out_coding,struct g72x_state *state_ptr); //  
//	int			dec_bits; //  
// //  
// //  
//	int			resid;   
int g711a_Encode( const char *src, char *dest, int srclen, int *dstlen);  
int g711a_Decode( const char *src, char *dest, int srclen, int *dstlen);  

#endif // G711A_H__