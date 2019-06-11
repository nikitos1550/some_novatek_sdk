#ifndef _WIFICMD_PARSER_H_
#define _WIFICMD_PARSER_H_

typedef enum _XML_WIFI_CMD_ID{
    XML_WIFI_CMD_1001      = 1001,
    XML_WIFI_CMD_1002      = 1002,
    XML_WIFI_CMD_1003      = 1003,
    XML_WIFI_CMD_2001      = 2001,
    XML_WIFI_CMD_2002      = 2002,
    XML_WIFI_CMD_2003      = 2003,
    XML_WIFI_CMD_2004      = 2004,
    XML_WIFI_CMD_2005      = 2005,
    XML_WIFI_CMD_2006      = 2006,
    XML_WIFI_CMD_2007      = 2007,
    XML_WIFI_CMD_2008      = 2008,
    XML_WIFI_CMD_2009      = 2009,
    XML_WIFI_CMD_2010      = 2010,
    XML_WIFI_CMD_2011      = 2011,
    XML_WIFI_CMD_2012      = 2012,
    XML_WIFI_CMD_2013      = 2013,
    XML_WIFI_CMD_2014      = 2014,
    XML_WIFI_CMD_2015      = 2015,
    XML_WIFI_CMD_2016      = 2016,
    XML_WIFI_CMD_2017      = 2017,
    XML_WIFI_CMD_2018      = 2018,
    XML_WIFI_CMD_2022      = 2022,
    XML_WIFI_CMD_2023      = 2023,
    XML_WIFI_CMD_2026      = 2026,
    XML_WIFI_CMD_2027      = 2027,
    XML_WIFI_CMD_2028      = 2028,
    XML_WIFI_CMD_2029      = 2029,
    XML_WIFI_CMD_2030      = 2030,
    XML_WIFI_CMD_2031      = 2031,
    XML_WIFI_CMD_2032      = 2032,
    XML_WIFI_CMD_2033      = 2033,
    XML_WIFI_CMD_2034      = 2034,
    XML_WIFI_CMD_2035      = 2035,
    XML_WIFI_CMD_3001      = 3001,
    XML_WIFI_CMD_3002      = 3002,
    XML_WIFI_CMD_3003      = 3003,
    XML_WIFI_CMD_3004      = 3004,
    XML_WIFI_CMD_3005      = 3005,
    XML_WIFI_CMD_3006      = 3006,
    XML_WIFI_CMD_3007      = 3007,
    XML_WIFI_CMD_3008      = 3008,
    XML_WIFI_CMD_3009      = 3009,
    XML_WIFI_CMD_3010      = 3010,
    XML_WIFI_CMD_3011      = 3011,
    XML_WIFI_CMD_3012      = 3012,
    XML_WIFI_CMD_3013      = 3013,
    XML_WIFI_CMD_3014      = 3014,
    XML_WIFI_CMD_3015      = 3015,
    XML_WIFI_CMD_3016      = 3016,
    XML_WIFI_CMD_3017      = 3017,
    XML_WIFI_CMD_3018      = 3018,///no response
    XML_WIFI_CMD_3019      = 3019,
    XML_WIFI_CMD_3020      = 3020,
    XML_WIFI_CMD_3021      = 3021,
    XML_WIFI_CMD_3022      = 3022,
    XML_WIFI_CMD_3023      = 3023,
    XML_WIFI_CMD_3024      = 3024,
    XML_WIFI_CMD_3025      = 3025,
    XML_WIFI_CMD_3025_DESC = 10003025,
    XML_WIFI_CMD_3026      = 3026,
    XML_WIFI_CMD_3027      = 3027,
    XML_WIFI_CMD_3028      = 3028,
    XML_WIFI_CMD_3030      = 3030,
    XML_WIFI_CMD_3032      = 3032,
    XML_WIFI_CMD_3033      = 3033,
    XML_WIFI_CMD_3034      = 3034,
    XML_WIFI_CMD_3035      = 3035,
    XML_WIFI_CMD_3036      = 3036,
    XML_WIFI_CMD_3037      = 3037,
    XML_WIFI_CMD_3038      = 3038,
    XML_WIFI_CMD_3039      = 3039,
    XML_WIFI_CMD_3040      = 3040,
    XML_WIFI_CMD_3041      = 3041,
    XML_WIFI_CMD_3042      = 3042,
    XML_WIFI_CMD_3043      = 3043,
    XML_WIFI_CMD_3044      = 3044,
    XML_WIFI_CMD_3045      = 3045,
    XML_WIFI_CMD_3046      = 3046,
    XML_WIFI_CMD_3047      = 3047,
    XML_WIFI_CMD_3048      = 3048,
    XML_WIFI_CMD_3049      = 3049,
    XML_WIFI_CMD_3050      = 3050,
    XML_WIFI_CMD_3051      = 3051,
    XML_WIFI_CMD_3052      = 3052,
    XML_WIFI_CMD_3053      = 3053,
    XML_WIFI_CMD_3054      = 3054,
    XML_WIFI_CMD_3055      = 3055,
    XML_WIFI_CMD_3056      = 3056,
    XML_WIFI_CMD_3057      = 3057,
    XML_WIFI_CMD_3058      = 3058,
    XML_WIFI_CMD_3059      = 3059,
    XML_WIFI_CMD_3060      = 3060,
    XML_WIFI_CMD_3061      = 3061,
    XML_WIFI_CMD_3062      = 3062,
    XML_WIFI_CMD_3063      = 3063,
    XML_WIFI_CMD_3064      = 3064,
    XML_WIFI_CMD_3065      = 3065,
    XML_WIFI_CMD_3066      = 3066,
    XML_WIFI_CMD_3067      = 3067,
    XML_WIFI_CMD_3072      = 3072,
    XML_WIFI_CMD_3073      = 3073,
    XML_WIFI_CMD_3074      = 3074,
    XML_WIFI_CMD_3075      = 3075,
    XML_WIFI_CMD_3068      = 3068,
    XML_WIFI_CMD_4001      = 4001,
    XML_WIFI_CMD_4002      = 4002,
    XML_WIFI_CMD_4003      = 4003,
    XML_WIFI_CMD_4004      = 4004,
    XML_WIFI_CMD_4005      = 4005,
    XML_WIFI_CMD_MAX       = 200    //max cmd number
}XML_WIFI_CMD_ID;

