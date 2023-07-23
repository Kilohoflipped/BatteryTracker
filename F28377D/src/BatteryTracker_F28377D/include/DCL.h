/* DCL.h - C2000 Digital Controller Library header file
 *
 * Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com/ 
 * ALL RIGHTS RESERVED 
 *
*/

#ifndef _C_DCL_H
#define _C_DCL_H

//! \file   		DCL.h
//! \brief  		Contains the public interface to the
//!         		Digital Controller Library functions
//!

#include <stdint.h>

//! \brief 			Function name compatibility with version 1
//!
#define	DCL_runPID		DCL_runPID_C1
#define	DCL_runPIDc		DCL_runPID_L1
#define DCL_runPI		DCL_runPI_C1
#define DCL_runPIc		DCL_runPI_L1
#define	DCL_runDF13		DCL_runDF13_C1
#define	DCL_runDF13i	DCL_runDF13_C2
#define	DCL_runDF13p	DCL_runDF13_C3
#define	DCL_runDF13c	DCL_runDF13_L1
#define	DCL_runDF13ic	DCL_runDF13_L2
#define	DCL_runDF13pc	DCL_runDF13_L3
#define	DCL_runDF22		DCL_runDF22_C1
#define	DCL_runDF22i	DCL_runDF22_C2
#define	DCL_runDF22p	DCL_runDF22_C3
#define	DCL_runDF22c	DCL_runDF22_L1
#define	DCL_runDF22ic	DCL_runDF22_L2
#define	DCL_runDF22pc	DCL_runDF22_L3
#define	DCL_runDF23		DCL_runDF23_C1
#define	DCL_runDF23i	DCL_runDF23_C2
#define	DCL_runDF23p	DCL_runDF23_C3
#define	DCL_runDF23c	DCL_runDF23_L1
#define	DCL_runDF23ic	DCL_runDF23_L2
#define	DCL_runDF23pc	DCL_runDF23_L3


//--- Linear PID controller ---------------------------------------------------

//! \brief 			Defines the linear PID controller structure
//!
typedef volatile struct {
	float Kp;		//!< Proportional gain
	float Ki;		//!< Integral gain
	float Kd;		//!< Derivative gain
	float Kr;		//!< Set point weight
	float c1;		//!< D-term filter coefficient 1
	float c2;		//!< D-term filter coefficient 2
	float d2;		//!< D-term filter intermediate storage 1
	float d3;		//!< D-term filter intermediate storage 2
	float i10;		//!< I-term intermediate storage
	float i14;		//!< Intermediate saturation storage
	float Umax;		//!< Upper saturation limit
	float Umin;		//!< Lower saturation limit
} PID;

//! \brief 			Defines default values to initialise the PID structure
//!
#define	PID_DEFAULTS {  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, \
						0.0f, 0.0f, 1.0f, -1.0f }

//! \brief     		Executes an ideal form PID controller on the FPU32
//! \param[in] p	Pointer to the PID structure
//! \param[in] rk	The controller set-point reference
//! \param[in] yk	The measured feedback value
//! \param[in] lk	External output clamp flag
//! \return    		The control effort
//!
extern float DCL_runPID_C1(PID *p, float rk, float yk, float lk);

//! \brief     		Executes an inline ideal form PID controller on the FPU32
//! \param[in] p	Pointer to the PID structure
//! \param[in] rk	The controller set-point reference
//! \param[in] yk	The measured feedback value
//! \param[in] lk	External output clamp flag
//! \return    		The control effort
//!
static inline float DCL_runPID_C2(PID *p, float rk, float yk, float lk)
{
	float v1, v4, v5, v8, v9, v10, v12;

	v5 = (p->Kr * rk) - yk;
	v8 = ((rk - yk) * p->Ki * p->Kp * p->i14) + p->i10;
	p->i10 = v8;
	v1 = yk * p->Kd * p->c1;
	v4 = v1 - p->d2 - p->d3;
	p->d2 = v1;
	p->d3 = v4 * p->c2;
	v9 = ((v5 - v4) * p->Kp) + v8;
	v10 = (v9 > p->Umax) ? p->Umax : v9;
	v10 = (v10 < p->Umin) ? p->Umin : v10;
	v12 = (v10 == v9) ? 1.0f : 0.0f;
	p->i14 = v12 * lk;

	return v10;
}

