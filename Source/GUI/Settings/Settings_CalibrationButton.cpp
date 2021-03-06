#include "Settings_CalibrationButton.h"
#include "Util_Commands.h"

// Localized text class key:
static const juce::Identifier localeClassKey = "Settings::CalibrationButton";

// Localized text string key:
static const constexpr char* calibrateKey = "calibrate";

// Loads localized button text and starts listening for click events on
// construction.
Settings::CalibrationButton::CalibrationButton() :
Locale::TextUser(localeClassKey)
{
    setButtonText(localeText(calibrateKey));
    addListener(this);
}


// Launches the calibration application when the button is clicked.
void Settings::CalibrationButton::buttonClicked(juce::Button* button)
{
    jassert(button == this);
    Util::Commands systemCommands;
    systemCommands.runActionCommand(Util::CommandTypes::Action::calibrate);
}
