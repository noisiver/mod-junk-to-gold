#include "Chat.h"
#include "Player.h"
#include "ScriptMgr.h"

class JunkToGold : public PlayerScript
{
public:
    JunkToGold() : PlayerScript("JunkToGold") {}

    void OnLootItem(Player* player, Item* item, uint32 count, ObjectGuid /*lootguid*/) override
    {
        if (item->GetTemplate()->Quality == ITEM_QUALITY_POOR)
        {
            SendTransactionInformation(player, item, count);
            player->ModifyMoney(item->GetTemplate()->SellPrice * count);
            player->DestroyItem(item->GetBagSlot(), item->GetSlot(), true);
        }
    }

private:
    void SendTransactionInformation(Player* player, Item* item, uint32 count)
    {
        std::string name;
        if (count > 1)
        {
            name = Acore::StringFormat("|cff9d9d9d|Hitem:%i::::::::80:::::|h[%s]|h|rx%i", item->GetTemplate()->ItemId, item->GetTemplate()->Name1, count);
        }
        else
        {
            name = Acore::StringFormat("|cff9d9d9d|Hitem:%i::::::::80:::::|h[%s]|h|r", item->GetTemplate()->ItemId, item->GetTemplate()->Name1);
        }

        uint32 money = item->GetTemplate()->SellPrice * count;
        uint32 gold = money / GOLD;
        uint32 silver = (money % GOLD) / SILVER;
        uint32 copper = (money % GOLD) % SILVER;

        std::string info;
        if (money < SILVER)
        {
            info = Acore::StringFormat("%s sold for %i copper.", name, copper);
        }
        else if (money < GOLD)
        {
            if (copper > 0)
            {
                info = Acore::StringFormat("%s sold for %i silver and %i copper.", name, silver, copper);
            }
            else
            {
                info = Acore::StringFormat("%s sold for %i silver.", name, silver);
            }
        }
        else
        {
            if (copper > 0 && silver > 0)
            {
                info = Acore::StringFormat("%s sold for %i gold, %i silver and %i copper.", name, gold, silver, copper);
            }
            else if (copper > 0)
            {
                info = Acore::StringFormat("%s sold for %i gold and %i copper.", name, gold, copper);
            }
            else if (silver > 0)
            {
                info = Acore::StringFormat("%s sold for %i gold and %i silver.", name, gold, silver);
            }
            else
            {
                info = Acore::StringFormat("%s sold for %i gold.", name, gold);
            }
        }

        ChatHandler(player->GetSession()).SendSysMessage(info);
    }
};

void Addmod_junk_to_goldScripts()
{
    new JunkToGold();
}
