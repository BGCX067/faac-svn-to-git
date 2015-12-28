#include <windows.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#include "getopt.h"
#include "input.h"
#include "faac.h"
#include "AACEnc.h"

faacEncConfigurationPtr myFormat;
aac_option_ptr aac_option;//new defined structure
faacEncHandle hEncoder;
pcmfile_t *infile = NULL;
unsigned long samplesInput, maxBytesOutput, totalBytesWritten=0;
faacEncHandle hEncoder;
char *audioFileName = "E:\\aac\\src\\faac\\FAAC-Enc-2011-09-28-v0.00E\\audio\\daolang.wav";
char *aacFileName = "E:\\aac\\src\\faac\\FAAC-Enc-2011-09-28-v0.00E\\audio\\daolang_0928.aac";
float *pcmbuf;
int *chanmap = NULL;
unsigned char *bitbuf;

void aac_enc_init (int new_channels,int new_sampleRate,int bitrate);
void aac_open_infile(void);
void aac_context_set(void);
void aac_write_outfile(void);
void end_aac_code(void);
/* --------------------------------------------------------------------------*/
/**
 * @brief main 
 *
 * @returns  zero if success
 */
/* ----------------------------------------------------------------------------*/
int main()//int argc, char *argv[])
{
//  int frames, currentFrame;
//	unsigned long samplesInput, maxBytesOutput, totalBytesWritten=0;
//	faacEncHandle hEncoder;
//	pcmfile_t *infile = NULL;
//  faacEncConfigurationPtr myFormat;
//  aac_option_ptr aac_option;//new defined structure
//  char *audioFileName = "E:\\aac\\src\\faac\\FAAC-Enc-2011-09-28-v0.00E\\audio\\daolang.wav";
//  char *aacFileName = "E:\\aac\\src\\faac\\FAAC-Enc-2011-09-28-v0.00E\\audio\\daolang_0928.aac";
//  float *pcmbuf;
//  int *chanmap = NULL;
//  unsigned char *bitbuf;
//  int samplesRead = 0;  move to the function aac_write_outfile()
//  FILE *outfile = NULL; move to the function aac_write_outfile()
	int new_channels=0,new_sampleRate=44100,new_bitrate=0;
	aac_enc_init(new_channels,new_sampleRate,new_bitrate);
	aac_open_infile();
	aac_context_set();
	aac_write_outfile();
	end_aac_code();
	return 0;
 }

/* --------------------------------------------------------------------------*/
/**
 * @brief aac_enc_init 
 *
 * @param channels
 * @param sampleRate
 * @param bitrate
 */
/* ----------------------------------------------------------------------------*/
void aac_enc_init(int new_channels,int new_sampleRate,int new_bitrate)
{
    aac_option=(aac_option_ptr)malloc(sizeof(my_aac_option)) ;
	init_option(aac_option);
	aac_option->rawChans=new_channels;
	aac_option->rawRate=new_sampleRate;
	aac_option->bitRate=new_bitrate;

	return ;
}


/* --------------------------------------------------------------------------*/
/**
 * @brief aac_open_infile 
 */
/* ----------------------------------------------------------------------------*/
void aac_open_infile()
{

	if(aac_option->rawChans > 0)//use raw input
	{
		infile=wav_open_read(audioFileName,1);
	if(infile)
	{
		infile->bigendian=aac_option->rawEndian;
		infile->channels= aac_option->rawChans;
		infile->samplebytes= aac_option->rawBits/8;//change from bits to bytes
		infile->samplerate= aac_option->rawRate;
		infile->samples/=(infile->channels * infile->samplebytes);
	}
    }
    else // header input
        infile = wav_open_read(audioFileName, 0);
    if (infile == NULL)
    {
        fprintf(stderr, "Couldn't open input file %s\n", audioFileName);
	      return 1;
    
			
	}
	return ;
}



/* --------------------------------------------------------------------------*/
/**
 * @brief aac_context_set 
 */
