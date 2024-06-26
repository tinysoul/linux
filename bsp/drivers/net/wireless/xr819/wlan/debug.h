/*
 * DebugFS code for XRadio drivers
 *
 * Copyright (c) 2013
 * Xradio Technology Co., Ltd. <www.xradiotech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef XRADIO_DEBUG_H_INCLUDED
#define XRADIO_DEBUG_H_INCLUDED

#include "itp.h"

/* Message always need to be present even in release version. */
#define XRADIO_DBG_ALWY   0x01

/* Error message to report an error, it can hardly works. */
#define XRADIO_DBG_ERROR  0x02

/* Warning message to inform us of something unnormal or
* something very important, but it still work. */
#define XRADIO_DBG_WARN   0x04

/* Important message we need to know in unstable version. */
#define XRADIO_DBG_NIY    0x08

/* Normal message just for debug in developing stage. */
#define XRADIO_DBG_MSG    0x10

/* Trace of functions, for sequence of functions called. Normally,
* don't set this level because there are too more print. */
#define XRADIO_DBG_TRC    0x20

#define XRADIO_DBG_LEVEL	0xFF

/* for host debuglevel*/
extern u8 dbg_common ;
extern u8 dbg_sbus   ;
extern u8 dbg_bh     ;
extern u8 dbg_txrx   ;
extern u8 dbg_wsm    ;
extern u8 dbg_sta    ;
extern u8 dbg_scan   ;
extern u8 dbg_ap     ;
extern u8 dbg_pm     ;
extern u8 dbg_itp    ;
extern u8 dbg_etf    ;
extern u8 dbg_logfile;

/*for sdio clk debug*/
extern u32 dbg_sdio_clk;

/* for ps debug */
extern u8  ps_disable;
extern u8  ps_idleperiod;
extern u8  ps_changeperiod;
/* info of bh thread*/
extern u32 irq_count;
extern u32 int_miss_cnt;
extern u32 fix_miss_cnt;
extern u32 next_rx_cnt;
extern u32 rx_total_cnt;
extern u32 tx_total_cnt;
extern u32 tx_buf_limit;
extern u32 dbg_txconfirm[32];
extern int tx_burst_limit;

#define WSM_DUMP_MAX_SIZE 20

#if defined(CONFIG_XRADIO_DEBUG)
#define DGB_LOG_FILE    0  /*Don't log more than 500byte once.*/
#define DGB_XRADIO_QC   1  /*Enable this only in QC test.*/
#define DGB_XRADIO_HWT  0  /*Only in hardware test with special fw.*/
#else
#define DGB_LOG_FILE    0  /*Don't log more than 500byte once.*/
#define DGB_XRADIO_QC   0  /*Enable this only in QC test.*/
#define DGB_XRADIO_HWT  0  /*Only in hardware test with special fw.*/
#endif

#if DGB_LOG_FILE
#define DGB_LOG_BUF_LEN 1500
#define DGB_LOG_PATH0    "/data/xradio_err.log"

extern u8 log_buffer[DGB_LOG_BUF_LEN];
extern u16 log_pos;
int xradio_logfile(char *buffer, int buf_len, u8 b_time);

#define LOG_FILE(b_time, msg) xradio_logfile(msg, -1, b_time)
#define LOG_FILE_VARS(b_time, ...)  do {    \
	if (!log_pos)   \
		memset(log_buffer, 0, DGB_LOG_BUF_LEN); \
	if (log_pos <= 1000)  \
		log_pos += sprintf((char *)(log_buffer+log_pos), __VA_ARGS__); \
	if (xradio_logfile(log_buffer, log_pos, b_time) >= 0)   \
		log_pos = 0; \
} while (0)

#else /*DGB_LOG_FILE disable*/

#define LOG_FILE(b_time, msg)
#define LOG_FILE_VARS(b_time, ...)
#endif

#define FW_RECORD_READ  1
#if FW_RECORD_READ
void fw_record_read(struct xradio_common *hw_priv);
#endif

#if (defined(CONFIG_XRADIO_DEBUG))
/****************************** debug version *******************************/
#if (defined(CONFIG_XRADIO_DUMP_ON_ERROR))
#define SYS_BUG(c)     BUG_ON(c)
#define SYS_WARN(c)    WARN_ON(c)
#else
#define SYS_BUG(c)     WARN_ON(c)
#define SYS_WARN(c)    WARN_ON(c)
#endif