typedef struct _XML_WIFI_DATA_1001_FILE{
    char name[64];
    char fpath[128];
}XML_WIFI_DATA_1001_FILE;


struct _XML_WIFI_DATA_1001{
    short cmd;
    short status;
    XML_WIFI_DATA_1001_FILE file;
    int freepicnum;
};

struct _XML_WIFI_DATA_1002{
    short cmd;
    short status;
};

struct _XML_WIFI_DATA_1003{
    short cmd;
    short status;
    char value[32];
};

struct _XML_WIFI_DATA_3002{
    short total_count;
    short cmd[XML_WIFI_CMD_MAX];
};

struct _XML_WIFI_DATA_3012{
    short cmd;
    short status;
    char strg[128];
};

struct _XML_WIFI_DATA_3014{
    short total_count;
    short cmd[XML_WIFI_CMD_MAX];
    short status[XML_WIFI_CMD_MAX];
};

struct _XML_WIFI_DATA_3015{

    char name[64];
    char fpath[128];
    unsigned long long size;
    char timecode[32];
    char time[64];
    int attr;
};

struct _XML_WIFI_DATA_3025_FILEDESC{
    char filepath[128];
    char version[32];
    char checkMethod[32];
    char checkValue[32];
};


struct _XML_WIFI_DATA_3030{
    char name[64];
    int index;
    char size[32];
    int framerate;
    int type;
};



struct _XML_WIFI_DATA_3033{

    short cmd;
    short status;
	short  sd_ready;
	short  sd_writable;
	char  sd_total_space[32];
	char  sd_free_space[32];

};

struct _XML_WIFI_DATA_3038{

    short cmd;
    short status;
    char  model[64];
	char  vendor[64];
};

struct _XML_WIFI_DATA_3052{

    char  ssid[32];
	int   rssi;
    short auth_type;

};
/////cmd need usging CB function
typedef struct _XML_WIFI_DATA_LIST_REGISTER{
    void (* WifiCmdXML_data_CB)(short bEnd, void *output_data);
}XML_WIFI_LIST_REGISTER;

