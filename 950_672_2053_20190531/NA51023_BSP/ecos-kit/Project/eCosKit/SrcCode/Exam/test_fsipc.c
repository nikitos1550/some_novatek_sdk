#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <cyg/fileio/fileio.h>
#include <cyg/infra/diag.h>
#include <cyg/infra/mainfunc.h>

#if 1 //debug
#define DBG_IND(fmtstr, args...) diag_printf("TEST_FSIPC: %s() " fmtstr, __func__, ##args)
#else //release
#define DBG_IND(...)
#endif
#define DBG_ERR(fmtstr, args...) diag_printf("TEST_FSIPC: ERR %s() " fmtstr, __func__, ##args)
#define DBG_DUMP(fmtstr, args...) diag_printf("%s" fmtstr, __func__, ##args)

#define FST_ATTRIB_READONLY 0x01                     ///< bit 0, indicates the file is read only.
#define FST_ATTRIB_HIDDEN 0x02

#define JD_1_JAN_1970 2440588 // 1 Jan 1970 in Julian day number

static void jdays_to_gdate(cyg_uint32 jd, int *day, int *month, int *year)
{
    cyg_uint32 l, n, i, j;

    l = jd + 68569;
    n = (4 * l) / 146097;
    l = l - (146097 * n + 3) / 4;
    i = (4000 * (l + 1)) / 1461001;
    l = l - (1461 * i) / 4 + 31;
    j = (80 * l) / 2447;
    *day = l - (2447 * j) / 80;

    l = j / 11;
    *month = j + 2 - (12 * l);
    *year = 100 * (n - 49) + i + l;
}

static void date_unix2dos(cyg_uint32  unix_timestamp, cyg_uint16 *dos_time, cyg_uint16 *dos_date)
{
    cyg_uint32 jd;
    cyg_uint16 dtime, ddate;
    int        hour, min, sec;
    int        day, month, year;

    hour = (unix_timestamp / 3600) % 24;
    min  = (unix_timestamp / 60) % 60;
    sec  =  unix_timestamp % 60;

    jd = JD_1_JAN_1970 + unix_timestamp / (3600 * 24);
    jdays_to_gdate(jd, &day, &month, &year);

    DBG_IND("timestamp=%d date=%d:%d:%d %d-%d-%d\r\n", unix_timestamp, hour, min, sec, year, month, day);

    if (year < 1980)
        year = 1980;

    dtime = (hour << 11) | (min << 5) | (sec >> 1);
    ddate = ((year - 1980) << 9) | (month << 5) | day;

    DBG_IND("dos time=%d date=%d\r\n", dtime, ddate);

    if (NULL != dos_time) *dos_time = dtime;
    if (NULL != dos_date) *dos_date = ddate;
}



static void fsipc_listdir( char *name, int statp)
{
    int err;
    DIR *dirp;
    int num=0;

    DBG_DUMP("<INFO>: reading directory %s\r\n",name);

    dirp = opendir( name );
    if( dirp == NULL ) DBG_ERR("dirp == NULL\r\n");

    for(;;)
    {
        struct dirent *entry = readdir( dirp );

        if( entry == NULL )
            break;
        num++;
        DBG_DUMP("<INFO>: entry %14s\r\n",entry->d_name);
        if( statp )
        {
            char fullname[PATH_MAX] = {0};
            struct stat sbuf;

            if( name[0] )
            {
                strcpy(fullname, name );
                if( !(name[0] == '/' && name[1] == 0 ) )
                    strcat(fullname, "/" );
            }
            else fullname[0] = 0;

            strcat(fullname, entry->d_name );

            err = stat( fullname, &sbuf );
            if( err < 0 )
            {
                if( errno == ENOSYS )
                    DBG_DUMP(" <no status available>");
                else DBG_ERR("stat return err = %d\r\n", err);
            }
            else
            {
                DBG_DUMP("[mode %08x ino %08x nlink %d size %d]",
                            sbuf.st_mode,sbuf.st_ino,sbuf.st_nlink,sbuf.st_size);
            }
        }

        DBG_DUMP("\r\n");
    }

    err = closedir( dirp );
    if( err < 0 ) DBG_ERR("closedir failed\r\n");
}

