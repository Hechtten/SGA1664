/*
 *
 *	xclibex1.c	External	16-Feb-2023
 *
 *	Copyright (C)  1996-2023  EPIX, Inc.	All rights reserved.
 *
 *	Example program for XCLIB C Library, SCF Level functions.
 *	Example assumes DOS, or Windows 'command line' environment.
 *
 */


/*
 *  INSTRUCTIONS:
 *
 *  1)	Set the 'define' options below according to the target camera and video format.
 *
 *	For PIXCI(R) SV2, SV3, SV4, SV5, SV5A, SV5B, and SV5L frame grabbers
 *	common choices are RS-170, NSTC, NTSC/YC, CCIR, PAL, or PAL/YC.
 *	(The SV5A and SV5B do not support NTSC/YC or PAL/YC).
 *	For PIXCI(R) SV7 frame grabbers
 *	common choices are RS-170, NSTC, CCIR, or PAL.
 *	For PIXCI(R) SV8 frame grabbers
 *	common choices are RS-170, NSTC, NTSC/YC, CCIR, PAL, PAL/YC.
 *
 *	For PIXCI(R) A, CL1, CL2, CL3SD, D, D24, D32, D2X, D3X, D3XE,
 *	E1, E1DB, E4, E4DB, E4G2, E4TX2, E8, E8CAM, E8DB, e104x4, EB1, EB1G2, EB1-POCL, EB1G2-POCL,
 *	EB1mini, miniH2B, miniH2F, miniH2x4F, mf2280, EC1, ECB1, ECB1-34, ECB2, EL1, EL1DB, ELS2, SI, SI1, SI2, and SI4
 *	frame grabbers, use "default" to select the default format for the camera
 *	for which the PIXCI(R) frame grabber is intended.
 *	For non-default formats, use XCAP to save video settings to a file,
 *	and set FORMATFILE to the name of that file.
 *	For camera's with RS-232 control, note that the saved
 *	video set-up only resets the PIXCI(R) frame grabber's
 *	settings, but XCLIB does not reset the camera's settings.
 *
 *	Alternately, this could be modified to use getenv("PIXCI"),
 *	GetPrivateProfileString(...), RegQueryValueEx(...), or any
 *	other convention chosen by the programmer to allow run time
 *	selection of the video format and resolution.
 *
 */


#if !defined(FORMAT) && !defined(FORMATFILE)
				    // For PIXCI(R) SV2, SV3, SV4, SV5, SV5A, SV5B, SV5L
  //#define FORMAT  "RS-170"	    // RS-170 on input 2
  //#define FORMAT  "NTSC"	    // NTSC on input 2
  //#define FORMAT  "NTSC/YC"	    // NSTC S-Video on input 1		    (N/A on SV5A,SV5B)
  //#define FORMAT  "CCIR"	    // CCIR on input 2
  //#define FORMAT  "PAL"	    // PAL (B,D,G,H,I) on input 2
  //#define FORMAT  "PAL/YC"	    // PAL (B,D,G,H,I) S-Video on input 1   (N/A on SV5A,SV5B)
  //#define FORMAT  "default"	    // NSTC S-Video on input 1

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

				  // For PIXCI(R) A, CL1, CL2, CL3SD, D, D24, D32,
				  // D2X, D3X, D3XE, E1, E1DB, E4, E4DB, E4G2, E4TX2,
				  // E8, E8CAM, E8DB, e104x4, EB1, EB1G2, EB1-POCL, EB1G2-POCL,
				  // EB1mini, miniH2B, miniH2F, miniH2x4F, mf2280,
				  // EC1, ECB1, ECB1-34, ECB2, EL1, EL1DB,
				  // ELS2, SI, SI1, SI2, SI4
    // #define FORMAT  "default"	  // as per board's intended camera

					  // For any PIXCI(R) frame grabber
  #define FORMATFILE	"xcvidset.fmt"	  // using format file saved by XCAP
#endif


/*
 *  2.1) 设置预期的PIXCI(R)帧抓取器数量。
 *  本示例要求所有板卡必须
 *  相同且在相同分辨率下工作。
 *
 *  对于具有多个功能单元的PIXCI(R)帧抓取器，
 *  XCLIB将PIXCI(R) E1DB、E4DB、E4G2-2F、E4TX2-2F、
 *  E8CAM、E8DB、e104x4-2f、ECB2、EL1DB、ELS2、miniH2B、SI2或SV7帧抓取器的两个部分，
 *  或PIXCI(R) E4G2-F2B、E4TX2-F2B、e104x4-f2b帧抓取器的三个部分，
 *  或PIXCI(R) E4G2-4B、E4TX2-4B、e104x4-4b或SI4帧抓取器的四个部分，
 *  分别作为两个、三个或四个独立的PIXCI(R)帧抓取器呈现。
 */
