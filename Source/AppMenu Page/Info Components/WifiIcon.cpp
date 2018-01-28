/*
  ==============================================================================

    WifiIcon.cpp
    Created: 29 Dec 2017 12:02:41pm
    Author:  Anthony Brown

  ==============================================================================
 */

#include "WifiIcon.h"
#include "../../Wifi/WifiStatus.h"
#include "../../PocketHomeApplication.h"

WifiIcon::WifiIcon() :
ConfigurableImageComponent(ComponentConfigFile::wifiIconKey)
{
    startTimer(1);
}

WifiIcon::~WifiIcon()
{
    stopTimer();
}

/**
 * Set the WiFi connection status image.
 */
void WifiIcon::setStatus(WifiIconImage wifiState)
{
    setImageAssetIndex((int) wifiState);
}

/**
 * Enable/disable the WiFi checking timer based on component visibility
 */
void WifiIcon::visibilityChanged()
{
    if (isVisible())
    {
        if (!isTimerRunning())
        {
            startTimer(10);
        }
    } else
    {
        stopTimer();
    }
}



/**
 * Check WiFi state and update the image.
 */
void WifiIcon::timerCallback()
{
        const WifiStatus& wifiStatus = PocketHomeApplication::getInstance()
                ->getWifiStatus();
        WifiIconImage wifiState = wifiOff;
        ScopedPointer<WifiAccessPoint> accessPoint =
                wifiStatus.connectedAccessPoint();
        if (wifiStatus.isConnected() && accessPoint != nullptr)
        {
            // 0 to 100
            float sigStrength = std::max(0,
                    std::min(99, accessPoint->signalStrength));
            wifiState = (WifiIconImage) (2 + (int) (sigStrength * 3 / 100));
        }// wifi on but no connection
        else if (wifiStatus.isEnabled())
        {
            wifiState = wifiStrength0;
        }// wifi off
        setStatus(wifiState);
        startTimer(frequency);
}