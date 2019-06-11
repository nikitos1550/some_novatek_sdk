/**
    Live555 inquiry functions.

    Sample module detailed description.
    @file nvtlive555.c
    @ingroup LIVE555
    @note Nothing (or anything need to be mentioned).
    Copyright Novatek Microelectronics Corp. 2014. All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
*/
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "nvtlive555.h"

#if defined(_WIN32) || defined(__UBUNTU)
int UCTRL_OPEN(void) { return 1; }
int UCTRL_CLOSE(int hdl) { return 0; }
int UCTRL_CMD(int hdl, char *cmd, char *retAddr, int retSize) { return 0; }
#elif defined(_NETWORK_ON_CPU1_)
int UCTRL_OPEN(void) { return 1; }
int UCTRL_CLOSE(int hdl) { return 0; }
int UCTRL_CMD(int hdl, char *cmd, char *retAddr, int retSize) { return 0; }
#elif defined(__LINUX680)
#include <nvtuctrl.h>
#define UCTRL_OPEN NvtUctrl_Open
#define UCTRL_CLOSE NvtUctrl_Close
#define UCTRL_CMD NvtUctrl_CmdSnd
#elif defined(__LINUX510)
#include <nvtuctrl.h>
#define UCTRL_OPEN nvtuctrl_open
#define UCTRL_CLOSE nvtuctrl_close
#define UCTRL_CMD nvtuctrl_send_cmd
#else
#error "unknown platform"
#endif

#define CFG_DROP_THRESHOLD_MS (-500)

static int m_uctrl_hdl = 0;

static int uctrl_cmd(char *fmtstr, ...)
{
#if defined(__LINUX)	
	int er;
	static char snd_buf[64];
	static char rcv_buf[16];
	va_list marker;
	va_start(marker, fmtstr);
	vsnprintf(snd_buf, sizeof(snd_buf)-1, fmtstr, marker);
	va_end(marker);

	if((er = UCTRL_CMD(m_uctrl_hdl, snd_buf, rcv_buf, sizeof(rcv_buf))) != 0) {
		printf("nvtrtspd: UCTRL_CMD failed. er=%d", er);
		return er;
	}
	er = atoi(rcv_buf);
	return er;
#else
	return 0;
#endif	
}

static int on_get_cfg(NVTLIVE555_CFG *p_cfg)
{
	p_cfg->support_rtcp = 1;
	p_cfg->support_meta = 0;
#if defined(__ECOS)
	p_cfg->max_clients = 1; // because ecos's heap is small.
#endif
#if !defined(_WIN32) 
	p_cfg->tos = 0xC0;
#endif
	return 0;
}

#define CFG_TAG_STREAM_ID "stream"
#define CFG_TAG_SVC_ID "/svc"
static int on_parse_url(const char *url, NVTLIVE555_URL_INFO *p_info)
{
	//an example for url 'stream0/svc0/'
	const char *p_stream_id = strstr(url, CFG_TAG_STREAM_ID);
	const char *p_svc_level = strstr(url, CFG_TAG_SVC_ID);

	p_info->total_ms = 0;

	if (p_stream_id == NULL) {
		//printf("url content: %s. set to default stream:0, svc:0\r\n", url);
		p_info->channel_id = 0;
		p_info->svc_level = 0;
	} else {
		const char *p = p_stream_id + strlen(CFG_TAG_STREAM_ID);
		p_info->channel_id = (int)(*p - '0');
	}
	
	if (p_svc_level == NULL) {
		//printf("no svc info, set to default 0 \r\n");
		p_info->svc_level = 0;
	} else {
		const char *p = p_svc_level + strlen(CFG_TAG_SVC_ID);
		p_info->svc_level = (int)(*p - '0');
		if (p_info->svc_level > 0) {
			printf("invalid svc level %d\r\n", p_info->svc_level);
			return -1;
		}
	}
	return 0;
}

static int on_get_brc(const NVTLIVE555_STATISTIC *p_statistic, NVTLIVE555_STRATEGY *p_strategy)
{
	return 0;
}

static int on_get_meta(char *p_txt, unsigned int txt_buf_size, int first_meta)
{
	return -1; //not support
}

static int on_notify_rtsp_event(LIVE555_SERVER_EVENT evt)
{
	//printf("event %d\r\n", evt);
	return 0;
}