#define xradio_dbg(level, ...)           \
	do {                                 \
		if ((level) & dbg_common & XRADIO_DBG_ERROR)      \
			printk(KERN_ERR "[XRADIO_ERR] " __VA_ARGS__); \
		else if ((level) & dbg_common & XRADIO_DBG_WARN)  \
			printk(KERN_ERR "[XRADIO_WRN] " __VA_ARGS__); \
		else if ((level) & dbg_common)                    \
			printk(KERN_ERR "[XRADIO] " __VA_ARGS__);     \
		if ((level) & dbg_logfile)                        \
			LOG_FILE_VARS(((level)&XRADIO_DBG_ERROR),     \
				       "[XRADIO_ERR] " __VA_ARGS__);  \
	} while (0)

#define sbus_printk(level, ...)     \
	do {                              \
		if ((level) & dbg_sbus & XRADIO_DBG_ERROR)       \
			printk(KERN_ERR "[SBUS_ERR] " __VA_ARGS__);  \
		else if ((level) & dbg_sbus & XRADIO_DBG_WARN)   \
			printk(KERN_ERR "[SBUS_WRN] " __VA_ARGS__);  \
		else if ((level) & dbg_sbus)                     \
			printk(KERN_ERR "[SBUS] " __VA_ARGS__);      \
		if ((level) & dbg_logfile)         \
			LOG_FILE_VARS(((level)&XRADIO_DBG_ERROR),    \
				      "[SBUS_ERR] " __VA_ARGS__);    \
	} while (0)

#define txrx_printk(level, ...)     \
	do {                              \
		if ((level) & dbg_txrx & XRADIO_DBG_ERROR)       \
			printk(KERN_ERR "[TXRX_ERR] " __VA_ARGS__);  \
		else if ((level) & dbg_txrx & XRADIO_DBG_WARN)   \
			printk(KERN_ERR "[TXRX_WRN] " __VA_ARGS__);  \
		else if ((level) & dbg_txrx)                     \
			printk(KERN_ERR "[TXRX] " __VA_ARGS__);      \
		if ((level) & dbg_logfile)         \
			LOG_FILE_VARS(((level)&XRADIO_DBG_ERROR),  \
				      "[TXRX_ERR] " __VA_ARGS__);  \
	} while (0)

#define bh_printk(level, ...)       \
	do {                              \
		if ((level) & dbg_bh & XRADIO_DBG_ERROR)       \
			printk(KERN_ERR "[BH_ERR] " __VA_ARGS__);  \
		else if ((level) & dbg_bh & XRADIO_DBG_WARN)   \
			printk(KERN_ERR "[BH_WRN] " __VA_ARGS__);  \
		else if ((level) & dbg_bh)                     \
			printk(KERN_ERR "[BH] " __VA_ARGS__);      \
		if ((level) & dbg_logfile)         \
			LOG_FILE_VARS(((level)&XRADIO_DBG_ERROR), \
				      "[BH_ERR] " __VA_ARGS__);   \
	} while (0)

#define wsm_printk(level, ...)      \
	do {                              \
		if ((level) & dbg_wsm & XRADIO_DBG_ERROR)       \
			printk(KERN_ERR "[WSM_ERR] " __VA_ARGS__);  \
		else if ((level) & dbg_wsm & XRADIO_DBG_WARN)   \
			printk(KERN_ERR "[WSM_WRN] " __VA_ARGS__);  \
		else if ((level) & dbg_wsm)          \
			printk(KERN_ERR "[WSM] " __VA_ARGS__); \
		if ((level) & dbg_logfile)         \
			LOG_FILE_VARS(((level)&XRADIO_DBG_ERROR), \
				      "[WSM_ERR] " __VA_ARGS__);      \
	} while (0)

