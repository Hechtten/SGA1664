/*
 *
 *	xclibexg.c	External	08-Jan-2023
 *
 *	Copyright (C)  1996-2023  EPIX, Inc.	All rights reserved.
 *
 *	Example program for PIXCI EB1tg frame generator.
 *
 *	Example program for XCLIB C Library, SCF Level functions.
 *	Example assumes Windows 'command line' environment.
 *
 */


/*
 *  INSTRUCTIONS:
 *
 *  1)	Set 'define' options below according to the intended
 *	video format.
 *
 *	Ue XCAP to save the video set-up to a
 *	file, and set FORMATFILE to the saved file's name.
 *
 */


#if !defined(FORMAT) && !defined(FORMATFILE)

  //#define FORMAT	"default"	// unlike frame grabbers, there
					// aren't any named formats

    #define FORMATFILE	"xcvidset.fmt"	// using format file saved by XCAP
#endif


/*
 *  2.1) Set number of expected PIXCI(R) frame generators.
 *  This example expects that the boards are
 *  identical and operated at the same resolution.
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
 *  selected PIXCI(R) frame generators; to simplify
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
 *  3)	Choose whether the PXIPL Image Processing Library
 *	is available, with which graphic or test pattern images can
 *	be created.
 */
#if !defined(USE_PXIPL)
    #define USE_PXIPL	0
#endif

/*
 *  4) Select directory for loading example ANTA*.TIF images
 *  from the PIXCI(R) CD.
 */
#if !defined(IMAGEFILE_DIR)
    #define IMAGEFILE_DIR    ""
#endif

/*
 *  5) Run without prompts?
 */
#if !defined(USER_PROMPTS)
    #define USER_PROMPTS   1
#endif

/*
 *  6)	Under Windows, compile
 *	    XCLIBEXG.C
 *	in the same manner as any Win32 or Win64 command line application,
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
#endif
#include "xcliball.h"		// function prototypes


/*
 *  SUPPORT STUFF:
 *
 *  Catch CTRL+C and floating point exceptions so that
 *  once opened, the PIXCI(R) driver and frame generator
 *  are always closed before exit.
 *  In most environments, this isn't critical; the operating system
 *  advises the PIXCI(R) driver that the program has terminated.
 *  In other environments, such as DOS, the operating system
 *  isn't as helpful and the driver & frame grabber would remain open.
 */
static void sigintfunc(int sig)
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
 * Open the XCLIB C Library for use.
 */
int do_open(void)
{
    int r;
    char driverparms[80];

    printf("Opening EPIX(R) PIXCI(R) Frame Grabber/Generator,\n");
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
 * Flash
 */
void do_flash1(void)
{
    int err = 0;
    //
    // Flash image from buffer 1.
    //
    printf("Field count before snap=%d\n", pxd_videoFieldCount(1));
    err = pxd_doSnap(UNITSMAP, 1, 0);
    printf("pxd_doSnap: %s\n", err>=0? "Ok": pxd_mesgErrorCode(err));
    printf("Field count after  snap=%d\n", pxd_videoFieldCount(1));
    //
    // Check for faults, such as insufficient PCI bus bandwidth
    //
    pxd_mesgFault(UNITSMAP);
    user("Image flashed from buffer 1");
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
    // This has no effect on other PIXCI(R) frame grabbers/generators
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
 * Stream sequence
 */
static void do_videosequence1(void)
{
    #define SUBRATE 1	// 1: video rate, 2: 1/2 video rate, 3: 1/3 video rate, etc.
    int err;

    //
    // Stream sequence of images from all frame buffers
    // This function returns immediately; we must wait till done.
    //
    err = pxd_goLiveSeq(UNITSMAP, 1, pxd_imageZdim(), 1, pxd_imageZdim(), pxd_imageIdim()*SUBRATE);
    printf("pxd_goLiveSeq: %s\n", err>=0? "Ok": pxd_mesgErrorCode(err));
    if (err >= 0) {
	printf("Streaming images ...");
	while (pxd_goneLive(UNITSMAP, 0))
	    printf(".");
	printf("\n");
    }
    //
    // Check for faults, such as insufficient PCI bus bandwidth
    //
    pxd_mesgFault(UNITSMAP);
    user("Image seqeunce streamed");
}

/*
 * Stream sequence, repeatedly.
 */
static void do_videosequence2(void)
{
    #define SUBRATE 1	// 1: video rate, 2: 1/2 video rate, 3: 1/3 video rate, etc.
    #define SECONDS 10
    int err;
    int i;

    //
    // Stream sequence of images from all frame buffers, repeatedly
    // for 10 seconds
    //
    err = pxd_goLiveSeq(UNITSMAP, 1, pxd_imageZdim(), 1, 0, pxd_imageIdim()*SUBRATE);
    printf("pxd_goLiveSeq: %s\n", err>=0? "Ok": pxd_mesgErrorCode(err));
    if (err >= 0) {
	printf("Streaming images ...");
	for (i = 0; i < SECONDS*10; i++) {
	    Sleep(100);
	    printf(".");
	}
	printf("\n");
	// Stop streaming at end of image,
	// in contrast to aborting immediately and mid-image.
	pxd_goUnLive(UNITSMAP);
	while (pxd_goneLive(UNITSMAP, 0))
	    printf(".");
	printf("\n");
    }
    //
    // Check for faults, such as insufficient PCI bus bandwidth
    //
    pxd_mesgFault(UNITSMAP);
    user("Image seqeunce streamed");
}

/*
 * Load image sequence.
 * Loads ANTA08.tif thru ANTA18.TIF; these are
 * provided on the PIXCI CD.
 */
static void do_loadsequence(void)
{
    int     u, ants, i, err = 0;
    char    name[MAX_PATH];

    for (u = 0; u < UNITS; u++) {
	printf("Loading images ...");
	ants = 0;
	for (i = 1; i <= pxd_imageZdim(); i++, ants++) {
	    _snprintf(name, sizeof(name)-1, "%s/ANTA%.2d.tif", IMAGEFILE_DIR, 8+(ants%11));
	    name[sizeof(name)-1] = 0;
	    //
	    // Load of image in TIFF format.
	    err = pxd_loadTiff(1<<u, name, i, 0, 0, -1, -1, 0x100|'b', 0);
	    if (err < 0) {
		printf("pxd_loadTiff: %s\n", pxd_mesgErrorCode(err));
		user("");
		return;
	    }
	    printf(".");
	}
	printf("\n");
	user("Image buffers loaded");
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
    printf("PIXCI(R) Frame Grabbers/Generators -  XCLIB 'C' Library\n");
    printf("XCLIBEX1.C - Example program\n");
    printf("Copyright (C)  1996-2023  EPIX, Inc.  All rights reserved.\n");
    printf("\n");
    printf("This program is best used by executing this program\n");
    printf("one step at a time, while simultaneously reading\n");
    printf("the XCLIB documentation and program listing.\n");
    user("");
}

/*
 *  MAIN:
 *
 *  Each library function is demonstrated in its own subroutine,
 *  the main calls each subroutine to produce the interactive demonstration.
 *
 *  It is suggested that this program source be read at the same time
 *  as the compiled program is executed.
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
    //
    signal(SIGINT, sigintfunc);
    signal(SIGFPE, sigintfunc);

    //
    // Open and set video format.
    //
    if (do_open() < 0)
	return(1);

    //
    // Video info
    //
    do_imsize();
    do_vidsize();

    //
    // Load images
    //
    do_loadsequence();

    //
    // Flash an image
    //
    do_flash1();
    do_flash1();

    //
    // Stream video
    //
    do_videosequence1();
    do_videosequence2();

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
