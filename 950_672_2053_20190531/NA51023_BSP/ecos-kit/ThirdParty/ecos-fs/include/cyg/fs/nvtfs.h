#ifndef CYGONCE_CYG_FS_NVT_H
#define CYGONCE_CYG_FS_NVT_H
//=============================================================================
//
//      nvtfs.h
//
//      Novatek FS attributes and stat information
//
//=============================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 2004 Red Hat, Inc.
// Copyright (C) 2005 eCosCentric Limited
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
//
// Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
// at http://sources.redhat.com/ecos/ecos-license/
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//=============================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):     David Brennam <eCos@brennanhome.com>
// Contributors:
// Date:          2004-10-22
// Purpose:
// Description:   This header contains attributes and stat like mode
//                information for FAT filesystems.
//
// Usage:
//              #include <cyg/fs/nvtfs.h>
//
//
//####DESCRIPTIONEND####
//
//=============================================================================
//define FSIPC_SIM for simulation
//#define FSIPC_SIM

//-------------------------------------------------------------------------
//FileSysECOS API return value
#define FSECOS_RET_OK           0
#define FSECOS_RET_OPENED       1
#define FSECOS_RET_ERR          (-1)

//FileSys callback API return value
#define FSIPC_CBRET_OK          0
#define FSIPC_CBRET_ERR         (-1)

#define FSIPC_TOKEN_PATH        "fsipc"
#define FSIPC_INTERFACE_VER     0x16061618

//from eCos unistd.h
#ifdef __ECOS
#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2

#define __stat_mode_DIR    (1<<0)
#define __stat_mode_REG    (1<<3)

#define S_IRUSR  (1<<16)
#define S_IWUSR  (1<<17)
#define S_IXUSR  (1<<18)
#define S_IRWXU  (S_IRUSR|S_IWUSR|S_IXUSR)

#define S_IRGRP  (1<<19)
#define S_IWGRP  (1<<20)
#define S_IXGRP  (1<<21)
#define S_IRWXG  (S_IRGRP|S_IWGRP|S_IXGRP)

#define S_IROTH  (1<<22)
#define S_IWOTH  (1<<23)
#define S_IXOTH  (1<<24)
#define S_IRWXO  (S_IROTH|S_IWOTH|S_IXOTH)

struct fsipc_stat {
    unsigned int    st_mode;     ///< File mode
    unsigned int    st_ino;      ///< File serial number
    short           st_dev;      ///< ID of device containing file
    unsigned short  st_nlink;    ///< Number of hard links
    unsigned int    st_uid;      ///< User ID of the file owner
    unsigned int    st_gid;      ///< Group ID of the file's group
    short           st_rdev;     ///< Device ID(if the file is a character or block special device)
    long long       st_size;     ///< File size (regular files only)
    int             st_atime;    ///< Last access time
    int             st_mtime;    ///< Last data modification time
    int             st_ctime;    ///< Last file status change time
    unsigned int    st_blksize;  ///< A file system-specific preferred I/O block size for this object. On some file system types, this may vary from file to file
    long            st_blocks;   ///< Number of blocks allocated for this file
};
#else //#ifdef __ECOS
#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2

#define __stat_mode_DIR    (1<<0)
#define __stat_mode_REG    (1<<3)

#define S_IRUSR  (1<<16)
#define S_IWUSR  (1<<17)
#define S_IXUSR  (1<<18)
#define S_IRWXU  (S_IRUSR|S_IWUSR|S_IXUSR)

#define S_IRGRP  (1<<19)
#define S_IWGRP  (1<<20)
#define S_IXGRP  (1<<21)
#define S_IRWXG  (S_IRGRP|S_IWGRP|S_IXGRP)

#define S_IROTH  (1<<22)
#define S_IWOTH  (1<<23)
#define S_IXOTH  (1<<24)
#define S_IRWXO  (S_IROTH|S_IWOTH|S_IXOTH)

