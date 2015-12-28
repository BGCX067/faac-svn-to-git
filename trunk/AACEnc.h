/*
 *       Filename:  AACEnc.h
 *        Created:  2011-9-25 21:51:43
 */
#ifndef _AACENC_H
#define _AACENC_H

#if !defined(HAVE_STRCASECMP) && !defined(_WIN32)
# define strcasecmp strcmp
#endif

#ifdef _WIN32
# undef stderr
# define stderr stdout
#endif


#ifndef min
#define min(a,b) ( (a) < (b) ? (a) : (b) )
#endif


enum stream_format {
  RAW_STREAM = 0,
  ADTS_STREAM = 1,
};

enum container_format {
  NO_CONTAINER,
  MP4_CONTAINER,
};

enum flags {
  SHORTCTL_FLAG = 300,
  TNS_FLAG = 301,
  NO_TNS_FLAG = 302,
  MPEGVERS_FLAG = 303,
  OBJTYPE_FLAG = 304,
  NO_MIDSIDE_FLAG = 306,
};


typedef struct  {
	unsigned int mpegVersion  ; 
	unsigned int objectType ;
	unsigned int useTns;
	unsigned int useMidSide ;
	enum container_format container :16;
	int optimizeFlag ;
	enum stream_format stream :16;
	int cutOff ;     //´ø¿í,bandWidth
	int bitRate ;
	unsigned long quantqual ;
	int chanC ;
	int chanLF ;
	int rawBits ;
	int rawChans ;
	int rawEndian ;
	int rawRate ;
	int shortctl ;
} my_aac_option,* aac_option_ptr ;


/* --------------------------------------------------------------------------*/
/**
 * @synopsis init_option 
 *
 * @param aac_option
 *
 * @returns 
 */
/* ----------------------------------------------------------------------------*/
int init_option(aac_option_ptr aac_option);


//static  2011-09-26 cannot be static
/* --------------------------------------------------------------------------*/
/**
 * @brief mkChanMap 
 *
 * @param channels
 * @param center
 * @param lf
 *
 * @returns map
 */
/* ----------------------------------------------------------------------------*/
int *mkChanMap(int channels, int center, int lf);

#endif
