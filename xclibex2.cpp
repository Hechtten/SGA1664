/*
 *
 *	xclibex2.cpp	External	16-Feb-2023
 *
 *	Copyright (C)  1998-2023  EPIX, Inc.	All rights reserved.
 *
 *	Example program for the XCLIB 'C' Library.
 *	Example assumes Windows 95/98/ME/NT/2000/XP/Vista/7/8/10/11 32/64-Bit.
 *
 *	Has dialog controls for PIXCI(R) SV2/SV3/SV4/SV5/SV5A/SV5B/SV5L/SV7/SV8/A110/A310
 *	Can be used with other PIXCI(R) frame grabbers,
 *	altho some controls may be meaningless.
 *
 *	Demonstrates XCLIB and/or PXIPL functions for capture and
 *	display of imagery and graphics.  Also demonstrates access
 *	to numeric pixel values, and saving of an image.
 *	This Windows program must, of course, also make use of
 *	various Windows GDI API functions; however, this is not
 *	intended to serve as a Windows tutorial.
 *
 *	Also optionally demonstrates use of multiple PIXCI(R)
 *	frame grabbers operating simultaneously.
 *
 *
 */

/*
 *  INSTRUCTIONS:
 *
 *  1)	Set 'define' options below according to the intended camera
 *	and video format.
 *
 *	For PIXCI(R) SV2, SV3, SV4, SV5, SV5A, SV5B, and SV5L frame grabbers
 *	common choices are RS-170, NSTC, NTSC/YC, CCIR, PAL, or PAL/YC.
 *	(The SV5A and SV5B do not support NTSC/YC or PAL/YC).
 *	For PIXCI(R) SV7 frame grabbers
 *	common choices are RS-170, NSTC, CCIR, or PAL.
 *	For PIXCI(R) SV8 frame grabbers
 *	common choices are RS-170, NSTC, NTSC/YC, CCIR, PAL, PAL/YC.
 *	For PIXCI(R) A110 frame grabbers
 *	common choices are RS-170, CCIR and others, see below.
 *	For PIXCI(R) A310 frame grabbers
 *	common choices are RS-170, CCIR and others, see below.
 *
 *
 *	For PIXCI(R) A, CL1, CL2, CL3SD, D, D24, D32, D2X, D3X, D3XE,
 *	E1, E1DB, E4, E4DB, E4G2, E4TX2, E8, E8CAM, E8DB, e104x4, EB1, EB1G2, EB1-POCL, EB1G2-POCL,
 *	EB1mini, miniH2B, miniH2F, miniH2x4F, mf2280, EC1, ECB1, ECB1-34, ECB2, EL1, EL1DB, ELS2, SI, SI1, SI2, and SI4
 *	frame grabbers, use "default" to select the default format for
 *	the camera for which the PIXCI(R) frame grabber is intended.
 *	For non default formats, use XCAP to save the video setup to
 *	a file, and set FORMATFILE_LOAD to the saved file's path name.
 *	For camera's with RS-232 control, note that the saved
 *	video setup only resets the PIXCI(R) frame grabber's
 *	settings, but XCLIB does not reset the camera's settings.
 *	For selected Camera Link cameras w. serial controls,
 *	the video setup file may include serial commands which are
 *	automatically sent by XCLIB to the camera.
 *
 *	Alternately, this could be modified to use getenv("PIXCI"),
 *	GetPrivateProfileString(...), RegQueryValueEx(...), or any
 *	other convention chosen by the programmer to allow run time
 *	selection of the video format and resolution.
 *
 */
#ifdef UNICODE
#undef UNICODE
#endif

#ifdef _UNICODE
#undef _UNICODE
#endif


#if !defined(FORMAT) && !defined(FORMATFILE_LOAD) && !defined(FORMATFILE_COMP)
				  // For PIXCI(R) SV2, SV3, SV4, SV5, SV5A, SV5B, SV5L
  //#define FORMAT  "RS-170"	  // RS-170 on input 2
  //#define FORMAT  "NTSC"	  // NTSC on input 2
  //#define FORMAT  "NTSC/YC"	  // NSTC S-Video on input 1		  (N/A on SV5A,SV5B)
  //#define FORMAT  "CCIR"	  // CCIR on input 2
  //#define FORMAT  "PAL"	  // PAL (B,D,G,H,I) on input 2
  //#define FORMAT  "PAL/YC"	  // PAL (B,D,G,H,I) S-Video on input 1   (N/A on SV5A,SV5B)
  //#define FORMAT  "default"	  // NSTC S-Video on input 1

				  // For PIXCI(R) SV7
  //#define FORMAT  "RS-170"	  // RS-170
  //#define FORMAT  "NTSC"	  // NTSC
  //#define FORMAT  "CCIR"	  // CCIR
  //#define FORMAT  "PAL"	  // PAL
  //#define FORMAT  "default"	  // NSTC

				  // For PIXCI(R) SV8
  //#define FORMAT  "RS-170"	  // RS-170 on BNC 0
  //#define FORMAT  "NTSC"	  // NTSC on BNC 0
  //#define FORMAT  "NTSC/YC"	  // NSTC S-Video
  //#define FORMAT  "CCIR"	  // CCIR on BNC 0
  //#define FORMAT  "PAL"	  // PAL on BNC 0
  //#define FORMAT  "PAL/YC"	  // PAL (B,D,G,H,I) S-Video
  //#define FORMAT  "default"	  // NSTC on BNC 0

				  // For PIXCI(R) A310
  //#define FORMAT  "default"	  // as preset in board's eeprom
  //#define FORMAT  "RS-170"	  // RS-170
  //#define FORMAT  "CCIR"	  // CCIR
  //#define FORMAT  "Video 720x480i 60Hz"	// RS-170
  //#define FORMAT  "Video 720x480i 60Hz RGB"
  //#define FORMAT  "Video 720x576i 50Hz"	// CCIR
  //#define FORMAT  "Video 720x576i 50Hz RGB"
  //#define FORMAT  "Video 640x480i 60Hz"	// RS-170
  //#define FORMAT  "Video 640x480i 60Hz RGB"
  //#define FORMAT  "Video 768x576i 50Hz"	// CCIR
  //#define FORMAT  "Video 768x576i 50Hz RGB"
  //#define FORMAT  "Video 1920x1080i 60Hz"
  //#define FORMAT  "Video 1920x1080i 60Hz RGB"
  //#define FORMAT  "Video 1920x1080i 50Hz"
  //#define FORMAT  "Video 1920x1080i 50Hz RGB"
  //#define FORMAT  "Video 1920x1080p 60Hz"
  //#define FORMAT  "Video 1920x1080p 60Hz RGB"
  //#define FORMAT  "Video 1920x1080p 50Hz"
  //#define FORMAT  "Video 1920x1080p 50Hz RGB"
  //#define FORMAT  "Video 1280x720p 50Hz"
  //#define FORMAT  "Video 1280x720p 50Hz RGB"
  //#define FORMAT  "Video 1280x720p 60Hz"
  //#define FORMAT  "Video 1280x720p 60Hz RGB"
  //#define FORMAT  "SVGA 800x600 60Hz RGB"
  //#define FORMAT  "SXGA 1280x1024 60Hz RGB"
  //#define FORMAT  "VGA 640x480 60Hz RGB"
  //#define FORMAT  "XGA 1024x768 60Hz RGB"
  //#define FORMAT  "RS343 875i 60Hz"
  //#define FORMAT  "RS343 875i 60Hz RGB"
  //#define FORMAT  "RS330 720x480i 60Hz"

				  // For PIXCI(R) A110
  //#define FORMAT  "default"	  // as preset in board's eeprom
  //#define FORMAT  "RS-170"	  // RS-170
  //#define FORMAT  "CCIR"	  // CCIR
  //#define FORMAT  "Video 720x480i 60Hz"	// RS-170
  //#define FORMAT  "Video 720x576i 50Hz"	// CCIR
  //#define FORMAT  "Video 640x480i 60Hz"	// RS-170
  //#define FORMAT  "Video 768x576i 50Hz"	// CCIR
  //#define FORMAT  "Video 1920x1080i 60Hz"
  //#define FORMAT  "Video 1920x1080i 50Hz"
  //#define FORMAT  "Video 1920x1080p 60Hz"
  //#define FORMAT  "Video 1920x1080p 50Hz"
  //#define FORMAT  "Video 1280x720p 50Hz"
  //#define FORMAT  "Video 1280x720p 60Hz"
  //#define FORMAT  "RS343 875i 60Hz"
  //#define FORMAT  "RS330 720x480i 60Hz"

				  // For PIXCI(R) A, CL1, CL2, CL3SD, D, D24, D32,
				  // D2X, D3X, D3XE, E1, E1DB, E4, E4DB, E4G2, E4TX2,
				  // E8, E8CAM, E8DB, e104x4, EB1, EB1G2, EB1-POCL, EB1G2-POCL,
				  // EB1mini, miniH2B, miniH2F, miniH2x4F, mf2280,
				  // EC1, ECB1, ECB1-34, ECB2, EL1, EL1DB,
				  // ELS2, SI, SI1, SI2, SI4
    // #define FORMAT  "default"	  // as per board's intended camera

				  // For any PIXCI(R) frame grabber
				  // using a format file saved by XCAP.
				  // This may be a file name in the default
				  // directory, or a full pathname and
				  // can either be either ..
  #define FORMATFILE_LOAD   "xcvidset.fmt"  // .. loaded from file during execution
  #define FORMATFILE_COMP   "xcvidset.fmt"  // .. or compiled into this application
#endif


/*
 *  2.1) Set number of expected PIXCI(R) frame grabbers, from 1 to 4.
 *  This example expects that the boards are
 *  identical and operated at the same resolution.
 *
 *  For PIXCI(R) frame grabbers with multiple, functional units,
 *  the XCLIB presents the two halves of the PIXCI(R) E1DB, E4DB, E4G2-2F, E4TX2-2F,
 *  E8CAM, E8DB, e104x4-2f, ECB2, EL1DB, ELS2, miniH2B, SI2, or SV7 frame grabbers,
 *  or the three parts of the PIXCI(R) E4G2-F2B, E4TX2-F2B, or e104x4-f2b frame grabbers,
 *  or the four quarters of the PIXCI(R) E4G2-4B, E4TX2-4B, e104x4-4b, or SI4 frame grabbers,
 *  as two, three, or four independent PIXCI(R) frame grabbers, respectively.
 *
 */
#if !defined(UNITS)
    #define UNITS	1
#endif
#define UNITSMAP    ((1<<UNITS)-1)  /* shorthand - bitmap of all units */
#if !defined(UNITSOPENMAP)
    #define UNITSOPENMAP UNITSMAP
#endif


/*
 *  2.2) Optionally, set driver configuration parameters.
 *  These are normally left to the default, "".
 *  The actual driver configuration parameters include the
 *  selected PIXCI(R) frame grabbers; to simplify
 *  configuration, code (below) will add the UNIT/UNITSMAP
 *  selection (above) to the driver configuration parameters.
 */
#if !defined(DRIVERPARMS)
  //#define DRIVERPARMS "-QU 0"   // don't use interrupts
    #define DRIVERPARMS ""	  // default
#endif


/*
 *  3.1) Choose which form of image display (SHOWIM_), or image and
 *  graphics overlay (SHOWIG_), is to be demonstrated. Some of these
 *  options expect that the optional PXIPL library is present.
 *  Others may expect that the Windows DirectDraw SDK is present
 *  (available from Microsoft) and that the S/VGA supports DirectDraw.
 *
 *  The _DIRECTVIDEO options send video directly from compatible
 *  PIXCI(R) imaging cards to compatible S/VGA cards, rather
 *  than capturing to PC memory and displaying therefrom.
 *  Only SV2, SV3, SV4, SV5, SV5A, SV5B, SV5L, SV7, SV8 models can use these modes,
 *  and only with compatible S/VGA cards. Further, some S/VGA cards
 *  may support direct video but not support overlay graphics.
 *  Within this example, these modes currently support only a single
 *  frame grabber.
 *
 *  Only one of these choices should have value 1, the others should be 0.
 */
