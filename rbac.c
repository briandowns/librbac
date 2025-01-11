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

#define CREATE_USERS_TABLE_SQL \
    "CREATE TABLE IF NOT EXISTS users (" \
    "id INTEGER PRIMARY KEY AUTOINCREMENT," \
    "name TEXT UNIQUE NOT NULL);"

#define CREATE_ROLES_TABLE_SQL \
    "CREATE TABLE IF NOT EXISTS roles (" \
    "id INTEGER PRIMARY KEY AUTOINCREMENT," \
    "name TEXT UNIQUE NOT NULL);"

#define CREATE_PERMISSIONS_TABLE_SQL \
    "CREATE TABLE IF NOT EXISTS permissions (" \
    "id INTEGER PRIMARY KEY AUTOINCREMENT," \
    "name TEXT UNIQUE NOT NULL);"

#define CREATE_ROLE_PERMISSIONS_TABLE_SQL \
    "CREATE TABLE IF NOT EXISTS role_permissions (" \
    "role_id INTEGER NOT NULL," \
    "permission_id INTEGER NOT NULL," \
    "FOREIGN KEY (role_id) REFERENCES roles(id)," \
    "FOREIGN KEY (permission_id) REFERENCES permissions(id)," \
    "PRIMARY KEY (role_id, permission_id));"

#define CREATE_USER_ROLES_TABLE_SQL \
    "CREATE TABLE IF NOT EXISTS user_roles (" \
    "user_id INTEGER NOT NULL," \
    "role_id INTEGER NOT NULL," \
    "FOREIGN KEY (user_id) REFERENCES users(id)," \
    "FOREIGN KEY (role_id) REFERENCES roles(id)," \
    "PRIMARY KEY (user_id, role_id));"

static sqlite3 *store;

static int
create_tables(char *err_msg)
{
    int rc = 0;
    rc = sqlite3_exec(store, CREATE_USERS_TABLE_SQL, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        const char *msg = sqlite3_errmsg(store);
        err_msg = calloc(strlen(msg)+1, sizeof(char));
        strcpy(err_msg, msg);
        return 1;
    }

    rc = sqlite3_exec(store, CREATE_ROLES_TABLE_SQL, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        const char *msg = sqlite3_errmsg(store);
        err_msg = calloc(strlen(msg)+1, sizeof(char));
        strcpy(err_msg, msg);
        return 1;
    }

    rc = sqlite3_exec(store, CREATE_PERMISSIONS_TABLE_SQL, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        const char *msg = sqlite3_errmsg(store);
        err_msg = calloc(strlen(msg)+1, sizeof(char));
        strcpy(err_msg, msg);
        return 1;
    }

    rc = sqlite3_exec(store, CREATE_ROLE_PERMISSIONS_TABLE_SQL, 0, 0,
                          &err_msg);
    if (rc != SQLITE_OK) {
        const char *msg = sqlite3_errmsg(store);
        err_msg = calloc(strlen(msg)+1, sizeof(char));
        strcpy(err_msg, msg);
        return 1;
    }

    rc = sqlite3_exec(store, CREATE_USER_ROLES_TABLE_SQL, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        const char *msg = sqlite3_errmsg(store);
        err_msg = calloc(strlen(msg)+1, sizeof(char));
        strcpy(err_msg, msg);
        return 1;
    }

    return 0;
}

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

    return create_tables(err_msg);
}

void
rbac_cleanup()
{
    sqlite3_close(store);
}

int
rbac_add_user(const char *name, char *err_msg)
{
    // create error message and copy into err_msg
    if (name == NULL || name[0] == '\0') {
        return 1;
    }
    const char *query = "INSERT INTO users (name) VALUES ('bdowns')";
    int rc = sqlite3_exec(store, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        const char *msg = sqlite3_errmsg(store);
        err_msg = calloc(strlen(msg)+1, sizeof(char));
        strcpy(err_msg, msg);
        return 1;
    }

    return 0;
}

int
rbac_add_role(const char *name, char *err_msg)
{
    if (name == NULL || name[0] == '\0') {
        return 1;
    }

    size_t query_len = strlen(name)+35;
    char *query = calloc(query_len, sizeof(char));
    sprintf(query, "INSERT INTO roles (name) VALUES (\'%s\')", name);

    int rc = sqlite3_exec(store, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        const char *msg = sqlite3_errmsg(store);
        err_msg = calloc(strlen(msg)+1, sizeof(char));
        strcpy(err_msg, msg);
        return 1;
    }

    return 0;
}

int
rbac_add_permission(const char *name, char *err_msg)
{
    if (name == NULL || name[0] == '\0') {
        return 1;
    }

    size_t query_len = strlen(name)+42;
    char *query = calloc(query_len, sizeof(char));
    sprintf(query, "INSERT INTO permissions (name) VALUES (\'%s\')", name);

    int rc = sqlite3_exec(store, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        const char *msg = sqlite3_errmsg(store);
        err_msg = calloc(strlen(msg)+1, sizeof(char));
        strcpy(err_msg, msg);
        return 1;
    }

    return 0;
}

int
rbac_add_user_to_role(const char *name, const char *role, char *err_msg)
{
    if (name == NULL || name[0] == '\0') {
        return 1;
    }
    if (role == NULL || role[0] == '\0') {
        return 1;
    }

    size_t query_len = strlen(name)+42;
    char *query = calloc(query_len, sizeof(char));
    sprintf(query, "INSERT INTO permissions (name) VALUES (\'%s\')", name);

    int rc = sqlite3_exec(store, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        const char *msg = sqlite3_errmsg(store);
        err_msg = calloc(strlen(msg)+1, sizeof(char));
        strcpy(err_msg, msg);
        return 1;
    }

    return 0;
}