#if !defined(UNITS)
    #define UNITS	1
#endif
#define UNITSMAP    ((1<<UNITS)-1)  // shorthand - bitmap of all units
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
 *
 *  Note: Under Windows, the image frame buffer memory can't be set as
 *  a run time option. It MUST be set via SYSTEM.INI for Win 95/98,
 *  or via the Registry for Win NT and later, so the memory can be
 *  reserved during Window's initialization.
 */
#if !defined(DRIVERPARMS)
  //#define DRIVERPARMS "-QU 0"     // don't use interrupts
  //#define DRIVERPARMS "-IM 8192"  // request 8192 mbyte of frame buffer memory
    #define DRIVERPARMS ""	    // default
#endif


/*
 *  3)	选择是否使用PXIPL图像处理库，
 *	该库可用于在S/VGA上显示图像。
 *	(在Windows下无论是否使用PXIPL库都可以在S/VGA上显示图像，
 *	但在这个命令行示例中没有展示)。
 *  0-不使用 1-使用
 */
#if !defined(USE_PXIPL)
    #define USE_PXIPL	0
#endif

/*
 *  4) 选择保存图像的目录。
 *  此示例不会覆盖现有的图像文件；
 *  因此目录选择不是很重要。
 */
#if !defined(IMAGEFILE_DIR)
    #define IMAGEFILE_DIR    "./images"
#endif

/*
 *  5) 是否运行时不显示提示信息?
 *  0-不显示 1-显示
 */
#if !defined(USER_PROMPTS)
    #define USER_PROMPTS   1
#endif


/*
 *  6)	在Windows下编译
 *	    XCLIBEX1.C
 *	与其他Win32或Win64命令行应用程序的编译方式相同，
 *	包含文件位于
 *	    c:\Program Files\EPIX\XCLIB\Inc
 *	(或其他XCLIB安装位置)
 *	并链接到以下库文件之一:
 *	    XCLIBWNT.LIB   (用于Win32应用程序的XCLIB)
 *	    XCLIBW64.LIB   (用于Win64应用程序的XCLIB)
 *	    XCLYBWNT.LIB   (用于Win32应用程序的XCLIB-Lite)
 *	    XCLYBW64.LIB   (用于Win64应用程序的XCLIB-Lite)
 *	    XCLIBWNT.LIB & PXIPLWNT.LIB   (用于Win32应用程序的XCLIB+PXIPL)
 *	    XCLIBW64.LIB & PXIPLW64.LIB   (用于Win64应用程序的XCLIB+PXIPL)
 *
 *	注意: 不要启用严格的ANSI C++兼容性；
 *	XCLIB需要预定义的#defines来指示主机环境。
 *	选择ASCII字符集，而不是Unicode或多字节。
 *	将64位应用程序链接到32位DLL，
 *	或反之亦然，是常见的错误来源。
 *
 */


/*
 *  NECESSARY INCLUDES:
 */
#include <stdio.h>		// c library
#include <signal.h>		// c library
#include <stdlib.h>		// c library
#include <stdarg.h>		// c library
#include <string.h>		// c library
#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64)
#include <windows.h>
#endif
#if USE_PXIPL
#include "pxipl.h"         
#include "pxextras.h"           
#endif
#include "xcliball.h"		// function prototypes


/*
 *  SUPPORT STUFF:
 *
 *  Catch CTRL+C and floating point exceptions so that
 *  once opened, the PIXCI(R) driver and frame grabber
 *  are always closed before exit.
 *  In most environments, this isn't critical; the operating system
 *  advises the PIXCI(R) driver that the program has terminated.
 *  In other environments, such as DOS, the operating system
 *  isn't as helpful and the driver & frame grabber would remain open.
 */
void signaled(int sig)
{
    /*
     * Some languages/environments don't allow
     * use of printf from a signal catcher.
     * Printing the message isn't important anyhow.
     *
    if (sig == SIGINT)
	printf("Break\n");
    if (sig == SIGFPE)
	printf("Float\n");
    */

    pxd_PIXCIclose();
    exit(1);
}

/*
 *  SUPPORT STUFF:
 *
 *  Slow down execution speed so
 *  each step/function can be observed.
 */
void user(const char *mesg)
{
    if (mesg && *mesg)
	printf("%s\n", mesg);
    #if USER_PROMPTS
	fprintf(stderr, "\n\nContinue (Key ENTER) ?");
	while (fgetchar() != '\n') ;
    #endif
    fprintf(stderr, "\n");
}

/*
 * Call back function, for demonstrating interrupt hooks under DOS.
 * This one function must always have stack checking disabled.
 * Since Watcom doesn't provide a pragma to restore stack checking
 * as per command line options, the '#pragma on (check_stack)'
 * is shown, but may be removed as per the programmer's
 * intent for the remainder of the program.
 *
 * This function and the data it references must not be pageable.
 * Here, we assume that VM isn't in use (VM is only an option
 * in the DOS4G version of the DOS4GW/DOS4GWPro/DOS4G family).
 */
