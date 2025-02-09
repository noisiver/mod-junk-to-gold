#ifndef MODULE_JUNKTOGOLD_H
#define MODULE_JUNKTOGOLD_H

#include "ScriptMgr.h"
#include "Config.h"
#include "Chat.h"
#include "Player.h"

class JunkToGold : public PlayerScript
{
public:
	JunkToGold() : PlayerScript("JunkToGold") {}
	
	void OnLootItem(Player* player, Item* item, uint32 count, ObjectGuid lootguid) override;
    void SendTransactionInformation(Player* player, Item* item, uint32 count);
};

void Addmod_junk_to_goldScripts()
{
	new JunkToGold();
}

#endif //MODULE_JUNKTOGOLD_H