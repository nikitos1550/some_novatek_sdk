////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "PStore.h"
//#include "NVTUserCommand.h"
#include "HwClock.h"
////////////////////////////////////////////////////////////////////////////////
#define LENSINFO_TRACEMSG     0
#define LENSINFO_WARNINGMSG   0
#define LENSINFO_ERRORMSG     0

//Trace Message
#if LENSINFO_TRACEMSG
#define LENSINFO_TRACE(...) debug_msg ("^RLensInfo:>"__VA_ARGS__)
#else
#define LENSINFO_TRACE(...)
#endif

#if LENSINFO_WARNINGMSG
#define LENSINFO_WARNING(...) debug_msg("^GLensInfo:>"__VA_ARGS__)
#else
#define LENSINFO_WARNING(...)
#endif

//Error Message
#if LENSINFO_ERRORMSG
#define LENSINFO_ERROR(...) debug_msg("^BLensInfo:>"__VA_ARGS__)
#else
#define LENSINFO_ERROR(...)
#endif

#define PS_LENS_FLAG DISABLE

#define PS_LENS_DATA            "LEND"
#define PS_STAT_DATA            "STAT"

#define LENSFLAG_DIRTY 0x5555AAAA
#define LENSFLAG_OK    0xAAAA5555

#define LENS_NOTOPEN   0xFF00
#define LENS_SAVEOK    0xFF01
#define LENS_ERROR     0xFF02


typedef struct _PS_SYS_RECORD
{
    UINT32 Flag;
    struct tm Curr_DateTime;
}
PS_SYS_RECORD;

void Save_LensInfo(UINT32 lensInfo)
{
    //TODO: Implemented by User
}

void LensTask2CB(UINT32 MessageID, UINT32 *Parameter)
{
    //TODO: Implemented by User
    switch(MessageID)
    {
        default:
            LENSINFO_ERROR("Parameter error in LensTask2CB() (%d)\r\n",MessageID);
        break;

    }
}

