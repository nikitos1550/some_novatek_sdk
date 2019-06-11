#ifndef _LENSINFO_H_
#define _LENSINFO_H_

extern UINT32  g_u32lensWriteCount;
//Lens Info
extern void     LensTask2CB(UINT32 MessageID, UINT32 *Parameter);
//#NT#2010/09/09#Photon Lin -end
//#NT#2010/10/27#Photon Lin -begin
//#fix typing error
extern void     Set_LensFlag(UINT32 LensFlag);
extern UINT32   Get_LensFlag(void);
//#NT#2010/10/27#Photon Lin -begin
extern UINT32   Load_LensInfo(void);
extern void     Save_LensInfo(UINT32 lensInfo);
extern BOOL     Lens_IsWrite(void);

#endif //_LENSINFO_H_