//! \brief     		Executes an parallel form PID controller on the FPU32
//! \param[in] p	Pointer to the PID structure (note: Kr unused in this implementation)
//! \param[in] rk	The controller set-point reference
//! \param[in] yk	The measured feedback value
//! \param[in] lk	External output clamp flag
//! \return    		The control effort
//!
static inline float DCL_runPID_C3(PID *p, float rk, float yk, float lk)
{
	float v1, v4, v5, v6, v8, v9, v10, v12;

	v5 = rk - yk;
	v6 = v5 * p->Kp;
	v8 = v5 * p->Ki * p->i14 + p->i10;
	p->i10 = v8;
	v1 = v5 * p->Kd * p->c1;
	v4 = v1 - p->d2 - p->d3;
	p->d2 = v1;
	p->d3 = v4 * p->c2;
	v9 = v6 + v8 + v4;
	v10 = (v9 > p->Umax) ? p->Umax : v9;
	v10 = (v10 < p->Umin) ? p->Umin : v10;
	v12 = (v10 == v9) ? 1.0f : 0.0f;
	p->i14 = v12 * lk;

	return v10;
}

//! \brief     		Executes a parallel form PID controller on the FPU32
//! \param[in] p	Pointer to the PID structure (note: Kr unused in this implementation)
//! \param[in] rk	The controller set-point reference
//! \param[in] yk	The measured feedback value
//! \param[in] lk	External output clamp flag
//! \return    		The control effort
//!
extern float DCL_runPID_C4(PID *p, float rk, float yk, float lk);

//! \brief     		Executes an ideal form PID controller on the CLA
//! \param[in] p	Pointer to the PID structure
//! \param[in] rk	The controller set-point reference
//! \param[in] yk	The measured feedback value
//! \param[in] lk	External output clamp flag
//! \return    		The control effort
//!
extern float DCL_runPID_L1(PID *p, float rk, float yk, float lk);

//! \brief     		Executes a parallel form PID controller on the CLA
//! \param[in] p	Pointer to the PID structure
//! \param[in] rk	The controller set-point reference
//! \param[in] yk	The measured feedback value
//! \param[in] lk	External output clamp flag
//! \return    		The control effort
//!
extern float DCL_runPID_L2(PID *p, float rk, float yk, float lk);


//--- Linear PI controller ----------------------------------------------------

//! \brief 			Defines the PI controller structure
//!
typedef volatile struct {
	float Kp;		//!< Proportional gain
 	float Ki;		//!< Integral gain
 	float i10;		//!< I storage
	float Umax;		//!< Upper saturation limit
	float Umin;		//!< Lower saturation limit
	float i6;		//!< Saturation storage
} PI;

//! \brief 	Defines default values to initialise the PI structure
//!
#define	PI_DEFAULTS { 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f }

//! \brief     		Executes a series form PI controller on the FPU32
//! \param[in] p	Pointer to the PI structure
//! \param[in] rk	The controller set-point reference
//! \param[in] yk	The measured feedback value
//! \return    		The control effort
//!
extern float DCL_runPI_C1(PI *p, float rk, float yk);

//! \brief     		Executes an inline series form PI controller on the FPU32
//! \param[in] p	Pointer to the PI structure
//! \param[in] rk	The controller set-point reference
//! \param[in] yk	The measured feedback value
//! \return    		The control effort
//!
static inline float DCL_runPI_C2(PI *p, float rk, float yk)
{
	float v2, v4, v5, v9;

	v2 = p->Kp * (rk - yk);
	v4 = p->i10 + (p->Ki * p->i6 * v2);
	v5 = v2 + v4;
	v9 = (v5 > p->Umax) ? p->Umax : v5;
	v9 = (v9 < p->Umin) ? p->Umin : v9;
	p->i10 = v4;
	p->i6 = (v5 == v9) ? 1.0f : 0.0f;

	return v9;
}

//! \brief     		Executes an parallel form PI controller on the FPU32
//!					Implemented as inline C function
//! \param[in] p	Pointer to the PI structure
//! \param[in] rk	The controller set-point reference
//! \param[in] yk	The measured feedback value
//! \return    		The control effort
//!
static inline float DCL_runPI_C3(PI *p, float rk, float yk)
{
	float v1, v2, v4, v5, v9;

	v1 = rk - yk;
	v2 = p->Kp * v1;
	v4 = (v1 * p->Ki * p->i6) + p->i10;
	p->i10 = v4;
	v5 = v2 + v4;
	v9 = (v5 > p->Umax) ? p->Umax : v5;
	v9 = (v9 < p->Umin) ? p->Umin : v9;
	p->i6 = (v5 == v9) ? 1.0f : 0.0f;

	return v9;
}

