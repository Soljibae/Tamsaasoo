/******************************************************************************/
/*!
\par        Project: Alpha Engine
\file       AEUtil.h

\author     Sun Tjen Fam
\date       February 02, 2008

\brief      Header file for the utility library.

\copyright  Copyright (C) 2013 DigiPen Institute of Technology. Reproduction 
            or disclosure of this file or its contents without the prior 
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#ifndef AE_UTIL_H
#define AE_UTIL_H

// ---------------------------------------------------------------------------
#ifdef __cplusplus 
extern "C"
{
#endif

/******************************************************************************/
/*!
\fn         f64 AEGetTime(f64* pTime)

\brief      Get the current time in seconds.

\param      [out] pTime
            Pointer to f64 to store the current time.
            May be left null if not needed.

\retval     f64
            Returns the current time in seconds.
*/
/******************************************************************************/
AE_API f64		AEGetTime(f64* pTime);

/******************************************************************************/
/*!
\fn         f32 AERandFloat()

\brief      Get a random real number between 0.0f to 1.0f.

\retval     f32
            Returns a random real number between 0.0f to 1.0f.
*/
/******************************************************************************/
AE_API f32		AERandFloat();


/******************************************************************************/
/*!
\fn         s32 AEIsF32Zero(f32 x)

\brief      Checks if x is either 0 or so close to 0 that it can be considered 0. 

\param      [in] x
            f32 value to check.

\retval     s32
            Returns 1 if x is or close to 0.
            Otherwise, it will return 0.
*/
/******************************************************************************/
AE_API s32       AEIsF32Zero(f32 x);

/******************************************************************************/
/*!
\fn         s32 AEIsF32Equal(f32 a, f32 b)

\brief      Checks whether a is equal to OR so close to b that they can
            be considered equal.

\param      [in] a
            f32 value to check

\param      [in] b
            f32 value to check

\retval     s32
            Returns 1 if a is or close to b.
            Otherwise, it will return 0.
*/
/******************************************************************************/
AE_API s32       AEIsF32Equal(f32 a, f32 b);

/******************************************************************************/
/*!
\fn         s32 AEIsF64NearZero(f64 x)

\brief      Checks if x is either 0 or so close to 0 that it can be considered 0.

\param      [in] x
            f64 value to check.

\retval     s32
            Returns 1 if x is or close to 0.
            Otherwise, it will return 0.
*/
/******************************************************************************/
AE_API s32       AEIsF64NearZero(f64 x);

/******************************************************************************/
/*!
\fn         s32 AEIsF64Equal(f64 a, f64 b)

\brief      Checks whether a is equal to OR so close to b that they can
            be considered equal.

\param      [in] a
            f64 value to check

\param      [in] b
            f64 value to check

\retval     s32
            Returns 1 if a is or close to b.
            Otherwise, it will return 0.
*/
/******************************************************************************/
AE_API s32       AEIsF64Equal(f64 a, f64 b);



#ifdef __cplusplus 
}
#endif


// ---------------------------------------------------------------------------

#endif // AE_UTIL_H