////////1001 structure type
typedef struct _XML_WIFI_DATA_1001 XML_WIFI_DATA_1001;
////////1002 structure type
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_1002;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_2001;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_2002;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_2003;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_2004;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_2005;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_2006;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_2007;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_2008;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_2010;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_2011;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_2012;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_2013;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_2014;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_2015;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_2017;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_2018;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_2022;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_2029;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_2031;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_2032;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_2033;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_2034;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3001;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3003;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3004;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3005;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3006;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3007;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3008;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3009;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3010;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3011;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3013;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3016;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3018;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3020;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3021;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3022;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3023;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3024;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3027;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3028;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3037;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3050;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3051;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3054;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3055;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3058;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3059;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3062;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3063;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3064;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3065;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3068;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_4001;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_4002;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_4003;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_4004;
///////1003 structure type
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_1003;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_2009;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_2016;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_2023;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_2026;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_2027;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_2028;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_2030;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_2035;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_3017;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_3019;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_3034;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_3035;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_3036;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_3039;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_3040;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_3041;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_3043;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_3044;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_3047;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_3048;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_3057;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_3060;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_3066;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_3067;
///////3002 structure type
typedef struct _XML_WIFI_DATA_3002 XML_WIFI_DATA_3002;
//////3012  structure type
typedef struct _XML_WIFI_DATA_3012 XML_WIFI_DATA_3012;
typedef struct _XML_WIFI_DATA_3012 XML_WIFI_DATA_3025;
typedef struct _XML_WIFI_DATA_3012 XML_WIFI_DATA_3026;
typedef struct _XML_WIFI_DATA_3012 XML_WIFI_DATA_3045;
typedef struct _XML_WIFI_DATA_3012 XML_WIFI_DATA_3046;
typedef struct _XML_WIFI_DATA_3012 XML_WIFI_DATA_3049;
typedef struct _XML_WIFI_DATA_3012 XML_WIFI_DATA_4005;
typedef struct _XML_WIFI_DATA_3012 XML_WIFI_DATA_3053;
typedef struct _XML_WIFI_DATA_3012 XML_WIFI_DATA_3056;
///////3014 structure type
typedef struct _XML_WIFI_DATA_3014 XML_WIFI_DATA_3014;
///////3015 structure type
typedef struct _XML_WIFI_DATA_3015 XML_WIFI_DATA_3015;
typedef struct _XML_WIFI_DATA_3015 XML_WIFI_DATA_3042;
typedef struct _XML_WIFI_DATA_3015 XML_WIFI_DATA_3061;
///////3025_filedesc structure type
typedef struct _XML_WIFI_DATA_3025_FILEDESC XML_WIFI_DATA_3025_FILEDESC;
//////3030 structure type
typedef struct _XML_WIFI_DATA_3030 XML_WIFI_DATA_3030;
//////3033 structure type
typedef struct _XML_WIFI_DATA_3033 XML_WIFI_DATA_3033;
//////3038 structure type
typedef struct _XML_WIFI_DATA_3038 XML_WIFI_DATA_3038;
//////3052 structure type
typedef struct _XML_WIFI_DATA_3052 XML_WIFI_DATA_3052;

typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3072;
typedef struct _XML_WIFI_DATA_1002 XML_WIFI_DATA_3073;
typedef struct _XML_WIFI_DATA_1003 XML_WIFI_DATA_3074;
typedef struct _XML_WIFI_DATA_3052 XML_WIFI_DATA_3075;

