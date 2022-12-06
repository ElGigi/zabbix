/*
** Zabbix
** Copyright (C) 2001-2022 Zabbix SIA
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

#ifndef ZABBIX_ZBX_RTC_CONSTANTS_H
#define ZABBIX_ZBX_RTC_CONSTANTS_H

/* runtime control commands */
#define ZBX_RTC_UNKNOWN				0
#define ZBX_RTC_LOG_LEVEL_INCREASE		1
#define ZBX_RTC_LOG_LEVEL_DECREASE		2
#define ZBX_RTC_HOUSEKEEPER_EXECUTE		3
#define ZBX_RTC_CONFIG_CACHE_RELOAD		8
#define ZBX_RTC_SNMP_CACHE_RELOAD		9
#define ZBX_RTC_DIAGINFO			10
#define ZBX_RTC_SECRETS_RELOAD			11
#define ZBX_RTC_SERVICE_CACHE_RELOAD		12
#define ZBX_RTC_TRIGGER_HOUSEKEEPER_EXECUTE	13
#define ZBX_RTC_HA_STATUS			14
#define ZBX_RTC_HA_REMOVE_NODE			15
#define ZBX_RTC_HA_SET_FAILOVER_DELAY		16
#define ZBX_RTC_USER_PARAMETERS_RELOAD		17
#define ZBX_RTC_PROXY_CONFIG_CACHE_RELOAD	18
#define ZBX_RTC_PROXYPOLLER_PROCESS		19

/* internal rtc messages */
#define ZBX_RTC_SUBSCRIBE			100
#define ZBX_RTC_SHUTDOWN			101
#define ZBX_RTC_CONFIG_CACHE_RELOAD_WAIT	102

/* runtime control notifications, must be less than 10000 */
#define ZBX_RTC_CONFIG_SYNC_NOTIFY		9999

#define ZBX_IPC_RTC_MAX				9999

/* runtime control options */
#define ZBX_CONFIG_CACHE_RELOAD		"config_cache_reload"
#define ZBX_SERVICE_CACHE_RELOAD	"service_cache_reload"
#define ZBX_SECRETS_RELOAD		"secrets_reload"
#define ZBX_HOUSEKEEPER_EXECUTE		"housekeeper_execute"
#define ZBX_LOG_LEVEL_INCREASE		"log_level_increase"
#define ZBX_LOG_LEVEL_DECREASE		"log_level_decrease"
#define ZBX_SNMP_CACHE_RELOAD		"snmp_cache_reload"
#define ZBX_DIAGINFO			"diaginfo"
#define ZBX_TRIGGER_HOUSEKEEPER_EXECUTE "trigger_housekeeper_execute"
#define ZBX_HA_STATUS			"ha_status"
#define ZBX_HA_REMOVE_NODE		"ha_remove_node"
#define ZBX_HA_SET_FAILOVER_DELAY	"ha_set_failover_delay"
#define ZBX_USER_PARAMETERS_RELOAD	"userparameter_reload"
#define ZBX_PROXY_CONFIG_CACHE_RELOAD	"proxy_config_cache_reload"

#endif