#define sta_printk(level, ...)      \
	do {                              \
		if ((level) & dbg_sta & XRADIO_DBG_ERROR)       \
			printk(KERN_ERR "[STA_ERR] " __VA_ARGS__);  \
		else if ((level) & dbg_sta & XRADIO_DBG_WARN)   \
			printk(KERN_ERR "[STA_WRN] " __VA_ARGS__);  \
		else if ((level) & dbg_sta)          \
			printk(KERN_ERR "[STA] " __VA_ARGS__); \
		if ((level) & dbg_logfile)         \
			LOG_FILE_VARS(((level)&XRADIO_DBG_ERROR),  \
				      "[STA_ERR] " __VA_ARGS__);   \
	} while (0)

#define scan_printk(level, ...)      \
	do {                              \
		if ((level) & dbg_scan & XRADIO_DBG_ERROR)       \
			printk(KERN_ERR "[SCAN_ERR] " __VA_ARGS__);  \
		else if ((level) & dbg_scan & XRADIO_DBG_WARN)   \
			printk(KERN_ERR "[SCAN_WRN] " __VA_ARGS__);  \
		else if ((level) & dbg_scan)                 \
			printk(KERN_ERR "[SCAN] " __VA_ARGS__);  \
		if ((level) & dbg_logfile)         \
			LOG_FILE_VARS(((level)&XRADIO_DBG_ERROR), \
				      "[SCAN_ERR] " __VA_ARGS__); \
	} while (0)

#define ap_printk(level, ...)       \
	do {                              \
		if ((level) & dbg_ap & XRADIO_DBG_ERROR)       \
			printk(KERN_ERR "[AP_ERR] " __VA_ARGS__);  \
		else if ((level) & dbg_ap & XRADIO_DBG_WARN)   \
			printk(KERN_ERR "[AP_WRN] " __VA_ARGS__);  \
		else if ((level) & dbg_ap)                \
			printk(KERN_ERR "[AP] " __VA_ARGS__); \
		if ((level) & dbg_logfile)         \
			LOG_FILE_VARS(((level)&XRADIO_DBG_ERROR), "[AP_ERR] " __VA_ARGS__);   \
	} while (0)

#define pm_printk(level, ...)       \
	do {                              \
		if ((level) & dbg_pm & XRADIO_DBG_ERROR)       \
			printk(KERN_ERR "[PM_ERR] " __VA_ARGS__);  \
		else if ((level) & dbg_pm & XRADIO_DBG_WARN)   \
			printk(KERN_ERR "[PM_WRN] " __VA_ARGS__);  \
		else if ((level) & dbg_pm)           \
			printk(KERN_ERR "[PM] " __VA_ARGS__); \
		if ((level) & dbg_logfile)         \
			LOG_FILE_VARS(((level)&XRADIO_DBG_ERROR), \
				      "[PM_ERR] " __VA_ARGS__);   \
	} while (0)

#define itp_printk(level, ...)      \
	do {                              \
		if ((level) & dbg_itp & XRADIO_DBG_ERROR)       \
			printk(KERN_ERR "[ITP_ERR] " __VA_ARGS__);  \
		else if ((level) & dbg_itp & XRADIO_DBG_WARN)   \
			printk(KERN_ERR "[ITP_WRN] " __VA_ARGS__);  \
		else if ((level) & dbg_itp)          \
			printk(KERN_ERR "[ITP] " __VA_ARGS__); \
		if ((level) & dbg_logfile)         \
			LOG_FILE_VARS(((level)&XRADIO_DBG_ERROR), \
				      "[ITP_ERR] " __VA_ARGS__);  \
	} while (0)

#define etf_printk(level, ...)      \
	do {                              \
		if ((level) & dbg_etf & XRADIO_DBG_ERROR)       \
			printk(KERN_ERR "[ETF_ERR] " __VA_ARGS__);  \
		else if ((level) & dbg_etf & XRADIO_DBG_WARN)   \
			printk(KERN_ERR "[ETF_WRN] " __VA_ARGS__);  \
		else if ((level) & dbg_etf)          \
			printk(KERN_ERR "[ETF] " __VA_ARGS__); \
		if ((level) & dbg_logfile)         \
			LOG_FILE_VARS(((level)&XRADIO_DBG_ERROR), \
				      "[ETF_ERR] " __VA_ARGS__);  \
	} while (0)


#define DBG_FUN_LINE   printk(KERN_ERR "%s,line=%d", __func__, __LINE__)
#define PARAM_CHECK_FALSE(p) \
	do {                      \
		if (!p) \
			DBG_FUN_LINE; \
	} while (0)