/* ----------------------------------------------------------------------------*/
void aac_context_set()
{
    /* open the encoder library */
    hEncoder = faacEncOpen(infile->samplerate, infile->channels,
        &samplesInput, &maxBytesOutput);
    pcmbuf = (float *)malloc(samplesInput*sizeof(float));
	//pcmbuf_short = (short *)malloc(samplesInput * sizeof(short));
	//	2011-09-25 23:39:55
    bitbuf = (unsigned char*)malloc(maxBytesOutput*sizeof(unsigned char));
    chanmap = mkChanMap(infile->channels, aac_option->chanC,aac_option->chanLF);
    if (chanmap)
    {
        fprintf(stderr, "Remapping input channels: Center=%d, LFE=%d\n",
            aac_option->chanC,aac_option->chanLF);
    }
    if (aac_option->cutOff <= 0)
    {
        if (aac_option->cutOff < 0) // default
            aac_option->cutOff= 0;
        else // disabled
            aac_option->cutOff = infile->samplerate / 2;
    }
    if (aac_option->cutOff > (infile->samplerate / 2))
        aac_option->cutOff = infile->samplerate / 2;

    /* put the options in the configuration struct */
    myFormat = faacEncGetCurrentConfiguration(hEncoder);
    myFormat->aacObjectType =aac_option->objectType;
    myFormat->mpegVersion = aac_option->mpegVersion;
    myFormat->useTns = aac_option->useTns;

    switch (aac_option->shortctl)
    {
    case SHORTCTL_NOSHORT:
      fprintf(stderr, "disabling short blocks\n");
      myFormat->shortctl = aac_option->shortctl;
      break;
    case SHORTCTL_NOLONG:
      fprintf(stderr, "disabling long blocks\n");
      myFormat->shortctl = aac_option->shortctl;
      break;
    }
    if (infile->channels >= 6)
        myFormat->useLfe = 1;
    myFormat->allowMidside = aac_option->useMidSide;
    if (aac_option->bitRate)
        myFormat->bitRate = aac_option->bitRate / infile->channels;
    myFormat->bandWidth = aac_option->cutOff;
    if (aac_option->quantqual > 0)
        myFormat->quantqual = aac_option->quantqual;
    myFormat->outputFormat = aac_option->stream;
    myFormat->inputFormat = FAAC_INPUT_FLOAT;//will be changed to 16bit int 
    if (!faacEncSetConfiguration(hEncoder, myFormat)) {
        fprintf(stderr, "Unsupported output format!\n");
        return 1;
    }

    aac_option->cutOff = myFormat->bandWidth;
    aac_option->quantqual = myFormat->quantqual;
    aac_option->bitRate = myFormat->bitRate;
    if (aac_option->bitRate)
      fprintf(stderr, "Average bitrate: %d kbps\n",
          (aac_option->bitRate + 500)/1000*infile->channels);
    fprintf(stderr, "Quantization quality: %ld\n", aac_option->quantqual);
    fprintf(stderr, "Bandwidth: %d Hz\n", aac_option->cutOff);
    fprintf(stderr, "Object type: ");
    switch(aac_option->objectType)
    {
    case LOW:
        fprintf(stderr, "Low Complexity");
        break;
    case MAIN:
        fprintf(stderr, "Main");
        break;
    case LTP:
        fprintf(stderr, "LTP");
        break;
    }
    fprintf(stderr, "(MPEG-%d)", (aac_option->mpegVersion == MPEG4) ? 4 : 2);
    if (myFormat->useTns)
        fprintf(stderr, " + TNS");
    if (myFormat->allowMidside)
        fprintf(stderr, " + M/S");
    fprintf(stderr, "\n");

    fprintf(stderr, "Container format: ");
    switch(aac_option->container)
    {
    case NO_CONTAINER:
      switch(aac_option->stream)
    {
    case RAW_STREAM:
      fprintf(stderr, "Headerless AAC (RAW)\n");
      break;
    case ADTS_STREAM:
      fprintf(stderr, "Transport Stream (ADTS)\n");
      break;
    }
        break;
    }
	return ;
}


/* --------------------------------------------------------------------------*/
/**
 * @brief aac_write_outfile 
 */