#if !(defined(_WIN32) || defined(__WIN32__) || defined(_WIN64))

#pragma off (check_stack)
int fieldirqcount = 0;
int _cfunfcc irqfunction(void *p, int a, int b)
{
    fieldirqcount++;
    return(0);
}
#pragma on (check_stack)    // optional

#endif


/*
 * Open the XCLIB C Library for use.
 */
int do_open(void)
{
    int r;
    char driverparms[80];

    printf("Opening EPIX(R) PIXCI(R) Frame Grabber,\n");
    printf("using configuration parameters '%s',\n", DRIVERPARMS? DRIVERPARMS: "default");

    driverparms[sizeof(driverparms)-1] = 0; // this & snprintf: overly conservative - avoids warning messages
    _snprintf(driverparms, sizeof(driverparms)-1, "-DM 0x%x %s", UNITSOPENMAP, DRIVERPARMS);

    //
    // Either FORMAT or FORMATFILE should have been
    // selected above.
    //
    #if defined(FORMAT)
	printf("and using predefined format '%s'.\n\n", FORMAT);
	r = pxd_PIXCIopen(driverparms, FORMAT, "");
    #elif defined(FORMATFILE)
	printf("and using format file '%s'.\n\n", FORMATFILE);
	r = pxd_PIXCIopen(driverparms, "", FORMATFILE);
    #else
	#error "Select a video format or a video format file"
    #endif
    if (r >= 0) {
	//
	// Rarely, a possible warning after open.
	pxd_mesgFault(UNITSMAP);
	//
	// For the sake of demonstrating optional interrupt hooks.
	//
	#if !(defined(_WIN32) || defined(__WIN32__) || defined(_WIN64))
	    pxd_eventCapturedFieldCreate(1, irqfunction, NULL);
	#endif

	user("Open OK");
    } else {
	printf("Open Error %s(%d)\a\a\n", pxd_mesgErrorCode(r), r);
	pxd_mesgFault(UNITSMAP);
	user("");
    }
    return(r);
}

/*
 * Report image frame buffer memory size
 */
void do_imsize(void)
{
    #if 1
	printf("Image frame buffer memory size: %.3f Kbytes\n", (double)pxd_infoMemsize(UNITSMAP)/1024);
    #elif defined(OS_WIN64)|defined(OS_WIN64_DLL)
	printf("Image frame buffer memory size: %I64u Kbytes\n", pxd_infoMemsize(UNITSMAP)/1024);
    #else
	printf("Image frame buffer memory size: %lu Kbytes\n", pxd_infoMemsize(UNITSMAP)/1024);
    #endif
    printf("Image frame buffers           : %d\n", pxd_imageZdim());
    printf("Number of boards              : %d\n", pxd_infoUnits());
    user("");
}

/*
 * Report image resolution.
 */
void do_vidsize(void)
{
    printf("Image resolution:\n");
    printf("xdim           = %d\n", pxd_imageXdim());
    printf("ydim           = %d\n", pxd_imageYdim());
    printf("colors         = %d\n", pxd_imageCdim());
    printf("bits per pixel = %d\n", pxd_imageCdim()*pxd_imageBdim());
    user("");
}


/*
 * Capture
 */
void do_video1(void)
{
    int err = 0;
    //
    // Capture image into buffer 1.
    //
    printf("Field count before snap=%d\n", pxd_videoFieldCount(1));
    err = pxd_doSnap(UNITSMAP, 1, 0);
    printf("pxd_doSnap: %s\n", err>=0? "Ok": pxd_mesgErrorCode(err));
    printf("Field count after  snap=%d\n", pxd_videoFieldCount(1));
    #if !(defined(_WIN32) || defined(__WIN32__) || defined(_WIN64))
	printf("Captured field count from IRQ hook=%d\n", fieldirqcount);
    #endif
    //
    // Check for faults, such as erratic sync
    // or insufficient PCI bus bandwidth
    //
    pxd_mesgFault(UNITSMAP);
    user("Image snapped into buffer 1");
}

/*
 * General purpose output will pulse, running for several seconds.
 */
void do_extout1(void)
{
    pxd_getGPOut(UNITSMAP, 0);
    user("Ready to watch general purpose output signal?");
}
void do_extout2(void)
{
    int     j;

    printf("Toggling general purpose output ..\n");
    for (j = 60*3; j--; ) {
	pxd_getGPOut(UNITSMAP, 0x1);
	pxd_getGPOut(UNITSMAP, 0x0);
    }
    user("general purpose output pulses done");
}

