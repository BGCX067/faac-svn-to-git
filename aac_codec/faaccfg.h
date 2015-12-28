/* Id: faaccfg.h,v 1.3 2004/07/04 12:12:05 corrados Exp */

#ifndef _FAACCFG_H_
#define _FAACCFG_H_

#define FAAC_CFG_VERSION 104

#ifndef MPEG_ID
/* MPEG ID's */
#define MPEG2 1
#define MPEG4 0
#endif

#ifndef AAC_OBJECT_TYPE
/* AAC object types */
#define MAIN 1
#define LOW  2
#define SSR  3
#define LTP  4
#endif


/* Input Formats */
#define FAAC_INPUT_NULL    0
#define FAAC_INPUT_16BIT   1
#define FAAC_INPUT_24BIT   2
#define FAAC_INPUT_32BIT   3
#define FAAC_INPUT_FLOAT   4


/*DEFAULT TNS*/
#define NOUSE_TNS 		   0
#define USE_TNS 		   1

#define USE_MS 			   1
#define NOUSE_MS 		   0

#define SHORTCTL_NORMAL    0
#define SHORTCTL_NOSHORT   1
#define SHORTCTL_NOLONG    2

#pragma pack(push, 1)
typedef struct faacEncConfiguration
{
    /* config version */
    int version;

    /* library version */
    char *name;

    /* copyright string */
    char *copyright;

    /* MPEG version, 2 or 4 */
    unsigned int mpegVersion;

	/*2011-09-25 19:53:21 object type is 1,2,3,4*/
    /* AAC object type */
	/*MAIN LOW SSR LTP*/
    unsigned int aacObjectType;

    /* Allow mid/side coding */
    unsigned int allowMidside;

    /* Use one of the channels as LFE channel */
    unsigned int useLfe;

    /* Use Temporal Noise Shaping */
    unsigned int useTns;

    /* bitrate / channel of AAC file */
    unsigned long bitRate;

    /* AAC file frequency bandwidth */
    unsigned int bandWidth;

    /* Quantizer quality */
    unsigned long quantqual;

    /* Bitstream output format (0 = Raw; 1 = ADTS) */
    unsigned int outputFormat;

    /* psychoacoustic model list */
    psymodellist_t *psymodellist;

    /* selected index in psymodellist */
    unsigned int psymodelidx;

    /*
		PCM Sample Input Format
		0	FAAC_INPUT_NULL			invalid, signifies a misconfigured config
		1	FAAC_INPUT_16BIT		native endian 16bit
		2	FAAC_INPUT_24BIT		native endian 24bit in 24 bits		(not implemented)
		3	FAAC_INPUT_32BIT		native endian 24bit in 32 bits		(DEFAULT)
		4	FAAC_INPUT_FLOAT		32bit floating point
    */
    unsigned int inputFormat;

    /* block type enforcing (SHORTCTL_NORMAL/SHORTCTL_NOSHORT/SHORTCTL_NOLONG) */
    int shortctl;
	
	/*
		Channel Remapping

		Default			0, 1, 2, 3 ... 63  (64 is MAX_CHANNELS in coder.h)

		WAVE 4.0		2, 0, 1, 3
		WAVE 5.0		2, 0, 1, 3, 4
		WAVE 5.1		2, 0, 1, 4, 5, 3
		AIFF 5.1		2, 0, 3, 1, 4, 5 
	*/
	int channel_map[64];	

} faacEncConfiguration, *faacEncConfigurationPtr;

#pragma pack(pop)

#endif /* _FAACCFG_H_ */
