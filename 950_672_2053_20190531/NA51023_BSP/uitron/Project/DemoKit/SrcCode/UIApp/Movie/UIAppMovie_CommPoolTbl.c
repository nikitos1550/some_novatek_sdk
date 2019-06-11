#include "SysCfg.h" //for memory POOL_ID
#include "nvtmpp.h"

#define TEST_510_64MB       DISABLE


#if (SENSOR_CAPS_COUNT == 1 && _BOARD_DRAM_SIZE_ > 0x04000000)
NVT_VB_CPOOL_S g_movie_common_pool_tbl[NVTMPP_VB_MAX_COMM_POOLS+1]= {
	{ 1920 * 1080 * 3 / 2 + 1024, 5, NVTMPP_DDR_1 }, // record
	{  720 *  576 * 3 / 2 + 1024, 4, NVTMPP_DDR_1 }, // display
	{  848 *  480 * 3 / 2 + 1024, 2, NVTMPP_DDR_1 }, // 2 wifi
	#if _TSR_FUNC_|| MOVIE_OT_FUNC
	{ 1920 * 1080 * 3 / 2 + 1024, 3, NVTMPP_DDR_1 }, // 2 ALG
	#else
	{  848 *  480 * 3 / 2 + 1024, 3, NVTMPP_DDR_1 }, // 2 ALG
	#endif
	#if (_ADAS_FUNC_ == ENABLE)
	{  (720 *  480 + 1024) *2,    4, NVTMPP_DDR_1 }, // ADAS
	#endif
	// last item should be 0
	{0, 0, NVTMPP_DDR_1}
};
#endif

#if (SENSOR_CAPS_COUNT == 2 && _BOARD_DRAM_SIZE_ > 0x04000000)
#if (_BOARD_DRAM_SIZE_ > 0x08000000)
NVT_VB_CPOOL_S g_movie_common_pool_tbl[NVTMPP_VB_MAX_COMM_POOLS+1]= {
	{ 2560 * 1440 * 3 / 2 + 1024, 4, NVTMPP_DDR_1 }, // record
	{ 1920 * 1080 * 3 / 2 + 1024, 4, NVTMPP_DDR_1 }, // record
	#if defined(_Disp_IFPA_LCD1_T397P04_)
	{  848 *  480 * 3 / 2 + 1024, 6, NVTMPP_DDR_1 }, // display
	#else
	{  720 *  576 * 3 / 2 + 1024, 4, NVTMPP_DDR_1 }, // display
	#endif
	{  848 *  480 * 3 / 2 + 1024, 4, NVTMPP_DDR_1 }, // 2 wifi
	// last item should be 0
	{0, 0, NVTMPP_DDR_1}
};
#else
NVT_VB_CPOOL_S g_movie_common_pool_tbl[NVTMPP_VB_MAX_COMM_POOLS+1]= {
	{ 2304 * 1296 * 3 / 2 + 1024, 4, NVTMPP_DDR_1 }, // record
	{ 1920 * 1080 * 3 / 2 + 1024, 4, NVTMPP_DDR_1 }, // record
	{  720 *  576 * 3 / 2 + 1024, 4, NVTMPP_DDR_1 }, // display
	{  848 *  480 * 3 / 2 + 1024, 4, NVTMPP_DDR_1 }, // 2 wifi
	// last item should be 0
	{0, 0, NVTMPP_DDR_1}
};
#endif
#endif

#if (TEST_510_64MB == ENABLE)
NVT_VB_CPOOL_S g_movie_common_pool_tbl[NVTMPP_VB_MAX_COMM_POOLS+1]= {
	{ 1920 * 1080 * 3 / 2 + 1024, 4, NVTMPP_DDR_1 },
	{  720 *  576 * 3 / 2 + 1024, 4, NVTMPP_DDR_1 },
	{  640 *  480 * 3 / 2 + 1024, 3, NVTMPP_DDR_1 },
	// last item should be 0
	{0, 0, NVTMPP_DDR_1}
};
#endif

#if (_BOARD_DRAM_SIZE_ == 0x04000000)
NVT_VB_CPOOL_S g_movie_common_pool_tbl[NVTMPP_VB_MAX_COMM_POOLS+1]= {
	{ ((1920* 3 /4) * 1080 * 3 / 2) + 1024, 4, NVTMPP_DDR_1 }, // record
	//{  960 *  180 * 3 / 2 + 1024, 6, NVTMPP_DDR_1 }, // display
  {  640 *  480 * 3 / 2 + 1024, 4, NVTMPP_DDR_1 }, // display//LIU 20190318 672 display buffer
	{  848 *  480 * 3 / 2 + 1024, 5, NVTMPP_DDR_1 }, // wifi
	// last item should be 0
	{0, 0, NVTMPP_DDR_1}
};
#endif





