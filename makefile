CFG=faac - Win32 Debug
NULL=nmake.txt
OUTDIR=.\Debug
INTDIR=.\Debug
SOURCE_DIR=.\aac_codec
ALL : "$(OUTDIR)\faac.exe" 
"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"
CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3  /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS"  /Fp"$(INTDIR)\faac.pch" /YX /Fo"$(INTDIR)\\"  /Fd"$(INTDIR)\\" /FD /GZ /c 


{aac-codec}.c{$(INTDIR)}.obj::
   $(CPP)  @<<
   $(CPP_PROJ)  $<
<<
.c{$(INTDIR)}.obj::
	$(CPP) @<<
	$(CPP_PROJ)  /I ".\aac-codec" $<
<<


LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /out:"$(OUTDIR)\faac.exe"
LINK32_OBJS= \
	$(INTDIR)\AACEnc.obj \
	$(INTDIR)\aacquant.obj \
	$(INTDIR)\backpred.obj \
	$(INTDIR)\bitstream.obj \
	$(INTDIR)\channels.obj \
	$(INTDIR)\fft.obj \
	$(INTDIR)\filtbank.obj \
	$(INTDIR)\frame.obj \
	$(INTDIR)\getopt.obj \
	$(INTDIR)\huffman.obj \
	$(INTDIR)\input.obj \
	$(INTDIR)\ltp.obj \
	$(INTDIR)\aac_code_main.obj \
	$(INTDIR)\midside.obj \
	$(INTDIR)\psychkni.obj \
	$(INTDIR)\tns.obj \
	$(INTDIR)\util.obj

"$(OUTDIR)\faac.exe" :  $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