MAINFUNC_ENTRY(fsipc, argc, argv)
{
    int fd;
    int err;
    char szPath[PATH_MAX] = "/sdcard/test.txt";
    char szReadBuf[PATH_MAX] = {0};

    DBG_IND("szPath addr = 0x%X\r\n", szPath);

    fd = open(szPath, O_WRONLY|O_CREAT);
    if( fd < 0 ) {
        DBG_ERR("open %s failed\r\n", szPath);
        return -1;
    }

    err = write(fd, szPath, strlen(szPath));
    if( err < 0 ) {
        DBG_ERR("write failed\r\n");
        return -1;
    }

    err = close(fd);
    if( err < 0 ) {
        DBG_ERR("close %s failed\r\n", szPath);
        return -1;
    }

    fd = open(szPath, O_RDONLY);
    if( fd < 0 ) {
        DBG_ERR("open %s failed\r\n", szPath);
        return -1;
    }

    memset(szReadBuf, 0, sizeof(szReadBuf));
    err = read(fd, szReadBuf, strlen(szPath));
    if( err < 0 ) {
        DBG_ERR("read failed\r\n");
        return -1;
    }

    DBG_IND("read string = %s\r\n", szReadBuf);

    DBG_IND("seek to SEEK_SET, offset 3 sizeof(szReadBuf) = %d\r\n", sizeof(szReadBuf));
    lseek(fd, 3, SEEK_SET);

    memset(szReadBuf, 0, sizeof(szReadBuf));
    err = read(fd, szReadBuf, strlen(szPath));
    if( err < 0 ) {
        DBG_ERR("read failed\r\n");
        return -1;
    }

    DBG_IND("read string = %s (offset 3 from head)\r\n", szReadBuf);

    {
        struct stat fstatus;
        unsigned short tmptime,tmpdate;

        DBG_IND("fstat %s\r\n", szPath);
        fstat(fd, &fstatus);

        DBG_IND("st_mode  = 0x%x\r\n", fstatus.st_mode);
        DBG_IND("st_size  = %lld\r\n", fstatus.st_size);
        DBG_IND("st_atime = %d\r\n", fstatus.st_atime);
        DBG_IND("st_mtime = %d\r\n", fstatus.st_mtime);
        DBG_IND("st_ctime = %d\r\n", fstatus.st_ctime);

        date_unix2dos(fstatus.st_atime, &tmptime, &tmpdate);
        printf("accessed time = %d, date = %d\n", tmptime, tmpdate);

        date_unix2dos(fstatus.st_mtime, &tmptime, &tmpdate);
        printf("modified time = %d, date = %d\n", tmptime, tmpdate);

        date_unix2dos(fstatus.st_ctime, &tmptime, &tmpdate);
        printf("created time = %d, date = %d\n", tmptime, tmpdate);
    }

    DBG_IND("Test fsync %s\r\n", szPath);
    err = fsync(fd);
    if( err < 0 ) {
        DBG_ERR("fsync %s failed\r\n", szPath);
        return -1;
    }

    err = close(fd);
    if( err < 0 ) {
        DBG_ERR("close %s failed\r\n", szPath);
        return -1;
    }

    {
        struct stat fstatus;
        cyg_uint16 tmptime,tmpdate;

        DBG_IND("stat %s\r\n", szPath);
        stat(szPath, &fstatus);

        DBG_IND("st_mode  = 0x%x\r\n", fstatus.st_mode);
        DBG_IND("st_size  = %lld\r\n", fstatus.st_size);
        DBG_IND("st_atime = %d\r\n", fstatus.st_atime);
        DBG_IND("st_mtime = %d\r\n", fstatus.st_mtime);
        DBG_IND("st_ctime = %d\r\n", fstatus.st_ctime);

        date_unix2dos(fstatus.st_atime, &tmptime, &tmpdate);
        printf("accessed time = %d, date = %d\n", tmptime, tmpdate);

        date_unix2dos(fstatus.st_mtime, &tmptime, &tmpdate);
        printf("modified time = %d, date = %d\n", tmptime, tmpdate);

        date_unix2dos(fstatus.st_ctime, &tmptime, &tmpdate);
        printf("created time = %d, date = %d\n", tmptime, tmpdate);
    }

    {//test attribute set and get
        cyg_fs_attrib_t attrib_tmp, attrib_org;

        //get the original attrib
        err = cyg_fs_get_attrib( szPath, &attrib_org);
        if( err < 0 ) {
            DBG_ERR("cyg_fs_get_attrib %s failed\r\n", szPath);
            return -1;
        }
        DBG_IND("Got attrib_org of %s = 0x%X\r\n", szPath, attrib_org);

        //set to the new attrib
        err = cyg_fs_set_attrib( szPath, FST_ATTRIB_HIDDEN|FST_ATTRIB_READONLY);
        if( err < 0 ) {
            DBG_ERR("cyg_fs_set_attrib %s failed\r\n", szPath);
            return -1;
        }

        err = cyg_fs_get_attrib( szPath, &attrib_tmp);
        if( err < 0 ) {
            DBG_ERR("cyg_fs_get_attrib %s failed\r\n", szPath);
            return -1;
        }
        DBG_IND("Got attrib_tmp of %s = 0x%X\r\n", szPath, attrib_tmp);

        //set back to the original attrib
        err = cyg_fs_set_attrib( szPath, attrib_org);
        if( err < 0 ) {
            DBG_ERR("cyg_fs_set_attrib %s failed\r\n", szPath);
            return -1;
        }

        err = cyg_fs_get_attrib( szPath, &attrib_tmp);
        if( err < 0 ) {
            DBG_ERR("cyg_fs_get_attrib %s failed\r\n", szPath);
            return -1;
        }
        DBG_IND("Got attrib_tmp of %s = 0x%X\r\n", szPath, attrib_tmp);
    }

    DBG_IND("Test rename %s to /sdcard/fullpath.txt\r\n", szPath);
    err = rename(szPath, "/sdcard/fullpath.txt");
    if( err < 0 ) {
        DBG_ERR("rename %s failed\r\n", szPath);
        return -1;
    }
    strcpy(szPath, "/sdcard/fullpath.txt");

    DBG_IND("Test rename %s to delete.txt\r\n", szPath);
    err = rename(szPath, "delete.txt");
    if( err < 0 ) {
        DBG_ERR("rename %s failed\r\n", szPath);
        return -1;
    }
    strcpy(szPath, "/sdcard/delete.txt");

    DBG_IND("Test unlink %s\r\n", szPath);
    err = unlink(szPath);
    if( err < 0 ) {
        DBG_ERR("unlink %s failed\r\n", szPath);
        return -1;
    }

    strcpy(szPath, "/sdcard/Mydir1/Mydir2");
    DBG_IND("Test mkdir %s\r\n", szPath);
    err = mkdir( szPath, 0);
    if( err < 0 ) {
        DBG_ERR("mkdir %s failed\r\n", szPath);
        return -1;
    }

    strcpy(szPath, "/sdcard/Mydir1");
    err = chdir(szPath);
    if( err < 0 ) {
        DBG_ERR("chdir %s failed\r\n", szPath);
        return -1;
    }

    {
        char cwdbuf[PATH_MAX] = {0};
        char *ret;

        ret = getcwd( cwdbuf, sizeof(cwdbuf));
        if( NULL == ret) {
            DBG_ERR("getcwd failed\r\n");
            return -1;
        }
        DBG_IND("cwd = %s\r\n", cwdbuf);
    }

    DBG_IND("Test rmdir %s\r\n", szPath);
    err = rmdir(szPath);
    if( err < 0 ) {
        DBG_ERR("rmdir %s failed\r\n", szPath);
        return -1;
    }

    fsipc_listdir("/sdcard", true);

    return 0;
}
