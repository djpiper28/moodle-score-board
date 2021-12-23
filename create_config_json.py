token = input("what is your bot token: ")
command_prefix = "!"

f = open("config.json", "r")
data = f.read()
f.close()

f = open("config.json", "w")
f.write(data.replace("YOUR-BOT-TOKEN", token).replace("YOUR-COMMANDS-PREFIX", command_prefix).replace("\"enable\": false", "\"enable\": true"))
f.close

