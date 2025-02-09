#ifndef MODULE_JUNKTOGOLD_H
#define MODULE_JUNKTOGOLD_H

#include "Chat.h"
#include "Player.h"
#include "ScriptMgr.h"

class JunkToGold : public PlayerScript
{
public:
	JunkToGold() : PlayerScript("JunkToGold") {}
	void OnLootItem(Player* player, Item* item, uint32 count, ObjectGuid lootguid) override;
	
private:
    void SendTransactionInformation(Player* player, Item* item, uint32 count);
};

void AddSC_JunkToGold()
{
	new JunkToGold();
}

#endif //MODULE_JUNKTOGOLD_H