//! \brief     		Executes a parallel form PI controller on the FPU32
//! \param[in] p	Pointer to the PI structure
//! \param[in] rk	The controller set-point reference
//! \param[in] yk	The measured feedback value
//! \return    		The control effort
//!
extern float DCL_runPI_C4(PI *p, float rk, float yk);

//! \brief     		Executes a series form PI controller on the CLA
//! \param[in] p	Pointer to the PI structure
//! \param[in] rk	The controller set-point reference
//! \param[in] yk	The measured feedback value
//! \return    		The control effort
//!
extern float DCL_runPI_L1(PI *p, float rk, float yk);

//! \brief     		Executes a parallel form PI controller on the CLA
//! \param[in] p	Pointer to the PI structure
//! \param[in] rk	The controller set-point reference
//! \param[in] yk	The measured feedback value
//! \return    		The control effort
//!
extern float DCL_runPI_L2(PI *p, float rk, float yk);


//--- Direct Form 1 - 3rd order -----------------------------------------------

//! \brief 			Defines the DF13 controller structure
//!
typedef volatile struct {
	// coefficients
	float b0;	//!< b0
	float b1;	//!< b1
	float b2;	//!< b2
	float b3;	//!< b3
	float a0;	//!< a0
	float a1;	//!< a1
	float a2;	//!< a2
	float a3;	//!< a3

	//data
	float d0;	//!< e(k)
	float d1;	//!< e(k-1)
	float d2;	//!< e(k-2)
	float d3;	//!< e(k-3)
	float d4;	//!< u(k)
	float d5;	//!< u(k-1)
	float d6;	//!< u(k-2)
	float d7;	//!< u(k-3)
} DF13;

//! \brief 			Defines default values to initialise the DF13 structure
//!
#define	DF13_DEFAULTS {	0.25f, 0.25f, 0.25f, 0.25f, 0.0f, 0.0f, 0.0f, 0.0f, \
						0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }

//! \brief     		Executes a full 3rd order Direct Form 1 controller on the FPU32
//! \param[in] p	Pointer to the DF13 controller structure
//! \param[in] ek	The servo error
//! \return    		The control effort
//!
extern float DCL_runDF13_C1(DF13 *p, float ek);

//! \brief     		Executes an immediate 3rd order Direct Form 1 controller on the FPU32
//! \param[in] p	Pointer to the DF13 controller structure
//! \param[in] ek	The servo error
//! \param[in] vk	The partial pre-computed control effort
//! \return    		The control effort
//!
extern float DCL_runDF13_C2(DF13 *p, float ek, float vk);

//! \brief     		Executes a partial pre-computed 3rd order Direct Form 1 controller on the FPU32
//! \param[in] p	Pointer to the DF13 controller structure
//! \param[in] ek	The servo error
//! \param[in] uk	The controller output in the previous sample interval
//! \return    		The control effort
//!
extern float DCL_runDF13_C3(DF13 *p, float ek, float uk);

//! \brief     		Executes a full 3rd order Direct Form 1 controller on the FPU32
//!					Implemented as inline C function
//!					Note: d0 not used
//! \param[in] p	Pointer to the DF13 controller structure
//! \param[in] ek	The servo error
//! \return    		The control effort
//!
static inline float DCL_runDF13_C4(DF13 *p, float ek)
{
	p->d4 = (ek * p->b0) + (p->d1 * p->b1) + (p->d2 * p->b2) + (p->d3 * p->b3) - (p->d5 * p->a1) - (p->d6 * p->a2) - (p->d7 * p->a3);
	p->d3 = p->d2;
	p->d2 = p->d1;
	p->d1 = ek;
	p->d7 = p->d6;
	p->d6 = p->d5;
	p->d5 = p->d4;

	return p->d4;
}