/*
 * The general purpose input is often used as a flag to start an
 * activity, with the application code looping until
 * the general purpose input changes.
 * No signal may be present during this demo, so as to avoid
 * indefinite loops the count is simply printed twice,
 * giving the user an opportunity to toggle the input.
 * This (silently) sets the GPOut, to provide a convenient
 * signal source for testing.
 */
void do_extin1(void)
{
    int err = 0;
    //
    // On PIXCI(R) D, D24, D32 frame grabbers the input, if available,
    // is latched, until explicitly reset.
    // This has no effect on other PIXCI(R) frame grabbers
    // in which the inputs are level sensitive.
    //
    err = pxd_setGPIn(UNITSMAP, 0x0);
    if (err < 0)
	printf("pxd_setGPIn: %s\n", pxd_mesgErrorCode(err));
    err = pxd_setGPOut(UNITSMAP, 0x0);
    if (err < 0)
	printf("pxd_setGPOut: %s\n", pxd_mesgErrorCode(err));
    printf("Current value of general purpose input: 0x%x\n", pxd_getGPIn(UNITSMAP, 0));
    user("");
}
void do_extin2(void)
{
    int err = 0;
    err = pxd_setGPIn(UNITSMAP, 0x0);
    if (err < 0)
	printf("pxd_setGPIn: %s\n", pxd_mesgErrorCode(err));
    err = pxd_setGPOut(UNITSMAP, 0x1);
    if (err < 0)
	printf("pxd_setGPOut: %s\n", pxd_mesgErrorCode(err));
    printf("Current value of general purpose input: 0x%x\n", pxd_getGPIn(UNITSMAP, 0));
    user("");
}

/*
 * Pixels are transferred to a PC buffer, and numerically displayed.
 */
#define AOI_XDIM    3
#define AOI_YDIM    10
#define COLORS	    3
uchar	colorimage_buf[AOI_XDIM*AOI_YDIM*COLORS];

void color_display1(void)
{
    int     i;
    int     cx = (pxd_imageXdim()-AOI_XDIM)/2;	// left coordinate of centered AOI
    int     cy = (pxd_imageYdim()-AOI_YDIM)/2;	// top	coordinate of centered AOI

    //
    // Transfer the color data from a selected AOI of
    // the image buffer into the PC buffer in
    // YCrCb (Intensity, Red Chroma and Blue Chroma)
    // format.	The number of bytes to be transfered
    // is 3 times the number of pixels.
    //
    if ((i = pxd_readuchar(UNITSMAP, 1, cx, cy, cx+AOI_XDIM, cy+AOI_YDIM, colorimage_buf, sizeof(colorimage_buf)/sizeof(uchar), "YCRCB")) != AOI_XDIM*AOI_YDIM*3) {
	if (i < 0)
	    printf("pxd_readuchar: %s\n", pxd_mesgErrorCode(i));
	else
	    printf("pxd_readuchar error: %d != %d\n", i, AOI_XDIM*AOI_YDIM*3);
	user("");
	return;
    }
    user("Image area of interest transfered for YCrCb");
}
void color_display2(void)
{
    int     x, y;
    //
    // Display data from the PC buffer.
    //
    for (y = 0; y < AOI_YDIM; y++) {
	for (x = 0; x < AOI_XDIM; x++) {
	    printf(" Y=%3d  ", colorimage_buf[(y*AOI_XDIM+x)*3+0]);
	    printf("Cr=%3d  ", colorimage_buf[(y*AOI_XDIM+x)*3+1]);
	    printf("Cb=%3d  ", colorimage_buf[(y*AOI_XDIM+x)*3+2]);
	}
	printf("\n");
    }
    user("");
}
void color_display3(void)
{
    int     i;
    int     cx = (pxd_imageXdim()-AOI_XDIM)/2;	// left coordinate of centered AOI
    int     cy = (pxd_imageYdim()-AOI_YDIM)/2;	// top	coordinate of centered AOI

    //
    // Transfer the color data from a selected AOI
    // in the image buffer into the PC buffer in
    // RGB format.
    //
    if ((i = pxd_readuchar(UNITSMAP, 1, cx, cy, cx+AOI_XDIM, cy+AOI_YDIM, colorimage_buf, sizeof(colorimage_buf)/sizeof(uchar), "RGB")) != AOI_XDIM*AOI_YDIM*3) {
	printf("pxd_readuchar error: %d != %d\n", i, AOI_XDIM*AOI_YDIM);
	user("");
	return;
    }
    user("Image area of interest transfered for RGB");
}
void color_display4(void)
{
    int     x, y;
    //
    // Display data from the PC buffer.
    //
    for (y = 0; y < AOI_YDIM; y++) {
	for (x = 0; x < AOI_XDIM; x++) {
	    printf("R=%3d ", colorimage_buf[(y*AOI_XDIM+x)*3+0]);
	    printf("G=%3d ", colorimage_buf[(y*AOI_XDIM+x)*3+1]);
	    printf("B=%3d ", colorimage_buf[(y*AOI_XDIM+x)*3+2]);
	}
	printf("\n");
    }
    user("");
}
void color_display5(void)
{
    int     i;
    int     cx = (pxd_imageXdim()-AOI_XDIM)/2;	// left coordinate of centered AOI
    int     cy = (pxd_imageYdim()-AOI_YDIM)/2;	// top	coordinate of centered AOI

    //
    // Transfer the color data from a selected AOI
    // in the image buffer into the PC buffer in
    // BSH format.
    //
    if ((i = pxd_readuchar(UNITSMAP, 1, cx, cy, cx+AOI_XDIM, cy+AOI_YDIM, colorimage_buf, sizeof(colorimage_buf)/sizeof(uchar), "BSH")) != AOI_XDIM*AOI_YDIM*3) {
	if (i < 0)
	    printf("pxd_readuchar: %s\n", pxd_mesgErrorCode(i));
	else
	    printf("pxd_readuchar error: %d != %d\n", i, AOI_XDIM*AOI_YDIM*3);
	user("");
	return;
    }
    user("Image area of interest transfered for BSH");
}
void color_display6(void)
{
    int     x, y;
    //
    // Display data from the PC buffer.
    // The HSB values, ranging from 0 to 255, are rescaled
    // as displayed into the more typical 0 to 1 for S and B,
    // and 0 to 360 for H.
    //
    for (y = 0; y < AOI_YDIM; y++) {
	for (x = 0; x < AOI_XDIM; x++) {
	    printf("B=%5.2f ", colorimage_buf[(y*AOI_XDIM+x)*3+0]/255.);
	    printf("S=%5.2f ", colorimage_buf[(y*AOI_XDIM+x)*3+1]/255.);
	    printf("H=%3.0f ", colorimage_buf[(y*AOI_XDIM+x)*3+2]*360./256.);
	}
	printf("\n");
    }
    user("");
}

