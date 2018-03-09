/**
 * @file OverlaySpinner.h
 * 
 * OverlaySpinner is a Spinner component that covers the entire window, and
 * optionally displays loading text.
 * @see Spinner.h
 */

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"
#include "ScalingLabel.h"
#include "Spinner.h"

class OverlaySpinner : public Spinner {
public:
    enum ColourIds{
        backgroundColourId = 0x1900400,
        textColourId = 0x1900401
    };
    
    /**
     * @param secondsToTimeout sets how long the spinner will show
     * itself before timing out. If this value is negative, the spinner
     * will never time out.
     */
    OverlaySpinner(int secondsToTimeout = -1);
    ~OverlaySpinner();
    
    /**
     * Sets text to display below the spinner
     * @param newText
     */
    void setLoadingText(String newText);
    
private:

    /**
     * Fills in the overlay background.
     * @param g
     */
    void paint(Graphics & g) override;
    
    /**
     * Update the spinner and loading text to fit overlay bounds.
     */
    void resized() override;
    
    ScalingLabel loadingText;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OverlaySpinner)
};