struct fsipc_stat {
    unsigned int    st_mode;     ///< File mode
    unsigned int    st_ino;      ///< File serial number
    short           st_dev;      ///< ID of device containing file
    unsigned short  st_nlink;    ///< Number of hard links
    unsigned int    st_uid;      ///< User ID of the file owner
    unsigned int    st_gid;      ///< Group ID of the file's group
    short           st_rdev;     ///< Device ID(if the file is a character or block special device)
    long long       st_size;     ///< File size (regular files only)
    int             st_atime;    ///< Last access time
    int             st_mtime;    ///< Last data modification time
    int             st_ctime;    ///< Last file status change time
    unsigned int    st_blksize;  ///< A file system-specific preferred I/O block size for this object. On some file system types, this may vary from file to file
    long            st_blocks;   ///< Number of blocks allocated for this file
};
#endif //#ifdef __ECOS
//-------------------------------------------------------------------------
//IPC command id and parameters
typedef enum{
    FSIPC_CMDID_GET_VER_INFO  =   0x00000000,
    FSIPC_CMDID_GET_BUILD_DATE,
    FSIPC_CMDID_OPEN,
    FSIPC_CMDID_CLOSE,
    FSIPC_CMDID_READ,
    FSIPC_CMDID_WRITE,
    FSIPC_CMDID_LSEEK,
    FSIPC_CMDID_FSTAT,
    FSIPC_CMDID_FSYNC,
    FSIPC_CMDID_UNLINK,
    FSIPC_CMDID_RENAME,
    FSIPC_CMDID_MOVE,
    FSIPC_CMDID_MKDIR,
    FSIPC_CMDID_RMDIR,
    FSIPC_CMDID_STAT,
    FSIPC_CMDID_GET_ATTRIB,
    FSIPC_CMDID_SET_ATTRIB,
    FSIPC_CMDID_GETCWD,
    FSIPC_CMDID_CHDIR,
    FSIPC_CMDID_OPENDIR,
    FSIPC_CMDID_READDIR,
    FSIPC_CMDID_CLOSEDIR,
    FSIPC_CMDID_REWINDDIR,
    FSIPC_CMDID_SYSREQ_ACK,
}FSIPC_CMDID;

typedef struct{
    FSIPC_CMDID CmdId;
    int Arg;
}FSIPC_MSG_FSAPI;

struct FSIPC_STRUCT_API_OPEN{
    const char *shaddr_pPath;
    int flag;
    int bIsAbsPath;
};

struct FSIPC_STRUCT_API_RW{
    int fd;
    void *shaddr_pBuf;
    long len;
};

struct FSIPC_STRUCT_API_LSEEK{
    int fd;
    long long offset64;
    int whence;
};

struct FSIPC_STRUCT_API_FSTAT{
    int fd;
    struct fsipc_stat *shaddr_pStat;
};

struct FSIPC_STRUCT_API_RENAME{
    const char *shaddr_pNewName;
    const char *shaddr_pPath;
    int bIsAbsPath;
};

struct FSIPC_STRUCT_API_MOVE{
    const char *shaddr_pPath_new;
    int bIsAbsPath_new;
    const char *shaddr_pPath_org;
    int bIsAbsPath_org;
};

struct FSIPC_STRUCT_API_STAT{
    const char *shaddr_pPath;
    struct fsipc_stat *shaddr_pStat;
    int bIsAbsPath;
};

struct FSIPC_STRUCT_API_GET_ATTRIB{
    const char *shaddr_pPath;
    unsigned int *shaddr_pAttrib;
    int bIsAbsPath;
};

struct FSIPC_STRUCT_API_SET_ATTRIB{
    const char *shaddr_pPath;
    unsigned int uiAttrib;
    int bIsAbsPath;
};

struct FSIPC_STRUCT_API_GETCWD{
    const char *shaddr_pPath;
    long MaxSize;
};

struct FSIPC_STRUCT_API_READDIR{
    int fd;
    char *shaddr_pName;
    long MaxSize;
};

struct FSIPC_STRUCT_PATH{
    const char *shaddr_pPath;
    int bIsAbsPath;
};

struct FSIPC_STRUCT_FD{
    int fd;
};