#if !defined(SHOWIM_STRETCHDIBITS)  && !defined(SHOWIG_STRETCHDIBITS)  && !defined(SHOWIM_STRETCHDIBITS2) \
 && !defined(SHOWIG_STRETCHDIBITS2) && !defined(SHOWIG_STRETCHDIBITS3) && !defined(SHOWIM_DRAWDIBDRAW)	  \
 && !defined(SHOWIM_DRAWDIBDISPLAY) && !defined(SHOWIM_GDIDISPLAY)     && !defined(SHOWIM_GDIDISPLAY2)	  \
 && !defined(SHOWIG_GDIDISPLAY)     && !defined(SHOWIM_DIRECTXDISPLAY) && !defined(SHOWIG_DIRECTVIDEO)	  \
 && !defined(SHOWIM_DIRECTVIDEO)

    #define SHOWIM_STRETCHDIBITS    1	// use XCLIB or XCLIB-Lite and GDI
    #define SHOWIG_STRETCHDIBITS    0	// use XCLIB or XCLIB-Lite and GDI
    #define SHOWIM_STRETCHDIBITS2   0	// use XCLIB or XCLIB-Lite and GDI
    #define SHOWIG_STRETCHDIBITS2   0	// use XCLIB or XCLIB-Lite and GDI
    #define SHOWIG_STRETCHDIBITS3   0	// use XCLIB, PXIPL and GDI
    #define SHOWIM_DRAWDIBDRAW	    0	// use XCLIB or XCLIB-Lite and Video for Windows
    #define SHOWIM_DRAWDIBDISPLAY   0	// use XCLIB and PXIPL and Video for Windows
    #define SHOWIM_GDIDISPLAY	    0	// use XCLIB and PXIPL
    #define SHOWIM_GDIDISPLAY2	    0	// use XCLIB and PXIPL
    #define SHOWIG_GDIDISPLAY	    0	// use XCLIB and PXIPL
    #define SHOWIM_DIRECTXDISPLAY   0	// use XCLIB and PXIPL and DirectDraw
    #define SHOWIG_DIRECTVIDEO	    0	// use XCLIB or XCLIB-Lite, DirectDraw, and S/VGA with support
					// for compatible pixel formats
    #define SHOWIM_DIRECTVIDEO	    0	// use XCLIB or XCLIB-Lite, DirectDraw, and S/VGA with support
					// for compatible pixel formats and overlay graphics
#endif


/*
 *  3.2) Optionally choose to show a pixel line profile
 *  graph, of the middle line of the first frame grabber.
 *  If used, the number of UNITS should be limited to 3,
 *  as the graph is drawn in the space otherwise used by
 *  the fourth board.
 *
 *  This feature is incompatible with SHOWIG_DIRECTVIDEO
 *  and SHOWIM_DIRECTVIDEO options, above.
 */
#if !defined(SHOW_GRAPH)
    #define SHOW_GRAPH	    0
#endif


/*
 *  3.3) Select how 'Live' mode should be demonstrated.
 *  Only one of these choices should have value 1, the others should be 0.
 *
 *  These options don't apply if the SHOWIG_DIRECTVIDEO
 *  or SHOWIM_DIRECTVIDEO options, above, are used.
 */
#if !defined(LIVE_LIVE) && !defined(LIVE_SNAP) && !defined(LIVE_LIVE2)

    #define LIVE_LIVE	    1	// Continuous capture into frame buffer,
				// display from same frame buffer while
				// next frame is being captured.
    #define LIVE_SNAP	    0	// Snap single frame into frame buffer,
				// display, repeat.
    #define LIVE_LIVE2	    0	// Continuous capture into alternate buffers,
				// display from frame buffer which is not being
				// captured into. Requires sufficient frame buffer
				// memory for at least two frame buffers.
#endif


/*
 *  3.4) Select how display updates should be triggered.
 *  Only one of these choices should have value 1, the others should be 0.
 */
#if !defined(UPDATE_TIMER) && !defined(UPDATE_EVENT)
    #define UPDATE_TIMER    1	// Use timer to periodically poll library
				// and test for a new image to be displayed.
				// Simpler for those not familiar with threads
				// and events.
    #define UPDATE_EVENT    0	// Use an event to trigger display of new image.
				// Slightly more efficient, but requires use of
				// threads and events.
#endif

/*
 *  3.5) Optionally, report and verify how long each snap takes.
 *  Only available with UPDATE_EVENT.
 *  In free-run mode, a snap will require 1 to 2 frame periods;
 *  up to one frame period to wait for the next start of frame,
 *  then one frame period to capture.
 */
#if !defined(SNAP_STATISTICS)
    #define SNAP_STATISTICS	 1
#endif

/*
 *  3.5) Option to report video & capture statistics as a field count
 *  instead of a rate.
 */
#if !defined(STATS_FIELDCOUNT)
    #define STATS_FIELDCOUNT	0
#endif

/*
 *  3.6) Option as to whether the 'Save' feature will use
 *  an auto generated file name derived from a time stamp,
 *  or prompt for a file name.
 */
#if !defined(SAVE_PROMPT)
    #define SAVE_PROMPT     0
#endif




/*
 *  4)	Compile
 *	    XCLIBEX2.CPP
 *	in the same manner as any Win32 or Win64 application,
 *	using resource
 *	    XCLIBEX2.RC
 *	with include files located at
 *	    c:\Program Files\EPIX\XCLIB\Inc
 *	(or other location, dependent on where XCLIB was installed)
 *	and linking to (one of)
 *	    XCLIBWNT.LIB   (XCLIB for Win32 apps)
 *	    XCLIBW64.LIB   (XCLIB for Win64 apps)
 *	    XCLYBWNT.LIB   (XCLIB-Lite for Win32 apps)
 *	    XCLYBW64.LIB   (XCLIB-Lite for Win64 apps)
 *	    XCLIBWNT.LIB & PXIPLWNT.LIB   (XCLIB+PXIPL for Win32 apps)
 *	    XCLIBW64.LIB & PXIPLW64.LIB   (XCLIB+PXIPL for Win64 apps)
 *
 *	Note: Do not enable strict ANSI C++ compatibility;
 *	XCLIB expects pre-defined #defines to indicate the host environment.
 *	Select ASCII character set, not Unicode or Multi-Byte.
 *	Compiling as a 64 bit app and linking to a 32 bit DLL,
 *	or vice versa, is a common source of errors.
 *
 */


/*
 *  NECESSARY INCLUDES:
 */
#define _CRT_SECURE_NO_DEPRECATE    1	// so newer Microsoft compilers don't
					// complain about legitimate ANSI C functions

#include <windows.h>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#if SHOWIM_DRAWDIBDRAW || SHOWIM_DRAWDIBDISPLAY
    #include <vfw.h>
#endif
#if SHOWIM_DIRECTXDISPLAY
    #include <compobj.h>
    #include <ddraw.h>
#endif
#if 1
    #include <time.h>
#endif

extern "C" {
    #include "xcliball.h"
    #include "xclibex2.h"
    #if SHOWIM_GDIDISPLAY || SHOWIG_GDIDISPLAY || SHOWIM_DIRECTXDISPLAY || SHOWIG_STRETCHDIBITS3 || SHOWIM_GDIDISPLAY2 || SHOWIM_DRAWDIBDISPLAY
	#include "pxipl.h"         
	#include "pximages.h"           
    #endif
}

/*
 * To improve readability when translating parameters,
 * such as gain, offset, etc., to/from scrollbar controls.
 */
#define ToScroll(Value, LoV, HiV)   (int)(((Value)-(LoV))*100./((HiV)-(LoV)))
#define UnScroll(Scroll, LoV, HiV)  ((LoV)+(double)(Scroll)*((HiV)-(LoV))/100.)


/*
 * Utility for SV 2/3/4/5.
 * Get current multiplexor input, and set controls.
 */
void setGuiMux(HWND hDlg)
{
    int   vidmux = pxd_getVidMux(1);

    SendMessage(GetDlgItem(hDlg, IDSVIDEO), BM_SETCHECK, vidmux == 1, 0);
    SendMessage(GetDlgItem(hDlg, IDBNC1),   BM_SETCHECK, vidmux == 2, 0);
    SendMessage(GetDlgItem(hDlg, IDBNC2),   BM_SETCHECK, vidmux == 3, 0);
    SendMessage(GetDlgItem(hDlg, IDBNC3),   BM_SETCHECK, vidmux == 4, 0);
    SendMessage(GetDlgItem(hDlg, IDBNC4),   BM_SETCHECK, vidmux == 5, 0);
}

/*
 * Utilities for SV2/3/4/5/7/A310/A110.
 * Get/set current brightness/offset and gain/contrast.
 * For the A310 & A110, only the analog offset & gain are set
 * by this example.
 * These translate the XCLIB's generic floating, scaled,
 * values to integers between 0 & 100, so they will be
 * useful for scroll bars.
 */
int getBright()
{
    switch (pxd_infoModel(UNITSMAP)) {
      case PIXCI_SV8:
	return(ToScroll(pxd_getBrightness(1), -100.0, 100.0));
      case PIXCI_SV7:
      //return((int)(50+pxd_getBrightness(1)*100/(76*2)));
	return(ToScroll(pxd_getBrightness(1), -76.22, 75.64));
      case PIXCI_A310:
      case PIXCI_A110:
      case PIXCI_A1107I1O:
	return(ToScroll(pxd_getAdcOffsetA(1), -124/1024., 124/1024.));
      default:
	//return((int)(pxd_getBrightness(1)+50));
	return(ToScroll(pxd_getBrightness(1), -50.0, 49.61));
    }
}
void setBright(int b)
{
    int err = 0;
    switch (pxd_infoModel(UNITSMAP)) {
      case PIXCI_SV8:
	err = pxd_setContrastBrightness(UNITSMAP, pxd_getContrast(1), UnScroll(b, -100.0, 100.0));
	break;
      case PIXCI_SV7:
      //err = pxd_setContrastBrightness(UNITSMAP, pxd_getContrast(1), (b-50)*76*2/100);
	err = pxd_setContrastBrightness(UNITSMAP, pxd_getContrast(1), UnScroll(b, -76.22, 75.64));
	break;
      case PIXCI_A310:
      case PIXCI_A110:
      case PIXCI_A1107I1O:
	err = pxd_setAdcGainOffset(UNITSMAP, 0, pxd_getAdcGainA(1), UnScroll(b, -124/1024., 124/1024.), pxd_getAdcGainB(1), pxd_getAdcOffsetB(1));
	break;
      default:
      //err = pxd_setContrastBrightness(UNITSMAP, pxd_getContrast(1), b-50);
	err = pxd_setContrastBrightness(UNITSMAP, pxd_getContrast(1), UnScroll(b, -50.0, 49.61));
	break;
    }
    if (err < 0)
	MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_setContrastBrightness", MB_OK|MB_TASKMODAL);
}
int getGain()
{
    switch (pxd_infoModel(UNITSMAP)) {
      case PIXCI_SV8:
	return(ToScroll(pxd_getContrast(1), 0.0, 200.0));
      case PIXCI_SV7:
      //return((int)(pxd_getContrast(1)/2));
	return(ToScroll(pxd_getContrast(1), 0.0, 199.22));
      case PIXCI_A310:
      case PIXCI_A110:
      case PIXCI_A1107I1O:
	return(ToScroll(pxd_getAdcGainA(1), 0, 6));
      default:
      //return((int)(pxd_getContrast(1)*100./237.07));
	return(ToScroll(pxd_getContrast(1), 0.0, 237.07));
    }
}
void setGain(int b)
{
    int err = 0;
    switch (pxd_infoModel(UNITSMAP)) {
      case PIXCI_SV8:
	err = pxd_setContrastBrightness(UNITSMAP, UnScroll(b, 0.0, 200.0), pxd_getBrightness(1));
	break;
      case PIXCI_SV7:
      //err = pxd_setContrastBrightness(UNITSMAP, b*2, pxd_getBrightness(1)*2);
	err = pxd_setContrastBrightness(UNITSMAP, UnScroll(b, 0.0, 199.22), pxd_getBrightness(1));
	break;
      case PIXCI_A310:
      case PIXCI_A110:
      case PIXCI_A1107I1O:
	err = pxd_setAdcGainOffset(UNITSMAP, 0, UnScroll(b, 0, 6), pxd_getAdcOffsetA(1), pxd_getAdcGainB(1), pxd_getAdcOffsetB(1));
	break;
      default:
      //err = pxd_setContrastBrightness(UNITSMAP, (b*237.07/100.), pxd_getBrightness(1));   // + 255 for rounding!
	err = pxd_setContrastBrightness(UNITSMAP, UnScroll(b, 0.0, 237.07), pxd_getBrightness(1));
    }
    if (err < 0)
	MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_setContrastBrightness", MB_OK|MB_TASKMODAL);
}
int getHue()
{
    switch (pxd_infoModel(UNITSMAP)) {
      case PIXCI_SV8:
	return(ToScroll(pxd_getHue(UNITSMAP), -90.0, +90.0));
      case PIXCI_SV7:
	return(ToScroll(pxd_getHue(UNITSMAP), -45.0, +45.0));
      case PIXCI_A310:
      case PIXCI_A110:
      case PIXCI_A1107I1O:
	return(0);
      default:
	return(ToScroll(pxd_getHue(UNITSMAP), -90.0, +89.3));
    }
}
void setHue(int h)
{
    int err = 0;
    switch (pxd_infoModel(UNITSMAP)) {
      case PIXCI_SV8:
	err = pxd_setHueSaturation(UNITSMAP, UnScroll(h, -90.0, +90.0), pxd_getUGain(UNITSMAP), pxd_getVGain(UNITSMAP));
	break;
      case PIXCI_SV7:
	err = pxd_setHueSaturation(UNITSMAP, UnScroll(h, -45.0, +45.0), pxd_getUGain(UNITSMAP), pxd_getVGain(UNITSMAP));
	break;
      case PIXCI_A310:
      case PIXCI_A110:
      case PIXCI_A1107I1O:
	break;
      default:
	err = pxd_setHueSaturation(UNITSMAP, UnScroll(h, -90.0, +89.3), pxd_getUGain(UNITSMAP), pxd_getVGain(UNITSMAP));
	break;
    }
    if (err < 0)
	MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_setHueSaturation", MB_OK|MB_TASKMODAL);
}