#define PARAM_CHECK_TRUE(p) \
	do { \
		if (p) \
			DBG_FUN_LINE; \
	} while (0)


/*interfaces to debug packet's information.*/
/*for802.11*/
#define PF_CTRL     0x0001
#define PF_MGMT     0x0002
#define PF_DATA     0x0004
#define PF_SCAN     0x0008

/*for ip data*/
#define PF_TCP      0x0010
#define PF_UDP      0x0020
#define PF_DHCP     0x0040
#define PF_ICMP     0x0080

/*for special frames or info.*/
#define PF_8021X    0x0100    /*action frames*/
#define PF_MAC_SN   0x0200    /*mac seq*/
#define PF_OWNMAC   0x0400    /*TA in Tx, RA in Rx.*/
#define PF_SA_DA    0x0800    /*SA, DA for Ethernet.*/

#define PF_MACADDR  0x1000    /*RA in Tx, TA in Rx.*/
#define PF_IPADDR   0x2000    /*ip address of ip packets.*/
#define PF_UNKNWN   0x4000    /*print frames of unknown flag.*/
#define PF_RX       0x8000    /*0:TX, 1:RX. So, need to add PF_RX in Rx path.*/
void xradio_parse_frame(void *skb, u8 iv_len, u16 flags, u8 if_id);

#if (DGB_XRADIO_HWT)  /*hardware test*/
typedef struct HWT_PARAMETERS_S {
	u16 Msglen;
	u16 MsgID;
	u16 TestID;
	u16 Params;
	u16 Datalen;
	u16 Data;
} HWT_PARAMETERS;
int get_hwt_hif_tx(struct xradio_common *hw_priv, u8 **data,
		   size_t *tx_len, int *burst, int *vif_selected);
#endif  /*DGB_XRADIO_HWT*/

#else
/****************************** release version *******************************/
#define SYS_BUG(c)  WARN_ON(c)
#define SYS_WARN(c) WARN_ON(c)

#define xradio_dbg(level, ...)
#define sbus_printk(level, ...)
#define txrx_printk(level, ...)
#define bh_printk(level, ...)
#define wsm_printk(level, ...)
#define sta_printk(level, ...)
#define scan_printk(level, ...)
#define ap_printk(level, ...)
#define pm_printk(level, ...)
#define itp_printk(level, ...)

#define DBG_FUN_LINE
#define PARAM_CHECK_FALSE
#define PARAM_CHECK_TRUE

static inline void xradio_parse_frame(void *skb, u8 iv_len,
									  u16 flags, u8 if_id)
{
}
#endif  /*CONFIG_XRADIO_DEBUG*/

#ifdef CONFIG_XRADIO_DEBUGFS
/************************* debugfs version *****************************/
struct xradio_debug_common {
	struct dentry *debugfs_phy;
	int tx_cache_miss;
	int tx_burst;
	int rx_burst;
	int ba_cnt;
	int ba_acc;
	int ba_cnt_rx;
	int ba_acc_rx;
#ifdef CONFIG_XRADIO_ITP
	struct xradio_itp itp;
#endif /* CONFIG_XRADIO_ITP */
};

struct xradio_debug_priv {
	struct dentry *debugfs_phy;
	int tx;
	int tx_agg;
	int rx;
	int rx_agg;
	int tx_multi;
	int tx_multi_frames;
	int tx_align;
	int tx_ttl;
};


#define DBG_INT_ADD(count)   (++count)
#define DBG_ARRY_ADD(a, i)    (++a[i])
int xradio_debug_init_common(struct xradio_common *hw_priv);
int xradio_debug_init_priv(struct xradio_common *hw_priv,
			   struct xradio_vif *priv);
void xradio_debug_release_common(struct xradio_common *hw_priv);
void xradio_debug_release_priv(struct xradio_vif *priv);

static inline void xradio_debug_txed(struct xradio_vif *priv)
{
	if (!priv->debug)
		return;
	++priv->debug->tx;
}

static inline void xradio_debug_txed_agg(struct xradio_vif *priv)
{
	if (!priv->debug)
		return;
	++priv->debug->tx_agg;
}

