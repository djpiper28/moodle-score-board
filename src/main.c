#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <orca/discord.h>
#include <sqlite3.h>
#include <pthread.h>

#define DB_FILE "moodle.db"
#define SQL_CREATE_TABLES_F "sql/create_tables.sql"

pthread_mutex_t db_mutex = PTHREAD_MUTEX_INITIALIZER;
sqlite3 *db = NULL;

void on_ready(struct discord *client)
{
    const struct discord_user *bot = discord_get_self(client);

    log_info("PingPong-Bot succesfully connected to Discord as %s#%s!",
             bot->username, bot->discriminator);
}

void on_info(struct discord *client, const struct discord_message *msg)
{
    char buffer[1024];
    if (msg->author->bot) return; // ignore bots

    snprintf(buffer,
             sizeof(buffer) / sizeof(* buffer),
             "moodle-score-board Version: %s built for %s.\nGithub: %s",
             VERSION,
             OS,
             REPO_URL);

    struct discord_create_message_params params = { .content = buffer };
    discord_create_message(client, msg->channel_id, &params, NULL);
}

int callback_create_db(void *NotUsed, int argc, char **argv, char **azColName)
{
    return 0;
}

int create_tables()
{
    FILE *f = fopen(SQL_CREATE_TABLES_F, "r");
    if (f == NULL) {
        log_error("Unable to read %s", SQL_CREATE_TABLES_F);
        return 0;
    }

    char buffer[8096], tmp_buffer[1024];
    int status = 1;

    memset(buffer, 0, sizeof(buffer));
    while (status) {
        status = fgets(tmp_buffer, sizeof(tmp_buffer), f) != NULL;
        strcat(buffer, tmp_buffer);
    }

    fclose(f);

    /* Execute SQL statement */
    char *zErrMsg = NULL;
    status = sqlite3_exec(db, buffer, callback_create_db, 0, &zErrMsg);

    if (status != SQLITE_OK) {
        log_error("SQL error: %s", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        log_info("Tables created successfully");
    }

    return status;
}

int main(void)
{
    FILE *f = fopen(DB_FILE, "rb");
    int exists = f != NULL;
    if (exists) {
        fclose(f);
    }

    int rc = sqlite3_open(DB_FILE, &db);
    if (rc) {
        log_error("Can't open database: %s", sqlite3_errmsg(db));
        return 1;
    } else {
        log_info("Opened database successfully");
    }

    if (!exists) {
        log_info("Creating tables for the database");
        create_tables();
    }

    struct discord *client = discord_config_init("./config.json");

    discord_set_on_ready(client, &on_ready);
    discord_set_on_command(client, "info", &on_info);

    discord_run(client);

    discord_cleanup(client);
    sqlite3_close(db);

    return 0;
}

