#pragma once
#include "PageComponent.h"
#include "ConfigurableImageButton.h"
#include "OverlaySpinner.h"
#include "SwitchComponent.h"
#include "LoginPage.h"
#include "Localized.h"

/**
 * @file PowerPageComponent.h
 * 
 * PowerPageComponent is a UI menu page that shows buttons that perform actions
 * related to system power state.  From this page, the user can shut down the
 * system, reboot the system, turn off the display, or enter PocketCHIP flashing
 * mode.
 * 
 * In debug builds, this page also contains a test button.  The test button
 * provides a convenient place to attach temporary debugging code so that it can
 * be triggered through the UI.
 *
 * TODO:
 * 
 * -Set the sleep mode command in config.json
 * -Restore build/version labels.
 * -Make background color configurable.
 */

class PowerPage : public PageComponent, private Localized
{
public:
    
    PowerPage();

    virtual ~PowerPage() { }
private:
    /**
     * Turns off the display until key or mouse input is detected.
     * The lock screen will be visible when the display turns on again.
     */
    void startSleepMode();

    /**
     * If the lock screen is visible, this will remove it from the screen.
     */
    void hideLockscreen();

    /**
     * Show the power spinner to indicate to the user that the system is
     * restarting or shutting down.
     */
    void showPowerSpinner();
    
    /**
     * Handles all button clicks.
     */
    void pageButtonClicked(Button*) override;

    /**
     * Resize the lock screen and overlay spinner to fit the page.
     */
    void pageResized() override;

    
    //Turns off the system using the shutdown command in the MainConfigFile.
    TextButton powerOffButton;
    //Starts sleep mode with startSleepMode()
    TextButton sleepButton;
    //Restarts the system using the reboot command in the MainConfigFile.
    TextButton rebootButton;
    //Shows a page that gives the user the option to enter flashing mode.
    TextButton felButton;
#if JUCE_DEBUG
    //In debug builds, this button provides a convenient place to tie in
    //temporary test routines.
    TextButton testButton;
#endif
    //Spinner to indicate that the system is rebooting/shutting down
    OverlaySpinner overlaySpinner;
    //The lock screen is displayed after entering sleep mode.
    LoginPage lockscreen;   
    
    //localized text keys;
    static const constexpr char * shutdown = "shutdown";
    static const constexpr char * reboot = "reboot";
    static const constexpr char * sleep = "sleep";
    static const constexpr char * flash_software = "flash_software";
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PowerPage)
};