static int on_require_key_frame(int channel)
{
	uctrl_cmd("urtsp -reset_iframe %d", channel);
	return 0;
}

static int on_seek(int channel, double request, double *response)
{
#if !defined(_WIN32) && !defined(__UBUNTU)
	*response = 0;
	return 0;
#else
	printf("seek: %f\r\n", request);
	extern int FrmSeek(int channel, double request, double *response);
	return FrmSeek(channel, request, response);
#endif
}

static int on_pause(int channel)
{
#if !defined(_WIN32) && !defined(__UBUNTU)
	//send uctrl
	return 0;
#else
	extern int FrmSetState(int channel, int state);
	return FrmSetState(channel, 1);
#endif
}

static int on_resume(int channel)
{
#if !defined(_WIN32) && !defined(__UBUNTU)
	//send uctrl
	return 0;
#else
	extern int FileFrmSetState(int channel, int state);
	return FileFrmSetState(channel, 0);
#endif
}

#if defined(_WIN32) || defined(__UBUNTU)
extern int FrmGetTotalTime(int channel, unsigned int *total_ms);
extern int FrmSeek(int channel, double request, double *response);
extern int FrmSetState(int channel, int state);
static int on_parse_url_win32(const char *url, NVTLIVE555_URL_INFO *p_info)
{
	int er = on_parse_url(url, p_info);
	if (er < 0) {
		return er;
	}
	er = FrmGetTotalTime(p_info->channel_id, &p_info->total_ms);
	return er;
}
static int on_seek_win32(int channel, double request, double *response)
{
	//printf("seek: %f\r\n", request);
	return FrmSeek(channel, request, response);
}
static int on_pause_win32(int channel)
{
	return FrmSetState(channel, 1);
}
static int on_resume_win32(int channel)
{
	return FrmSetState(channel, 0);
}
#endif

#if defined(_WIN32)
int nvtrtspd_open(void)
#else
static int nvtrtspd_open(void)
#endif
{
	int er;

	// init uctrl
	if ((m_uctrl_hdl = UCTRL_OPEN()) == 0) {
		printf("nvtlive555: failed to open uctrl.\n");
		return -1;
	}

	// init live555
	NVTLIVE555_INIT init = { 0 };
	init.version = NVTLIVE555_INTERFACE_VER_VERSION;
	init.require_cb.get_cfg = on_get_cfg;
	init.require_cb.parse_url = on_parse_url;
	init.require_cb.get_brc = on_get_brc;
	init.require_cb.get_meta = on_get_meta;
	init.require_cb.notify_rtsp_event = on_notify_rtsp_event;
	init.require_cb.require_key_frame = on_require_key_frame;
	init.require_cb.seek = on_seek;
	init.require_cb.pause = on_pause;
	init.require_cb.resume = on_resume;
#if defined(_WIN32) || defined(__UBUNTU)
	init.require_cb.parse_url = on_parse_url_win32;
	init.require_cb.seek = on_seek_win32;
	init.require_cb.pause = on_pause_win32;
	init.require_cb.resume = on_resume_win32;
#endif
	er = nvtlive555_open(&init);
	return er;
}

#if defined(_WIN32) || defined(__ECOS)
#if defined(_WIN32)
int nvtrtspd_close(void)
#else
static int nvtrtspd_close(void)
#endif
{
	// live555 exits
	nvtlive555_close();

	// uctrl exits
	if (m_uctrl_hdl) {
		UCTRL_CLOSE(m_uctrl_hdl);
	}
	return 0;
}
#endif

#if defined(__LINUX) || defined(__UBUNTU)
int main(int argc, char *argv[])
{
	if (argc > 1) {
		// debug command system
		nvtlive555_dbgcmd(argc, argv);
	} else {
		nvtrtspd_open();
	}
	return 0;
}
#elif defined(__ECOS)
#include <cyg/infra/mainfunc.h>
MAINFUNC_ENTRY(nvtrtspd_ecos, argc, argv)
{
	if (argc < 2) {
		printf("usage: nvtrtspd_ecos open or nvtrtspd_ecos close.\r\n");
	} else if (strcmp(argv[1], "open") == 0) {
		nvtrtspd_open();
	} else if (strcmp(argv[1], "close") == 0) {
		nvtrtspd_close();
	} else {
		printf("nvtrtspd_ecos: unknown command %s\r\n", argv[1]);
	}
	return 0;
}
#endif
