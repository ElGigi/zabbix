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

#ifndef ZABBIX_ZBXVAULT_H
#define ZABBIX_ZBXVAULT_H

#include "common.h"
#include "zbxjson.h"
#include "zbxalgo.h"

typedef	int (*zbx_vault_kvs_get_cb_t)(const char *vault_url, const char *token, const char *ssl_cert_file,
		const char *ssl_key_file, const char *path, long timeout, zbx_hashset_t *kvs, char **error);

int	zbx_vault_init(const char *vault, char **error);
int	zbx_vault_init_token_from_env(char **error);
int	zbx_vault_init_db_credentials(char **dbuser, char **dbpassword, char **error);
int	zbx_vault_kvs_get(const char *path, zbx_hashset_t *kvs, char **error);

#endif
