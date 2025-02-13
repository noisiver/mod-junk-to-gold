#ifndef MODULE_JUNKTOGOLD_H
#define MODULE_JUNKTOGOLD_H

#include "ScriptMgr.h"
#include "Config.h"
#include "Chat.h"
#include "Player.h"
#include "Log.h"
#include "QuestDef.h"
#include <unordered_set>
#include <string>
#include <sstream>

class JunkToGold : public PlayerScript
{
private:
    bool IsEnabled;
    bool isLogEnabled;

public:
	JunkToGold() : PlayerScript("JunkToGold")
    {
        IsEnabled = sConfigMgr->GetOption<bool>("JunkToGold.Enable", true);
        isLogEnabled = sConfigMgr->GetOption<bool>("JunkToGold.Logging.Enable", true);

        std::string SkipList = sConfigMgr->GetOption<std::string>("JunkToGold.SkipItems", "");

        std::stringstream ss(SkipList);
        std::string token;

        while (std::getline(ss, token, ','))
        {
            uint32 classId, subClassId;

            if (sscanf(token.c_str(), "%u:%u", &classId, &subClassId) == 2)
                skipItems.insert({classId, subClassId});
        }

        std::string SkipItemIDList = sConfigMgr->GetOption<std::string>("JunkToGold.SkipItemsID", "");
        std::stringstream ssil(SkipItemIDList);

        while(std::getline(ssil, token, ','))
        {
            uint32 itemID;

            if(sscanf(token.c_str(), "%u", &itemID) == 1)
                skipItemsID.insert(itemID);
        }

        LOG_DEBUG("junktogold", "Loaded {} skipped item class-subclass pairs.", skipItems.size());
        LOG_DEBUG("junktogold", "Loaded {} skipped item IDs.", skipItemsID.size());
    }

	void OnLootItem(Player* player, Item* item, uint32 count, ObjectGuid lootguid) override;
	void OnQuestRewardItem(Player* player, Item* item, uint32 count) override;

private:
    bool IsQuestItem(Player* player, Item* item);
    void Process(Player* player, Item* item, uint32 count);
    void SendTransactionInformation(Player* player, Item* item, uint32 count);

    struct PairHash {
        std::size_t operator()(const std::pair<uint32, uint32>& p) const noexcept {
            return std::hash<uint32>{}(p.first) ^ (std::hash<uint32>{}(p.second) << 1);
        }
    };

    std::unordered_set<std::pair<uint32, uint32>, PairHash> skipItems;
    std::unordered_set<uint32> skipItemsID;
};

void Addmod_junk_to_goldScripts()
{
	new JunkToGold();
}

#endif //MODULE_JUNKTOGOLD_H
