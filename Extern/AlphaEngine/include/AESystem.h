/******************************************************************************/
/*!
\par        Project: Alpha Engine
\file       AESystem.h

\author     Sun Tjen Fam
\date       January 31, 2007

\brief      Header file for the system library.

\copyright  Copyright (C) 2013 DigiPen Institute of Technology. Reproduction 
            or disclosure of this file or its contents without the prior 
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#ifndef AE_SYSTEM_H
#define AE_SYSTEM_H


// ---------------------------------------------------------------------------
#ifdef __cplusplus 
extern "C"
{
#endif

/******************************************************************************/
/*!
\fn         s32 AESysInit (HINSTANCE hAppInstance, 
                           s32 show, 
                           s32 WinWidth, 
                           s32 WinHeight, 
                           s32 CreateConsole, 
                           u32 FrameRateMax, 
                           LRESULT (*pWinCallBack)(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp))

\brief      Initialize the Alpha Engine.

\details    Call this function once at the start of the program
            to initialize the system.

\param      [in] hAppInstance
            Handle to the current instance of the application.

\param      [in] show
            Set how the the window is to be shown.

\param      [in] WinWidth
            Set the width of the window.

\param      [in] WinHeight
            Set the height of the window.

\param      [in] CreateConsole
            Input 1 if a console window should be created.
            Else input 0.

\param      [in] FrameRateMax
            Set the maximum frames per second.

\param      [in] vsync
			enables/disables VSync

\param      [in] pWinCallBack
            Pointer to a callback function. 
            May be left null if not needed.

\param      [in] IconFilename
            CString that represents the filename
            of the icon to be loaded.
            Maybe left null if not needed; a default icon will display.
            The file MUST be of .ico format

\param      [in] IconWidth
            The width of the icon image specified by IconFilename.

\param      [in] IconHeight
            The height of the icon image specified by IconFilename.
            
            
*/
/******************************************************************************/
AE_API s32  AESysInit				(HINSTANCE hAppInstance, s32 show, s32 WinWidth, s32 WinHeight, s32 CreateConsole, u32 FrameRateMax, bool vsync, LRESULT (CALLBACK *pWinCallBack)(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp));


/******************************************************************************/
/*!

\fn         void AESysSetWindowIcon()

\brief      

*/
/******************************************************************************/
AE_API void AESysSetWindowIcon(const char* filename, s32 width, s32 height);


/******************************************************************************/
/*!
\fn         void AESysReset()

\brief      Reset the Alpha Engine.

\details    Call this function when changing gamestates to reset 
            the system.

*/
/******************************************************************************/
AE_API void AESysReset				();

// Already called in AESysFrameEnd
//AE_API void AESysUpdate				();

/******************************************************************************/
/*!
\fn         void AESysExit()

\brief      Free the Alpha Engine.

\details    Call this function once at the end of the program
            to free the system.

*/
/******************************************************************************/
AE_API void AESysExit				();

/******************************************************************************/
/*!
\fn         void AESysFrameStart()

\brief      Start of frame.

\details    Call this function once at the start of the frame to
            inform the system a new frame is starting.

*/
/******************************************************************************/
AE_API void AESysFrameStart();

/******************************************************************************/
/*!
\fn         void AESysFrameEnd()

\brief      End of frame.

\details    Call this function once at the end of the frame to
            inform the system the current frame is ending.

*/
/******************************************************************************/
AE_API void AESysFrameEnd();

/******************************************************************************/
/*!
\fn         void AESysIsFullScreen()

\brief      Returns whether we are in full screen mode.

\retval     s32
            Returns 1 if we are in full screen mode. 
            Otherwise, it will return 0.

*/
/******************************************************************************/
AE_API s32 AESysIsFullScreen();

/******************************************************************************/
/*!
\fn         void AESysIsFocus()

\brief      Returns whether the application window is currently the focused
            window of the operating system.

\retval     s32
            Returns 1 if we are being foscued.
            Otherwise, it will return 0.

*/
/******************************************************************************/
AE_API s32 AESysIsFocus();

/******************************************************************************/
/*!
\fn         void AESysSetFullScreen(s32 fullScreen)

\brief      Sets fullscreen.

\param      [in] fullScreen
            If 0, sets the window to windowed mode.
            Otherwise, sets the window to full screen mode.
*/
/******************************************************************************/
AE_API void AESysSetFullScreen(s32 fullScreen);


/******************************************************************************/
/*!
\fn         HWND AESysGetWindowHandle()

\brief      Get the handle to the window.

\retval     HWND
            Returns the handle to the window.
*/
/******************************************************************************/
AE_API HWND AESysGetWindowHandle	();


/******************************************************************************/
/*!
\fn         void AESysSetWindowTitle (const char *pTitle)

\brief      Set the title of the window.

\param      [in] pTitle
            Pointer to a null-terminated string to set as the title
            of the window.

*/
/******************************************************************************/
AE_API void AESysSetWindowTitle		(const char* pTitle);

/******************************************************************************/
/*!
\fn         s32 AESysDoesWindowExist()

\brief      Check if the window has been closed.

\retval     s32
            Return 1 if the window exist.
            Else return 0.
*/
/******************************************************************************/
AE_API s32 AESysDoesWindowExist		();

// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------


#ifdef __cplusplus 
}
#endif

#endif // AE_SYSTEM_H