/*
 * Utilities for optional reporting of capture statistics
 */
#if defined(SNAP_STATISTICS)
double QueryPerformanceMillis()
{
    LARGE_INTEGER   pf = {0,0};
    LARGE_INTEGER   pc = {0,0};

    QueryPerformanceFrequency(&pf);
    QueryPerformanceCounter(&pc);

    __int64 f = (((unsigned __int64)pf.HighPart&0xFFFFFFFF)<<32) | ((unsigned __int64)pf.LowPart&0xFFFFFFFF);
    __int64 c = (((unsigned __int64)pc.HighPart&0xFFFFFFFF)<<32) | ((unsigned __int64)pc.LowPart&0xFFFFFFFF);
    if (f == 0)
	return(0);
    return( (c*1.0E3)/f );
}
#endif


/*
 * Global variables.
 */
static	HWND	    hWnd;				// the main window
static	HWND	    hDlg;				// the main dialog
static	HWND	    hWndImage;				// child window of dialog for image display
static	pxvbtime_t  lastcapttime[UNITS] = {0};		// when was image last captured
static	struct	    pxywindow windImage[std::max(4,UNITS)];	// subwindow of child window for image display
static	ulong	    displaycount = 0;			// display rate statistics
static	CRITICAL_SECTION critsect;			// for use with Events
static	int	    liveon = 0;
static	UINT	    svgaBits;				// pixel format of S/VGA
static	DWORD	    unliveclick = 0;
/*
 * For optional reporting of capture statistics
 */
static	double	    snapTimeStart;
static	double	    snapTime = 0.0;
static	double	    snapTimeMax = 0.0;
static	double	    snapTimeMin = 99E99;
static	double	    fieldTimeStart;
static	double	    fieldTime = 0.0;
static	double	    fieldTimeMax = 0.0;
static	double	    fieldTimeMin = 99E99;
static	double	    triggerTimeStart;
static	double	    triggerTime = 0.0;
static	double	    triggerTimeMax = 0.0;
static	double	    triggerTimeMin = 99E99;
static	DWORD	    tickInit;


/*
 * Subroutine to display image.
 * Could be in-line, once, if this were hard-coded
 * for either UPDATE_TIMER vs UPDATE_EVENT.
 * Since we have those options, a subroutine avoids code
 * duplication.
 */
