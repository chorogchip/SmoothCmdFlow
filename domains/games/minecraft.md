
# original commands
```
/advancement (grant|revoke) <targets> <advancement>
/attribute <target> <attribute> (get|base|modifier)
/ban <targets> [<reason>]
/ban-ip <target> [<reason>]
/banlist [ips|players]
/bossbar (add|get|list|remove|set)
/clear [<targets>] [<item>] [<maxCount>]
/clone <begin> <end> <destination> [replace|masked|filtered]
/damage <target> <amount> [<damageType>] [at <location>|by <entity> [from <cause>]]
/data (get|merge|modify|remove)
/datapack (disable|enable|list)
/debug (start|stop|function)
/defaultgamemode (survival|creative|adventure|spectator)
/deop <targets>
/difficulty [peaceful|easy|normal|hard]
/effect (clear|give)
/enchant <targets> <enchantment> [<level>]
/execute ...
/experience (add|query|set)
/fill <from> <to> <block> [destroy|hollow|keep|outline|replace]
/fillbiome <from> <to> <biome> [replace <filter>]
/forceload (add|query|remove)
/function <name>
/gamemode (survival|creative|adventure|spectator) [<target>]
/gamerule <rule> [<value>]
/give <targets> <item> [<count>]
/help [<command>]
/item (modify|replace)
/jfr (start|stop)
/kick <targets> [<reason>]
/kill [<targets>]
/list [uuids]
/locate (biome|poi|structure)
/loot (give|insert|replace|spawn) ...
/me <action>
/msg <targets> <message>
/op <targets>
/pardon <targets>
/pardon-ip <target>
/particle <name> [<pos>] [<delta>] [<speed>] [<count>] [force|normal] [<viewers>]
/perf (start|stop)
/place (feature|jigsaw|structure|template)
/playsound <sound> <source> <targets> [<pos>] [<volume>] [<pitch>] [<minVolume>]
/publish [allowCommands] [gamemode] [port]
/random (roll|value|reset)
/recipe (give|take) <targets> <recipe>
/reload
/return <value>
/ride <target> (mount|dismount)
/save-all [flush]
/save-off
/save-on
/say <message>
/schedule (function|clear)
/scoreboard (objectives|players)
/seed
/setblock <pos> <block> [destroy|keep|replace]
/setidletimeout <minutes>
/setworldspawn [<pos>] [<angle>]
/spawnpoint [<targets>] [<pos>] [<angle>]
/spectate [<target>] [<player>]
/spreadplayers <center> <spreadDistance> <maxRange> [under <maxHeight>] <respectTeams> <targets>
/stop
/stopsound <targets> [<source>] [<sound>]
/summon <entity> [<pos>] [<nbt>]
/tag <targets> (add|list|remove)
/team (add|empty|join|leave|list|modify|remove)
/teammsg <message>
/teleport <destination>
/teleport <targets> <destination>
/teleport <targets> <location> [<rotation>]
/tell <targets> <message>
/tellraw <targets> <message>
/tick (freeze|query|rate|sprint|step|unfreeze)
/time (add|query|set)
/title <targets> (clear|reset|title|subtitle|actionbar|times)
/tm <message>
/tp <destination>
/tp <targets> <destination>
/tp <targets> <location> [<rotation>]
/transfer <hostname> [<port>] [<players>]
/trigger <objective> [add|set] [<value>]
/w <targets> <message>
/weather (clear|rain|thunder) [<duration>]
/whitelist (add|list|off|on|reload|remove)
/worldborder (add|center|damage|get|set|warning)
/xp (add|query|set)
```

# SCF command
```
entities.name.is "playername" | misc.advancement.grant shoot_arrow
entities.name.is "playername" | attrib.speed.set 10
gamestate.network.banned.add "playername"
gamestate.network.banned.players
entities.name.is "playername" | inventory.clear
entities.type.is creeper | state.hp.damage 10
gamestate.rule.gamemode.set survival
gamestate.privilege.op.add "playername"
gamestate.rule.difficulty.set hard
entities.name.is "playername" | state.effect.add night_vision.set_duration 10
entities.name.is "playername" | inventory.hand.enchant hardness.set_level 2
entities.name.is "playername" | inventory.hand.is none | damage 1
entities.name.is "playername" | inventory.add diamond_sword
entities.name.is "playername" | state.experience.add 10

world.blocks.pos.x 50 | y 100 | z 0 | setblock world.blocks.pos.x 13 | y 20 | z 50 | pos.to.x 17 | y 30 | z 65 | mask.not air
world.blocks.pos.x 50 | y 100 | z 0 | setblock sand




/item (modify|replace)
/jfr (start|stop)
/kick <targets> [<reason>]
/kill [<targets>]
/list [uuids]
/locate (biome|poi|structure)
/loot (give|insert|replace|spawn) ...
/me <action>
/msg <targets> <message>
/op <targets>
/pardon <targets>
/pardon-ip <target>
/particle <name> [<pos>] [<delta>] [<speed>] [<count>] [force|normal] [<viewers>]
/perf (start|stop)
/place (feature|jigsaw|structure|template)
/playsound <sound> <source> <targets> [<pos>] [<volume>] [<pitch>] [<minVolume>]
/publish [allowCommands] [gamemode] [port]
/random (roll|value|reset)
/recipe (give|take) <targets> <recipe>
/reload
/return <value>
/ride <target> (mount|dismount)
/save-all [flush]
/save-off
/save-on
/say <message>
/schedule (function|clear)
/scoreboard (objectives|players)
/seed
/setblock <pos> <block> [destroy|keep|replace]
/setidletimeout <minutes>
/setworldspawn [<pos>] [<angle>]
/spawnpoint [<targets>] [<pos>] [<angle>]
/spectate [<target>] [<player>]
/spreadplayers <center> <spreadDistance> <maxRange> [under <maxHeight>] <respectTeams> <targets>
/stop
/stopsound <targets> [<source>] [<sound>]
/summon <entity> [<pos>] [<nbt>]
/tag <targets> (add|list|remove)
/team (add|empty|join|leave|list|modify|remove)
/teammsg <message>
/teleport <destination>
/teleport <targets> <destination>
/teleport <targets> <location> [<rotation>]
/tell <targets> <message>
/tellraw <targets> <message>
/tick (freeze|query|rate|sprint|step|unfreeze)
/time (add|query|set)
/title <targets> (clear|reset|title|subtitle|actionbar|times)
/tm <message>
/tp <destination>
/tp <targets> <destination>
/tp <targets> <location> [<rotation>]
/transfer <hostname> [<port>] [<players>]
/trigger <objective> [add|set] [<value>]
/w <targets> <message>
/weather (clear|rain|thunder) [<duration>]
/whitelist (add|list|off|on|reload|remove)
/worldborder (add|center|damage|get|set|warning)
/xp (add|query|set)
```