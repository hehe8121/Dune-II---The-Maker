/* 

  Dune II - The Maker

  Author : Stefan Hendriks
  Contact: stefanhen83@gmail.com
  Website: http://dune2themaker.fundynamic.com

  2001 - 2020 (c) code by Stefan Hendriks

  */

#ifndef D2TM_UNIT_H
#define D2TM_UNIT_H

// Define TRANSFER stuff for reinforcements	
class cRectangle; // forward declaration :(

#define TRANSFER_NONE	-1				// nothing to transfer
#define TRANSFER_NEW_STAY	0			// bring a new unit, and let the carryall stay
#define TRANSFER_NEW_LEAVE	2			// bring a new unit, and let the carryall go back (and die)
#define TRANSFER_PICKUP		1			// carry an existing unit, bring to new location
#define TRANSFER_DIE		3			// will die when reaching goal-cell (added for proper animation)

#define ACTION_GUARD        0           // on guard (scanning for enemy activitiy)
#define ACTION_MOVE         1           // moving
#define ACTION_CHASE        2           // chasing a unit to attack
#define ACTION_ATTACK       3           // attacking (not moving)

// Reinforcement data (loaded from ini file)
struct sReinforcement
{
    int iSeconds;       // FPS based, every second this decreases (0 = deliver)
    int iUnitType;      // what unit?
    int iPlayer;         // to who?
    int iCell;          // Where to?
};

class cUnit {

public:

    cUnit();
    ~cUnit();

	float fExperience;	// experience gained by unit

    int iID;            // index of unit in the unit array

    int iCell;          // cell of unit    
   
    int iType;          // type of unit

    int iGroup;         // belongs to group...

    int iTempHitPoints; // temp hold back for 'reinforced' / 'dumping' and 'repairing' units

    int iPlayer;        // belongs to player

    // Movement
    int iNextCell;      // where to move to (next cell)
    int iGoalCell;      // the goal cell (goal of path)    
    int iCellX;         // my cell x
    int iCellY;         // my cell y
    float iOffsetX;       // X offset
    float iOffsetY;       // Y offset
    int iPath[MAX_PATH_SIZE];    // path of unit
    int iPathIndex;     // where are we?
    int iPathFails;     // failed...
    bool bCalculateNewPath; // calculate new path?	

    // carryall stuff
	bool bCarryMe;		// carry this unit when moving it around?
	int  iCarryAll;		// any carry-all that will pickup this unit... so this unit knows
						// it should wait when moving, etc, etc


    // WHEN ATTACKING
    int iAttackUnit;      // attacking unit id
    int iAttackStructure; // attack structure id
    int iAttackCell;      // attacking a cell (which is force attack)

    // Action its doing:
    int iAction;        // ACTION_MOVE; ACTION_GUARD; ACTION_CHASE;
    eUnitActionIntent intent;

    // Action given code 
    int iUnitID;        // Unit ID to attack/pickup, etc
    int iStructureID;   // structure ID to attack/bring to (refinery)/capture

    // Harvester specific
    int iCredits;       // credits stored in this baby

	// Carry-All specific
	int iTransferType;	// -1 = none, 0 = new (and stay), 1 = carrying existing unit , 2 = new (and leave)
						// iUnitID = unit we CARRY (when TransferType == 1)
						// iTempHitPoints = hp of unit when transfertype = 1

	int iCarryTarget;	// Unit ID to carry, but is not carried yet
	int iBringTarget;	// Where to bring the carried unit (when iUnitID > -1)
	int iNewUnitType;	// new unit that will be brought, will be this type
	bool bPickedUp;		// picked up the unit?
	
    // Drawing
    int iBodyFacing;    // Body of tanks facing
    int iHeadFacing;    // Head of tanks facing
    int iBodyShouldFace;    // where should the unit body look at?
    int iHeadShouldFace;    // where should the unit look at?
    int iFrame;         // framed (animated stuff)

    // selected
    bool bSelected;     // selected or not?
	
	float fExpDamage();	// experience damage by bullet (extra damage that is)

    // ------------
    void init(int i);        // inits units
    int draw_x();
    int draw_y();
    int pos_x();
    int pos_y();
    int center_draw_x();
    int center_draw_y();

    void draw_health();
	void draw_experience();
	void draw_spice();
    void draw();
	void draw_path();
    bool isValid();     // valid unit?