void displayImage(int force)
{
    int err = 0;
    //
    // Has a new field or frame been captured
    // since the last time we checked?
    // This check is redundant when using Events
    // (at least with a single unit) but it doesn't hurt.
    //
    for (int u = 0; u < UNITS; u++) {
	HDC  hDC;
	pxvbtime_t lasttime = pxd_capturedFieldCount(1<<u);
	if (lastcapttime[u] == lasttime && !force)
	    continue;
	lastcapttime[u] = lasttime;

	//
	// In SHOWIG_DIRECTVIDEO mode, display is
	// already done.
	//
	#if SHOWIG_DIRECTVIDEO | SHOWIM_DIRECTVIDEO
	    continue;
	#endif

	hDC = GetDC(hWndImage);

	//
	// In LIVE_LIVE2 mode the captured buffer
	// rotates between 1 and 2.
	// In other modes, the code above always
	// does snap or live into buffer 1.
	//
	pxbuffer_t  buf = 1;
	#if LIVE_LIVE2
	    buf = pxd_capturedBuffer(1<<u);
	    if (buf == 0)   // nothing ever captured?
		buf = 1;
	#endif

	//
	// Show image using pxd_renderStretchDIBits.
	//
	#if SHOWIM_STRETCHDIBITS
	    SetStretchBltMode(hDC, STRETCH_DELETESCANS);
	    err = pxd_renderStretchDIBits(1<<u, buf, 0, 0, -1, -1, 0,
			     hDC, windImage[u].nw.x, windImage[u].nw.y,
			     windImage[u].se.x-windImage[u].nw.x,
			     windImage[u].se.y-windImage[u].nw.y, 0);
	    if (err < 0)
		MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_renderStretchDIBits", MB_OK|MB_TASKMODAL);
	#endif

	//
	// Show image using pxd_renderStretchDIBits
	// and graphics using GDI.
	// As this draws graphics after the image, a fast
	// update rate may cause the graphics to appear to flicker
	// (depending on the vga speed, vga display rate, etc).
	//
	#if SHOWIG_STRETCHDIBITS
	{
	    RECT rect;
	    SetStretchBltMode(hDC, STRETCH_DELETESCANS);
	    err = pxd_renderStretchDIBits(1<<u, buf, 0, 0, -1, -1, 0,
			      hDC, windImage[u].nw.x, windImage[u].nw.y,
			      windImage[u].se.x-windImage[u].nw.x,
			      windImage[u].se.y-windImage[u].nw.y, 0);
	    if (err < 0)
		MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_renderStretchDIBits", MB_OK|MB_TASKMODAL);
	    SetRect(&rect, windImage[u].nw.x+(windImage[u].se.x-windImage[u].nw.x)/4,
			   windImage[u].nw.y+(windImage[u].se.y-windImage[u].nw.y)/4,
			   windImage[u].nw.x+(windImage[u].se.x-windImage[u].nw.x)*3/4,
			   windImage[u].nw.y+(windImage[u].se.y-windImage[u].nw.y)*3/4);
	    DrawFocusRect(hDC, &rect);
	    //
	    // This, optionally, slows down the image update rate, reducing flicker.
	    //
	    lastcapttime[u] += 10;
	}
	#endif

	//
	// Show image using pxd_renderDIBCreate and StretchDIBits.
	// This may be less efficient than pxd_renderStretchDIBits,
	// but would allow the application to modify the bitmap
	// before display, using any of the many Windows functions
	// that operate on a DIB.
	//
	#if SHOWIM_STRETCHDIBITS2
	{
	    HGLOBAL hDIB;
	    BITMAPINFOHEADER FAR *dib;

	    hDIB = pxd_renderDIBCreate(1<<u, buf, 0, 0, -1, -1, 0, 0);
	    if (hDIB) {
		if (dib = (BITMAPINFOHEADER FAR *)GlobalLock(hDIB)) {
		    SetStretchBltMode(hDC, STRETCH_DELETESCANS);
		    StretchDIBits(hDC, windImage[u].nw.x, windImage[u].nw.y,
				       windImage[u].se.x-windImage[u].nw.x, windImage[u].se.y-windImage[u].nw.y,
				       0, 0, pxd_imageXdim(), pxd_imageYdim(),
				(uchar FAR*)dib+dib->biSize+dib->biClrUsed*sizeof(RGBQUAD),
				(BITMAPINFO *)dib, DIB_RGB_COLORS, SRCCOPY);
		    GlobalUnlock(hDIB);
		}
		pxd_renderDIBFree(hDIB);
	    } else
		MessageBox(NULL, "Error", "pxd_renderDIBCreate", MB_OK|MB_TASKMODAL);
	}
	#endif


	//
	// Show image using pxd_renderDIBCreate and graphics using GDI.
	// Uses an off-screen buffer so the displayed
	// graphics do not flicker. Note that the graphics
	// are drawn in (a copy of) the image before resizing,
	// thus the graphics are resized.
	// It also end up performing one extra copying of the image,
	// from the DIB to the DDB.
	//
	#if SHOWIG_STRETCHDIBITS2
	{
	    HGLOBAL hDIB = pxd_renderDIBCreate(1<<u, buf, 0, 0, -1, -1, 0, 0);
	    if (hDIB) {
		BITMAPINFOHEADER FAR *dib = (BITMAPINFOHEADER FAR *)GlobalLock(hDIB);
		if (dib) {
		    HBITMAP hDIB2 = CreateDIBitmap(hDC, dib, CBM_INIT,
				(uchar FAR*)dib+dib->biSize+dib->biClrUsed*sizeof(RGBQUAD),
				(BITMAPINFO*)dib, DIB_RGB_COLORS);
		    if (hDIB2) {
			HDC hdcDIB = CreateCompatibleDC(hDC);
			if (hdcDIB) {
			    HGDIOBJ  ohDIB = SelectObject(hdcDIB, hDIB2);
			    TextOut(hdcDIB, 20, 20, "This is text", 12);
			    SetStretchBltMode(hDC, STRETCH_DELETESCANS);
			    StretchBlt(hDC, windImage[u].nw.x, windImage[u].nw.y,
					    windImage[u].se.x-windImage[u].nw.x,
					    windImage[u].se.y-windImage[u].nw.y,
				    hdcDIB, 0, 0, pxd_imageXdim(), pxd_imageYdim(), SRCCOPY);
			    SelectObject(hdcDIB, ohDIB);
			    DeleteDC(hdcDIB);
			}
			DeleteObject(hDIB2);
		    }
		    GlobalUnlock(hDIB);
		}
		pxd_renderDIBFree(hDIB);
	    } else
		MessageBox(NULL, "Error", "pxd_renderDIBCreate", MB_OK|MB_TASKMODAL);
	}
	#endif

	//
	// Show image using pxio8_GDIDisplay and graphics using GDI.
	// Uses an off-screen buffer so the displayed
	// graphics do not flicker. Note that the graphics
	// are drawn in (a copy of) the image after resizing,
	// thus the graphics are not resized, yielding crisper graphics.
	//
	#if SHOWIG_STRETCHDIBITS3
	{
	    HBITMAP hDIB = NULL;
	    HGDIOBJ hODIB;
	    HDC     hDCDIB;
	    RECT    rect;
	    struct  pxywindow	windDIB;

	    //
	    // Create HBITMAP as off-screen buffer.
	    // And create DC for it.
	    //
	    hDIB = CreateCompatibleBitmap(hDC, windImage[u].se.x-windImage[u].nw.x,
					       windImage[u].se.y-windImage[u].nw.y);
	    hDCDIB = CreateCompatibleDC(hDC);
	    hODIB = SelectObject(hDCDIB, hDIB);

	    //
	    // Draw image into it, resizing.
	    // The "Display" selection is equivalent to using
	    //	   pxd_imageCdim()==1?"GREY":"BGR"
	    //
	    windDIB.nw.x = 0;
	    windDIB.nw.y = 0;
	    windDIB.se.x = windImage[u].se.x-windImage[u].nw.x;
	    windDIB.se.y = windImage[u].se.y-windImage[u].nw.y;
	    err = pxio8_GDIDisplay(NULL, pxd_defineImage(1<<u, buf, 0, 0, -1, -1, "Display"),
			    NULL, 0, 'n', 0, 0, hDCDIB, &windDIB, // not &windImage[u]!
			    NULL, NULL);
	    if (err < 0)
		MessageBox(NULL, pxd_mesgErrorCode(err), "pxio8_GDIDisplay", MB_OK|MB_TASKMODAL);

	    //
	    // Use GDI to draw graphics over image.
	    //
	    SetRect(&rect, windImage[u].nw.x+(windImage[u].se.x-windImage[u].nw.x)/4,
			   windImage[u].nw.y+(windImage[u].se.y-windImage[u].nw.y)/4,
			   windImage[u].nw.x+(windImage[u].se.x-windImage[u].nw.x)*3/4,
			   windImage[u].nw.y+(windImage[u].se.y-windImage[u].nw.y)*3/4);
	    DrawFocusRect(hDCDIB, &rect);
	    DrawsText(hDCDIB, "This is text", 12, &rect, 0);

	    //
	    // Display image & graphics.
	    //
	    BitBlt(hDC, windImage[u].nw.x, windImage[u].nw.y,
			windImage[u].se.x-windImage[u].nw.x,
			windImage[u].se.y-windImage[u].nw.y,
		   hDCDIB, 0, 0, SRCCOPY);

	    SelectObject(hDCDIB, hODIB);
	    DeleteDC(hDCDIB);
	    DeleteObject(hDIB);
	}
	#endif


	//
	// Show image using PXIPL pxio8_GDIDisplay.
	// Altho not used in this example, pxio8_GDIDisplay also allows drawing
	// a full screen cross hair cursor over the image.
	//
	#if SHOWIM_GDIDISPLAY
	    err = pxio8_GDIDisplay(NULL, pxd_defineImage(1<<u, buf, 0, 0, -1, -1, "Display"),
			     NULL, 0, 'n', 0, 0,hDC,&windImage[u], NULL, NULL);
	    if (err < 0)
		MessageBox(NULL, pxd_mesgErrorCode(err), "pxio8_GDIDisplay", MB_OK|MB_TASKMODAL);
	#endif

	//
	// Show image using PXIPL pxio8_GDIDisplay with cursor overlay.
	//
	#if SHOWIM_GDIDISPLAY2
	{
	    struct  pximage cursimage;
	    void    *	    cursbuf = NULL;
	    uint    color[3] = {255, 0, 255};
	    struct  pxy     xy;
	    int     r;

	    //
	    // Obtain buffer and pximage
	    // to hold cursor data; i.e. the pixel values
	    // that will form the cursor.
	    //
	    xy.x =   (windImage[u].se.x-windImage[u].nw.x)
		   * (windImage[u].se.y-windImage[u].nw.y);
	    xy.y = 1;
	    r = pximage_memmalloc(&cursimage, &cursbuf, &xy, PXDATUCHAR, 8, 3, PXHINTBGR);
	    if (r >= 0) {
		//
		// Init cursor pixel values.
		//
		pxip8_pixset3(NULL, &cursimage, color);

		//
		// Display image, with cursor in middle
		//
		xy.x = pxd_imageXdim()/2;
		xy.y = pxd_imageYdim()/2;
		err = pxio8_GDIDisplay(NULL, pxd_defineImage(1<<u, buf, 0, 0, -1, -1, "Display"),
				 NULL, 0, 'n', 0, 0,hDC, &windImage[u], &xy, &cursimage);
		if (err < 0)
		    MessageBox(NULL, pxd_mesgErrorCode(err), "pxio8_GDIDisplay", MB_OK|MB_TASKMODAL);

		//
		// The cursimage not contains the pixel values which were
		// overwritten by the cursor. The cursor can be removed
		// without redrawing the image by
		//
		// pxio8_GDICursor(NULL, &image[u], 'n', 0, hDC, &windImage[u],  &xy, &cursimage);
		//
		// And then redrawn in a new position by
		//
		// xy.x /= 2;
		// xy.y /= 2;
		// pxio8_GDICursor(NULL, pxd_defineImage(1<<u, 1, 0, 0, -1, -1, "Display"),
		//		   'n', 0, hDC, &windImage[u],	&xy, &cursimage);
		//
		// Although redrawing the cursor here, now, isn't
		// really useful, so it is commented out.
		//

		//
		// All done.
		//
		pximage_memfree(&cursimage, &cursbuf);
	    }
	}
	#endif


	//
	// Show image and graphics using PXIPL pxio8_GDIDisplay.
	// This draws graphics in an off-line, pre-resized, buffer
	// so the displayed graphics aren't resized and don't flicker.
	//
	#if SHOWIG_GDIDISPLAY
	{
	    pximage_s	tempimage;
	    void       *tempbuf = NULL;
	    pxywindow_s box;
	    uint    color[3] = {0, 255, 255};
	    int     r;

	    //
	    // First, acquire a temporary image buffer,
	    // sized to the display dimensions.
	    //
	    r = pximage_memmalloc(&tempimage, &tempbuf, &windImage[u].se, PXDATUCHAR, 8, 3, PXHINTBGR);
	    if (r >= 0) {
		//
		// Next, 'display' the image into the temporary buffer.
		//
		err = pxio8_vgadisplay(NULL, pxd_defineImage(1<<u, buf, 0, 0, -1, -1, pxd_imageCdim()==1?"GREY":"BGR"),
				 NULL, 0, 'n', 0, 0, &tempimage, NULL, NULL);
		if (err < 0)
		    MessageBox(NULL, pxd_mesgErrorCode(err), "pxio8_vgadisplay", MB_OK|MB_TASKMODAL);

		//
		// Prepare to Draw Graphics.
		// Since this is drawn in the already resized image,
		// we map the desired image coordinates into actual
		// display coordinates.
		//
		box.nw.x = pxd_imageXdim()/4;	   // desired
		box.nw.y = pxd_imageYdim()/4;
		box.se.x = pxd_imageXdim()*3/4;
		box.se.y = pxd_imageYdim()*3/4;
		pxio8_vgacoord(pxd_defineImage(1<<u, 1, 0, 0, -1, -1, pxd_imageCdim()==1?"GREY":"BGR"),'n',0, &tempimage, &box.nw);
		pxio8_vgacoord(pxd_defineImage(1<<u, 1, 0, 0, -1, -1, pxd_imageCdim()==1?"GREY":"BGR"),'n',0, &tempimage, &box.se);

		//
		// Draw Graphics.
		// We choose to make the box out of thick dots.
		//
		err = pxip8_drawbox(NULL, &tempimage, &box, 6, 3, 's', color, NULL, NULL);
		if (err < 0)
		    MessageBox(NULL, pxd_mesgErrorCode(err), "pxip8_drawbox", MB_OK|MB_TASKMODAL);

		//
		// Display image and graphics.
		// The image and graphics are already resized,
		// so this draws 1-1.
		//
		err = pxio8_GDIDisplay(NULL, &tempimage, NULL, 0, '1', 0, 0,hDC,&windImage[u],
		     NULL, NULL);
		if (err < 0)
		    MessageBox(NULL, pxd_mesgErrorCode(err), "pxio8_GDIDisplay", MB_OK|MB_TASKMODAL);

		//
		// All done with temp image and its buffer.
		//
		pximage_memfree(&tempimage, &tempbuf);
	    }
	}
	#endif

	//
	// Show image using DirectDraw.
	// Note that this only supports an S/VGA in
	// 24 or 32 bits per pixel mode.
	// This might be slightly quicker if the primary surface
	// were acquired once, and kept till the program exits.
	//
	// This can be extended to draw graphics in
	// the same manner as for pxio8_GDIDisplay, above.
	//
	// The pixel format of the S/VGA may not be acceptable!
	// Error reporting should be added!
	//
	#if SHOWIM_DIRECTXDISPLAY
	{
	    DDSURFACEDESC   surfacedesc;
	    LPDIRECTDRAWSURFACE ddrs = NULL;
	    HRESULT	h;
	    POINT	pt;
	    if (lpDD) {
		//
		// Get primary surface of full screen.
		//
		surfacedesc.dwFlags = DDSD_CAPS;
		surfacedesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE|DDSCAPS_VIDEOMEMORY;
		surfacedesc.dwSize = sizeof(surfacedesc);
		h = lpDD->CreateSurface(&surfacedesc, &ddrs, NULL);
		if (ddrs) {
		    //
		    // Adjust for position of dialog on screen.
		    //
		    pt.x = pt.y = 0;
		    ClientToScreen(hWndImage, &pt);
		    windImage[u].nw.x += pt.x;
		    windImage[u].nw.y += pt.y;
		    windImage[u].se.x += pt.x;
		    windImage[u].se.y += pt.y;
		    err = pxio8_DirectXDisplay(NULL, pxd_defineImage(1<<u, buf, 0, 0, -1, -1, "Display"),
					 NULL, 0, 'n', 0, 0, ddrs, &windImage[u], NULL, hDC, NULL, NULL);
		    windImage[u].nw.x -= pt.x;
		    windImage[u].nw.y -= pt.y;
		    windImage[u].se.x -= pt.x;
		    windImage[u].se.y -= pt.y;
		    ((LPDIRECTDRAWSURFACE)ddrs)->Release();
		}
	    }
	    if (err < 0)
		MessageBox(NULL, pxd_mesgErrorCode(err), "pxio8_DirectXDisplay", MB_OK|MB_TASKMODAL);
	}
	#endif

	//
	// Show image using pxd_renderDIBCreate to create a
	// standard Windows DIB, and display with Video for Windows.
	//
	#if SHOWIM_DRAWDIBDRAW
	    BITMAPINFOHEADER FAR *dib;
	    HGLOBAL hDIB;

	    hDIB = pxd_renderDIBCreate(1<<u, buf, 0, 0, -1, -1, 0, 0);
	    if (hDIB) {
		if (dib = (BITMAPINFOHEADER FAR *)GlobalLock(hDIB)) {
		    DrawDibDraw(hDrawDib, hDC, windImage[u].nw.x, windImage[u].nw.y,
					       windImage[u].se.x-windImage[u].nw.x, windImage[u].se.y-windImage[u].nw.y,
				(BITMAPINFOHEADER *)dib,
				(uchar FAR*)dib+dib->biSize+dib->biClrUsed*sizeof(RGBQUAD),
				0, 0, pxd_imageXdim(), pxd_imageYdim(), 0);
		    GlobalUnlock(hDIB);
		}
		pxd_renderDIBFree(hDIB);
	    } else
		MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_renderDIBCreate", MB_OK|MB_TASKMODAL);
	#endif

	//
	// Show image using PXIPL and Video for Windows.
	// Altho not used in this example, pxio8_DrawDibDisplay also allows drawing
	// a full screen cross hair cursor over the image.
	//
	#if SHOWIM_DRAWDIBDISPLAY
	    err = pxio8_DrawDibDisplay(NULL, pxd_defineImage(1<<u, buf, 0, 0, -1, -1, "Display"),
			     NULL, 0, 'n', 0, 0,hDrawDib,hDC,&windImage[u], NULL, NULL);
	    if (err < 0)
		MessageBox(NULL, pxd_mesgErrorCode(err), "pxio8_DrawDibDisplay", MB_OK|MB_TASKMODAL);
	#endif


	//
	// Optionally, draw a graph of the pixel values
	// from the middle image line. This is primarily
	// intended to demonstrate access to pixel data,
	// rather than to demonstrate use of the Windows GDI.
	//
	// Note that use of pxd_readuchar() is valid
	// even for frame grabbers that have more than 8 bits,
	// letting the XCLIB handle conversion.
	// Also note that having requested RGB data, XCLIB will
	// supply data as requested, even if the frame grabber
	// is outputting monochrome or YUYV.
	//
	if (SHOW_GRAPH) {
	    HBRUSH  hbr;
	    HGDIOBJ hbrold;
	    RECT    rect;
	    uchar   *buffer;

	    hbr = CreateSolidBrush(RGB(0,0,0));
	    hbrold = SelectObject(hDC, hbr);
	    rect.left	= windImage[3].nw.x;
	    rect.top	= windImage[3].nw.y;
	    rect.right	= windImage[3].se.x+1;
	    rect.bottom = windImage[3].se.y+1;
	    FillRect(hDC, &rect, hbr);
	    SelectObject(hDC, hbrold);
	    DeleteObject(hbr);

	    buffer = (uchar*)malloc(sizeof(uchar)*3*pxd_imageXdim());
	    if (buffer) {
		err = pxd_readuchar(1<<0, buf, 0, pxd_imageYdim()/2, -1, 1+pxd_imageYdim()/2, buffer, 3*pxd_imageXdim(), "RGB");
		for (int i = 0; i < pxd_imageXdim(); i++) {
		    SetPixel(hDC,
			     windImage[3].nw.x+((windImage[3].se.x-windImage[3].nw.x)*i)/pxd_imageXdim(),
			     windImage[3].se.y-1-(((windImage[3].se.y-windImage[3].nw.y)*buffer[i*3+0])/255),
			     RGB(255,0,0));
		    SetPixel(hDC,
			     windImage[3].nw.x+((windImage[3].se.x-windImage[3].nw.x)*i)/pxd_imageXdim(),
			     windImage[3].se.y-1-(((windImage[3].se.y-windImage[3].nw.y)*buffer[i*3+1])/255),
			     RGB(0,255,0));
		    SetPixel(hDC,
			     windImage[3].nw.x+((windImage[3].se.x-windImage[3].nw.x)*i)/pxd_imageXdim(),
			     windImage[3].se.y-1-(((windImage[3].se.y-windImage[3].nw.y)*buffer[i*3+2])/255),
			     RGB(0,0,255));
		}
		free(buffer);
	    }
	    if (err < 0)
		MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_readuchar", MB_OK|MB_TASKMODAL);
	}
	ReleaseDC(hWndImage, hDC);

	//
	// After display, if in live mode using LIVE_SNAP,
	// having displayed one image start snapping the next.
	//
	#if LIVE_SNAP
	    if (liveon) {
		err = pxd_goSnap(1<<u, 1);
		if (err < 0)
		    MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_goSnap", MB_OK|MB_TASKMODAL);
	    }
	#endif

	//
	// For display rate statistics.
	//
	if (u == 0)
	    displaycount++;
    }
}