void color_display7(int component)
{
    int     i;
    int     cx = (pxd_imageXdim()-AOI_XDIM)/2;	// left coordinate of centered AOI
    int     cy = (pxd_imageYdim()-AOI_YDIM)/2;	// top	coordinate of centered AOI
    char    *colorspace = "";

    //
    // Transfer one component of the color data from a selected AOI
    // in the image buffer into the PC buffer in RGB or BSH format.
    //
    switch (component) {
      case 0:	colorspace = "RofRGB";	break;
      case 1:	colorspace = "GofRGB";	break;
      case 2:	colorspace = "BofRGB";	break;
      case 3:	colorspace = "BofBSH";	break;
      case 4:	colorspace = "SofBSH";	break;
      case 5:	colorspace = "HofBSH";	break;
      case 6:	colorspace = "Gray";	break;
      default:	return;
    }
    if ((i = pxd_readuchar(UNITSMAP, 1, cx, cy, cx+AOI_XDIM, cy+AOI_YDIM, colorimage_buf, sizeof(colorimage_buf)/sizeof(uchar), colorspace)) != AOI_XDIM*AOI_YDIM) {
	if (i < 0)
	    printf("pxd_readuchar: %s\n", pxd_mesgErrorCode(i));
	else
	    printf("pxd_readuchar error: %d != %d\n", i, AOI_XDIM*AOI_YDIM);
	user("");
	return;
    }
    user("Image area of interest transfered for one color component");
}

void color_display8(int component)
{
    int     x, y;
    //
    // Display data from the PC buffer.
    // The HSB values, ranging from 0 to 255, are rescaled
    // as displayed into the more typical 0 to 1 for S and B,
    // and 0 to 360 for H.
    //
    for (y = 0; y < AOI_YDIM; y++) {
	for (x = 0; x < AOI_XDIM; x++) {
	    switch (component) {
	      case 0:
		printf("RofRGB=%3d ", colorimage_buf[(y*AOI_XDIM+x)*1+0]);
		break;
	      case 1:
		printf("GofRGB=%3d ", colorimage_buf[(y*AOI_XDIM+x)*1+0]);
		break;
	      case 2:
		printf("BofRGB=%3d ", colorimage_buf[(y*AOI_XDIM+x)*1+0]);
		break;
	      case 3:
		printf("BofBSH=%5.2f ", colorimage_buf[(y*AOI_XDIM+x)*1+0]/255.);
		break;
	      case 4:
		printf("SofBSH=%5.2f ", colorimage_buf[(y*AOI_XDIM+x)*1+0]/255.);
		break;
	      case 5:
		printf("HofBSH=%3.0f ", colorimage_buf[(y*AOI_XDIM+x)*1+0]*360./256.);
		break;
	      case 6:
		printf("Grey=%3d ", colorimage_buf[(y*AOI_XDIM+x)*1+0]);
		break;
	      default:
		return;
	    }
	}
	printf("\n");
    }
    user("");
}

