#ifndef MODULE_JUNKTOGOLD_H
#define MODULE_JUNKTOGOLD_H

#include "ScriptMgr.h"
#include "Config.h"
#include "Chat.h"
#include "Player.h"
#include "Log.h"
#include <unordered_set>
#include <string>
#include <sstream>

class JunkToGold : public PlayerScript
{
private:
    bool IsEnabled;
    bool isLogEnabled;

public:
    /**
     * @brief Constructor for the JunkToGold class.
     *
     * This constructor initializes the JunkToGold player script and reads configuration options
     * to determine if the script and logging are enabled. It also parses a list of item class-subclass
     * pairs to be skipped from the configuration.
     *
     * Configuration options:
     * - "JunkToGold.Enable" (bool): Determines if the JunkToGold script is enabled. Default is true.
     * - "JunkToGold.Logging.Enable" (bool): Determines if logging is enabled for the JunkToGold script. Default is true.
     * - "JunkToGold.SkipItems" (string): A comma-separated list of item class-subclass pairs to be skipped.
     *
     * The skipItems set is populated with the parsed class-subclass pairs.
     *
     * @note The configuration options are read using the sConfigMgr singleton.
     */
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
            {
                skipItems.insert({classId, subClassId});
            }
        }

        LOG_DEBUG("junktogold", "Loaded {} skipped item class-subclass pairs.", skipItems.size());
    }

	void OnLootItem(Player* player, Item* item, uint32 count, ObjectGuid lootguid) override;
	void OnQuestRewardItem(Player* player, Item* item, uint32 count) override;

private:
    void Process(Player* player, Item* item, uint32 count);
    void SendTransactionInformation(Player* player, Item* item, uint32 count);

    /**
     * @struct PairHash
     * @brief A hash function object for hashing pairs of uint32 values.
     *
     * This struct provides a custom hash function for std::pair<uint32, uint32>
     * to be used in unordered containers like std::unordered_map or std::unordered_set.
     *
     * @note The hash function combines the hash values of the two elements in the pair
     *       using bitwise XOR and a left shift operation to produce a single hash value.
     */
    struct PairHash {
        std::size_t operator()(const std::pair<uint32, uint32>& p) const noexcept {
            return std::hash<uint32>{}(p.first) ^ (std::hash<uint32>{}(p.second) << 1);
        }
    };

    /**
     * @brief A set of pairs of uint32 values representing items to be skipped.
     *
     * This unordered set stores pairs of uint32 values, where each pair represents
     * an item that should be skipped. The set uses a custom hash function, PairHash,
     * to efficiently manage the pairs.
     */
    std::unordered_set<std::pair<uint32, uint32>, PairHash> skipItems;
};

/**
 * @brief Adds the JunkToGold script to the AzerothCore module.
 *
 * This function creates a new instance of the JunkToGold class and registers it
 * with the AzerothCore module system. It is typically called during the module
 * initialization process to ensure that the JunkToGold script is available for
 * use in the game.
 */
void Addmod_junk_to_goldScripts()
{
	new JunkToGold();
}

#endif //MODULE_JUNKTOGOLD_H