/*
 * Subroutine to enable live video capture,
 * as per options.
 */
void live()
{
    int err = 0;
    #if SHOWIG_DIRECTVIDEO
	if ((err = pxd_renderDirectVideoLive(UNITSMAP, hWndImage, windImage[0].nw.x, windImage[0].nw.y,
				windImage[0].se.x, windImage[0].se.y, CLR_INVALID, CLR_INVALID)) < 0)
	    MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_renderDirectVideoLive", MB_OK|MB_TASKMODAL);
    #elif SHOWIM_DIRECTVIDEO
	if ((err = pxd_renderDirectVideoLive(UNITSMAP, hWndImage, windImage[0].nw.x, windImage[0].nw.y,
			    windImage[0].se.x, windImage[0].se.y, RGB(0,0,189), RGB(0,0,189))) < 0)
	    MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_renderDirectVideoLive", MB_OK|MB_TASKMODAL);
	//
	// If we are using Chroma keying, then any graphics or text
	// written via GDI (i.e. the 'normal' method of putting
	// graphics on the window) will appear, so long as the
	// graphics isn't drawn in the RGB(0,0,189) color.
	//
	HDC hDC;
	if (hDC = GetDC(hWndImage)) {
	    RECT rect;
	    SetRect(&rect, 20, 20, 40, 40);
	    //DrawFocusRect(hDC, &rect);
	    InvertRect(hDC, &rect);
	    ReleaseDC(hWnd, hDC);
	}
    #elif LIVE_LIVE
	err = pxd_goLive(UNITSMAP, 1L);
	if (err < 0)
	    MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_goLive", MB_OK|MB_TASKMODAL);
    #elif LIVE_LIVE2
	err = pxd_goLivePair(UNITSMAP, 1L, 2L);
	if (err < 0)
	    MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_goLivePair", MB_OK|MB_TASKMODAL);
    #elif LIVE_SNAP
	err = pxd_goSnap(UNITSMAP, 1);
	if (err < 0)
	    MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_goSnap", MB_OK|MB_TASKMODAL);
    #endif
}

/*
 * Subroutine to disable live video capture,
 * as per options.
 * Normally, live capture is disabled after
 * the video field or frame; an abort option
 * disables capture mode immediately.
 */
void unlive(int abort)
{

    #if SHOWIG_DIRECTVIDEO | SHOWIM_DIRECTVIDEO
	pxd_renderDirectVideoUnLive(UNITSMAP, hWndImage);
    #elif LIVE_LIVE
	if (abort)
	    pxd_goAbortLive(UNITSMAP);
	else
	    pxd_goUnLive(UNITSMAP);
    #elif LIVE_LIVE2
	if (abort)
	    pxd_goAbortLive(UNITSMAP);
	else
	    pxd_goUnLive(UNITSMAP);
    #elif LIVE_SNAP
	;
    #endif
}



/*
 * Thread to watch for end of captured field events, and advise
 * when the image should be redrawn.
 */
#if UPDATE_EVENT

DWORD WINAPI CapturedFieldServiceThread(PVOID hEventR3)
{
    for (;;) {
	//
	// Wait for signal.
	//
	WaitForSingleObject((HANDLE)hEventR3, INFINITE);

	//
	// Record snap time.
	// For the sake of interlaced video, check
	// that this isn't the first of two captured fields.
	//
	#if defined(SNAP_STATISTICS)
	    if (!pxd_goneLive(UNITSMAP, 0)) {
		if (snapTimeStart != 0 && snapTime == 0) {
		    snapTime = QueryPerformanceMillis() - snapTimeStart;
		    snapTimeMax = max(snapTime, snapTimeMax);
		    snapTimeMin = min(snapTime, snapTimeMin);
		    snapTimeStart = 0;
		    if (fieldTimeStart != 0) {
			fieldTime = QueryPerformanceMillis() - fieldTimeStart;
			fieldTimeMax = max(fieldTime, fieldTimeMax);
			fieldTimeMin = min(fieldTime, fieldTimeMin);
			fieldTimeStart = 0;
		    }
		    if (triggerTimeStart != 0) {
			triggerTime = QueryPerformanceMillis() - triggerTimeStart;
			triggerTimeMax = max(triggerTime, triggerTimeMax);
			triggerTimeMin = min(triggerTime, triggerTimeMin);
			triggerTimeStart = 0;
		    }
		}
	    }
	#endif


	//
	// Update image display
	//
	EnterCriticalSection(&critsect);
	displayImage(false);
	LeaveCriticalSection(&critsect);
    }
    return(0);
}

//
// Thread to wait for fault and display same.
//
DWORD WINAPI FaultServiceThread(PVOID hEventR3)
{
    for (;;) {
	//
	// Wait for signal.
	//
	WaitForSingleObject((HANDLE)hEventR3, INFINITE);
	EnterCriticalSection(&critsect);
	pxd_mesgFault(UNITSMAP);
	LeaveCriticalSection(&critsect);
    }
    return(0);
}


/*
 * Thread to watch for field events, captured or otherwise,
 * needed only for the sake of reporting capture statistics.
 */
#if SNAP_STATISTICS
DWORD WINAPI FieldServiceThread(PVOID hEventR3) {
    for (;;) {
	//
	// Wait for signal.
	//
	WaitForSingleObject((HANDLE)hEventR3, INFINITE);
	//
	// Note time for statistics.
	//
	fieldTimeStart = QueryPerformanceMillis();
    }
    return(0);
}
/*
 * Thread to watch for triggers, needed only for the sake
 * of reporting capture stats.
 */
DWORD WINAPI GPTriggerServiceThread(PVOID hEventR3) {
    for (;;) {
	//
	// Wait for signal.
	//
	WaitForSingleObject((HANDLE)hEventR3, INFINITE);
	//
	// Note time for statistics.
	//
	triggerTimeStart = QueryPerformanceMillis();
    }
    return(0);
}
#endif	// SNAP_STATISTICS

#endif	// UPDATE_EVENT


/*
 * The Dialog Guts
 */
