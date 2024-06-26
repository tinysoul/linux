/*
 * Copyright 2003-2005	Devicescape Software, Inc.
 * Copyright (c) 2006	Jiri Benc <jbenc@suse.cz>
 * Copyright 2007	Johannes Berg <johannes@sipsolutions.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kobject.h>
#include <linux/slab.h>
#include "ieee80211_i.h"
#include "key.h"
#include "debugfs.h"
#include "debugfs_key.h"

#ifdef CONFIG_XRMAC_DEBUGFS

#define KEY_READ(name, prop, format_string)				\
static ssize_t key_##name##_read(struct file *file,			\
				 char __user *userbuf,			\
				 size_t count, loff_t *ppos)		\
{									\
	struct ieee80211_key *key = file->private_data;			\
	return xrmac_format_buffer(userbuf, count, ppos,		\
				      format_string, key->prop);	\
}
#define KEY_READ_D(name) KEY_READ(name, name, "%d\n")
#define KEY_READ_X(name) KEY_READ(name, name, "0x%x\n")

#define KEY_OPS(name)							\
static const struct file_operations key_ ##name## _ops = {		\
	.read = key_##name##_read,					\
	.open = mac80211_open_file_generic,				\
	.llseek = generic_file_llseek,					\
}

#define KEY_CONF_READ(name, format_string)				\
	KEY_READ(conf_##name, conf.name, format_string)
#define KEY_CONF_READ_D(name) KEY_CONF_READ(name, "%d\n")

#define KEY_CONF_OPS(name)						\
static const struct file_operations key_ ##name## _ops = {		\
	.read = key_conf_##name##_read,					\
	.open = mac80211_open_file_generic,				\
	.llseek = generic_file_llseek,					\
}


KEY_CONF_READ_D(keylen);
KEY_CONF_OPS(keylen);

KEY_CONF_READ_D(keyidx);
KEY_CONF_OPS(keyidx);

KEY_CONF_READ_D(hw_key_idx);
KEY_CONF_OPS(hw_key_idx);

KEY_READ_X(flags);
KEY_OPS(flags);

KEY_READ_D(tx_rx_count);
KEY_OPS(tx_rx_count);

KEY_READ(ifindex, sdata->name, "%s\n");
KEY_OPS(ifindex);

static ssize_t key_algorithm_read(struct file *file,
				  char __user *userbuf,
				  size_t count, loff_t *ppos)
{
	char buf[15];
	struct ieee80211_key *key = file->private_data;
	u32 c = key->conf.cipher;

	sprintf(buf, "%.2x-%.2x-%.2x:%d\n",
		c >> 24, (c >> 16) & 0xff, (c >> 8) & 0xff, c & 0xff);
	return simple_read_from_buffer(userbuf, count, ppos, buf, strlen(buf));
}
KEY_OPS(algorithm);

static ssize_t key_tx_spec_read(struct file *file, char __user *userbuf,
				size_t count, loff_t *ppos)
{
	u64 pn;
	char buf[20];
	int len;
	struct ieee80211_key *key = file->private_data;

	switch (key->conf.cipher) {
	case WLAN_CIPHER_SUITE_WEP40:
	case WLAN_CIPHER_SUITE_WEP104:
		len = scnprintf(buf, sizeof(buf), "\n");
		break;
	case WLAN_CIPHER_SUITE_TKIP:
		len = scnprintf(buf, sizeof(buf), "%08x %04x\n",
				key->u.tkip.tx.iv32,
				key->u.tkip.tx.iv16);
		break;
	case WLAN_CIPHER_SUITE_CCMP:
		pn = atomic64_read(&key->u.ccmp.tx_pn);
		len = scnprintf(buf, sizeof(buf), "%02x%02x%02x%02x%02x%02x\n",
				(u8)(pn >> 40), (u8)(pn >> 32), (u8)(pn >> 24),
				(u8)(pn >> 16), (u8)(pn >> 8), (u8)pn);
		break;
	case WLAN_CIPHER_SUITE_AES_CMAC:
		pn = atomic64_read(&key->u.aes_cmac.tx_pn);
		len = scnprintf(buf, sizeof(buf), "%02x%02x%02x%02x%02x%02x\n",
				(u8)(pn >> 40), (u8)(pn >> 32), (u8)(pn >> 24),
				(u8)(pn >> 16), (u8)(pn >> 8), (u8)pn);
		break;
	default:
		return 0;
	}
	return simple_read_from_buffer(userbuf, count, ppos, buf, len);
}
KEY_OPS(tx_spec);

static ssize_t key_rx_spec_read(struct file *file, char __user *userbuf,
				size_t count, loff_t *ppos)
{
	struct ieee80211_key *key = file->private_data;
	char buf[14*NUM_RX_DATA_QUEUES+1], *p = buf;
	int i, len;
	const u8 *rpn;

	switch (key->conf.cipher) {
	case WLAN_CIPHER_SUITE_WEP40:
	case WLAN_CIPHER_SUITE_WEP104:
		len = scnprintf(buf, sizeof(buf), "\n");
		break;
	case WLAN_CIPHER_SUITE_TKIP:
		for (i = 0; i < NUM_RX_DATA_QUEUES; i++)
			p += scnprintf(p, sizeof(buf)+buf-p,
				       "%08x %04x\n",
				       key->u.tkip.rx[i].iv32,
				       key->u.tkip.rx[i].iv16);
		len = p - buf;
		break;
	case WLAN_CIPHER_SUITE_CCMP:
		for (i = 0; i < NUM_RX_DATA_QUEUES + 1; i++) {
			rpn = key->u.ccmp.rx_pn[i];
			p += scnprintf(p, sizeof(buf)+buf-p,
				       "%02x%02x%02x%02x%02x%02x\n",
				       rpn[0], rpn[1], rpn[2],
				       rpn[3], rpn[4], rpn[5]);
		}
		len = p - buf;
		break;
	case WLAN_CIPHER_SUITE_AES_CMAC:
		rpn = key->u.aes_cmac.rx_pn;
		p += scnprintf(p, sizeof(buf)+buf-p,
			       "%02x%02x%02x%02x%02x%02x\n",
			       rpn[0], rpn[1], rpn[2],
			       rpn[3], rpn[4], rpn[5]);
		len = p - buf;
		break;
	default:
		return 0;
	}
	return simple_read_from_buffer(userbuf, count, ppos, buf, len);
}
KEY_OPS(rx_spec);

static ssize_t key_replays_read(struct file *file, char __user *userbuf,
				size_t count, loff_t *ppos)
{
	struct ieee80211_key *key = file->private_data;
	char buf[20];
	int len;

	switch (key->conf.cipher) {
	case WLAN_CIPHER_SUITE_CCMP:
		len = scnprintf(buf, sizeof(buf), "%u\n", key->u.ccmp.replays);
		break;
	case WLAN_CIPHER_SUITE_AES_CMAC:
		len = scnprintf(buf, sizeof(buf), "%u\n",
				key->u.aes_cmac.replays);
		break;
	default:
		return 0;
	}
	return simple_read_from_buffer(userbuf, count, ppos, buf, len);
}
KEY_OPS(replays);

static ssize_t key_icverrors_read(struct file *file, char __user *userbuf,
				  size_t count, loff_t *ppos)
{
	struct ieee80211_key *key = file->private_data;
	char buf[20];
	int len;

	switch (key->conf.cipher) {
	case WLAN_CIPHER_SUITE_AES_CMAC:
		len = scnprintf(buf, sizeof(buf), "%u\n",
				key->u.aes_cmac.icverrors);
		break;
	default:
		return 0;
	}
	return simple_read_from_buffer(userbuf, count, ppos, buf, len);
}
KEY_OPS(icverrors);

static ssize_t key_key_read(struct file *file, char __user *userbuf,
			    size_t count, loff_t *ppos)
{
	struct ieee80211_key *key = file->private_data;
	int i, bufsize = 2 * key->conf.keylen + 2;
	char *buf = kmalloc(bufsize, GFP_KERNEL);
	char *p = buf;
	ssize_t res;

	if (!buf)
		return -ENOMEM;

	for (i = 0; i < key->conf.keylen; i++)
		p += scnprintf(p, bufsize + buf - p, "%02x", key->conf.key[i]);
	p += scnprintf(p, bufsize+buf-p, "\n");
	res = simple_read_from_buffer(userbuf, count, ppos, buf, p - buf);
	kfree(buf);
	return res;
}
KEY_OPS(key);

#define DEBUGFS_ADD(name) \
	debugfs_create_file(#name, 0400, key->debugfs.dir, \
			    key, &key_##name##_ops);

void xrmac80211_debugfs_key_add(struct ieee80211_key *key)
{
	static int keycount;
	char buf[50];
	struct sta_info *sta;

	if (!key->local->debugfs.keys)
		return;

	sprintf(buf, "%d", keycount);
	key->debugfs.cnt = keycount;
	keycount++;
	key->debugfs.dir = debugfs_create_dir(buf,
					key->local->debugfs.keys);

	if (!key->debugfs.dir)
		return;

	sta = key->sta;
	if (sta) {
		sprintf(buf, "../../stations/%pM", sta->sta.addr);
		key->debugfs.stalink =
			debugfs_create_symlink("station", key->debugfs.dir, buf);
	}

	DEBUGFS_ADD(keylen);
	DEBUGFS_ADD(flags);
	DEBUGFS_ADD(keyidx);
	DEBUGFS_ADD(hw_key_idx);
	DEBUGFS_ADD(tx_rx_count);
	DEBUGFS_ADD(algorithm);
	DEBUGFS_ADD(tx_spec);
	DEBUGFS_ADD(rx_spec);
	DEBUGFS_ADD(replays);
	DEBUGFS_ADD(icverrors);
	DEBUGFS_ADD(key);
	DEBUGFS_ADD(ifindex);
};

void mac80211_debugfs_key_remove(struct ieee80211_key *key)
{
	if (!key)
		return;

	debugfs_remove_recursive(key->debugfs.dir);
	key->debugfs.dir = NULL;
}

void mac80211_debugfs_key_update_default(struct ieee80211_sub_if_data *sdata)
{
	char buf[50];
	struct ieee80211_key *key;

	if (!sdata->debugfs.dir)
		return;

	lockdep_assert_held(&sdata->local->key_mtx);

	if (sdata->default_unicast_key) {
		key = key_mtx_dereference(sdata->local,
					  sdata->default_unicast_key);
		sprintf(buf, "../keys/%d", key->debugfs.cnt);
		sdata->debugfs.default_unicast_key =
			debugfs_create_symlink("default_unicast_key",
					       sdata->debugfs.dir, buf);
	} else {
		debugfs_remove(sdata->debugfs.default_unicast_key);
		sdata->debugfs.default_unicast_key = NULL;
	}

	if (sdata->default_multicast_key) {
		key = key_mtx_dereference(sdata->local,
					  sdata->default_multicast_key);
		sprintf(buf, "../keys/%d", key->debugfs.cnt);
		sdata->debugfs.default_multicast_key =
			debugfs_create_symlink("default_multicast_key",
					       sdata->debugfs.dir, buf);
	} else {
		debugfs_remove(sdata->debugfs.default_multicast_key);
		sdata->debugfs.default_multicast_key = NULL;
	}
}

void mac80211_debugfs_key_add_mgmt_default(struct ieee80211_sub_if_data *sdata)
{
	char buf[50];
	struct ieee80211_key *key;

	if (!sdata->debugfs.dir)
		return;

	key = key_mtx_dereference(sdata->local,
				  sdata->default_mgmt_key);
	if (key) {
		sprintf(buf, "../keys/%d", key->debugfs.cnt);
		sdata->debugfs.default_mgmt_key =
			debugfs_create_symlink("default_mgmt_key",
					       sdata->debugfs.dir, buf);
	} else
		mac80211_debugfs_key_remove_mgmt_default(sdata);
}

void mac80211_debugfs_key_remove_mgmt_default(struct ieee80211_sub_if_data *sdata)
{
	if (!sdata)
		return;

	debugfs_remove(sdata->debugfs.default_mgmt_key);
	sdata->debugfs.default_mgmt_key = NULL;
}

void mac80211_debugfs_key_sta_del(struct ieee80211_key *key,
				   struct sta_info *sta)
{
	debugfs_remove(key->debugfs.stalink);
	key->debugfs.stalink = NULL;
}
#endif /* CONFIG_XRMAC_DEBUGFS */
