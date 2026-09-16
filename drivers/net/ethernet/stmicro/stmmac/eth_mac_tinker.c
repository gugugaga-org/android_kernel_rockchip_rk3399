/*
 * Copyright (C) 2010 ROCKCHIP, Inc.
 * Author: roger_chen <cz@rock-chips.com>
 *
 * This program is the virtual flash device
 * used to store bd_addr or MAC
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include "eth_mac_tinker.h"
#include <linux/platform_data/at24.h>

#if 1
#define DBG(x...)   printk("eth_mac:" x)
#else
#define DBG(x...)
#endif

#define VERSION "0.2"
#define WLAN_MAC_FILE "/data/misc/wifi/wlan_mac"

int eth_mac_eeprom(u8 *eth_mac, int gmac_num)
{
#ifdef CONFIG_EEPROM_AT24
	int i;

	memset(eth_mac, 0, 6);
	pr_info("GMAC%d Read the Ethernet MAC address from EEPROM:", gmac_num);
	if (gmac_num == 1)
		at24_read_eeprom(eth_mac, 6, 6);
	else
		at24_read_eeprom(eth_mac, 0, 6);

	for(i=0; i<5; i++)
		pr_info("%2.2x:", eth_mac[i]);
	pr_info("%2.2x\n", eth_mac[i]);
#else
	/*
	 * Without an EEPROM there is nothing to read here.  Do not clear the
	 * address: it may have been filled in by the bootloader through the
	 * device tree 'local-mac-address' property (vendor storage MAC) and it
	 * must stay stable across reboots.
	 */
	pr_info("GMAC%d keeps bootloader MAC address: %pM\n", gmac_num, eth_mac);
#endif

	return 0;
}
