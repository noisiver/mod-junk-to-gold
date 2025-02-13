#include "junk_to_gold.h"

void JunkToGold::OnLootItem(Player* player, Item* item, uint32 count, ObjectGuid /*lootguid*/)
{
    if(IsEnabled)
    {
        if (!item || !item->GetTemplate())
            return;

        if(isLogEnabled)
            LOG_INFO("junktogold", "Player {} Looted Item: {} (Count: {})", player->GetName(), item->GetTemplate()->Name1, count);

        if(!IsQuestItem(player, item))
            Process(player, item, count);
        else
            LOG_INFO("junktogold", "Player {} looted Quest Item: {} (Count: {})", player->GetName(), item->GetTemplate()->Name1, count);
    }
}

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

bool JunkToGold::IsQuestItem(Player* player, Item* item)
{
    if(!item || !item->GetTemplate())
        return false;

    uint32 itemId = item->GetTemplate()->ItemId;
    QuestStatusMap& quests = player->GetQuestStatusMap();

    for(const auto& QUEST_ENTRY : quests)
    {
        uint32 questId = QUESTS_ENTRY.first;
        const Quest* quest = sObjectMgr->GetQuestTemplate(questId);

        if(!quest) continue;

        for(uint8 i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
        {
            if(quest->RequiredItemId[i] == itemId)
                return true;
        }
    }

    return false;
}

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
    uint32 itemId = item->GetTemplate()->ItemId;

    if (quality < QUALITY_ARRAY.size() && sConfigMgr->GetOption<bool>(QUALITY_ARRAY[quality], true))
    {
        if (skipItemsID.find(itemId) != skipItemsID.end())
        {
            LOG_INFO("junktogold", "Skipping Item: {} | ID: {} | Class: {} | SubClass: {}", item->GetTemplate()->Name1, item->GetTemplate()->ItemId, iclass, isubclass);
            return;
        }

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
