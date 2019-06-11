/**
 * Novatek AF algorithm parameters.
 * 
 * Reference AF parameters.
 * 
 * @file af_param_evb.h
 * @note Nothing.
 *
 * Copyright (C) 2017 Novatek Microelectronics Corp. All rights reserved.
 *
 */

#ifndef _AF_PARAM_EVB_H_
#define _AF_PARAM_EVB_H_

#include "Type.h"


//=============================================================================
// constant define
//=============================================================================
#define AF_SEARCH_METHOD            0    // 0: hill climb / 1: hybrid (hill climb + 3rd order curve fitting) / 2: 2nd order curve fitting
#define AF_THRES_ROUGH_PERCENT      20
#define AF_THRES_FINE_PERCENT       80
#define AF_THRES_FINAL_PERCENT      95
#define AF_THRES_RESTART_PERCENT    25
#define AF_ROUGH_STEP_SIZE          6
#define AF_FINE_STEP_SIZE           3
#define AF_FINAL_STEP_SIZE          1
#define AF_MAX_SEARCH_COUNT         200


//=============================================================================
// algorithm parameters
//=============================================================================
#if (AF_WIN_ALG_NUM == (4*4))    // 4x4 windows partition
const UINT8 g_ucAfWinWeight4x4[AF_WIN_ALG_NUM] = { 1,1,1,1,
                                                   1,1,1,1,
                                                   1,1,1,1,
                                                   1,1,1,1 };
#elif (AF_WIN_ALG_NUM == (8*8))    // 8x8 windows partition
const UINT8 g_ucAfWinWeight8x8_4[AF_WIN_ALG_NUM]  = { 0,0,0,0,0,0,0,0,
                                                      0,0,0,0,0,0,0,0,
                                                      0,0,0,0,0,0,0,0,
                                                      0,0,0,1,1,0,0,0,
                                                      0,0,0,1,1,0,0,0,
                                                      0,0,0,0,0,0,0,0,
                                                      0,0,0,0,0,0,0,0,
                                                      0,0,0,0,0,0,0,0 };

const UINT8 g_ucAfWinWeight8x8_36[AF_WIN_ALG_NUM] = { 0,0,0,0,0,0,0,0,
                                                      0,1,1,1,1,1,1,0,
                                                      0,1,1,1,1,1,1,0,
                                                      0,1,1,1,1,1,1,0,
                                                      0,1,1,1,1,1,1,0,
                                                      0,1,1,1,1,1,1,0,
                                                      0,1,1,1,1,1,1,0,
                                                      0,0,0,0,0,0,0,0 };

const UINT8 g_ucAfWinWeight8x8_64[AF_WIN_ALG_NUM] = { 1,1,1,1,1,1,1,1,
                                                      1,1,1,1,1,1,1,1,
                                                      1,1,1,1,1,1,1,1,
                                                      1,1,1,1,1,1,1,1,
                                                      1,1,1,1,1,1,1,1,
                                                      1,1,1,1,1,1,1,1,
                                                      1,1,1,1,1,1,1,1,
                                                      1,1,1,1,1,1,1,1 };
#endif

const UINT16 g_usVaNoiseThreshold[][2] = {{  5,  7 },    // <=100
                                          {  6,  8 },    // <=200
                                          {  7,  9 },    // <=400
                                          {  9, 12 },    // <=800
                                          { 12, 16 },    // <=1600
                                          { 15, 19 },    // <=3200
                                          { 21, 26 },    // <=6400
                                          { 29, 35 },    // <=12800
                                          { 39, 46 },    // <=25600
                                          { 39, 46 },    // <=51200
                                          { 39, 46 },    // <=102400
                                          { 39, 46 }};   // >=102400


#endif    // _AF_PARAM_EVB_H_

