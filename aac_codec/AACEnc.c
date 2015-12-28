/*
 *       Filename:  AACEnc.c
 *        Created:  2011-9-25 21:55:19
 */
#include "AACEnc.h"
//#include "faaccfg.h"

#ifndef NULL
#define NULL 0
#endif

/*2011-09-27 15:34:16 add function init_option */
/* --------------------------------------------------------------------------*/
/**
 * @brief init_option 
 *
 * @param aac_option
 *
 * @returns  zero if success
 */
/* ----------------------------------------------------------------------------*/
int init_option(aac_option_ptr  aac_option)
{
	memset(aac_option,0,sizeof(my_aac_option));
//    struct set_option * options=&aac_option;
	aac_option->mpegVersion=1;//1:MPEG2,2:MPEG4
	aac_option->objectType=2;//LOW;//2
	aac_option->stream=1;//ADTS_STREAM;//ADTS_STREAM:1
	aac_option->useMidSide=1;//USE_MS;//USE_MS:1
	aac_option->cutOff=-1;
	aac_option->chanC=3;
	aac_option->chanLF=4;
//	aac_option->rawChans=0;
	aac_option->rawBits=16;
	aac_option->rawRate=44100;
	aac_option->rawEndian=1;
	return 0;
}

//static cannot be static  2011-09-26
/* --------------------------------------------------------------------------*/
/**
 * @brief mkChanMap 
 *
 * @param channels
 * @param center
 * @param lf
 *
 * @returns  map
 */
/* ----------------------------------------------------------------------------*/
int *mkChanMap(int channels, int center, int lf)
{
    int *map;
    int inpos;
    int outpos;

    if (!center && !lf)
        return NULL;

    if (channels < 3)
        return NULL;

    if (lf > 0)
        lf--;
    else
        lf = channels - 1; // default AAC position

    if (center > 0)
        center--;
    else
        center = 0; // default AAC position

    map = malloc(channels * sizeof(map[0]));
    memset(map, 0, channels * sizeof(map[0]));

    outpos = 0;
    if ((center >= 0) && (center < channels))
        map[outpos++] = center;

    inpos = 0;
    for (; outpos < (channels - 1); inpos++)
    {
        if (inpos == center)
            continue;
        if (inpos == lf)
            continue;

        map[outpos++] = inpos;
    }
    if (outpos < channels)
    {
        if ((lf >= 0) && (lf < channels))
            map[outpos] = lf;
        else
            map[outpos] = inpos;
    }

    return map;
}

