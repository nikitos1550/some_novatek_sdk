#include "Type.h"
#include "ae_alg.h"
#include "sensor.h"
#include "ae_api.h"
#include "ae_proc_nvt_int.h"
#include "pwm.h"

#define AE_ISO_BASE             100
#define AE_PRV_ISO_MIN          AE_ISO_BASE
#define AE_PRV_ISO_MAX          3200
#define AE_CAP_ISO_MIN          AE_ISO_BASE
#define AE_CAP_ISO_MAX          3200

static AE_EXPECT_LUM expect_lum = {
    480, //mov
    480, //photo
    {50, 50, 50, 50, 50, 50, 50, 50, 58, 82, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
    {33, 33, 33, 33, 33, 33, 50, 70, 80, 90, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
    {33, 33, 33, 33, 33, 33, 50, 70, 80, 90, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
};

static AE_LA_CLAMP la_clamp = {
    // For normal mode, LA clamp value
    {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    // For SHDR mode, ratio value, base on Target Y to calculate clamp boundary.
    {125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

static AE_HISTO histogram = {
    AE_FUNC_AUTO,
    486,
    {45, 40, 40, 40, 50, 50, 50, 50, 50, 70, 90, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
};

static AE_OVER_EXPOSURE over_exposure  = {
    ENABLE,
    869,
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
    {10, 10, 20, 30, 40, 50, 64, 80, 96, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
    {10, 10, 20, 30, 40, 50, 64, 80, 96, 114, 137, 147, 152, 156, 156, 156, 156, 156, 156, 156, 156 },
    {10, 10, 20, 30, 40, 50, 64, 80, 96, 114, 137, 147, 152, 156, 156, 156, 156, 156, 156, 156, 156 },
    {28, 30, 40, 40, 40, 40, 40, 40, 30, 30, 20, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18 },
    {8, 8, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 8, 8, 7, 3, 3, 3, 3, 3, 3 },
};

static AE_CONVERGENCE convergence  = {
    AEALG_SPEED_P1,
    {6, 6},
    1000,     // smooth ratio thr
    ENABLE, // De-Pulse enable
    150,     // De-Pulse Ratio
    2,       // De-Pulse Frame Number
    0,
    10, // Slow shoot cyc (Ex. 10 = 1s), auto reset counter when unstable ratio not over thr)
    40, // Slow shoot thr (Sum of AE unstable ratio during slow shoot cyc)
    0, // reserved
    0 // reserved
};

static AE_IRISINFO iris_info = {
    AE_IRIS_TYPE_FIXED, AE_IRIS_MODE_OPEN, 10, AEALG_IRIS_BIG, {AEALG_IRIS_BIG, AEALG_IRIS_SMALL},
};

static AE_PROC_BOUNDARY proc_boundary = {
    {AE_PRV_ISO_MAX, AE_PRV_ISO_MIN},
    {AE_CAP_ISO_MAX, AE_CAP_ISO_MIN},
};
static AE_CURVE_GEN_PARAM curve_gen_param  = {
    FNO_SEL_USER,
    100,
    FNO_F18,
    AE_FLICKER_50HZ,
    {
        {30, 100, 0, 0},
        {20000, 200, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    },
    2,
    AE_PRV_ISO_MAX, // IsoMax
    {64, 512},
    DISABLE,
    {{300, 300}, {240, 240}, {200, 200}, {150, 150}, {120, 120}},
    AE_ISOMODE_MOV_PARAM,
    AE_PRV_ISO_MAX,
    AE_FLICKER_ON
};

static UINT32 ae_meter_window[AE_WIN_NUM] = {
    0, 1, 0, 0, 0, 0, 1, 0, 
    1, 1, 0, 0, 0, 0, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 2, 3, 3, 2, 1, 1, 
    1, 1, 3, 5, 5, 3, 1, 1, 
    1, 2, 3, 5, 5, 3, 2, 1, 
    0, 1, 2, 2, 2, 2, 1, 0, 
    0, 0, 0, 0, 0, 0, 0, 0
};

static AE_LUM_GAMMA lum_gamma = {
    AE_LUMGAMMA_PREDEF,
    {
    0,27,52,77,101,124,146,168,189,209,
    228,247,266,283,301,318,334,350,365,380,
    395,409,423,436,449,462,474,487,498,510,
    521,532,543,554,564,574,584,593,603,612,
    621,630,638,647,655,663,671,679,687,694,
    701,709,716,723,729,736,743,749,755,762,
    768,774,780,785,791,797,802,808,813,818,
    823,828,833,838,843,848,853,857,862,866,
    871,875,879,883,888,892,896,900,904,907,
    911,915,919,922,926,930,933,937,940,943,
    947,950,953,956,960,963,966,969,972,975,
    978,981,983,986,989,992,995,997,1000,1003,
    1005,1008,1010,1013,1015,1018,1020,1022,1023
    }
};

static AE_SHDR shdr = {
    486,
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
    AE_SHDR_EV_OPT_FIXED,
    1024,
    80,
    {2, 1},
    {4095, 960},
    {1024, 256},
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
    100,
    0
};

static AE_IRIS_CFG dciris_cfg = {
    DISABLE,
    0,
    0,
    110,
    0,
    {3, 1, 1},
    0,
    200, 500, 1000
};

static AE_CURVE_GEN_PHOTO curve_gen_photo  = {
    {
        {30, 100, 0, 0},
        {33333, 800, 0, 0},
        {66666, 1600, 0, 0},
        {100000, 3200, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    },
    4,
    66666,
    AE_CAP_ISO_MAX,
    AE_ISOMODE_PHOTO_SPEED,
    AE_CAP_ISO_MAX,
};

AE_PARAM_INFO ae_param_GC2053_evb = {
    AE_PRV_ISO_MIN,
    1000,
    &expect_lum,
    &la_clamp,
    &histogram,
    &over_exposure,
    &convergence,
    &iris_info,
    &proc_boundary,
    &curve_gen_param,
    &ae_meter_window[0],
    &lum_gamma,
    &shdr,
    &dciris_cfg,
    &curve_gen_photo
};

void ae_get_param_GC2053_evb(UINT32* param);
void ae_get_param_GC2053_evb(UINT32* param)
{
   *param =  (UINT32)&ae_param_GC2053_evb;
}