//! \brief     		Executes an immediate 3rd order Direct Form 1 controller on the FPU32
//!					Implemented as inline C function
//! \param[in] p	Pointer to the DF13 controller structure
//! \param[in] ek	The servo error
//! \param[in] vk	The partial pre-computed control effort
//! \return    		The control effort
//!
static inline float DCL_runDF13_C5(DF13 *p, float ek, float vk)
{
	p->d4 = (ek * p->b0) + vk;

	return p->d4;
}

//! \brief     		Executes a partial pre-computed 3rd order Direct Form 1 controller on the FPU32
//!					Implemented as inline C function
//!					Note: d0 not used
//! \param[in] p	Pointer to the DF13 controller structure
//! \param[in] ek	The servo error
//! \param[in] uk	The controller output in the previous sample interval
//! \return    		The control effort
//!
static inline float DCL_runDF13_C6(DF13 *p, float ek, float uk)
{
	float v9;

	v9 = (ek * p->b1) + (p->d1 * p->b2) + (p->d2 * p->b3) - (uk * p->a1) - (p->d5 * p->a2) - (p->d6 * p->a3);
	p->d2 = p->d1;
	p->d1 = ek;
	p->d6 = p->d5;
	p->d5 = uk;

	return v9;
}

//! \brief     		Executes a full 3rd order Direct Form 1 controller on the CLA
//! \param[in] p	Pointer to the DF13 controller structure
//! \param[in] ek	The servo error
//! \return    		The control effort
//!
extern float DCL_runDF13_L1(DF13 *p, float ek);

//! \brief     		Executes an immediate 3rd order Direct Form 1 controller on the CLA
//! \param[in] p	Pointer to the DF13 controller structure
//! \param[in] ek	The servo error
//! \param[in] vk	The partial pre-computed control effort
//! \return    		The control effort
//!
extern float DCL_runDF13_L2(DF13 *p, float ek, float vk);

//! \brief     		Executes a partial pre-computed 3rd order Direct Form 1 controller on the CLA
//! \param[in] p	Pointer to the DF13 controller structure
//! \param[in] ek	The servo error
//! \param[in] uk	The controller output in the previous sample interval
//! \return    		The control effort
//!
extern float DCL_runDF13_L3(DF13 *p, float ek, float uk);


//--- Direct Form 2 - 2nd order -----------------------------------------------

//! \brief 			Defines the DF22 controller structure
//!
typedef volatile struct {
	float b0;	//!< b0
	float b1;	//!< b1
	float b2;	//!< b2
	float a1;	//!< a1
	float a2;	//!< a2
	float x1;	//!< x1
	float x2;	//!< x2
} DF22;

//! \brief 			Defines default values to initialise the DF22 structure
//!
#define	DF22_DEFAULTS { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }

//! \brief     		Executes a full 2nd order Direct Form 2 controller on the FPU32
//! \param[in] p	Pointer to the DF22 controller structure
//! \param[in] ek	The servo error
//! \return    		The control effort
//!
extern float DCL_runDF22_C1(DF22 *p, float ek);

//! \brief     		Executes an immediate 2nd order Direct Form 2 controller on the FPU32
//! \param[in] p	Pointer to the DF22 controller structure
//! \param[in] ek	The servo error
//! \return    		The control effort
//!
extern float DCL_runDF22_C2(DF22 *p, float ek);

//! \brief     		Executes a partial pre-computed 2nd order Direct Form 2 controller on the FPU32
//! \param[in] p	Pointer to the DF22 controller structure
//! \param[in] ek	The servo error
//! \param[in] uk	The controller output in the previous sample interval
//!
extern void DCL_runDF22_C3(DF22 *p, float ek, float uk);

//! \brief     		Executes a full 2nd order Direct Form 2 controller on the FPU32
//!					Implemented as inline C function
//! \param[in] p	Pointer to the DF22 controller structure
//! \param[in] ek	The servo error
//! \return    		The control effort
//!
static inline float DCL_runDF22_C4(DF22 *p, float ek)
{
	float v7;

	v7 = (ek * p->b0) + p->x1;
	p->x1 = (ek * p->b1) + p->x2 - (v7 * p->a1);
	p->x2 = (ek * p->b2) - (v7 * p->a2);

	return v7;
}

//! \brief     		Executes an immediate 2nd order Direct Form 2 controller on the FPU32
//!					Implemented as inline C function
//! \param[in] p	Pointer to the DF22 controller structure
//! \param[in] ek	The servo error
//! \return    		The control effort
//!
static inline float DCL_runDF22_C5(DF22 *p, float ek)
{
	return ((ek * p->b0) + p->x1);
}