BOOL CALLBACK
PIXCIDialogProc1(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    int     err = 0;

    #if SHOWIM_DIRECTXDISPLAY
	static LPDIRECTDRAW lpDD = NULL;
	static HINSTANCE    hDDLibrary = NULL;
    #endif
    #if SHOWIM_DRAWDIBDRAW || SHOWIM_DRAWDIBDISPLAY
	static HDRAWDIB     hDrawDib = NULL;
    #endif

    switch (wMsg) {
    case WM_INITDIALOG:
    {
	RECT	rectImage;

	//
	// Open the PIXCI(R) frame grabber.
	// If this program were to only support a single PIXCI(R)
	// frame grabber, the first parameter could be simplified to:
	//
	//	if (pxd_PIXCIopen("", FORMAT, NULL) < 0)
	//	    pxd__mesgFault(1);
	//
	// But, for the sake of multiple PIXCI(R) frame grabbers
	// specify which units are to be used.
	//
	char driverparms[80];
	driverparms[sizeof(driverparms)-1] = 0; // this & snprintf: overly conservative - avoids warning messages
	_snprintf(driverparms, sizeof(driverparms)-1, "-DM 0x%x %s", UNITSOPENMAP, DRIVERPARMS);
	//
	// Either FORMAT or FORMATFILE_LOAD or FORMATFILE_COMP
	// should have been selected above.
	//
	#if defined(FORMAT)
	    if (pxd_PIXCIopen(driverparms, FORMAT, "") < 0)
		pxd_mesgFault(UNITSMAP);
	#elif defined(FORMATFILE_LOAD)
	    //
	    // The FORMATFILE can be read and loaded
	    // during the pxd_PIXCIopen(), for convenience
	    // of changing the format file without recompiling.
	    //
	    if (pxd_PIXCIopen(driverparms, "", FORMATFILE_LOAD) < 0)
		// TODO: 取消错误显示
		//pxd_mesgFault(UNITSMAP);
	#elif defined(FORMATFILE_COMP)
	    //
	    // Or the FORMATFILE can be compiled into this application,
	    // reducing the number of files that must be distributed, or
	    // possibly lost.
	    //
	    // Note: On MSVC 6.0, if the precompiled header option is used,
	    // the compiler objects to this code (error C2006) when
	    // FORMATFILE_COMP is not defined and thus not intended to be used,
	    // even though this code shouldn't be compiled when
	    // FORMATFILE_COMP is not defined.
	    // Either turn off the 'Use Precompiled Headers' option,
	    // remove this code, or choose to use the FORMATFILE_COMP option.
	    //
	    if (pxd_PIXCIopen(driverparms, "Default", "") < 0)
		pxd_mesgFault(UNITSMAP);
	    {
		#include FORMATFILE_COMP
		pxd_videoFormatAsIncludedInit(0);
		err = pxd_videoFormatAsIncluded(0);
		if (err < 0)
		    MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_videoFormatAsIncluded", MB_OK|MB_TASKMODAL);
	    }
	#endif


	//
	// Set our title.
	//
	SetWindowText(hDlg, "EPIX(R) PIXCI(R) Frame Grabber Example");


	//
	// Tweak controls.
	// The SV5A has a extra BNC connector, rather than an S-Video connector.
	// The SV5B does not have an S-Video connector.
	//
	if (pxd_infoModel(1) == PIXCI_SV2
	 || pxd_infoModel(1) == PIXCI_SV3
	 || pxd_infoModel(1) == PIXCI_SV4
	 || pxd_infoModel(1) == PIXCI_SV6) {
	    ShowWindow(GetDlgItem(hDlg, IDBNC3), SW_HIDE);
	    ShowWindow(GetDlgItem(hDlg, IDBNC4), SW_HIDE);
	}
	if (pxd_infoModel(1) == PIXCI_SV5 && pxd_infoSubmodel(1) == PIXCI_SV5_SV5A) {
	    SetWindowText(GetDlgItem(hDlg, IDSVIDEO), "BNC 0");
	    ShowWindow(GetDlgItem(hDlg, IDBNC3), SW_HIDE);
	    ShowWindow(GetDlgItem(hDlg, IDBNC4), SW_HIDE);
	}
	if (pxd_infoModel(1) == PIXCI_SV5 && pxd_infoSubmodel(1) == PIXCI_SV5_SV5B) {
	    ShowWindow(GetDlgItem(hDlg, IDSVIDEO), SW_HIDE);
	    ShowWindow(GetDlgItem(hDlg, IDBNC3), SW_HIDE);
	    ShowWindow(GetDlgItem(hDlg, IDBNC4), SW_HIDE);
	}
	if (pxd_infoModel(1) == PIXCI_SV7) {
	    ShowWindow(GetDlgItem(hDlg, IDSVIDEO), SW_HIDE);
	    ShowWindow(GetDlgItem(hDlg, IDBNC1), SW_HIDE);
	    ShowWindow(GetDlgItem(hDlg, IDBNC2), SW_HIDE);
	    ShowWindow(GetDlgItem(hDlg, IDBNC3), SW_HIDE);
	    ShowWindow(GetDlgItem(hDlg, IDBNC4), SW_HIDE);
	}
	if (pxd_infoModel(1) == PIXCI_A310
	 || pxd_infoModel(1) == PIXCI_A110
	 || pxd_infoModel(1) == PIXCI_A1107I1O) {
	    ShowWindow(GetDlgItem(hDlg, IDSVIDEO), SW_HIDE);
	    ShowWindow(GetDlgItem(hDlg, IDBNC1), SW_HIDE);
	    ShowWindow(GetDlgItem(hDlg, IDBNC2), SW_HIDE);
	    ShowWindow(GetDlgItem(hDlg, IDBNC3), SW_HIDE);
	    ShowWindow(GetDlgItem(hDlg, IDBNC4), SW_HIDE);
	    EnableWindow(GetDlgItem(hDlg, IDHUESCROLL), FALSE);

	}
	if (pxd_infoModel(1) == PIXCI_SV8) {
	    ;
	}
	if (pxd_imageCdim()==1) {
	    //ShowWindow(GetDlgItem(hDlg, IDHUESCROLL),  SW_HIDE);
	    EnableWindow(GetDlgItem(hDlg, IDHUESCROLL), FALSE);
	}


	//
	// Enable timer, for live video updates.
	// Or, create event and thread for live video updates.
	// Use of the timer is simpler; use of an event and thread
	// is only slightly more efficient.
	//
	#if UPDATE_TIMER
	    SetTimer(hDlg, 1, 5, NULL);
	#endif
	#if UPDATE_EVENT
	    HANDLE  Event;
	    DWORD   ThreadId;

	    Event = pxd_eventCapturedFieldCreate(0x1);
	    if (!Event)
		MessageBox(NULL, "Can't create image update display event!", "XCLIBEX2", MB_OK|MB_TASKMODAL);
	    else
		CreateThread(0, 0x1000, CapturedFieldServiceThread, Event, 0, &ThreadId);

	    #if SNAP_STATISTICS
		Event = pxd_eventFieldCreate(0x1);
		if (!Event)
		    MessageBox(NULL, "Can't create video field event!", "XCLIBEX2", MB_OK|MB_TASKMODAL);
		else
		    CreateThread(0, 0x1000, FieldServiceThread, Event, 0, &ThreadId);
		//
		// The pxd_eventGPTriggerCreate, for async triggers, isn't available
		// on all boards. And it is used only for the sake of gathering statistics.
		// So, no error message if pxd_eventGPTriggerCreate() fails.
		Event = pxd_eventGPTriggerCreate(0x1, 0, 0);
		if (Event)
		    CreateThread(0, 0x1000, GPTriggerServiceThread, Event, 0, &ThreadId);
	    #endif

	    Event = pxd_eventFaultCreate(UNITSMAP, 0);
	    if (!Event)
		MessageBox(NULL, "Can't create fault monitoring event!", "XCLIBEX2", MB_OK|MB_TASKMODAL);
	    else
		CreateThread(0, 0x1000, FaultServiceThread, Event, 0, &ThreadId);

	    // Timer pops every 1 second to display the optional status and statistics messages;
	    // when using events the timer is not needed to display video images
	    // or to check for faults.
	    SetTimer(hDlg, 1, 1000, NULL);
	#endif

	//
	// Get handle to image display area of dialog,
	// then get its device context and size.
	//
	hWndImage = GetDlgItem(hDlg, IDIMAGE);
	{
	    HDC  hDC = GetDC(hWndImage);
	    GetClientRect(hWndImage, &rectImage);
	    svgaBits = GetDeviceCaps(hDC, PLANES) * GetDeviceCaps(hDC, BITSPIXEL);
	    ReleaseDC(hWndImage, hDC);
	}

	//
	// Determine displayed size.
	// We could simply fill up the hWndImage, but
	// much rather adjust the displayed image for
	// correct aspect ratio.
	//
	windImage[0].nw.x = windImage[0].nw.y = 0;
	windImage[0].se.x = rectImage.right+1;		 // inclusive->exclusive
	windImage[0].se.y = rectImage.bottom+1; 	 // inclusive->exclusive
	{
	    double  scalex, scaley, aspect;
	    aspect = pxd_imageAspectRatio();
	    if (aspect == 0.0)
		aspect = 1.0;
	    scalex = windImage[0].se.x/(double)pxd_imageXdim();
	    scaley = windImage[0].se.y/((double)pxd_imageYdim()*aspect);
	    scalex = std::min(scalex, scaley);
	    windImage[0].se.x = (int)(pxd_imageXdim() * scalex);
	    windImage[0].se.y = (int)(pxd_imageYdim() * scalex * aspect);
	}

	//
	// If StrecthDIBits is to be used, some VGA card drivers
	// abhor horizontal dimensions which are not a multiple of 4.
	// This isn't needed for other rendering methods, but doesn't hurt.
	//
	windImage[0].se.x &= ~3;

	//
	// For multiple units, display each of four units
	// in quadrant of display area.
	// Or, even for one unit, with the optional graph enabled.
	//
	if (UNITS > 1 || SHOW_GRAPH) {
	    windImage[0].se.x &= ~0xF;	 // See above StretchDIBits comment above
	    windImage[1] = windImage[0];
	    windImage[2] = windImage[0];
	    windImage[3] = windImage[0];
	    windImage[0].se.x /= 2;
	    windImage[0].se.y /= 2;
	    windImage[1].nw.x = windImage[1].se.x/2;
	    windImage[1].se.y /= 2;
	    windImage[2].se.x /= 2;
	    windImage[2].nw.y = windImage[2].se.y/2;
	    windImage[3].nw.x = windImage[3].se.x/2;
	    windImage[3].nw.y = windImage[3].se.y/2;
	}

	//
	// Customize and init dialog controls as per current PIXCI(R)
	// frame grabber model. Note that the pxd_infoModel()
	// for the SV5, SV5A, SV5B, and SV5L is the same.
	//
	switch (pxd_infoModel(UNITSMAP)) {
	  case PIXCI_SV2:
	    EnableWindow(GetDlgItem(hDlg, IDBNC2), FALSE);
	    // fall thru
	  case PIXCI_SV3:
	    // fall thru
	  case PIXCI_SV4:
	    // fall thru
	  case PIXCI_SV5:
	    SetScrollRange(GetDlgItem(hDlg, IDGAINSCROLL),   SB_CTL, 0, 100, TRUE);
	    SetScrollRange(GetDlgItem(hDlg, IDOFFSETSCROLL), SB_CTL, 0, 100, TRUE);
	    SetScrollRange(GetDlgItem(hDlg, IDHUESCROLL),    SB_CTL, 0, 100, TRUE);
	    setGuiMux(hDlg);
	    SetScrollPos(GetDlgItem(hDlg, IDOFFSETSCROLL), SB_CTL, getBright(), TRUE);
	    SetScrollPos(GetDlgItem(hDlg, IDGAINSCROLL),   SB_CTL, getGain(), TRUE);
	    SetScrollPos(GetDlgItem(hDlg, IDHUESCROLL),    SB_CTL, getHue(), TRUE);
	    break;
	  case PIXCI_SV8:
	    SetScrollRange(GetDlgItem(hDlg, IDOFFSETSCROLL), SB_CTL, 0, 100, TRUE);
	    SetScrollRange(GetDlgItem(hDlg, IDGAINSCROLL),   SB_CTL, 0, 100, TRUE);
	    SetScrollRange(GetDlgItem(hDlg, IDHUESCROLL),    SB_CTL, 0, 100, TRUE);
	    setGuiMux(hDlg);
	    SetScrollPos(GetDlgItem(hDlg, IDOFFSETSCROLL), SB_CTL, getBright(), TRUE);
	    SetScrollPos(GetDlgItem(hDlg, IDGAINSCROLL),   SB_CTL, getGain(), TRUE);
	    SetScrollPos(GetDlgItem(hDlg, IDHUESCROLL),    SB_CTL, getHue(), TRUE);
	    break;
	  case PIXCI_SV7:
	  case PIXCI_A310:
	    SetScrollRange(GetDlgItem(hDlg, IDGAINSCROLL),   SB_CTL, 0, 100, TRUE);
	    SetScrollRange(GetDlgItem(hDlg, IDOFFSETSCROLL), SB_CTL, 0, 100, TRUE);
	    SetScrollRange(GetDlgItem(hDlg, IDHUESCROLL),    SB_CTL, 0, 100, TRUE);
	    SetScrollPos(GetDlgItem(hDlg,   IDOFFSETSCROLL), SB_CTL, getBright(), TRUE);
	    SetScrollPos(GetDlgItem(hDlg,   IDGAINSCROLL),   SB_CTL, getGain(), TRUE);
	    SetScrollPos(GetDlgItem(hDlg,   IDHUESCROLL),    SB_CTL, getHue(), TRUE);
	    break;
	  case PIXCI_A110:
	  case PIXCI_A1107I1O:
	    SetScrollRange(GetDlgItem(hDlg, IDGAINSCROLL),   SB_CTL, 0, 100, TRUE);
	    SetScrollRange(GetDlgItem(hDlg, IDOFFSETSCROLL), SB_CTL, 0, 100, TRUE);
	    SetScrollPos(GetDlgItem(hDlg, IDOFFSETSCROLL), SB_CTL, getBright(), TRUE);
	    SetScrollPos(GetDlgItem(hDlg, IDGAINSCROLL),   SB_CTL, getGain(), TRUE);
	    EnableWindow(GetDlgItem(hDlg, IDBNC2),   FALSE);
	    EnableWindow(GetDlgItem(hDlg, IDBNC3),   FALSE);
	    EnableWindow(GetDlgItem(hDlg, IDBNC4),   FALSE);
	    EnableWindow(GetDlgItem(hDlg, IDHUESCROLL),    FALSE);
	    break;
	  case PIXCI_D:
	  case PIXCI_D24:
	  case PIXCI_D32:
	  case PIXCI_A:
	  case PIXCI_D2X:
	  case PIXCI_D3X:
	  case PIXCI_D3XE:
	  case PIXCI_CL1:
	  case PIXCI_CL2:
	  case PIXCI_EC1:
	  case PIXCI_ECB1:
	  case PIXCI_ECB134:
	  case PIXCI_ECB2:
	  case PIXCI_EB1:
	  case PIXCI_EB1POCL:
	  case PIXCI_EB1MINI:
	  case PIXCI_MINIH2DB:
	  case PIXCI_MINIH2F:
	  case PIXCI_MINIH2X4F:
	  case PIXCI_MF2280:
	  case PIXCI_EL1:
	  case PIXCI_ELS2:
	  case PIXCI_E1:
	  case PIXCI_E4:
	  case PIXCI_E8:
	  case PIXCI_E1DB:
	  case PIXCI_EL1DB:
	  case PIXCI_E4DB:
	  case PIXCI_E8CAM:
	  case PIXCI_E8DB:
	  case PIXCI_E104X42F:
	  case PIXCI_E104X44B:
	  case PIXCI_E104X4F2B:
	  case PIXCI_E4G22F:
	  case PIXCI_E4G24B:
	  case PIXCI_E4G2F2B:
	  case PIXCI_E4TX22F:
	  case PIXCI_E4TX24B:
	  case PIXCI_E4TX2F2B:
	  case PIXCI_CL3SD:
	  case PIXCI_SI:
	  case PIXCI_SI1:
	  case PIXCI_SI2:
	  case PIXCI_SI4:
	  default:
	    EnableWindow(GetDlgItem(hDlg, IDBNC1),   FALSE);
	    EnableWindow(GetDlgItem(hDlg, IDBNC2),   FALSE);
	    EnableWindow(GetDlgItem(hDlg, IDBNC3),   FALSE);
	    EnableWindow(GetDlgItem(hDlg, IDBNC4),   FALSE);
	    EnableWindow(GetDlgItem(hDlg, IDSVIDEO), FALSE);
	    EnableWindow(GetDlgItem(hDlg, IDGAINSCROLL),   FALSE);
	    EnableWindow(GetDlgItem(hDlg, IDOFFSETSCROLL), FALSE);
	    EnableWindow(GetDlgItem(hDlg, IDHUESCROLL),    FALSE);
	    break;
	}

	//
	// If using DirectDraw, initialize access to it.
	//
	// DirectDraw may not be available!
	// Error reporting should be added!
	//
	#if SHOWIM_DIRECTXDISPLAY
	{
	    HRESULT	    h;
	    hDDLibrary = LoadLibrary("DDRAW");
	    if (hDDLibrary) {
		typedef HRESULT (WINAPI* OPEN)(void FAR*,LPDIRECTDRAW FAR*, void FAR*);
		OPEN	lpfnDM;
		lpfnDM = (OPEN)GetProcAddress(hDDLibrary, "DirectDrawCreate");
		if (lpfnDM) {
		    h = (*lpfnDM)(NULL, &lpDD, NULL);
		    if (lpDD) {
			h = lpDD->SetCooperativeLevel((HWND)hWnd, DDSCL_NORMAL);
		    }
		}
	    }
	}
	#endif

	//
	// If using Video for Windows, initialize access to it.
	//
	#if SHOWIM_DRAWDIBDRAW || SHOWIM_DRAWDIBDISPLAY
	    hDrawDib = DrawDibOpen();
	#endif

	//
	// If using Direct Video to S/VGA, initialize access to it.
	//
	#if SHOWIG_DIRECTVIDEO | SHOWIM_DIRECTVIDEO
	    if ((err = pxd_renderDirectVideoInit(UNITSMAP, hWnd)) < 0)
		MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_renderDirectVideoInit", MB_OK|MB_TASKMODAL);
	#endif

	//
	// Warn if LIVE_LIVE2 was selected and there is only one buffer.
	//
	#if LIVE_LIVE2
	    if (pxd_imageZdim() < 2)
		MessageBox(NULL, "Only 1 image frame buffer available, can't use LIVE_LIVE2 mode!!", "XCLIBEX2", MB_OK|MB_TASKMODAL);
	#endif

	tickInit = GetTickCount();

	return(TRUE);
    }

    case WM_COMMAND:
	switch (LOWORD(wParam)) {

	case IDSNAP:
	    if (HIWORD(wParam) != BN_CLICKED)
		return(FALSE);
	    #if SHOWIG_DIRECTVIDEO | SHOWIM_DIRECTVIDEO
		if (liveon) {
		    pxd_renderDirectVideoUnLive(UNITSMAP, hWndImage);
		    liveon = FALSE;
		} else {
		    MessageBox(NULL, "Snap not available with the SHOWIG_DIRECTVIDEO option!", "XCLIBEX2", MB_OK|MB_TASKMODAL);
		}
	    #else
		// For this application, we choose to let the
		// Snap button be the means of ending continuous live mode.
		if (liveon) {
		    err = pxd_goUnLive(UNITSMAP);
		    if (err < 0)
			MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_goUnLive", MB_OK|MB_TASKMODAL);
		    liveon = FALSE;
		    return(TRUE);
		}

		//
		// As the UPDATE_TIMER or UPDATE_EVENT code monitors
		// completed capture buffers, there is no need to wait
		// for the snap to be done.
		//
		if (!pxd_goneLive(UNITSMAP, 0)) {

		    err = pxd_goSnap(UNITSMAP, 1);
		    if (err < 0)
			MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_goSnap", MB_OK|MB_TASKMODAL);

		    #if defined(SNAP_STATISTICS)
			snapTime = 0;
			snapTimeStart = QueryPerformanceMillis();
		    #endif
		    //
		    // Alternately, we can snap and explicitly wait.
		    // Waiting here - in the middle of the Dialog's Proc - isn't
		    // very GUI friendly, especially with slow cameras, but it
		    // is worthwhile noting. If explicitly waiting, providing a timeout
		    // is nice just in case the camera is powered off and not outputting video.
		    //
		    #if 0
		    {
			DWORD ticks = GetTickCount();
			while (pxd_goneLive(UNITSMAP, 1)) {
			    if (GetTickCount() > ticks+5000)
				break;
			}
			#if defined(SNAP_STATISTICS)
			    snapTime = QueryPerformanceMillis() - snapTimeStart;
			    snapTimeMax = max(snapTime, snapTimeMax);
			    snapTimeMin = min(snapTime, snapTimeMin);
			    snapTimeStart = 0;
			    if (fieldTimeStart != 0) {
				fieldTime = QueryPerformanceMillis() - fieldTimeStart;
				fieldTimeMax = max(fieldTime, fieldTimeMax);
				fieldTimeMin = min(fieldTime, fieldTimeMin);
				fieldTimeStart = 0;
			    }
			    if (triggerTimeStart != 0) {
				triggerTime = QueryPerformanceMillis() - triggerTimeStart;
				triggerTimeMax = max(triggerTime, triggerTimeMax);
				triggerTimeMin = min(triggerTime, triggerTimeMin);
				triggerTimeStart = 0;
			    }
			#endif
		    }
		    #endif
		}

	    #endif
	    return(TRUE);

	case IDLIVE:
	    if (HIWORD(wParam) != BN_CLICKED)
		return(FALSE);
	    liveon = TRUE;
	    live();
	    return(TRUE);

	case IDRESET:
	    if (HIWORD(wParam) != BN_CLICKED)
		return(FALSE);
	    // This might be used in trigger modes,
	    // where the Video Engine has ben armed to snap
	    // but there no trigger has arrived.
	    // Rarely used, button may not be shown in the dialog;
	    // instead click Unlive button twice.
	    err = pxd_goAbortLive(UNITSMAP);
	    if (err < 0)
		MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_goAbortLive", MB_OK|MB_TASKMODAL);
	    liveon = FALSE;
	    return(TRUE);

	case IDUNLIVE:
	    if (HIWORD(wParam) != BN_CLICKED)
		return(FALSE);
	    //
	    // 'Unlive' normally stops capture after the current video frame.
	    // For trigger modes, an abort live that can be used when the
	    // trigger isn't active would be nice. We do both with one button;
	    // clicking twice within 2 sec does abort of live mode.
	    {
		DWORD t = GetTickCount();
		unlive((t-unliveclick) < 2000);
		unliveclick = t;
	    }
	    liveon = FALSE;
	    return(TRUE);

	case IDSVIDEO:
	    if (HIWORD(wParam) != BN_CLICKED)
		return(FALSE);
	    err = pxd_setVidMux(UNITSMAP, 1);
	    if (err < 0)
		MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_setVidMux", MB_OK|MB_TASKMODAL);
	    setGuiMux(hDlg);
	    return(TRUE);

	case IDBNC1:
	    if (HIWORD(wParam) != BN_CLICKED)
		return(FALSE);
	    err = pxd_setVidMux(UNITSMAP, 2);
	    if (err < 0)
		MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_setVidMux", MB_OK|MB_TASKMODAL);
	    setGuiMux(hDlg);
	    return(TRUE);

	case IDBNC2:
	    if (HIWORD(wParam) != BN_CLICKED)
		return(FALSE);
	    err = pxd_setVidMux(UNITSMAP, 3);
	    if (err < 0)
		MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_setVidMux", MB_OK|MB_TASKMODAL);
	    setGuiMux(hDlg);
	    return(TRUE);

	case IDBNC3:
	    if (HIWORD(wParam) != BN_CLICKED)
		return(FALSE);
	    err = pxd_setVidMux(UNITSMAP, 4);
	    if (err < 0)
		MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_setVidMux", MB_OK|MB_TASKMODAL);
	    setGuiMux(hDlg);
	    return(TRUE);

	case IDBNC4:
	    if (HIWORD(wParam) != BN_CLICKED)
		return(FALSE);
	    err = pxd_setVidMux(UNITSMAP, 5);
	    if (err < 0)
		MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_setVidMux", MB_OK|MB_TASKMODAL);
	    setGuiMux(hDlg);
	    return(TRUE);

	case IDSAVE:
	    if (HIWORD(wParam) != BN_CLICKED)
		return(FALSE);
	    for (int u = 0; u < UNITS; u++) {
		char	pathname[_MAX_PATH] = "";
		int	r = 1;
		#if SAVE_PROMPT   // prompt for file name?
		    OPENFILENAME ofn;
		    char    title[80] = "Save";
		    memset(&ofn, 0, sizeof(ofn));
		    ofn.lStructSize = sizeof(ofn);
		    ofn.hwndOwner   = hWnd;
		    ofn.lpstrFilter = "TIFF Files (*.tif)\0*.tif\0\0";
		    ofn.lpstrFile   = pathname;
		    ofn.nMaxFile    = _MAX_PATH;
		    if (UNITS > 1) {
			_snprintf(title, sizeof(title), "Save Unit %d", u);
			title[sizeof(title)-1] = 0;
		    }
		    ofn.lpstrTitle  = title;
		    ofn.Flags	|= OFN_EXPLORER|OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY;
		    r = GetSaveFileName(&ofn);
		#else	// no prompt. generate file name based on date & time ?
		    time_t  atime;
		    struct  tm *tmp;
		    time(&atime);
		    tmp = localtime(&atime);
		    _snprintf(pathname, sizeof(pathname),
			    "Image_%.4d-%.2d-%.2d_%.2d-%.2d-%.2d-%.3d.tif",
			    tmp->tm_year+1900, tmp->tm_mon+1, tmp->tm_mday,
			    tmp->tm_hour, tmp->tm_min, tmp->tm_sec, GetTickCount()%1000);
		    pathname[sizeof(pathname)-1] = 0;
		    //MessageBox(NULL, pathname, pathname, MB_OK|MB_TASKMODAL);
		#endif
		// We can save while capturing live video,
		// but in LIVE_LIVE mode the image might contain data from two video fields.
		if (liveon) {
		    unlive(FALSE);
		    for (DWORD t = GetTickCount(); GetTickCount() < t+15000; )
			if (pxd_goneLive(UNITSMAP, 0) == 0)
			    break;
		}
		if (r != 0) {
		    #if 1
			err = pxd_saveTiff(1<<u, pathname, 1, 0, 0, -1, -1, 0, 0);
			if (err < 0)
			    MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_saveTiff", MB_OK|MB_TASKMODAL);
		    #else
			err = pxd_saveBmp(1<<u, pathname, 1, 0, 0, -1, -1, 0, 0);
			if (err < 0)
			    MessageBox(NULL, pxd_mesgErrorCode(err), "pxd_saveBmp", MB_OK|MB_TASKMODAL);
		    #endif
		}
		MessageBeep(0xFFFFFFFF);
		if (liveon)
		    live();
	    }
	    return(TRUE);
	}
	break;

    case WM_HSCROLL:
      {
	HWND hCtrl = (HWND)lParam;
	switch (GetWindowLong(hCtrl, GWL_ID)) {
	case IDGAINSCROLL:
	  {
	    int g = getGain();
	    switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    g += 5;	break;
	    case SB_LINEDOWN:	    g += 1;	break;
	    case SB_PAGEUP:	    g -= 5;	break;
	    case SB_LINEUP:	    g -= 1;	break;
	    case SB_TOP:	    g = 100;	break;
	    case SB_BOTTOM:	    g = 0;	break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     g = HIWORD(wParam); break;
	    default:
		return(FALSE);
	    }
	    setGain(g);
	    SetScrollPos(GetDlgItem(hDlg, IDGAINSCROLL), SB_CTL, getGain(), TRUE);
	    return(TRUE);
	  }
	case IDOFFSETSCROLL:
	  {
	    int g = getBright();
	    switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    g += 5;	break;
	    case SB_LINEDOWN:	    g += 1;	break;
	    case SB_PAGEUP:	    g -= 5;	break;
	    case SB_LINEUP:	    g -= 1;	break;
	    case SB_TOP:	    g = 100;	break;
	    case SB_BOTTOM:	    g = 0;	break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     g = HIWORD(wParam); break;
	    default:
		return(FALSE);
	    }
	    setBright(g);
	    SetScrollPos(GetDlgItem(hDlg, IDOFFSETSCROLL), SB_CTL, getBright(), TRUE);
	    return(TRUE);
	  }
	case IDHUESCROLL:
	  {
	    int g = getHue();
	    switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    g += 5;	break;
	    case SB_LINEDOWN:	    g += 1;	break;
	    case SB_PAGEUP:	    g -= 5;	break;
	    case SB_LINEUP:	    g -= 1;	break;
	    case SB_TOP:	    g = 100;	break;
	    case SB_BOTTOM:	    g = 0;	break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     g = HIWORD(wParam); break;
	    default:
		return(FALSE);
	    }
	    setHue(g);
	    SetScrollPos(GetDlgItem(hDlg, IDHUESCROLL), SB_CTL, getHue(), TRUE);
	    return(TRUE);
	  }
	}
	return(FALSE);
      }

    case WM_CLOSE:
	pxd_PIXCIclose();
	//DestroyWindow(GetParent(hDlg));
	#if SHOWIM_DIRECTXDISPLAY
	    if (lpDD)
		lpDD->Release();
	    if (hDDLibrary)
		FreeLibrary(hDDLibrary);
	    lpDD = NULL;
	    hDDLibrary = NULL;
	#endif
	#if SHOWIM_DRAWDIBDRAW || SHOWIM_DRAWDIBDISPLAY
	    if (hDrawDib)
		DrawDibClose(hDrawDib);
	    hDrawDib = NULL;
	#endif
	#if SHOWIG_DIRECTVIDEO | SHOWIM_DIRECTVIDEO
	    pxd_renderDirectVideoDone(UNITSMAP, hWnd);
	#endif

	DestroyWindow(hWnd);
	EndDialog(hDlg, 0);
	return(TRUE);

    case WM_WINDOWPOSCHANGING:
    case WM_WINDOWPOSCHANGED:
	//
	// When using pxd_renderDirectVideo,
	// the window position must be tracked.
	//
	#if SHOWIG_DIRECTVIDEO | SHOWIM_DIRECTVIDEO
	    if (liveon)
		pxd_renderDirectVideoUnLive(UNITSMAP, hWndImage);
	#endif
	break;

    case WM_MOVE:
	#if SHOWIG_DIRECTVIDEO
	    if (liveon)
		pxd_renderDirectVideoLive(UNITSMAP, hWndImage, windImage[0].nw.x, windImage[0].nw.y,
				    windImage[0].se.x, windImage[0].se.y, CLR_INVALID, CLR_INVALID);
	#elif SHOWIM_DIRECTVIDEO
	    if (liveon)
		pxd_renderDirectVideoLive(UNITSMAP, hWndImage, windImage[0].nw.x, windImage[0].nw.y,
				    windImage[0].se.x, windImage[0].se.y, RGB(0,0,189), CLR_INVALID);
	#endif
	break;

      case WM_PAINT:
	// our window was covered & uncovered
	if (pxd_imageXdim() > 0)    // if pxd_PIXCIopen open?
	    displayImage(true);
	return(FALSE);

    case WM_TIMER:
	//
	// Monitor for asynchronous faults, such as video
	// being disconnected while capturing. These faults
	// can't be reported by functions such as pxd_goLive()
	// which initiate capture and return immediately.
	//
	// Should there be a fault and pxd_mesgFault() pop up a dialog,
	// the Windows TIMER will continue in a new thread. Thus the
	// 'faulting' variable and logic to limit to one dialog at a time.
	//
	if (pxd_infoUnits()) {	 // implies whether library is open
	    #if UPDATE_TIMER
		static int faulting = 0;
		if (!faulting) {
		    faulting++;
			// TODO: 取消错误显示
		    //pxd_mesgFault(UNITSMAP);
		    faulting--;
		}
	    #endif
	    //
	    // Show
	    //	spatial resolution
	    //	bit depth & color
	    //	measured video rate
	    //	measured display rate
	    //	selected video format
	    // Optionally, for use in triggered applications, show
	    // video field count (whether captured or not) instead of
	    // the video rate and display rate.
	    //
	    static DWORD      lasttickcount = 0;
	    static pxvbtime_t lastfieldcount = 0;
	    DWORD ticks = GetTickCount();
	    if (ticks - lasttickcount > 1000) {
		pxvbtime_t fields = pxd_videoFieldCount(1);
		char	   buf[200];
		buf[sizeof(buf)-1] = 0; // this & snprintf: overly conservative - avoids warning messages
		#if SHOWIG_DIRECTVIDEO || SHOWIM_DIRECTVIDEO
		    _snprintf(buf, sizeof(buf)-1, "dim: %dx%d  fps: %.5g",
			    pxd_imageXdim(), pxd_imageYdim(),
			    lasttickcount?1000.0*(double)(fields-lastfieldcount)/((ticks-lasttickcount)*pxd_videoFieldsPerFrame()):0);
		#elif !STATS_FIELDCOUNT
		    _snprintf(buf, sizeof(buf)-1, "dim: %dx%d  res: %dx%d  fps: %.5g disp: %.5g",
			    pxd_imageXdim(), pxd_imageYdim(), pxd_imageBdim(), pxd_imageCdim(),
			    lasttickcount?1000.0*(double)(fields-lastfieldcount)/((ticks-lasttickcount)*pxd_videoFieldsPerFrame()):0,
			    1000.0*(double)displaycount/(ticks-lasttickcount));
		#else
		    _snprintf(buf, sizeof(buf)-1, "dim: %dx%d  res: %dx%d  fields: %d",
			    pxd_imageXdim(), pxd_imageYdim(), pxd_imageBdim(), pxd_imageCdim(),
			    fields);
		#endif
		SetWindowText(GetDlgItem(hDlg, IDSTATUS1), buf);
		#if defined(SNAP_STATISTICS)
		    if (snapTime != 0) {
			_snprintf(buf, sizeof(buf)-1, "snap->image: %.1f (min: %.1f max: %.1f)", snapTime, snapTimeMin, snapTimeMax);
			SetWindowText(GetDlgItem(hDlg, IDSTATUS3), buf);
			if (fieldTime != 0) {
			    _snprintf(buf, sizeof(buf)-1, "field->image: %.1f (min: %.1f max: %.1f)", fieldTime, fieldTimeMin, fieldTimeMax);
			    SetWindowText(GetDlgItem(hDlg, IDSTATUS4), buf);
			}
			if (triggerTime != 0) {
			    _snprintf(buf, sizeof(buf)-1, "trigger->image: %.1f (min: %.1f max: %.1f)", triggerTime, triggerTimeMin, triggerTimeMax);
			    SetWindowText(GetDlgItem(hDlg, IDSTATUS5), buf);
			}
		    } else {
			// The explicit confirmation of capture in progress
			// is only needed w. slow cameras, so we tie it
			// to STATS_FIELDCOUNT
			#if STATS_FIELDCOUNT
			    if (pxd_goneLive(UNITSMAP, 0) != 0)
				SetWindowText(GetDlgItem(hDlg, IDSTATUS3), "Snap/Live Capture in Progress");
			    else
				SetWindowText(GetDlgItem(hDlg, IDSTATUS3), "");
			#else
			    SetWindowText(GetDlgItem(hDlg, IDSTATUS3), "");
			#endif
			SetWindowText(GetDlgItem(hDlg, IDSTATUS4), "");
			SetWindowText(GetDlgItem(hDlg, IDSTATUS5), "");
		    }
		#endif
		#if defined(FORMAT)
		    _snprintf(buf, sizeof(buf)-1, "fmt: %s", FORMAT);
		    SetWindowText(GetDlgItem(hDlg, IDSTATUS2), buf);
		#elif defined(FORMATFILE_LOAD)
		    _snprintf(buf, sizeof(buf)-1, "fmt: %s", FORMATFILE_LOAD);
		    SetWindowText(GetDlgItem(hDlg, IDSTATUS2), buf);
		#elif defined(FORMATFILE_COMP)
		    _snprintf(buf, sizeof(buf)-1, "fmt: %s compiled", FORMATFILE_COMP);
		    SetWindowText(GetDlgItem(hDlg, IDSTATUS2), buf);
		#endif
		lasttickcount  = ticks;
		lastfieldcount = fields;
		displaycount   = 0;
	    }
	}
	//
	// If using the timer to update image display, do so.
	//
	#if UPDATE_TIMER
	    displayImage(false);
	#endif

	#if defined(TIMEDEXIT)
	    if ((GetTickCount() - tickInit) > TIMEDEXIT) {
		DestroyWindow(hWnd);
		EndDialog(hDlg, 0);
	    }
	#endif



	return(TRUE);

    }
    return(FALSE);
}

