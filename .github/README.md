# ![logo](https://raw.githubusercontent.com/azerothcore/azerothcore.github.io/master/images/logo-github.png) AzerothCore

# Junk to Gold
This module will automatically sell gray items when the player loots them

```
[worldserver]

########################################
# Junk To Gold configuration
########################################
#
#    JunkToGold.Enable
#        Description: (Default: 1) Enable or Disable Module
#        Default:     0 - Disabled
#                     1 - Enabled
#

JunkToGold.Enable = 1

########################################
#
#    JunkToGold.Quality.X
#        Description: Enable or Disable converting specific item qualities to gold.
#        Default:     0 - Disabled
#                     1 - Enabled
#
#   JunkToGold.Quality.0 ITEM_QUALITY_POOR (Gray items)
#   JunkToGold.Quality.1 ITEM_QUALITY_NORMAL (White items)
#   JunkToGold.Quality.2 ITEM_QUALITY_UNCOMMON (Green items)
#   JunkToGold.Quality.3 ITEM_QUALITY_RARE (Blue items)
#   JunkToGold.Quality.4 ITEM_QUALITY_EPIC (Purple items)
#   JunkToGold.Quality.5 ITEM_QUALITY_LEGENDARY (Orange items)
#   JunkToGold.Quality.6 ITEM_QUALITY_ARTIFACT (Golden items)
#   JunkToGold.Quality.7 ITEM_QUALITY_HEIRLOOM (Light yellow items)

JunkToGold.Quality.0 = 1
JunkToGold.Quality.1 = 0
JunkToGold.Quality.2 = 0
JunkToGold.Quality.3 = 0
JunkToGold.Quality.4 = 0
JunkToGold.Quality.5 = 0
JunkToGold.Quality.6 = 0
JunkToGold.Quality.7 = 0

########################################
#
#    JunkToGold.SkipItems
#        Description: Define Which (class, subclass) pairs should be skipped
#        Format: "class:subclass, class:subclass, ...."
#        Refer to this link for class and subclass ID https://www.azerothcore.org/wiki/item_template
#

JunkToGold.SkipItems = "12:0, 0:1, 0:2, 0:3, 0:4, 0:5, 0:6, 0:7, 0:8, 1:0, 1:2, 1:3, 1:4, 1:5, 1:6, 1:7, 1:8, 2:14"

########################################
#
#    JunkToGold.Logging.Enable
#        Description: (Default: 1) Enable Logging
#        Default:     0 - Disabled
#                     1 - Enabled
#

JunkToGold.Logging.Enable = 1
```

also add this to your `worldserver.conf` file you can find the other Appender configuration in line `590`
```
Appender.JunkToGold=2,5,0,JunkToGold.log,w
Logger.junktogold=5,Console JunkToGold
```

## Credits

- noisiver [Author of the idea and part of the initial code.](https://github.com/noisiver/mod-junk-to-gold)
