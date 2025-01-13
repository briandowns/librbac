/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2025 Brian J. Downs
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>

#include "rbac.h"

static const char *roles[3] = {
    "user",
    "admin"
};

static const char *users[5] = {
    "bdowns",
    "jmoore",
    "jdowns",
    "mdowns",
    "dlafrance"
};

static const char *permissions[3] = {
    "read",
    "write",
    "exec"
};

int
seed_roles()
{
    char *err_msg = {0};
    for (int i = 0; i < 3; i++) {
        int ret = rbac_add_role(roles[i], err_msg);
        if (ret != 0) {
            fprintf(stderr, "%s\n", err_msg);
            free(err_msg);
            return 1;
        }
    }

    return 0;
}

int
seed_users()
{
    char *err_msg = {0};
    for (int i = 0; i < 3; i++) {
        int ret = rbac_add_user(users[i], err_msg);
        if (ret != 0) {
            fprintf(stderr, "%s\n", err_msg);
            free(err_msg);
            return 1;
        }
    }

    return 0;
}

int
seed_permissions()
{
    char *err_msg = {0};
    for (int i = 0; i < 3; i++) {
        int ret = rbac_add_permission(permissions[i], err_msg);
        if (ret != 0) {
            fprintf(stderr, "%s\n", err_msg);
            free(err_msg);
            return 1;
        }
    }

    return 0;
}

int
seed_role_permissions()
{
    char *err_msg = {0};
    int ret = rbac_add_permission_to_role("admin", "read", err_msg);
    if (ret != 0) {
        fprintf(stderr, "%s\n", err_msg);
        free(err_msg);
        return 1;
    }

    int ret = rbac_add_permission_to_role("admin", "write", err_msg);
    if (ret != 0) {
        fprintf(stderr, "%s\n", err_msg);
        free(err_msg);
        return 1;
    }

    int ret = rbac_add_permission_to_role("admin", "exec", err_msg);
    if (ret != 0) {
        fprintf(stderr, "%s\n", err_msg);
        free(err_msg);
        return 1;
    }

    return 0;
}

int
main(int argc, char **argv)
{
    char *err_msg = {0};
    int ret = rbac_init_store("rbac.db", err_msg);
    if (ret != 0) {
        fprintf(stderr, "%s\n", err_msg);
        free(err_msg);
        return 1;
    }

    if (seed_roles() != 0) {
        return 1;
    }

    if (seed_users() != 0) {
        return 1;
    }

    if (seed_permissions() != 0) {
        return 1;
    }

    if (seed_role_permissions() != 0) {
        return 1;
    }

    rbac_cleanup();

    return 0;
}