typedef union {
	XML_WIFI_DATA_1001 data_1001;
	XML_WIFI_DATA_1002 data_1002;
	XML_WIFI_DATA_1003 data_1003;
	XML_WIFI_DATA_2001 data_2001;
	XML_WIFI_DATA_2002 data_2002;
	XML_WIFI_DATA_2003 data_2003;
	XML_WIFI_DATA_2004 data_2004;
	XML_WIFI_DATA_2005 data_2005;
	XML_WIFI_DATA_2006 data_2006;
	XML_WIFI_DATA_2007 data_2007;
	XML_WIFI_DATA_2008 data_2008;
	XML_WIFI_DATA_2009 data_2009;
	XML_WIFI_DATA_2010 data_2010;
	XML_WIFI_DATA_2011 data_2011;
	XML_WIFI_DATA_2012 data_2012;
	XML_WIFI_DATA_2013 data_2013;
	XML_WIFI_DATA_2014 data_2014;
	XML_WIFI_DATA_2015 data_2015;
	XML_WIFI_DATA_2016 data_2016;
	XML_WIFI_DATA_2017 data_2017;
	XML_WIFI_DATA_2018 data_2018;
	XML_WIFI_DATA_2022 data_2022;
	XML_WIFI_DATA_2023 data_2023;
	XML_WIFI_DATA_2026 data_2026;
	XML_WIFI_DATA_2027 data_2027;
	XML_WIFI_DATA_2028 data_2028;
	XML_WIFI_DATA_2029 data_2029;
	XML_WIFI_DATA_2030 data_2030;
	XML_WIFI_DATA_2031 data_2031;
	XML_WIFI_DATA_2032 data_2032;
	XML_WIFI_DATA_2033 data_2033;
	XML_WIFI_DATA_2034 data_2034;
	XML_WIFI_DATA_2035 data_2035;
	XML_WIFI_DATA_3001 data_3001;
	XML_WIFI_DATA_3002 data_3002;
	XML_WIFI_DATA_3003 data_3003;
	XML_WIFI_DATA_3004 data_3004;
	XML_WIFI_DATA_3005 data_3005;
	XML_WIFI_DATA_3006 data_3006;
	XML_WIFI_DATA_3007 data_3007;
	XML_WIFI_DATA_3008 data_3008;
	XML_WIFI_DATA_3009 data_3009;
	XML_WIFI_DATA_3010 data_3010;
	XML_WIFI_DATA_3011 data_3011;
	XML_WIFI_DATA_3012 data_3012;
	XML_WIFI_DATA_3013 data_3013;
	XML_WIFI_DATA_3014 data_3014;
	XML_WIFI_DATA_3015 data_3015;
	XML_WIFI_DATA_3016 data_3016;
	XML_WIFI_DATA_3017 data_3017;
	XML_WIFI_DATA_3019 data_3019;
	XML_WIFI_DATA_3020 data_3020;
	XML_WIFI_DATA_3021 data_3021;
	XML_WIFI_DATA_3022 data_3022;
	XML_WIFI_DATA_3023 data_3023;
	XML_WIFI_DATA_3024 data_3024;
	XML_WIFI_DATA_3025 data_3025;
	XML_WIFI_DATA_3025_FILEDESC data_filedesc;
	XML_WIFI_DATA_3026 data_3026;
	XML_WIFI_DATA_3027 data_3027;
	XML_WIFI_DATA_3028 data_3028;
	XML_WIFI_DATA_3030 data_3030;
	XML_WIFI_DATA_3033 data_3033;
	XML_WIFI_DATA_3034 data_3034;
	XML_WIFI_DATA_3035 data_3035;
	XML_WIFI_DATA_3036 data_3036;
	XML_WIFI_DATA_3037 data_3037;
	XML_WIFI_DATA_3038 data_3038;
	XML_WIFI_DATA_3039 data_3039;
	XML_WIFI_DATA_3040 data_3040;
	XML_WIFI_DATA_3041 data_3041;
	XML_WIFI_DATA_3042 data_3042;
	XML_WIFI_DATA_3043 data_3043;
	XML_WIFI_DATA_3044 data_3044;
	XML_WIFI_DATA_3045 data_3045;
	XML_WIFI_DATA_3046 data_3046;
	XML_WIFI_DATA_3047 data_3047;
	XML_WIFI_DATA_3048 data_3048;
	XML_WIFI_DATA_3049 data_3049;
	XML_WIFI_DATA_3050 data_3050;
	XML_WIFI_DATA_3051 data_3051;
	//XML_WIFI_DATA_3052 data_3052; //comment out, get by callback
	XML_WIFI_DATA_3053 data_3053;
	XML_WIFI_DATA_3054 data_3054;
	XML_WIFI_DATA_3055 data_3055;
	XML_WIFI_DATA_3056 data_3056;
	XML_WIFI_DATA_3057 data_3057;
	XML_WIFI_DATA_3058 data_3058;
	XML_WIFI_DATA_3059 data_3059;
	XML_WIFI_DATA_3060 data_3060;
	XML_WIFI_DATA_3061 data_3061;
	XML_WIFI_DATA_3062 data_3062;
	XML_WIFI_DATA_3063 data_3063;
	XML_WIFI_DATA_3064 data_3064;
	XML_WIFI_DATA_3065 data_3065;
	XML_WIFI_DATA_3066 data_3066;
	XML_WIFI_DATA_3067 data_3067;
	XML_WIFI_DATA_3068 data_3068;
	XML_WIFI_DATA_3072 data_3072;
	XML_WIFI_DATA_3073 data_3073;
	XML_WIFI_DATA_3074 data_3074;
	XML_WIFI_DATA_3075 data_3075;
	XML_WIFI_DATA_4001 data_4001;
	XML_WIFI_DATA_4002 data_4002;
	XML_WIFI_DATA_4003 data_4003;
	XML_WIFI_DATA_4004 data_4004;
	XML_WIFI_DATA_4005 data_4005;
} XML_WIFI_DATA_UNION;

