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
#include <string.h>

#include <sqlite3.h>

#include "rbac.h"

#define CREATE_USERS_TABLE sql \
      "CREATE TABLE USERS (" \
      "id INT PRIMARY KEY AUTOINCREMENT NOT NULL," \
      "name TEXT);";

#define CREATE_ROLES_TABLE sql \
      "CREATE TABLE ROLES (" \
      "id INT PRIMARY KEY AUTOINCREMENT NOT NULL," \
      "name TEXT);";

#define CREATE_PERMISSIONS_TABLE sql \
      "CREATE TABLE ROLES (" \
      "id INT PRIMARY KEY AUTOINCREMENT NOT NULL," \
      "name TEXT);";

static sqlite3 *store;

int
rbac_init_store(const char *name, char *err_msg)
{
    int rc = sqlite3_open(name, &store);
    if (rc != SQLITE_OK) {
        const char *msg = sqlite3_errmsg(store);
        err_msg = calloc(strlen(msg)+1, sizeof(char));
        strcpy(err_msg, msg);
        sqlite3_close(store);
        return 1;
    }

    return 0;
}

void
rbac_stop()
{
    sqlite3_close(store);
}

void
rbac_role_free(rbac_role_t* role)
{
    if (role != NULL) {
        if (role->name != NULL) {
            free(role->name);
        }

        free(role);
    }
}

rbac_role_t*
rbac_new_role(const char *name)
{
    if (name == NULL || name[0] == '\0') {
        return NULL;
    }

    rbac_role_t *role = calloc(1, sizeof(rbac_role_t));
    role->name = calloc(strlen(name)+1, sizeof(char));
    strcpy(role->name, name);

    return role;
}