typedef struct FSIPC_STRUCT_API_OPEN        FSIPC_PARAM_OPEN;
typedef struct FSIPC_STRUCT_FD              FSIPC_PARAM_CLOSE;
typedef struct FSIPC_STRUCT_API_RW          FSIPC_PARAM_READ;
typedef struct FSIPC_STRUCT_API_RW          FSIPC_PARAM_WRITE;
typedef struct FSIPC_STRUCT_API_LSEEK       FSIPC_PARAM_LSEEK;
typedef struct FSIPC_STRUCT_API_FSTAT       FSIPC_PARAM_FSTAT;
typedef struct FSIPC_STRUCT_FD              FSIPC_PARAM_FSYNC;
typedef struct FSIPC_STRUCT_PATH            FSIPC_PARAM_UNLINK;
typedef struct FSIPC_STRUCT_API_RENAME      FSIPC_PARAM_RENAME;
typedef struct FSIPC_STRUCT_API_MOVE        FSIPC_PARAM_MOVE;
typedef struct FSIPC_STRUCT_PATH            FSIPC_PARAM_MKDIR;
typedef struct FSIPC_STRUCT_PATH            FSIPC_PARAM_RMDIR;
typedef struct FSIPC_STRUCT_API_STAT        FSIPC_PARAM_STAT;
typedef struct FSIPC_STRUCT_API_GET_ATTRIB  FSIPC_PARAM_GET_ATTRIB;
typedef struct FSIPC_STRUCT_API_SET_ATTRIB  FSIPC_PARAM_SET_ATTRIB;
typedef struct FSIPC_STRUCT_API_GETCWD      FSIPC_PARAM_GETCWD;
typedef struct FSIPC_STRUCT_PATH            FSIPC_PARAM_CHDIR;
typedef struct FSIPC_STRUCT_PATH            FSIPC_PARAM_OPENDIR;
typedef struct FSIPC_STRUCT_API_READDIR     FSIPC_PARAM_READDIR;
typedef struct FSIPC_STRUCT_FD              FSIPC_PARAM_REWINDDIR;
typedef struct FSIPC_STRUCT_FD              FSIPC_PARAM_CLOSEDIR;

//-------------------------------------------------------------------------
//FileSysECOS definition
typedef struct{
    char*   (*pf_FileSys_GetVerInfo)(void);
    char*   (*pf_FileSys_GetBuildDate)(void);

    int     (*pf_FileSys_OpenFile)(const char *path, int oflag, int bIsAbsPath);//open (with path)
    int     (*pf_FileSys_CloseFile)(int fd );//close
    int     (*pf_FileSys_ReadFile)(int fd, void *buf, long len );//read
    int     (*pf_FileSys_WriteFile)(int fd, const void *buf, long len );//write
    int     (*pf_FileSys_SeekFile)(int fd, long long pos, int whence );//lseek
    int     (*pf_FileSys_StatFile)(int fd, struct fsipc_stat *pStat );//fstat
    int     (*pf_FileSys_FlushFile)(int fd );//fsync
    int     (*pf_FileSys_DeleteFile)(const char *path, int bIsAbsPath);//unlink (with path)
    int     (*pf_FileSys_RenameFile)(const char *pNewName, const char *path, int bIsAbsPath);//rename (with path)
    int     (*pf_FileSys_MoveFile)(const char *path_new, int bIsAbsPath_new, const char *path_org, int bIsAbsPath_org);//move (with path)

    int     (*pf_FileSys_MakeDir)(const char *path, int bIsAbsPath );//mkdir (with path)
    int     (*pf_FileSys_DeleteDir)(const char *path, int bIsAbsPath );//rmdir (with path)

    int     (*pf_FileSys_Stat)(const char *path, struct fsipc_stat *pStat, int bIsAbsPath);//stat (with path)
    int     (*pf_FileSys_GetAttrib)(const char *path, unsigned int *pAttrib, int bIsAbsPath);//cyg_fs_get_attrib (with path)
    int     (*pf_FileSys_SetAttrib)(const char *path, unsigned int uiAttrib, int bIsAbsPath);//cyg_fs_set_attrib (with path)

    int     (*pf_fs_getcwd)(char *path, long MaxSize);//getcwd
    int     (*pf_fs_chdir)(const char *path, int bIsAbsPath );//chdir (with path)

    int     (*pf_fs_SearchFileOpen)(const char *path, int bIsAbsPath);//opendir (with path)
    int     (*pf_fs_SearchFile)(int fd, char *pName, long MaxSize);//readdir
    int     (*pf_fs_SearchFileRewind)(int fd );//rewinddir
    int     (*pf_fs_SearchFileClose)(int fd );//closedir
}FSECOS_OUTSRC_CB;

typedef struct _FSECOS_OPEN_OBJ{
    FSECOS_OUTSRC_CB*   pOutSrcCB;
    unsigned int        SharedParamAddr;
    unsigned int        InterfaceVer;
}FSECOS_OPEN_OBJ;

/**
    Register Novatek File System callback functions to eCos file system "nvtfs"

    @return void
*/
#ifndef __externC
#ifdef __cplusplus
# define __externC extern "C"
#else
# define __externC extern
#endif
#endif

__externC void FileSysECOS_CmdLine(char *szCmd);

#endif // CYGONCE_CYG_FS_NVT_H
// End of nvtfs.h