    void shoot(int iShootCell);  // shoot at goalcell

	void die(bool bBlowUp, bool bSquish);			// die!

    void updateCellXAndY();        // updateCellXAndY status

    void think();       // thinking in general
    void think_move_air();  // aircraft specific
    void think_move_foot(); // soldiers specific
    void think_move();  // thinking about movement (which is called upon a faster rate)
                        // for wheeled/tracked units
    void think_attack();
    void think_guard();

	void LOG(const char *txt);

    void think_hit(int iShotUnit, int iShotStructure);

    int getNextCellToMoveTo(); // what is the next cell to move to

    void move_to(int iGoalCell);
    void move_to(int iCll, int iStrucID, int iUnitID, eUnitActionIntent intent);
    void move_to(int iCll, int iStrucID, int iUnitID);

	// carryall-functions:
	void carryall_order(int iuID, int iTransfer, int iBring, int iTpe);

    // -------------
	int TIMER_blink;	// blink

    int TIMER_move;     // movement timer
    int TIMER_movewait; // wait for move think...

    int TIMER_thinkwait;    // wait with normal thinking..

    int TIMER_turn;     // turning around
    int TIMER_frame;    // frame

    int TIMER_harvest;  // harvesting

    int TIMER_guard;    // guard scanning timer
    int TIMER_bored;    // how long are we bored?

    int TIMER_attack;   // when to shoot?

    int TIMER_wormeat;  // when do we eat? (when worm)

    s_UnitP& getUnitType();

    /**
     * Return current health normalized (between 0.0 and 1.0)
     * @return
     */
    float getHealthNormalized();

    /**
     * is this unit an airborn unit?
     * @return
     */
    bool isAirbornUnit() {
        return iType == CARRYALL || iType == ORNITHOPTER || iType == FRIGATE;
    }

    /**
     * An air unit that may be attacked according to generic game rules, ie this does not say anything
     * about if the unit *can* attack it. (use canAttackAirUnits function for that)
     * @return
     */
    bool isAttackableAirUnit() {
        return iType == ORNITHOPTER;
    }

    bool isHarvester() {
        return iType == HARVESTER;
    }

    bool isSandworm() const;

    /**
     * Can this unit attack air units?
     * @return
     */
    bool canAttackAirUnits() {
        return getUnitType().canAttackAirUnits;
    }

    bool isInfantryUnit();

    int getBmpWidth() const;

    int getBmpHeight() const;

    cRectangle * dimensions;

    void recreateDimensions();

    void think_position();

    bool isMovingBetweenCells();

    bool isIdle() { return iAction == ACTION_GUARD; }

    void takeDamage(int damage) {
        iHitPoints -= damage;
        if (isDead()) {
            die(true, false);
        }
    }

    /**
     * Returns true if dead (ie hitpoints <= 0)
     * @return
     */
    bool isDead() {
        return iHitPoints <= 0;
    }

    bool isDamaged();

    void restoreFromTempHitPoints();

    void setMaxHitPoints();

    cPlayer *getPlayer();

private:
    int iHitPoints;     // hitpoints of unit

    bool isUnitWhoCanSquishInfantry();

    bool isSaboteur();

    void forgetAboutCurrentPathAndPrepareToCreateNewOne();
    void forgetAboutCurrentPathAndPrepareToCreateNewOne(int timeToWait);

    eUnitMoveToCellResult moveToNextCellLogic();
};



int UNIT_CREATE(int iCll, int unitType, int iPlayer, bool bOnStart);
int CREATE_PATH(int iUnitId, int iPathCountUnits);
int RETURN_CLOSE_GOAL(int iCll, int iMyCell, int iID);

void UNIT_deselect_all();
void UNIT_ORDER_MOVE(int iUnitID, int iGoalCell);

void UNIT_ORDER_ATTACK(int iUnitID, int iGoalCell, int iUnit, int iStructure, int iAttackCell);

int UNIT_find_harvest_spot(int id);
void REINFORCE(int iPlr, int iTpe, int iCll, int iStart);
int CARRYALL_TRANSFER(int iuID, int iGoal);

int UNIT_FREE_AROUND_MOVE(int iUnit);

void THINK_REINFORCEMENTS();
void SET_REINFORCEMENT(int iCll, int iPlyr, int iTime, int iUType);
void INIT_REINFORCEMENT();

#endif