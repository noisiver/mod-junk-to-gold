#include "junk_to_gold.h"

/**
 * @brief Handles the event when a player loots an item.
 *
 * This function is called whenever a player loots an item. If the module is enabled,
 * it processes the looted item and logs the event if logging is enabled.
 *
 * @param player Pointer to the player who looted the item.
 * @param item Pointer to the item that was looted.
 * @param count The number of items looted.
 * @param lootguid The GUID of the loot (unused).
 */
void JunkToGold::OnLootItem(Player* player, Item* item, uint32 count, ObjectGuid /*lootguid*/)
{
    if(IsEnabled)
    {
        if (!item || !item->GetTemplate())
            return;

        if(isLogEnabled)
            LOG_INFO("junktogold", "Player {} Looted Item: {} (Count: {})", player->GetName(), item->GetTemplate()->Name1, count);

        Process(player, item, count);
    }
}

/**
 * @brief Handles the event when a player receives a quest reward item.
 *
 * This function is called when a player receives an item as a reward for completing a quest.
 * It checks if the module is enabled and if the item and its template are valid.
 * If logging is enabled, it logs the event with the player's name, item name, and count.
 * Finally, it processes the item for the player.
 *
 * @param player Pointer to the Player object who received the quest reward.
 * @param item Pointer to the Item object that was received as a quest reward.
 * @param count The number of items received as a quest reward.
 */
void JunkToGold::OnQuestRewardItem(Player* player, Item* item, uint32 count)
{
    if(IsEnabled)
    {
        if (!item || !item->GetTemplate())
            return;

        if(isLogEnabled)
            LOG_INFO("junktogold", "Player {} Recieved Quest Reward: {} (Count: {})", player->GetName(), item->GetTemplate()->Name1, count);

        Process(player, item, count);
    }
}

/**
 * @brief Processes the given item for the player, converting it to gold if it meets certain criteria.
 *
 * This function checks the quality of the item and verifies if it should be converted to gold based on configuration settings.
 * If the item is not in the skip list, it will send transaction information to the player, modify the player's money by the item's sell price,
 * and destroy the item from the player's inventory.
 *
 * @param player Pointer to the Player object who owns the item.
 * @param item Pointer to the Item object to be processed.
 * @param count The number of items to be processed.
 */
void JunkToGold::Process(Player* player, Item* item, uint32 count)
{
    static const std::array<std::string, 8> QUALITY_ARRAY = {
        "JunkToGold.Quality.0", "JunkToGold.Quality.1", "JunkToGold.Quality.2",
        "JunkToGold.Quality.3", "JunkToGold.Quality.4", "JunkToGold.Quality.5",
        "JunkToGold.Quality.6", "JunkToGold.Quality.7"
    };

    uint32 quality = item->GetTemplate()->Quality;
    uint32 iclass = item->GetTemplate()->Class;
    uint32 isubclass = item->GetTemplate()->SubClass;

    if (quality < QUALITY_ARRAY.size() && sConfigMgr->GetOption<bool>(QUALITY_ARRAY[quality], true))
    {
        if(!skipItems.contains({iclass, isubclass}))
        {
            SendTransactionInformation(player, item, count);
            player->ModifyMoney(item->GetTemplate()->SellPrice * count);
            player->DestroyItem(item->GetBagSlot(), item->GetSlot(), true);
        }
        else
            LOG_INFO("junktogold", "Skipping Item: {} | ID: {} | Class: {} | SubClass: {}", item->GetTemplate()->Name1, item->GetTemplate()->ItemId, iclass, isubclass);
    }
}

/**
 * @brief Sends transaction information to the player and logs the transaction if logging is enabled.
 *
 * This function formats and sends a message to the player about the sale of an item, including the amount of gold, silver, and copper received.
 * It also logs the transaction details if logging is enabled.
 *
 * @param player Pointer to the Player object representing the player who sold the item.
 * @param item Pointer to the Item object representing the item that was sold.
 * @param count The number of items sold.
 */
void JunkToGold::SendTransactionInformation(Player* player, Item* item, uint32 count)
{
    std::string name;
    std::string NID;

    if (count > 1)
    {
        name = Acore::StringFormat("|cff9d9d9d|Hitem:{}::::::::80:::::|h[{}]|h|rx{}", item->GetTemplate()->ItemId, item->GetTemplate()->Name1, count);
        NID = Acore::StringFormat("[Item Name: {} | Item ID: {} | Count: {}]", item->GetTemplate()->Name1, item->GetTemplate()->ItemId, count);
    }
    else
    {
        name = Acore::StringFormat("|cff9d9d9d|Hitem:{}::::::::80:::::|h[{}]|h|r", item->GetTemplate()->ItemId, item->GetTemplate()->Name1);
        NID = Acore::StringFormat("[Item Name: {} | Item ID: {}]", item->GetTemplate()->Name1, item->GetTemplate()->ItemId);
    }

    uint32 money = item->GetTemplate()->SellPrice * count;
    uint32 gold = money / GOLD;
    uint32 silver = (money % GOLD) / SILVER;
    uint32 copper = (money % GOLD) % SILVER;

    std::string info;
    std::string loginfo;

    if (money < SILVER)
    {
        info = Acore::StringFormat("{} sold for {} copper.", name, copper);
        loginfo = Acore::StringFormat("Player {} has Sold Item {} for {} copper.", player->GetName(), NID, copper);
    }
    else if (money < GOLD)
    {
        if (copper > 0)
        {
            info = Acore::StringFormat("{} sold for {} silver and {} copper.", name, silver, copper);
            loginfo = Acore::StringFormat("Player {} has Sold Item {} for {} silver and {} copper.", player->GetName(), NID, silver, copper);
        }
        else
        {
            info = Acore::StringFormat("{} sold for {} silver.", name, silver);
            loginfo = Acore::StringFormat("Player {} has Sold Item {} for {} silver.", player->GetName(), NID, silver);
        }
    }
    else
    {
        if (copper > 0 && silver > 0)
        {
            info = Acore::StringFormat("{} sold for {} gold, {} silver and {} copper.", name, gold, silver, copper);
            loginfo = Acore::StringFormat("Player {} has Sold Item {} for {} gold, {} silver and {} copper.", player->GetName(), NID, gold, silver, copper);
        }
        else if (copper > 0)
        {
            info = Acore::StringFormat("{} sold for {} gold and {} copper.", name, gold, copper);
            loginfo = Acore::StringFormat("Player {} has Sold Item {} for {} gold and {} copper.", player->GetName(), NID, gold, copper);
        }
        else if (silver > 0)
        {
            info = Acore::StringFormat("{} sold for {} gold and {} silver.", name, gold, silver);
            loginfo = Acore::StringFormat("Player {} has Sold Item {} for {} gold and {} silver.", player->GetName(), NID, gold, silver);
        }
        else
        {
            info = Acore::StringFormat("{} sold for {} gold.", name, gold);
            loginfo = Acore::StringFormat("Player {} has Sold Item {} for {} gold.", player->GetName(), NID, gold);
        }
    }

    ChatHandler(player->GetSession()).SendSysMessage(info);

    if(isLogEnabled)
    {
        LOG_INFO("junktogold", "{}", loginfo);
    }
}