/* ----------------------------------------------------------------------------*/
void aac_write_outfile()
{
	/* open the aac output file */
    int frames, currentFrame;
    FILE *outfile = NULL;
	outfile = fopen(aacFileName, "wb");
	if (!outfile)
	{
		fprintf(stderr, "Couldn't create output file %s\n", aacFileName);
		return 1;
	}

    if (outfile
       )
    {
        int showcnt = 0;
#ifdef _WIN32
        long begin = GetTickCount();
#endif
        if (infile->samples)
            frames = ((infile->samples + 1023) / 1024) + 1;
        else
            frames = 0;
        currentFrame = 0;

        fprintf(stderr, "Encoding %s to %s\n", audioFileName, aacFileName);
        if (frames != 0)
            fprintf(stderr, "   frame          | bitrate | elapsed/estim | "
            "play/CPU | ETA\n");
        else
            fprintf(stderr, " frame | elapsed | play/CPU\n");

        /* encoding loop */
//		2011-09-26 00:22:44

		
    for (;;)
        {
			int samplesRead=0;
            int bytesWritten;
            samplesRead = wav_read_float32(infile, pcmbuf, samplesInput, chanmap);
			//for(i=0;i<samplesRead;i++)
			// 	2011-09-25 23:59:51
			// 	pcmbuf_short[i]=(short)pcmbuf[i];
            /* call the actual encoding routine */
            bytesWritten = faacEncEncode(hEncoder,
                (int32_t *)pcmbuf,
                samplesRead,
                bitbuf,
                maxBytesOutput);

            if (bytesWritten)
            {
                currentFrame++;
                showcnt--;
        totalBytesWritten += bytesWritten;
            }

            if ((showcnt <= 0) || !bytesWritten)
            {
                double timeused;
#ifdef _WIN32
                char percent[MAX_PATH + 20];
                timeused = (GetTickCount() - begin) * 1e-3;
#else
#endif
                if (currentFrame && (timeused > 0.1))
                {
                    showcnt += 50;

                    if (frames != 0)
                        fprintf(stderr,
                            "\r%5d/%-5d (%3d%%)|  %5.1f  | %6.1f/%-6.1f | %7.2fx | %.1f ",
                            currentFrame, frames, currentFrame*100/frames,
                ((double)totalBytesWritten * 8.0 / 1000.0) /
                ((double)infile->samples / infile->samplerate * currentFrame / frames),
                            timeused,
                            timeused * frames / currentFrame,
                            (1024.0 * currentFrame / infile->samplerate) / timeused,
                            timeused  * (frames - currentFrame) / currentFrame);
                    else
                        fprintf(stderr,
                            "\r %5d |  %6.1f | %7.2fx ",
                            currentFrame,
                            timeused,
                            (1024.0 * currentFrame / infile->samplerate) / timeused);

                    fflush(stderr);
					/*2011-09-26 00:17:27 not clear about how to use fflush*/

#ifdef _WIN32
                    if (frames != 0)
                    {
                        sprintf(percent, "%.2f%% encoding %s",
                            100.0 * currentFrame / frames, audioFileName);
                        SetConsoleTitle(percent);
                    }
#endif
                }
            }

            /* all done, bail out */
            if (!samplesRead && !bytesWritten)
                break ;

            if (bytesWritten < 0)
            {
                fprintf(stderr, "faacEncEncode() failed\n");
                break ;
            }

            if (bytesWritten > 0)
            {
                    /* write bitstream to aac file */
                    fwrite(bitbuf, 1, bytesWritten, outfile);
            }
        }
        fprintf(stderr, "\n\n");

            fclose(outfile);
    }
	return ;
}


/* --------------------------------------------------------------------------*/
/**
 * @brief end_aac_code 
 */
/* ----------------------------------------------------------------------------*/
void end_aac_code()
{

    faacEncClose(hEncoder);

    wav_close(infile);

    if (pcmbuf) free(pcmbuf);
    if (bitbuf) free(bitbuf);
    free(aac_option); //2011-09-27
/*    if (aacFileNameGiven) free(aacFileName);*/
//	2011-09-26 00:21:26

    return ;
}
