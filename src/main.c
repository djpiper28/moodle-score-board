#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <orca/discord.h>

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
  					"moodle-score-board version %s build for %s.\n%s",
  					VERSION,
  					OS,
  					REPO_URL);

  struct discord_create_message_params params = { .content = buffer };
  discord_create_message(client, msg->channel_id, &params, NULL);
}

int main(void)
{
  struct discord *client = discord_config_init("./config.json");

  discord_set_on_ready(client, &on_ready);
  discord_set_on_command(client, "info", &on_info);

  discord_run(client);

  discord_cleanup(client);

  return 0;
}

