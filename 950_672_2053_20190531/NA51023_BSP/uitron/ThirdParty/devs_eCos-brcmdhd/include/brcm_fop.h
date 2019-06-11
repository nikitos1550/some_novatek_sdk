/*
 * Copyright (c) 2015 Broadcom Corporation
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef BRCM_DHD_ECOS_FOP_H
#define BRCM_DHD_ECOS_FOP_H

void dhd_ecos_fop_set_firmware_type(int type);
int dhd_ecos_fop_get_firmware_type(void);
void dhd_ecos_fop_dump_fw_info(void);
void dhd_ecos_fop_dump_nv_info(void);
char * dhd_ecos_get_firmware_array(void);
int dhd_ecos_get_firmware_size(void);
char * dhd_ecos_get_nvram_array(void);
int dhd_ecos_get_nvram_size(void);

#endif
