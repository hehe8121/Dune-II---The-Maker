#ifndef D2TM_CPLAYERBRAINMISSION_H
#define D2TM_CPLAYERBRAINMISSION_H

#include <vector>
#include "cPlayerBrainData.h"
#include <observers/cScenarioObserver.h>


enum ePlayerBrainMissionKind {
    /**
     * Attack enemy player (generally)
     */
    PLAYERBRAINMISSION_KIND_ATTACK,

    /**
     * Explore the map
     */
    PLAYERBRAINMISSION_KIND_EXPLORE,

    /**
     * Explore the map, find spice patch(es)
     */
    PLAYERBRAINMISSION_KIND_FIND_SPICE,

    /**
     * Attack enemy player (generally) from the air
     */
    PLAYERBRAINMISSION_KIND_AIRSTRIKE,

    /**
     * Defend area?
     */
    PLAYERBRAINMISSION_KIND_DEFEND,

    /**
     * Harass enemy ... (harvesters?)
     */
    PLAYERBRAINMISSION_KIND_HARASS,

};

enum ePlayerBrainMissionState {

    /**
     * Initial state (begin state)
     */
    PLAYERBRAINMISSION_STATE_INITIAL,

    /**
     * Gather required resources (units), either via selecting existing ones or by building new ones
     */
    PLAYERBRAINMISSION_STATE_PREPARE_GATHER_RESOURCES,

    /**
     * Await construction of all units
     */
    PLAYERBRAINMISSION_STATE_PREPARE_AWAIT_RESOURCES,

    /**
     * Determine which target...
     */
    PLAYERBRAINMISSION_STATE_SELECT_TARGET,

    /**
     * Start executing the mission
     */
    PLAYERBRAINMISSION_STATE_EXECUTE,

};

struct S_groupKind {
    /**
     * Which kind of unit?
     */
    int type;

    /**
     * How many of that kind?
     */
    int amount;
};


class cPlayerBrainMission : public cScenarioObserver {

public:

    cPlayerBrainMission(cPlayer * player, const ePlayerBrainMissionKind & kind);
    ~cPlayerBrainMission();

    std::vector<S_buildOrder> think();

    void onNotify(const s_GameEvent &event);


private:

    cPlayer * player;

    const ePlayerBrainMissionKind kind;

    ePlayerBrainMissionState state;

    // store which units where created (and thus part of this team)
    std::vector<int> units;

    // how the group of units look like that would execute the mission
    std::vector<S_groupKind> group;

    std::vector<S_buildOrder> buildOrders;

    void thinkState_Initial();


    void thinkState_PrepareGatherResources();

    void thinkState_SelectTarget();

    void thinkState_Execute();

    void thinkState_PrepareAwaitResources();
};


#endif //D2TM_CPLAYERBRAINMISSION_H