#undef	AOI_XDIM
#undef	AOI_YDIM
#undef	COLORS



/*
 * Pixels are transferred to a PC buffer, and numerically displayed.
 * AOI 感兴趣区域
 */
#define AOI_XDIM    9
#define AOI_YDIM    10
uchar	monoimage_buf8[AOI_XDIM*AOI_YDIM];
ushort	monoimage_buf16[AOI_XDIM*AOI_YDIM];

void bw_display1(void)
{
    int     i;
    int     cx = (pxd_imageXdim()-AOI_XDIM)/2;	// left coordinate of centered AOI
    int     cy = (pxd_imageYdim()-AOI_YDIM)/2;	// top	coordinate of centered AOI
    //
    // Transfer the monochrome data from a selected AOI of
    // the image buffer into the PC buffer, as 8 bit pixels.
    // Or,
    // Transfer the monochrome data from a selected AOI of
    // the image buffer into the PC buffer, as 8 to 16 bit pixels.
    //
    // The ushort array could be used for both for 8 bit pixels, but
    // users of 8 bit pixels commonly assume pixel<=>byte,
    // and is more efficient.
    //
    if (pxd_imageBdim() <= 8) {
	if ((i = pxd_readuchar(UNITSMAP, 1, cx, cy, cx+AOI_XDIM, cy+AOI_YDIM, monoimage_buf8, sizeof(monoimage_buf8)/sizeof(uchar), "Grey")) != AOI_XDIM*AOI_YDIM) {
	    if (i < 0)
		printf("pxd_readuchar: %s\n", pxd_mesgErrorCode(i));
	    else
		printf("pxd_readuchar error: %d != %d\n", i, AOI_XDIM*AOI_YDIM);
	    user("");
	    return;
	}
    } else {
	if (i = pxd_readushort(UNITSMAP, 1, cx, cy, cx+AOI_XDIM, cy+AOI_YDIM, monoimage_buf16, sizeof(monoimage_buf16)/sizeof(ushort), "Grey") != AOI_XDIM*AOI_YDIM) {
	    if (i < 0)
		printf("pxd_readushort: %s\n", pxd_mesgErrorCode(i));
	    else
		printf("pxd_readushort error: %d != %d\n", i, AOI_XDIM*AOI_YDIM);
	    user("");
	    return;
	}
    }
    user("Image area of interest transfered");
}
void bw_display2(void)
{
    int     x, y;
    //
    // Display data from the PC buffer.
    //
    for (y = 0; y < AOI_YDIM; y++) {
	printf("Grey = ");
	for (x = 0; x < AOI_XDIM; x++)
	    if (pxd_imageBdim() <= 8)
		printf("%4d ", monoimage_buf8[y*AOI_XDIM+x]);
	    else
		printf("%4d ", monoimage_buf16[y*AOI_XDIM+x]);
	printf("\n");
    }
    user("");
}


/*
 * Be nice and careful: For sake of this example which uses a
 * file name not selected by the user, and which might already exist,
 * don't overwrite the file if it already exists.
 * This check for a pre-existant file is the only reason
 * that fopen() need be done; this section of code
 * isn't a requirement for saving images.
 */
int checkexist(char *name)
{
    FILE    *fp;
    if ((fp = fopen(name, "rb")) != NULL) {
	fclose(fp);
	printf("Image not saved, file %s already exists\n", name);
	user("");
	return(1);
    }
    return(0);
}

/*
 * Save image in industry standard .tif format.
 */
void do_savetif()
{
    int     u, err = 0;
    char    name[] = IMAGEFILE_DIR "/" "image0.tif";

    for (u = 0; u < UNITS; u++) {
	name[strlen(name)-5] = '0'+u;

	//
	// Don't overwrite existing file.
	//
	if (checkexist(name))
	    return;

	//
	// Do save of entire image in TIFF format.
	//
	if (UNITS == 1)
	    printf("Image buffer 1 being saved to file %s\n", name);
	else
	    printf("Unit %d Image buffer 1 being saved to file %s\n", u, name);
	err = pxd_saveTiff(1<<u, name, 1, 0, 0, -1, -1, 0, 0); // 将图像采集卡buffer 1完整地存入tif文件(不使用AOI)
	if (err < 0) {
	    printf("pxd_saveTiff: %s\n", pxd_mesgErrorCode(err));
	    user("");
	    return;
	}
	user("Image buffer saved");
    }
}



/*
 * Save image in the Windows .bmp format.
 * This feature is available under DOS as well.
 */