/********************************************
*******WifiCmdXML_parser function************

    this function use for parsing wifi xml buf,

    there are three input parameters:

    cmd_id  -->   this is cmd id   ex: XML_WIFI_CMD_3015, XML_WIFI_CMD_2004

    xml_buf -->   the input xml buf need to be parsed.
                  ex:"<?xml version=1.0 encoding=UTF-8?><Function>\r\n
                    <Cmd>4004</Cmd><Status>1</Status>\r\n</Function>\r\n"

    output  -->  the data is parsed, need trasfer to mapping structure


    the return value:

    if value = 0, parse OK
    others       , parse fail


<HOW TO USING>
    there are two  xml types
    type A:
        <function>
        .....
        </function>

    type B:
        <LIST>
        .......
        </LIST>


if your cmd  belongs to type A, please using following example
example:

main(){
    char xml_buf[]="<?xml version=1.0 encoding=UTF-8?><Function>\r\n<Cmd>2006</Cmd><Status>1</Status>\r\n</Function>\r\n";
    XML_WIFI_DATA_2006 output={0};
    int ret=0;
    ret = WifiCmdXML_parser(XML_WIFI_CMD_2006, xml_buf, (void *)&output);

    if(ret < 0){
        DBG_ERR("parsing error!\r\n");
    }
    else{
        DBG_DUMP("cmd=%d\r\n",output.cmd);
        DBG_DUMP("status=d\r\n",output.status);
    }

}


if your cmd belongs to type b,need using CB function to get data. Because of list type,
there are lots of sets of information, if get a set information, CB function will be called,
the CB function type as following:
    void (* WifiCmdXML_data_CB)(short bEnd, void *output_data);

there are two parameters as following:
bEnd        --> if be set 1, this is the last information set, otherwise set 0
output_data --> the data is parsed, need trasfer type


please using following example
example:

static void CB_function(short bEnd, void * data){

    XML_WIFI_DATA_3052 * pdata = (XML_WIFI_DATA_3052 *)data;
    DBG_DUMP("\r\n===========test_3015==========\r\n");


}

main(){


    char xml_buf[]="<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
                   "<LIST>"
                   "<AP_index>"
                   "<SSID>NVT_CARDV_xxxxxx</SSID>"
                   "<Auth_type>1</Auth_type>"
                   "<RSSI>-10</RSSI>"
                   "</AP_index>"
                   "<AP_index>"
                   "<SSID>NVT_CARDV_xxxxx1</SSID>"
                   "<Auth_type>2</Auth_type>"
                   "<RSSI>-30</RSSI>"
                   "</AP_index>"
                   "<AP_index>"
                   "<SSID>NVT_CARDV_xxxxx2</SSID>"
                   "<Auth_type>1</Auth_type>"
                   "<RSSI>-2</RSSI>"
                   "</AP_index>"
                   "</LIST>";


    XML_WIFI_LIST_REGISTER output={0};
    output.WifiCmdXML_data_CB = &test_function;
    int ret =0;
    ret = WifiCmdXML_parser(XML_WIFI_CMD_3052,xml_buf,&output);
    if(ret < 0){
        DBG_ERR("parsing error\r\n");
    }
}
***********************************
***********************************/
int wificmd_parser_xml(XML_WIFI_CMD_ID cmd_id, char *xml_buf, void *output);



#endif //_WIFICMD_PARSER_H_
