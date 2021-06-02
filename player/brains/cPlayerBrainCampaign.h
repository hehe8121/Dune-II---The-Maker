#ifndef D2TM_CPLAYERBRAINCAMPAIGN_H
#define D2TM_CPLAYERBRAINCAMPAIGN_H

#include "player/brains/cPlayerBrain.h"
#include "player/brains/cPlayerBrainData.h"
#include "player/brains/missions/cPlayerBrainMission.h"
#include <set>

namespace brains {

/**
 * The cPlayerBrainCampaign is responsible for being an AI brain during campaign missions. This is different
 * compared to the Skirmish brain.
 */
    class cPlayerBrainCampaign : public cPlayerBrain {

    public:
        cPlayerBrainCampaign(cPlayer *player);

        ~cPlayerBrainCampaign();

        void think() override;

        void onNotify(const s_GameEvent &event) override;

        void addBuildOrder(S_buildOrder order);

    private:
        ePlayerBrainState state;

        ePlayerBrainCampaignThinkState thinkState;

        int TIMER_rest;

        // at which cells did we detect an enemy? Remember those.
        std::set<int> discoveredEnemyAtCell;

        std::vector<cPlayerBrainMission> missions;
        std::vector<S_structurePosition> myBase;
        std::vector<S_buildOrder> buildOrders;

        void onMyStructureDestroyed(const s_GameEvent &event);

        void onMyStructureCreated(const s_GameEvent &event);

        void onMyStructureAttacked(const s_GameEvent &event);

        void onMyStructureDecayed(const s_GameEvent &event);

        void thinkState_ScanBase();

        void thinkState_Missions();

        void thinkState_Evaluate();

        void thinkState_EndGame();

        void thinkState_ProcessBuildOrders();

        void changeThinkStateTo(const ePlayerBrainCampaignThinkState& newState);

        static const char* ePlayerBrainCampaignThinkStateString(const ePlayerBrainCampaignThinkState &state) {
            switch (state) {
                case ePlayerBrainCampaignThinkState::PLAYERBRAIN_SCENARIO_STATE_EVALUATE: return "PLAYERBRAIN_SCENARIO_STATE_EVALUATE";
                case ePlayerBrainCampaignThinkState::PLAYERBRAIN_SCENARIO_STATE_PROCESS_BUILDORDERS: return "PLAYERBRAIN_SCENARIO_STATE_PROCESS_BUILDORDERS";
                case ePlayerBrainCampaignThinkState::PLAYERBRAIN_SCENARIO_STATE_SCAN_BASE: return "PLAYERBRAIN_SCENARIO_STATE_SCAN_BASE";
                case ePlayerBrainCampaignThinkState::PLAYERBRAIN_SCENARIO_STATE_ENDGAME: return "PLAYERBRAIN_SCENARIO_STATE_ENDGAME";
                case ePlayerBrainCampaignThinkState::PLAYERBRAIN_SCENARIO_STATE_MISSIONS: return "PLAYERBRAIN_SCENARIO_STATE_MISSIONS";
                case ePlayerBrainCampaignThinkState::PLAYERBRAIN_SCENARIO_STATE_REST: return "PLAYERBRAIN_SCENARIO_STATE_REST";
                default:
                    assert(false);
                    break;
            }
            return "";
        }

        static const char* ePlayerBrainStateString(const ePlayerBrainState &state) {
            switch (state) {
                case ePlayerBrainState::PLAYERBRAIN_ENEMY_DETECTED: return "PLAYERBRAIN_ENEMY_DETECTED";
                case ePlayerBrainState::PLAYERBRAIN_LOSING: return "PLAYERBRAIN_LOSING";
                case ePlayerBrainState::PLAYERBRAIN_PEACEFUL: return "PLAYERBRAIN_PEACEFUL";
                case ePlayerBrainState::PLAYERBRAIN_WINNING: return "PLAYERBRAIN_WINNING";
                default:
                    assert(false);
                    break;
            }
            return "";
        }

        void thinkState_Rest();

        void onEntityDiscoveredEvent(const s_GameEvent &event);

        void produceMissions();

        void addMission(ePlayerBrainMissionKind kind, const std::vector<S_groupKind> &group, int initialDelay,
                        int id);

        bool hasMission(const int id);

        void produceLevel2Missions(int soldierKind, int infantryKind);

        void produceLevel3Missions(int trikeKind, int soldierKind, int infantryKind);

        void produceLevel4Missions(int trikeKind, int infantryKind);

        void produceLevel5Missions(int trikeKind, int infantryKind);

        void produceLevel6Missions(int trikeKind, int infantryKind);

        void produceLevel7Missions(int trikeKind, int infantryKind);

        void produceLevel8Missions(int trikeKind, int infantryKind);

        void produceLevel9Missions(int trikeKind, int infantryKind);
    };

}

#endif //D2TM_CPLAYERBRAINCAMPAIGN_H
