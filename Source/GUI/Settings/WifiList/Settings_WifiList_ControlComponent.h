#pragma once
/**
 * @file  Settings_WifiList_ControlComponent.h
 *
 * @brief  Provides reusable control and information components for a single
 *         access point.
 */

#include "Locale_TextUser.h"
#include "Wifi_AccessPoint.h"
#include "Layout_Group_Manager.h"
#include "Widgets_BoundedLabel.h"
#include "Widgets_Spinner.h"
#include "Widgets_Switch.h"
#include "Wifi_Connection_Control_Handler.h"
#include "JuceHeader.h"

namespace Settings { namespace WifiList { class ControlComponent; } }

/**
 * @brief  A collection of components that provide detailed information and
 *         controls for a single Wifi access point.
 */
class Settings::WifiList::ControlComponent : public juce::Component,
    public Locale::TextUser
{
public:
    /**
     * @brief  Starts listening for input from child components.
     */
    ControlComponent();

    virtual ~ControlComponent() { }

    /**
     * @brief  Updates all components to represent a specific Wifi access point.
     *
     * @param newAP  The access point the ControlComponent should represent.
     */
    void updateComponentsForAP(Wifi::AccessPoint newAP);

private:
    /**
     * @brief  Updates the layout of all child components when the main
     *         component is resized.
     */
    virtual void resized() override;

    /**
     * @brief  Attempts to open a connection using the selected access point.
     */
    void connect();

    /**
     * @brief  A TextButton with text that can be replaced by a Spinner to
     *         indicate that Wifi is busy.
     */
    class ConnectionButton : public juce::TextButton
    {
    public:
        ConnectionButton();

        virtual ~ConnectionButton() { }

        /**
         * @brief  Shows or hide the spinner, ensuring the button is enabled
         *         when text is visible and disabled when the spinner is
         *         visible.
         *
         * @param showSpinner  True to show the spinner, false to show the
         *                     button text.
         */
        void setSpinnerVisible(bool showSpinner);

    private:
        /**
         * @brief  Ensures the spinner bounds are updated with connection
         *         button bounds.
         */
        virtual void resized() override;

        // The loading spinner component:
        Widgets::Spinner spinner;

        // Holds the button text while the spinner is enabled.
        juce::String savedText;
    };

    /**
     * @brief  Listens for input from the password field, connection button, and
     *         show password switch.
     */
    class ControlListener : public juce::Button::Listener,
            public juce::TextEditor::Listener
    {
    public:
        /**
         * @brief  Connects this listener to its ControlComponent.
         *
         * @param controlComponent  The ControlComponent that owns this
         *                          ControlListener.
         */
        ControlListener(ControlComponent& controlComponent);

        virtual ~ControlListener() { }

    private:
        /**
         * @brief  Either deletes a saved connection, attempts to open or close
         *         a connection, or toggles password visibility, depending on
         *         which button is clicked.
         *
         * @param button  Whichever one of the three ControlComponent buttons
         *                was just clicked.
         */
        virtual void buttonClicked(juce::Button* button) final override;

        /**
         * @brief  Attempts to connect if return is pressed after entering a
         *         password.
         *
         * @param editor  This should always be the password field.
         */
        void textEditorReturnKeyPressed(juce::TextEditor& editor) override;

        ControlComponent& controlComponent;
    };
    ControlListener controlListener;

    // Handles Wifi connection actions, and ensures the Wifi::Resource remains
    // available:
    Wifi::Connection::Control::Handler connectionController;

    // Manages the layout of all child components:
    Layout::Group::Manager layoutControl;

    // The access point object represented and controlled by this component:
    Wifi::AccessPoint selectedAP;

    // Displays an access point's last connection time if applicable.
    Widgets::BoundedLabel lastConnectionLabel;

    // Allows the user to delete all saved connections linked to the access
    // point.
    juce::TextButton savedConnectionDeleteButton;

    // Used for entering a password for a secured access point.
    Widgets::BoundedLabel passwordLabel;
    juce::TextEditor passwordEditor;

    // Sets whether the password should be shown.
    Widgets::BoundedLabel showPasswordLabel;
    Widgets::Switch showPasswordSwitch;

    // Clicked to connect or disconnect.
    ConnectionButton connectionButton;

    // Shows an error message if the connection fails.
    Widgets::BoundedLabel errorLabel;
};
