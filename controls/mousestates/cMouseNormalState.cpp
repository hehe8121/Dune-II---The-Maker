#include "cMouseNormalState.h"

#include "controls/cGameControlsContext.h"

#include "utils/cRectangle.h"
#include "gameobjects/units/cUnit.h"
#include "gameobjects/structures/cAbstractStructure.h"
#include "player/cPlayer.h"
//#include "sidebar/cSideBar.h"

#include "d2tmc.h"
#include "cGame.h"

#include "data/gfxaudio.h"
#include "data/gfxdata.h"

#include <algorithm>

cMouseNormalState::cMouseNormalState(cPlayer *player, cGameControlsContext *context, cMouse *mouse) :
    cMouseState(player, context, mouse),
    state(SELECT_STATE_NORMAL) {
}

void cMouseNormalState::onNotifyMouseEvent(const s_MouseEvent &event) {

    // these methods can have a side-effect which changes mouseTile...
    switch (event.eventType) {
        case MOUSE_LEFT_BUTTON_PRESSED:
            mouse->boxSelectLogic(context->getMouseCell());
            break;
        case MOUSE_LEFT_BUTTON_CLICKED:
            onMouseLeftButtonClicked(event);
            break;
        case MOUSE_RIGHT_BUTTON_PRESSED:
            onMouseRightButtonPressed(event);
            break;
        case MOUSE_RIGHT_BUTTON_CLICKED:
            onMouseRightButtonClicked(event);
            break;
        case MOUSE_MOVED_TO:
            onMouseMovedTo(event);
            break;
        default:
            break;
    }

    // ... so set it here
    if (context->isState(MOUSESTATE_SELECT)) { // if , required in case we switched state
        mouse->setTile(mouseTile);
    }
}

void cMouseNormalState::onMouseLeftButtonClicked(const s_MouseEvent &event) {
    bool selectedUnits = false;
    if (mouse->isBoxSelecting()) {
        player->deselectAllUnits();

        // remember, these are screen coordinates
        // TODO: Make it use absolute coordinates? (so we could have a rectangle bigger than the screen at one point?)
        cRectangle boxSelectRectangle = mouse->getBoxSelectRectangle();

        const std::vector<int> &ids = player->getAllMyUnitsWithinViewportRect(boxSelectRectangle);
        selectedUnits = player->selectUnits(ids);
    } else {
        bool infantrySelected = false;
        bool unitSelected = false;

        if (state == SELECT_STATE_NORMAL) {
            // single click, no box select
            int hoverUnitId = context->getIdOfUnitWhereMouseHovers();
            if (hoverUnitId > -1) {
                player->deselectAllUnits();

                cUnit &pUnit = unit[hoverUnitId];
                if (pUnit.isValid() && pUnit.belongsTo(player) && !pUnit.bSelected) {
                    pUnit.bSelected = true;
                    if (pUnit.isInfantryUnit()) {
                        infantrySelected = true;
                    } else {
                        unitSelected = true;
                    }
                }
            }

            int hoverStructureId = context->getIdOfStructureWhereMouseHovers();
            if (hoverStructureId > -1) {
                player->selected_structure = hoverStructureId;
                cAbstractStructure *pStructure = player->getSelectedStructure();
                if (pStructure && pStructure->isValid() && pStructure->belongsTo(player)) {
                    player->getSideBar()->setSelectedListId(pStructure->getAssociatedListID());
                } else {
                    player->selected_structure = -1;
                }
            }
        } else if (state == SELECT_STATE_RALLY) {
            // setting a rally point!
            cAbstractStructure *pStructure = player->getSelectedStructure();
            if (pStructure && pStructure->isValid()) {
                pStructure->setRallyPoint(context->getMouseCell());
            }
        }

        if (unitSelected) {
            play_sound_id(SOUND_REPORTING);
        }

        if (infantrySelected) {
            play_sound_id(SOUND_YESSIR);
        }

        selectedUnits = unitSelected || infantrySelected;
    }

    if (selectedUnits) {
        context->setMouseState(MOUSESTATE_UNITS_SELECTED);
    }

    mouse->resetBoxSelect();
}

void cMouseNormalState::onMouseRightButtonPressed(const s_MouseEvent &event) {
    mouse->dragViewportInteraction();
}

void cMouseNormalState::onMouseRightButtonClicked(const s_MouseEvent &event) {
    mouse->resetDragViewportInteraction();
}

void cMouseNormalState::onMouseMovedTo(const s_MouseEvent &event) {
    if (state == SELECT_STATE_NORMAL) {
        mouseTile = getMouseTileForNormalState();
    } else if (state == SELECT_STATE_RALLY) {
        mouseTile = MOUSE_RALLY;
    }
}

int cMouseNormalState::getMouseTileForNormalState() const {
    int hoverUnitId = context->getIdOfUnitWhereMouseHovers();
    if (hoverUnitId > -1) {
        cUnit &pUnit = unit[hoverUnitId];
        if (pUnit.isValid() && pUnit.belongsTo(player)) {
            // only show this for units
            return MOUSE_PICK;
        }
        // non-selectable units (all from other players), don't give a "pick" mouse tile
        return MOUSE_NORMAL;
    }
    return MOUSE_NORMAL;
}

void cMouseNormalState::onStateSet() {
    mouseTile = MOUSE_NORMAL;
    mouse->setTile(mouseTile);
}

void cMouseNormalState::onNotifyKeyboardEvent(const s_KeyboardEvent &event) {
    // these methods can have a side-effect which changes mouseTile...
    switch (event.eventType) {
        case KEY_HOLD:
            onKeyDown(event);
            break;
        case KEY_PRESSED:
            onKeyPressed(event);
            break;
        default:
            break;
    }

    // ... so set it here
    if (context->isState(MOUSESTATE_SELECT)) { // if , required in case we switched state
        mouse->setTile(mouseTile);
    }
}

void cMouseNormalState::onKeyDown(const s_KeyboardEvent &event) {
    if (event.key == KEY_LCONTROL || event.key == KEY_RCONTROL) {
        cAbstractStructure *pSelectedStructure = player->getSelectedStructure();
        // when selecting a structure
        if (pSelectedStructure && pSelectedStructure->belongsTo(player) && pSelectedStructure->canSpawnUnits()) {
            state = SELECT_STATE_RALLY;
            mouseTile = MOUSE_RALLY;
        }
    }
}

void cMouseNormalState::onKeyPressed(const s_KeyboardEvent &event) {
    if (event.key == KEY_LCONTROL || event.key == KEY_RCONTROL) {
        state = SELECT_STATE_NORMAL;
        mouseTile = MOUSE_NORMAL;
    }

    if (event.key == KEY_R) {
        context->setMouseState(MOUSESTATE_REPAIR);
    }

    int iGroup = game.getGroupNumberFromScanCode(event.key);

    if (iGroup > 0) {
        // select all units for group
        player->deselectAllUnits();
        player->selectUnitsFromGroup(iGroup);
        player->getGameControlsContext()->setMouseState(MOUSESTATE_UNITS_SELECTED);
    }
}