//! \brief     		Executes a partial pre-computed 2nd order Direct Form 2 controller on the FPU32
//!					Implemented as inline C function
//! \param[in] p	Pointer to the DF22 controller structure
//! \param[in] ek	The servo error
//! \param[in] uk	The controller output in the previous sample interval
//!
static inline void DCL_runDF22_C6(DF22 *p, float ek, float uk)
{
	p->x1 = (ek * p->b1) + p->x2 - (uk * p->a1);
	p->x2 = (ek * p->b2) - (uk * p->a2);
}

//! \brief     		Executes a full 2nd order Direct Form 2 controller on the CLA
//! \param[in] p	Pointer to the DF22 controller structure
//! \param[in] ek	The servo error
//! \return    		The control effort
//!
extern float DCL_runDF22_L1(DF22 *p, float ek);

//! \brief     		Executes an immediate 2nd order Direct Form 2 controller on the CLA
//! \param[in] p	Pointer to the DF22 controller structure
//! \param[in] ek	The servo error
//! \return    		The control effort
//!
extern float DCL_runDF22_L2(DF22 *p, float ek);

//! \brief     		Executes a partial pre-computed 2nd order Direct Form 2 controller on the CLA
//! \param[in] p	Pointer to the DF22 controller structure
//! \param[in] ek	The servo error
//! \param[in] uk	The controller output in the previous sample interval
//!
extern void DCL_runDF22_L3(DF22 *p, float ek, float uk);


//--- Direct Form 2 - 3rd order -----------------------------------------------

//! \brief 			Defines the DF23 controller structure
//!
typedef volatile struct {
	float b0;	//!< b0
	float b1;	//!< b1
	float b2;	//!< b2
	float b3;	//!< b3
	float a1;	//!< a1
	float a2;	//!< a2
	float a3;	//!< a3
	float x1;	//!< x1
	float x2;	//!< x2
	float x3;	//!< x3
} DF23;

//! \brief 			Defines default values to initialise the DF23 structure
//!
#define	DF23_DEFAULTS { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }

//! \brief     		Executes a full 3rd order Direct Form 2 controller on the FPU32
//! \param[in] p	Pointer to the DF23 controller structure
//! \param[in] ek	The servo error
//! \return    		The control effort
//!
extern float DCL_runDF23_C1(DF23 *p, float ek);

//! \brief     		Executes an immediate 3rd order Direct Form 2 controller on the FPU32
//! \param[in] p	Pointer to the DF23 controller structure
//! \param[in] ek	The servo error
//! \return    		The control effort
//!
extern float DCL_runDF23_C2(DF23 *p, float ek);

//! \brief     		Executes a partial pre-computed 3rd order Direct Form 2 controller on the FPU32
//! \param[in] p	Pointer to the DF23 controller structure
//! \param[in] ek	The servo error
//! \param[in] uk	The controller output in the previous sample interval
//!
extern void DCL_runDF23_C3(DF23 *p, float ek, float uk);

//! \brief     		Executes a full 3rd order Direct Form 2 controller on the FPU32
//!					Implemented as inline C function
//! \param[in] p	Pointer to the DF23 controller structure
//! \param[in] ek	The servo error
//! \return    		The control effort
//!
static inline float DCL_runDF23_C4(DF23 *p, float ek)
{
	float v7;

	v7 = (ek * p->b0) + p->x1;
	p->x1 = (ek * p->b1) + p->x2 - (v7 * p->a1);
	p->x2 = (ek * p->b2) + p->x3 - (v7 * p->a2);
	p->x3 = (ek * p->b3) - (v7 * p->a3);

	return v7;
}

//! \brief     		Executes an immediate 3rd order Direct Form 2 controller on the FPU32
//!					Implemented as inline C function
//! \param[in] p	Pointer to the DF23 controller structure
//! \param[in] ek	The servo error
//! \return    		The control effort
//!
static inline float DCL_runDF23_C5(DF23 *p, float ek)
{
	return ((ek * p->b0) + p->x1);
}

