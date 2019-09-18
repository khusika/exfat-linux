// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  Copyright (C) 2012-2013 Samsung Electronics Co., Ltd.
 *
 *  xattr.c: exFAT code for supporting xattr(Extended File Attributes)
 */

#include "config.h"

#ifdef CONFIG_EXFAT_VIRTUAL_XATTR

#include <linux/file.h>
#include <linux/fs.h>
#include <linux/xattr.h>
#include <linux/dcache.h>
#include "exfat.h"

#ifndef CONFIG_EXFAT_VIRTUAL_XATTR_SELINUX_LABEL
#define CONFIG_EXFAT_VIRTUAL_XATTR_SELINUX_LABEL	("undefined")
#endif

static const char default_xattr[] = CONFIG_EXFAT_VIRTUAL_XATTR_SELINUX_LABEL;

static int can_support(const char *name)
{
	if (!name || strcmp(name, "security.selinux"))
		return -1;
	return 0;
}

ssize_t exfat_listxattr(struct dentry *dentry, char *list, size_t size)
{
	return 0;
}

static int exfat_xattr_get(const struct xattr_handler *handler,
		struct dentry *dentry, struct inode *inode,
		const char *name, void *buffer, size_t size)
{
	if (can_support(name))
		return -EOPNOTSUPP;

	if ((size > strlen(default_xattr)+1) && buffer)
		strcpy(buffer, default_xattr);

	return strlen(default_xattr);
}

static int exfat_xattr_set(const struct xattr_handler *handler,
		struct dentry *dentry, struct inode *inode,
		const char *name, const void *value, size_t size,
		int flags)
{
	if (can_support(name))
		return -EOPNOTSUPP;

	return 0;
}

const struct xattr_handler exfat_xattr_handler = {
	.prefix = "",  /* match anything */
	.get = exfat_xattr_get,
	.set = exfat_xattr_set,
};

const struct xattr_handler *exfat_xattr_handlers[] = {
	&exfat_xattr_handler,
	NULL
};

void setup_exfat_xattr_handler(struct super_block *sb)
{
	sb->s_xattr = exfat_xattr_handlers;
}

#endif /* CONFIG_EXFAT_VIRTUAL_XATTR */