static inline void xradio_debug_txed_multi(struct xradio_vif *priv,
					   int count)
{
	if (!priv->debug)
		return;
	++priv->debug->tx_multi;
	priv->debug->tx_multi_frames += count;
}

static inline void xradio_debug_rxed(struct xradio_vif *priv)
{
	if (!priv->debug)
		return;
	++priv->debug->rx;
}

static inline void xradio_debug_rxed_agg(struct xradio_vif *priv)
{
	if (!priv->debug)
		return;
	++priv->debug->rx_agg;
}

static inline void xradio_debug_tx_cache_miss(struct xradio_common *hw_priv)
{
	if (!hw_priv->debug)
		return;
	++hw_priv->debug->tx_cache_miss;
}

static inline void xradio_debug_tx_align(struct xradio_vif *priv)
{
	if (!priv->debug)
		return;
	++priv->debug->tx_align;
}

static inline void xradio_debug_tx_ttl(struct xradio_vif *priv)
{
	if (!priv->debug)
		return;
	++priv->debug->tx_ttl;
}

static inline void xradio_debug_tx_burst(struct xradio_common *hw_priv)
{
	if (!hw_priv->debug)
		return;
	++hw_priv->debug->tx_burst;
}

static inline void xradio_debug_rx_burst(struct xradio_common *hw_priv)
{
	if (!hw_priv->debug)
		return;
	++hw_priv->debug->rx_burst;
}

static inline void xradio_debug_ba(struct xradio_common *hw_priv,
				   int ba_cnt, int ba_acc, int ba_cnt_rx,
				   int ba_acc_rx)
{
	if (!hw_priv->debug)
		return;
	hw_priv->debug->ba_cnt = ba_cnt;
	hw_priv->debug->ba_acc = ba_acc;
	hw_priv->debug->ba_cnt_rx = ba_cnt_rx;
	hw_priv->debug->ba_acc_rx = ba_acc_rx;
}

int xradio_print_fw_version(struct xradio_common *hw_priv,
							u8 *buf, size_t len);

int   xradio_host_dbg_init(void);
void  xradio_host_dbg_deinit(void);

#else /* CONFIG_XRADIO_DEBUGFS */
/************************ no debugfs version **************************/
#define DBG_INT_ADD(count)
#define DBG_ARRY_ADD(a, i)

static inline int xradio_debug_init_common(struct xradio_common *hw_priv)
{
	return 0;
}

static inline int xradio_debug_init_priv(struct xradio_common *hw_priv,
			   struct xradio_vif *priv)
{
	return 0;
}

static inline void xradio_debug_release_common(struct xradio_common *hw_priv)
{
}

static inline void xradio_debug_release_priv(struct xradio_vif *priv)
{
}

static inline void xradio_debug_txed(struct xradio_vif *priv)
{
}

static inline void xradio_debug_txed_agg(struct xradio_vif *priv)
{
}

static inline void xradio_debug_txed_multi(struct xradio_vif *priv,
					   int count)
{
}

static inline void xradio_debug_rxed(struct xradio_vif *priv)
{
}

static inline void xradio_debug_rxed_agg(struct xradio_vif *priv)
{
}

static inline void xradio_debug_tx_cache_miss(struct xradio_common *hw_priv)
{
}

static inline void xradio_debug_tx_align(struct xradio_vif *priv)
{
}

static inline void xradio_debug_tx_ttl(struct xradio_vif *priv)
{
}

static inline void xradio_debug_tx_burst(struct xradio_common *hw_priv)
{
}

static inline void xradio_debug_rx_burst(struct xradio_common *hw_priv)
{
}

static inline void xradio_debug_ba(struct xradio_common *hw_priv,
				   int ba_cnt, int ba_acc, int ba_cnt_rx,
				   int ba_acc_rx)
{
}

static inline int xradio_print_fw_version(struct xradio_vif *priv,
									u8 *buf, size_t len)
{
	return 0;
}

static inline int   xradio_host_dbg_init(void)
{
	return 0;
}

static inline void  xradio_host_dbg_deinit(void)
{
}
#endif /* CONFIG_XRADIO_DEBUGFS */

#endif /* XRADIO_DEBUG_H_INCLUDED */
