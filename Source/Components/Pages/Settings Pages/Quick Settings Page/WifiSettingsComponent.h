#pragma once
#include "Localized.h"
#include "WifiStateManager.h"
#include "ConnectionSettingsComponent.h"

/**
 * @file WifiSettingsComponent.h
 * 
 * WifiSettingsComponent shows wifi state, lets the user enable or disable wifi,
 * and opens the wifi settings page.
 */

class WifiSettingsComponent : public ConnectionSettingsComponent,
public WifiStateManager::Listener, private Localized
{
public:
    /**
     * @param openWifiPage   Callback function that opens the wifi page.
     */
    WifiSettingsComponent(std::function<void() > openWifiPage);

    virtual ~WifiSettingsComponent() { }
private:
    /**
     * Checks if wifi is currently turned on.
     * 
     * @return true if wifi is enabled, false if disabled.
     */
    bool connectionEnabled() override;
       
    /**
     * This method is used by the component to determine if it should show the
     * loading spinner.
     * 
     * @return true whenever wifi is in a transition state (connecting,
     *         disconnecting, etc.).
     */
    bool shouldShowSpinner() override;

    /**
     * This method is used by the component to determine if the connection 
     * switch should be enabled.
     * 
     * @return true iff the wifi device is present and isn't being turned on or
     *         off.
     */
    bool allowConnectionToggle() override;

    
    /**
     * This method is used by the component to determine if the connection 
     * page should be accessible.
     * 
     * @return true whenever wifi is enabled and not being disabled.
     */
    bool connectionPageAvailable() override;

    /**
     * @return the wifi icon
     */
    juce::String getIconAsset() override;

    /**
     * Enable or disable the wifi radio.
     * 
     * @param enabled   This sets which action to take.
     */
    void enabledStateChanged(bool enabled) override;

    /**
     * Sets the wifi button text based on the current wifi state.
     */
    juce::String updateButtonText() override;

    /**
     * Use wifi status updates to keep the component updated.
     * 
     * @param state
     */
    void wifiStateChanged(WifiStateManager::WifiState state) override;

    //if true, show the loading spinner and disable controls.
    bool wifiBusy = false;


    //localized text keys
    static const constexpr char * wifi_not_found = "wifi_not_found";
    static const constexpr char * wifi_disabled = "wifi_disabled";
    static const constexpr char * wifi_turning_on = "wifi_turning_on";
    static const constexpr char * not_connected = "not_connected";
    static const constexpr char * wifi_turning_off = "wifi_turning_off";
    static const constexpr char * connecting_to_ap = "connecting_to_ap";
    static const constexpr char * connecting_to_unknown
            = "connecting_to_unknown";   
    static const constexpr char * missing_psk = "missing_psk";
    static const constexpr char * disconnecting = "disconnecting";

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WifiSettingsComponent)
};