/*
 * The Dialog
 * When using events from a different thread,
 * we must serialize access to XCLIB.
 * For simplicitty, we simply wrap this around the
 * non-serialized guts of the dialog.
 */
BOOL CALLBACK
PIXCIDialogProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    #if UPDATE_EVENT
	BOOL	b;
	EnterCriticalSection(&critsect);
	b = PIXCIDialogProc1(hDlg, wMsg, wParam, lParam);
	LeaveCriticalSection(&critsect);
	return(b);
    #else
	return(PIXCIDialogProc1(hDlg, wMsg, wParam, lParam));
    #endif

}


LRESULT CALLBACK MainWndProc(
    HWND	hWnd,
    unsigned	message,
    WPARAM	wParam,
    LPARAM	lParam
){
    switch (message) {
	case WM_CLOSE:
	    DestroyWindow(hWnd);
	    return(0);
	case WM_DESTROY:
	    PostQuitMessage(0);
	    return(0);
    }
    return(DefWindowProc(hWnd, message, wParam, lParam));
}

/*
 * The Main
 */
int APIENTRY WinMain(
    HINSTANCE  hInstance,
    HINSTANCE  hPrevInstance,
    LPSTR      lpCmdLine,
    int        nCmdShow
){
    MSG       msg;
    WNDCLASS  wc;

    wc.style	     = CS_BYTEALIGNWINDOW;
    wc.lpfnWndProc   = MainWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon	     = 0;
    wc.hCursor	     = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName  =	NULL;
    wc.lpszClassName = "PXlibWClass";

    if (!RegisterClass(&wc))
	return (FALSE);

    if (hPrevInstance)
	return(FALSE);

    hWnd = CreateWindow("PXlibWClass",
			"XCLIBEX2 Windows Example",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			0, 0, hInstance, NULL );
    if (!hWnd)
	return (FALSE);

    //
    // This main window of this example does nothing useful.
    // Don't bother showing it.
    //
    //ShowWindow(hWnd, nCmdShow);
    //UpdateWindow(hWnd);
    //
    // Launch dialog.
    //
    InitializeCriticalSection(&critsect);
    hDlg = CreateDialogParam(hInstance, "PIXCISVXDIALOG", NULL, (DLGPROC)PIXCIDialogProc, NULL);
    if (!hDlg) {
	MessageBox(NULL, "Missing Dialog Resource - Compilation or Link Error!", "XCLIBEX2", MB_OK|MB_TASKMODAL);
	return(FALSE);
    }

    while (GetMessage(&msg, 0, 0, 0)) {
	if (!hDlg || !IsDialogMessage(hDlg, &msg)) {
	    TranslateMessage(&msg);
	    DispatchMessage(&msg);
	}
    }
    return ((int)msg.wParam);
}
