#ifndef D2TM_cMousePlaceState_H
#define D2TM_cMousePlaceState_H

#include "cMouseState.h"

class cBuildingListItem;

/**
 * Mouse placing structure at battlefield
 */
class cMousePlaceState : public cMouseState {

public:
    explicit cMousePlaceState(cPlayer * player, cGameControlsContext *context, cMouse * mouse);

    void onNotifyMouseEvent(const s_MouseEvent &event) override;
    void onNotifyKeyboardEvent(const s_KeyboardEvent &event) override;
    void onStateSet() override;

    void onFocus() override {};
    void onBlur() override {};

private:
    void onMouseLeftButtonClicked(const s_MouseEvent &event);

    void onMouseRightButtonPressed(const s_MouseEvent &event);

    void onMouseRightButtonClicked(const s_MouseEvent &event);

    void onMouseMovedTo(const s_MouseEvent &event);

    bool mayPlaceIt(cBuildingListItem *itemToPlace, int mouseCell);
};


#endif //D2TM_cMousePlaceState_H