void do_savebmp()
{
    int     u, err = 0;
    char    name[] = IMAGEFILE_DIR "/" "image0.bmp";

    for (u = 0; u < UNITS; u++) {
	name[strlen(name)-5] = '0'+u;

	//
	// Don't overwrite existing file.
	//
	if (checkexist(name))
	    return;

	//
	// Do save of entire image in Bitmap format.
	// Monochrome image buffers are saved as an 8 bit monochrome image,
	// color image buffers are saved as an 24 bit RGB color image.
	//
	if (UNITS == 1)
	    printf("Image buffer 1 being saved to file %s\n", name);
	else
	    printf("Unit %d Image buffer 1 being saved to file %s\n", u, name);
	err = pxd_saveBmp(1<<u, name, 1, 0, 0, -1, -1, 0, 0);
	if (err < 0) {
	    printf("pxd_saveBmp: %s\n", pxd_mesgErrorCode(err));
	    user("");
	    return;
	}
	user("Image buffer saved");
    }
}

/*
 * Save image in the JPEG format.
 * This requires the PXIPL library.
 */
void do_savejpeg()
{
    #if USE_PXIPL
	int	u, err = 0;
	char	name[] = IMAGEFILE_DIR "/" "image0.jpg";

	for (u = 0; u < UNITS; u++) {
	    name[strlen(name)-5] = '0'+u;

	    //
	    // Don't overwrite existing file.
	    //
	    if (checkexist(name))
		return;

	    //
	    // Do save of entire image in JPEG format.
	    //
	    if (UNITS == 1)
		printf("Image buffer 1 being saved to file %s\n", name);
	    else
		printf("Unit %d Image buffer 1 being saved to file %s\n", u, name);
	    err = pxio8_jpegwrite(NULL, pxd_defineImage(u,1,0,0,-1,-1,"RGB"), NULL, name, 8, NULL);
	    if (err < 0) {
		printf("pxio8_jpegwrite: %s\n", pxd_mesgErrorCode(err));
		user("");
		return;
	    }
	    user("Image buffer saved");
	}
    #endif
}

/*
 * Display image on the S/VGA, using the PXIPL Library.
 * (Image display on S/VGA is available under Windows with or
 * without the PXIPL library, but not shown in this command-line
 * oriented example).
 */
#if !(defined(_WIN32) || defined(__WIN32__) || defined(_WIN64))
void do_vgadisplay()
{
  #if USE_PXIPL

    struct  pximage vga;

    //
    // Put S/VGA in graphics mode. The vga_open supports high resolution,
    // but it can't detect which monitor is connected. Use a (monitor) safe
    // 640x480 resolution.
    //
    vga_open(0, 640, 480, 24, 1, &vga, NULL);

    //
    // One way to check success is whether the `vga' access
    // has a nonzero dimension, or non zero number of pixies.
    //
    if (!vga.d.pixies) {
	user("Can't put S/VGA into graphics mode");
	return;
    }

    for (u = 0; u < UNITS; u++) {
	//
	// Show image buffer 1 in center 1/4 of screen.
	// Selecting 7 bits in certain modes reflects how vga_open()
	// optionally set up the vga palette, for 8 bit per pixel VGA modes.
	//
	vga.wind.nw.x = vga.wind.se.x/4;
	vga.wind.nw.y = vga.wind.se.y/4;
	vga.wind.se.x = 3*vga.wind.se.x/4;
	vga.wind.se.y = 3*vga.wind.se.y/4;
	(vga.xwind)(&vga, &vga.wind, 's');
	err = pxio8_vgadisplay(NULL, pxd_defineImage(1<<u, 1, 0, 0, -1, -1, "Display"), NULL,
		(vga.d.pixies == 1 && vga.d.u.i.bitsused == 8)? 7: vga.d.u.i.bitsused,
		'n', 1, 0, &vga, NULL, NULL);
	if (err < 0)
	    printf("pxio8_vgadisplay: %s\n", pxd_mesgErrorCode(err));

	//
	// Show message.
	// Assume worst case, that S/VGA BIOS doesn't support printf in this mode.
	//
	{
	    struct pxy xy	 = {0, 0};
	    uint   forecolor[3]  = {255, 255, 0};
	    uint   backcolor[3]  = {0, 0, 0};

	    (vga.xwind)(&vga, NULL, 's');   // set full window
	    err = pxip8_drawchars(NULL, &vga, &xy,
			    "S/VGA graphics mode set, and image displayed.",
			    45, 8, 8, 0, 1, 0, backcolor, forecolor, 0);
	    if (err < 0)
		printf("pxip8_drawchars: %s\n", pxd_mesgErrorCode(err));
	    xy.y += 10;
	    err = pxip8_drawchars(NULL, &vga, &xy, "Continue (Key ENTER) ?",
			    22, 8, 8, 0, 1, 0, backcolor, forecolor, 0);
	    if (err < 0)
		printf("pxip8_drawchars: %s\n", pxd_mesgErrorCode(err));
	}

	//
	// Wait for user
	//
	user("");
    }

    //
    // Reset S/VGA so the remainder of this example,
    // which uses printf(), can proceed.
    //
    vga_close(&vga, NULL);

  #endif
}
#endif