//! \brief     		Executes a partial pre-computed 3rd order Direct Form 2 controller on the FPU32
//!					Implemented as inline C function
//! \param[in] p	Pointer to the DF23 controller structure
//! \param[in] ek	The servo error
//! \param[in] uk	The controller output in the previous sample interval
//!
static inline void DCL_runDF23_C6(DF23 *p, float ek, float uk)
{
	p->x1 = (ek * p->b1) + p->x2 - (uk * p->a1);
	p->x2 = (ek * p->b2) + p->x3 - (uk * p->a2);
	p->x3 = (ek * p->b3) - (uk * p->a3);
}

//! \brief     		Executes a full 3rd order Direct Form 2 controller on the CLA
//! \param[in] p	Pointer to the DF23 controller structure
//! \param[in] ek	The servo error
//! \return    		The control effort
//!
extern float DCL_runDF23_L1(DF23 *p, float ek);

//! \brief     		Executes an immediate 3rd order Direct Form 2 controller on the CLA
//! \param[in] p	Pointer to the DF23 controller structure
//! \param[in] ek	The servo error
//! \return    		The control effort
//!
extern float DCL_runDF23_L2(DF23 *p, float ek);

//! \brief     		Executes a partial pre-computed 2nd order Direct Form 2 controller on the CLA
//! \param[in] p	Pointer to the DF23 controller structure
//! \param[in] ek	The servo error
//! \param[in] uk	The controller output in the previous sample interval
//!
extern void DCL_runDF23_L3(DF23 *p, float ek, float uk);


//--- Direct Form 2 - clamp ---------------------------------------------------

//! \brief 			Saturates a control variable and returns 1 if either limit is exceeded
//!
//! \details		Can be used to saturate a pre-computed Direct Form 2 controller.
//!					If the immediate result is in range it can be used, otherwise
//!					it can be clamped and the next partial pre-computation skipped.
//!					An example of use with a pre-computed DF22 controller follows:
//!
//! \code
//! uk = DCL_runDF22_C2(&arma2, rk);				// immediate result from pre-computed controller
//! i = DCL_runClamp_C1(&uk, 1.0f, -1.0f);			// clamp immediate result to +/-1.0
//! // ...use uk here...
//! if (0 == i)										// if immediate result is in range...
//! {
//!		DCL_runDF22_C3(&arma2, rk, uk);				// ...pre-compute the next partial result
//! }
//! \endcode
//!
//! \param[in] data	The address of the data variable
//! \param[in] Umax	The upper limit
//! \param[in] Umin	The lower limit
//! \return			Returns 0 if (Umin < data < Umax), else 1
//!
extern uint16_t DCL_runClamp_C1(float *data, float Umax, float Umin);

//! \brief 			Saturates a control variable and returns 1 if either limit is exceeded
//! \param[in] data	The address of the data variable
//! \param[in] Umax	The upper limit
//! \param[in] Umin	The lower limit
//! \return			Returns 0 if (Umin < data < Umax), else 1
//!
static inline uint16_t DCL_runClamp_C2(float *data, float Umax, float Umin)
{
	float iv = *(data);

	*(data) = (*(data) > Umax) ? Umax : *(data);
	*(data) = (*(data) < Umin) ? Umin : *(data);

	return ((iv < Umax) && (iv > Umin)) ? 0 : 1;
}

//! \brief 			Saturates a control variable and returns 1.0f if either limit is exceeded
//!
//! \details		Can be used to saturate a pre-computed Direct Form 2 controller.
//!					If the immediate result is in range it can be used, otherwise
//!					it can be clamped and the next partial pre-computation skipped.
//!					An example of use with a pre-computed DF22 controller follows:
//!
//! \code
//! uk = DCL_runDF22_L2(&arma2, rk);				// immediate result from pre-computed controller
//! f = DCL_runClamp_L1(&uk, 1.0f, -1.0f);			// clamp immediate result to +/-1.0
//! // ...use uk here...
//! if (0.5f > f)									// if immediate result is in range...
//! {
//!		DCL_runDF22_L3(&arma2, rk, uk);				// ...pre-compute the next partial result
//! }
//! \endcode
//!
//! \param[in] data	The address of the data variable
//! \param[in] Umax	The upper limit
//! \param[in] Umin	The lower limit
//! \return			Returns 0.0f if (Umin < data < Umax), else 1.0f
//!
extern float DCL_runClamp_L1(float *data, float Umax, float Umin);

#endif // _C_DCL_H

/* end of file */
