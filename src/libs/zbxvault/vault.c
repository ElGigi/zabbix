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

#include "zbxvault.h"
#include "../zbxhashicorp/hashicorp.h"
#include "../zbxcyberark/cyberark.h"
#include "../zbxkvs/kvs.h"

#define ZBX_VAULT_TIMEOUT	SEC_PER_MIN

extern char	*CONFIG_VAULTTOKEN;
extern char	*CONFIG_VAULTURL;
extern char	*CONFIG_VAULTTLSCERTFILE;
extern char	*CONFIG_VAULTTLSKEYFILE;
extern char	*CONFIG_VAULTDBPATH;

extern char	*CONFIG_DBUSER;
extern char	*CONFIG_DBPASSWORD;

static zbx_vault_kvs_get_cb_t	zbx_vault_kvs_get_cb;
static char			*zbx_vault_key_dbuser, *zbx_vault_key_dbpassword;

int	zbx_vault_init_token_from_env(char **error)
{
#if defined(HAVE_GETENV) && defined(HAVE_UNSETENV)
	char	*ptr;

	if (NULL == (ptr = getenv("VAULT_TOKEN")))
		return SUCCEED;

	if (NULL != CONFIG_VAULTTOKEN)
	{
		*error = zbx_dsprintf(*error, "both \"VaultToken\" configuration parameter"
				" and \"VAULT_TOKEN\" environment variable are defined");
		return FAIL;
	}

	CONFIG_VAULTTOKEN = zbx_strdup(NULL, ptr);
	unsetenv("VAULT_TOKEN");
#endif
	return SUCCEED;
}

static void	zbx_vault_init_cb(zbx_vault_kvs_get_cb_t vault_kvs_get_cb)
{
	zbx_vault_kvs_get_cb = vault_kvs_get_cb;
}

int	zbx_vault_init(const char *vault, char **error)
{
	if (NULL == vault || '\0' == *vault || 0 == strcmp(vault, ZBX_VAULT_HASHICORP))
	{
		if (NULL == CONFIG_VAULTTOKEN && 0 == zbx_strcmp_null(vault, ZBX_VAULT_HASHICORP))
		{
			*error = zbx_dsprintf(*error, "\"Vault\" value \"%s\" requires \"VaultToken\" configuration"
					" parameter or \"VAULT_TOKEN\" environment variable", vault);
			return FAIL;
		}

		zbx_vault_init_cb(zbx_hashicorp_kvs_get);
		zbx_vault_key_dbuser = ZBX_KEY_HASHICORP_USERNAME;
		zbx_vault_key_dbpassword = ZBX_KEY_HASHICORP_PASSWORD;
	}
	else if (0 == strcmp(vault, ZBX_VAULT_CYBERARKCPP))
	{
		if (NULL != CONFIG_VAULTTOKEN)
		{
			*error = zbx_dsprintf(*error, "\"Vault\" value \"%s\" requires \"VaultToken\" configuration"
					" parameter and \"VAULT_TOKEN\" environment variable not to be defined", vault);
			return FAIL;
		}

		zbx_vault_init_cb(zbx_cyberark_kvs_get);
		zbx_vault_key_dbuser = ZBX_KEY_CYBERARK_USERNAME;
		zbx_vault_key_dbpassword = ZBX_KEY_CYBERARK_PASSWORD;
	}
	else
	{
		*error = zbx_dsprintf(*error, "invalid \"Vault\" configuration parameter: '%s'", vault);
		return FAIL;
	}

	return SUCCEED;
}

int	zbx_vault_kvs_get(const char *path, zbx_hashset_t *kvs, char **error)
{
	if (NULL == zbx_vault_kvs_get_cb)
	{
		*error = zbx_dsprintf(*error, "missing vault library");
		THIS_SHOULD_NEVER_HAPPEN;
		return FAIL;
	}

	return zbx_vault_kvs_get_cb(CONFIG_VAULTURL, CONFIG_VAULTTOKEN, CONFIG_VAULTTLSCERTFILE, CONFIG_VAULTTLSKEYFILE,
			path, ZBX_VAULT_TIMEOUT, kvs, error);
}

int	zbx_vault_init_db_credentials(char **dbuser, char **dbpassword, char **error)
{
	int		ret = FAIL;
	zbx_hashset_t	kvs;
	zbx_kv_t	*kv_username, *kv_password, kv_local;

	if (NULL == zbx_vault_kvs_get_cb)
	{
		*error = zbx_dsprintf(*error, "missing vault library");
		return FAIL;
	}

	if (NULL == CONFIG_VAULTDBPATH)
		return SUCCEED;

	if (NULL != *dbuser)
	{
		*error = zbx_dsprintf(*error, "\"DBUser\" configuration parameter cannot be used when \"VaultDBPath\""
				" is defined");
		return FAIL;
	}

	if (NULL != *dbpassword)
	{
		*error = zbx_dsprintf(*error, "\"DBPassword\" configuration parameter cannot be used when"
				" \"VaultDBPath\" is defined");
		return FAIL;
	}

	zbx_hashset_create_ext(&kvs, 2, zbx_kv_hash, zbx_kv_compare, zbx_kv_clean,
			ZBX_DEFAULT_MEM_MALLOC_FUNC, ZBX_DEFAULT_MEM_REALLOC_FUNC, ZBX_DEFAULT_MEM_FREE_FUNC);

	if (SUCCEED != zbx_vault_kvs_get_cb(CONFIG_VAULTURL, CONFIG_VAULTTOKEN, CONFIG_VAULTTLSCERTFILE,
			CONFIG_VAULTTLSKEYFILE, CONFIG_VAULTDBPATH, ZBX_VAULT_TIMEOUT, &kvs,
			error))
	{
		goto fail;
	}

	kv_local.key = zbx_vault_key_dbuser;

	if (NULL == (kv_username = (zbx_kv_t *)zbx_hashset_search(&kvs, &kv_local)))
	{
		*error = zbx_dsprintf(*error, "cannot retrieve value of key \"%s\"", ZBX_PROTO_TAG_USERNAME);
		goto fail;
	}

	kv_local.key = zbx_vault_key_dbpassword;

	if (NULL == (kv_password = (zbx_kv_t *)zbx_hashset_search(&kvs, &kv_local)))
	{
		*error = zbx_dsprintf(*error, "cannot retrieve value of key \"%s\"", ZBX_PROTO_TAG_PASSWORD);
		goto fail;
	}

	*dbuser = zbx_strdup(NULL, kv_username->value);
	*dbpassword = zbx_strdup(NULL, kv_password->value);

	ret = SUCCEED;
fail:
	zbx_hashset_destroy(&kvs);

	return ret;
}