/*
 * Capture sequence
 */
static void do_videosequence(void)
{
    int err;
    //
    // Capture sequence of images into all frame buffers
    // This function returns immediately; we must wait till done.
    //
    err = pxd_goLiveSeq(UNITSMAP, 1, pxd_imageZdim(), 1, pxd_imageZdim(), pxd_imageIdim());
    printf("pxd_goLiveSeq: %s\n", err>=0? "Ok": pxd_mesgErrorCode(err));
    if (err >= 0) {
	printf("Capturing images ...");
	while (pxd_goneLive(UNITSMAP, 0))
	    printf(".");
	printf("\n");
    }
    //
    // Check for faults, such as erratic sync
    // or insufficient PCI bus bandwidth
    //
    pxd_mesgFault(UNITSMAP);
    user("Image seqeunce captured");
}

/*
 * Save sequence
 */
static void do_savesequence(void)
{
    int     u, i, err = 0;
    char    name[MAX_PATH];

    for (u = 0; u < UNITS; u++) {
	printf("Saving images ...");
	for (i = 1; i <= pxd_imageZdim(); i++) {
	    _snprintf(name, sizeof(name)-1, "%s/image%d_%.6d.tif", IMAGEFILE_DIR, u, i);
	    name[sizeof(name)-1] = 0;
	    //
	    // Don't overwrite existing file.
	    //
	    if (checkexist(name))
		continue;

	    //
	    // Do save of entire image in TIFF format.
	    //
	    err = pxd_saveTiff(1<<u, name, i, 0, 0, -1, -1, 0, 0);
	    if (err < 0) {
		printf("pxd_saveTiff: %s\n", pxd_mesgErrorCode(err));
		user("");
		return;
	    }
	    printf(".");
	}
	printf("\n");
	user("Image buffers saved");
    }
}



/*
 * Close the PIXCI(R) frame grabber
 */
void do_close(void)
{
    pxd_PIXCIclose();
    user("PIXCI(R) frame grabber closed");
}


void hello(void)
{
    printf("\n\n");
    printf("PIXCI(R) Frame Grabbers -  XCLIB 'C' Library\n");
    printf("XCLIBEX1.C - Example program\n");
    printf("Copyright (C)  1996-2010  EPIX, Inc.  All rights reserved.\n");
    printf("\n");
    printf("This program is best used by executing this program\n");
    printf("one step at a time, while simultaneously reading\n");
    printf("the XCLIB documentation and program listing.\n");
    user("");
}

/*
 *  MAIN:
 *
 *  每个库函数都在其自己的子程序中进行演示，
 *  main函数调用每个子程序来产生交互式演示。
 *
 *  建议在编译程序执行的同时阅读此程序源代码。
 *
 */
main(void)
{ 
    //
    // Say Hello
    //
    hello();

    //
    // Catch signals.
    // 受到中断信号和浮点异常信号就执行pxd_PIXCIclose(void);
    signal(SIGINT, signaled);
    signal(SIGFPE, signaled);

    //
    // Open and set video format.
    // 确保采集卡已经连接，驱动正确安装，对video进行配置
    if (do_open() < 0)
	return(1);

    //
    // Basic video operations
    //
    do_imsize(); // 获取图像的尺寸  
    do_vidsize(); // 获取视频的尺寸
    do_video1(); // 采集一帧图像，存入图像采集卡buffer 1
    do_video1(); // 采集一帧图像，存入图像采集卡buffer 1

    //
    // Show pixel values
    //
    if (pxd_imageCdim() == 1) { // 单色图片
	bw_display1(); // 将图像采集卡buffer 1存入PC buf
	bw_display2(); // 将PC buf打印到终端
    } else {
	int i;
	color_display1();
	color_display2();
	color_display3();
	color_display4();
	color_display5();
	color_display6();
	for (i = 0; i < 7; i++) { // 彩色图片
	    color_display7(i);
	    color_display8(i);
	}
    }

    //
    // Save image
    //
    // do_savetif();
    // do_savebmp();
    #if USE_PXIPL
	do_savejpeg();
    #endif

    //
    // Display image.
    //
    #if !(defined(_WIN32) || defined(__WIN32__) || defined(_WIN64))
	do_vgadisplay();
    #endif

    //
    // Do sequence capture & save
    //
    do_videosequence();
    do_savesequence();

    //
    // Do General Purpose Input/Output signals
    //
    do_extin1();
    do_extin2();
    do_extout1();
    do_extout2();

    //
    // All done
    //
    do_close();
    return(0);
}
