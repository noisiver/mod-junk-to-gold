#ifndef MODULE_JUNKTOGOLD_H
#define MODULE_JUNKTOGOLD_H

#include "ScriptMgr.h"
#include "Config.h"
#include "Chat.h"
#include "Player.h"
#include "Errors.h"
#include "Log.h"

class JunkToGold : public PlayerScript
{
public:
	JunkToGold() : PlayerScript("JunkToGold")
    {
        IsEnabled = sConfigMgr->GetOption<bool>("JunkToGold.Enable", true);
        isLogEnabled = sConfigMgr->GetOption<bool>("JunkToGold.Logging.Enable", true);
    }

    bool IsEnabled;
    bool isLogEnabled;

	void OnLootItem(Player* player, Item* item, uint32 count, ObjectGuid lootguid) override;
	void OnQuestRewardItem(Player* player, Item* item, uint32 count) override;

private:
    void Process(Player* player, Item* item, uint32 count);
    void SendTransactionInformation(Player* player, Item* item, uint32 count);
};

void Addmod_junk_to_goldScripts()
{
	new JunkToGold();
}

#endif //MODULE_JUNKTOGOLD_H
