#define WIFI_IMPLEMENTATION
#include "Settings_WifiList_ListComponent.h"
#include "Settings_WifiList_ListButton.h"
#include "Wifi_Connection_Record_Handler.h"
#include "Wifi_Connection_Saved_Reader.h"
#include "Wifi_Connection_Event.h"
#include "Wifi_Device_Controller.h"
#include "Wifi_APList_Reader.h"
#include "Config_MainFile.h"
#include "Layout_Component_ConfigFile.h"
#include "Layout_Transition_Animator.h"
#include "Wifi_LibNM_SecurityType.h"
#include "Locale_Time.h"

#ifdef JUCE_DEBUG
// Print the full class name before debug output:
static const constexpr char* dbgPrefix = "Settings::WifiList::ListComponent::";
#endif

// Animation duration in milliseconds:
static const constexpr int animationDuration = 300;

// Maximum time to wait for list items to finish animating:
static const constexpr int animationTimeout = animationDuration * 5;

// Anonymous comparator class object for Wifi access point array sorting.
static class
{
public:
    /**
     * @brief  Compares Wifi access points in order to sort the access point
     *         list.
     *
     *  The connected access point will always come before all others, and
     * saved access points will come before new ones. Otherwise, access points
     * are sorted by signal strength.
     *
     * @param first    Some access point in the list.
     *
     * @param second   Some other access point in the list.
     *
     * @return         A negative number if first should come before second,
     *                 zero if the two connection points are equal, or a
     *                 positive number if second should come before first.
     */
    static int compareElements(const Wifi::AccessPoint& first,
            const Wifi::AccessPoint& second)
    {
        using namespace Wifi;
        jassert(!first.isNull() && !second.isNull());
        const Connection::Record::Handler recordHandler;
        AccessPoint activeAP = recordHandler.getActiveAP();
        if (!activeAP.isNull())
        {
            if (first == activeAP)
            {
                return -1;
            }
            else if (second == activeAP)
            {
                return 1;
            }
        }
        bool firstSaved = first.hasSavedConnection();
        bool secondSaved = second.hasSavedConnection();
        if (firstSaved && !secondSaved)
        {
            return -1;
        }
        else if (secondSaved && !firstSaved)
        {
            return 1;
        }
        return second.getSignalStrength() - first.getSignalStrength();
    }
} apComparator;

Settings::WifiList::ListComponent::ListComponent()
{
#if JUCE_DEBUG
    setName("Settings::WifiList::ListComponent");
#endif
    animationCheck.setCheckInterval(animationDuration);
    loadAccessPoints();
}


// Sets the number of items in the list to match the number of visible Wifi
// access points.
unsigned int Settings::WifiList::ListComponent::getListSize() const
{
    return visibleAPs.size();
}

/**
 * @brief  Converts a list item's generic Button pointer back to a ListButton
 *         pointer, or creates a new ListButton if the given pointer is null.
 *
 * @param button  A list item button pointer to convert, or nullptr.
 *
 * @return        A valid, non-null ListButton pointer.
 */
static Settings::WifiList::ListButton* getOrCreateListButton
(juce::Button* button)
{
    using namespace Settings::WifiList;
    if (button == nullptr)
    {
        return new ListButton;
    }
    else
    {
        return static_cast<ListButton*>(button);
    }
}


// Creates or updates an unselected list item.
juce::Button* Settings::WifiList::ListComponent::updateUnselectedListItem
(juce::Button* listItem, const unsigned int index)
{
    ListButton* listButton = getOrCreateListButton(listItem);
    listButton->updateForAccessPoint(visibleAPs[index]);
    listButton->removeControlComponent();
    return listButton;
}


// Creates or updates a component so it can be used as the selected list item.
juce::Button* Settings::WifiList::ListComponent::updateSelectedListItem
(juce::Button* listItem)
{
    ListButton* listButton = getOrCreateListButton(listItem);
    Wifi::AccessPoint selectedAP = visibleAPs[getSelectedIndex()];
    listButton->updateForAccessPoint(selectedAP);
    listButton->addControlComponent(controlComponent);
    controlComponent.updateComponentsForAP(selectedAP);
    return listButton;
}


// Reloads the list of wifi access points within range of the wifi device,
// and updates the access point list.
void Settings::WifiList::ListComponent::loadAccessPoints()
{
    visibleAPs.clearQuick();
    const Wifi::APList::Reader apListReader;
    visibleAPs = apListReader.getAccessPoints();
    DBG(dbgPrefix << __func__ << ": Found " << visibleAPs.size()
            << " access points.");
    visibleAPs.sort(apComparator, false);
    updateList();
    updateListItems(false);
}


// Refreshes all visible list items without changing their order in the list.
void Settings::WifiList::ListComponent::updateListItems(const bool animate)
{
    // Ensure list items are done animating before updating the list:
    animationCheck.startCheck([this]()
    {
        for (juce::Component* child : getChildren())
        {
            if (Layout::Transition::Animator::isAnimating(child))
            {
                return false;
            }
        }
        return true;
    },
    [this, animate]()
    {
        DBG(dbgPrefix << __func__ << ": Refreshing list content");
        if (animate)
        {
            refreshListContent(Layout::Transition::Type::moveLeft,
                    animationDuration, false);
        }
        else
        {
            refreshListContent();
        }
    }, animationTimeout,
    []()
    {
        DBG(dbgPrefix
                << "updateListItems: timed out while waiting to update!");
    });
}


// Removes any lost access points and sorts the access point list.
void Settings::WifiList::ListComponent::updateList()
{
    DBG(dbgPrefix << __func__ << ": Sorting and pruning AP list.");
    if (invalidSelectionIndex >= 0)
    {
        DBG(dbgPrefix << __func__ << ": Former selected AP \""
                << visibleAPs[invalidSelectionIndex].getSSID().toString()
                << "\" was lost and will now be removed.");
        visibleAPs.remove(invalidSelectionIndex);
        invalidSelectionIndex = -1;
    }
    visibleAPs.removeIf([this](Wifi::AccessPoint listAP)
    {
        return listAP.isNull();
    });
    visibleAPs.sort(apComparator, false);
}


// Updates the list normally if no list item is selected. If a list item is
// selected, it will instead update list components, and wait to update list
// order and remove missing items until the full list is shown again.
void Settings::WifiList::ListComponent::scheduleListUpdate()
{
    if (getSelectedIndex() == -1)
    {
        updateList();
        updateListItems(true);
    }
    else
    {
        updateListItems();
        fullUpdateNeeded = true;
    }
}


// If necessary, updates the list order and removes lost access points when the
// selection is removed to reveal the full list.
void Settings::WifiList::ListComponent::selectionChanged()
{
    if (getSelectedIndex() == -1 && fullUpdateNeeded)
    {
        updateList();
    }
    fullUpdateNeeded = false;
}


// Schedules a list update when access point strength changes.
void Settings::WifiList::ListComponent::signalStrengthUpdate
(const Wifi::AccessPoint updatedAP)
{
    scheduleListUpdate();
}


// Adds a newly discovered access point to the list, and schedules a list
// update.
void Settings::WifiList::ListComponent::accessPointAdded
(const Wifi::AccessPoint addedAP)
{
    visibleAPs.addIfNotAlreadyThere(addedAP);
    if (invalidSelectionIndex >= 0)
    {
        if (addedAP == visibleAPs[invalidSelectionIndex])
        {
            DBG(dbgPrefix << __func__ << ": Selected AP \""
                    << addedAP.getSSID().toString()
                    << "\" found again, cancel pending removal.");
            invalidSelectionIndex = -1;
        }
    }
    scheduleListUpdate();
}


// Replaces the removed access point with a null access point and schedules a
// list update whenever an access point is lost.
void Settings::WifiList::ListComponent::accessPointRemoved
(const Wifi::AccessPoint removedAP)
{
    int removedIndex = visibleAPs.indexOf(removedAP);
    if (removedIndex >= 0)
    {
        if (removedIndex == getSelectedIndex())
        {
            if (invalidSelectionIndex != removedIndex)
            {
                DBG(dbgPrefix << __func__ << ": Selected AP \""
                        << removedAP.getSSID().toString()
                        << "\" was lost and will be removed once deselected");
                invalidSelectionIndex = removedIndex;
                fullUpdateNeeded = true;
            }
        }
        else
        {
            visibleAPs.set(removedIndex, Wifi::AccessPoint());
            scheduleListUpdate();
        }
    }
}


// Updates access point connection controls when a connection starts to
// activate.
void Settings::WifiList::ListComponent::startedConnecting
(const Wifi::AccessPoint connectingAP)
{
    const int selectedIndex = getSelectedIndex();
    if (selectedIndex >= 0 && connectingAP == visibleAPs[selectedIndex])
    {
        controlComponent.updateComponentsForAP(connectingAP);
        fullUpdateNeeded = true;
    }
    else
    {
        scheduleListUpdate();
    }
}


// Updates access point connection controls when connection authentication
// fails.
void Settings::WifiList::ListComponent::connectionAuthFailed
(const Wifi::AccessPoint connectingAP)
{
    startedConnecting(connectingAP);
}


// Schedules a list update whenever an access point connects.
void Settings::WifiList::ListComponent::connected
(const Wifi::AccessPoint connectedAP)
{
    startedConnecting(connectedAP);
}


// Schedules a list update whenever an access point disconnects.
void Settings::WifiList::ListComponent::disconnected
(const Wifi::AccessPoint disconnectedAP)
{
    scheduleListUpdate();
}


// Starts the first access point scan, and begins running additional scans at
// an interval defined in the main configuration file.
Settings::WifiList::ListComponent::ScanTimer::ScanTimer()
{
    timerCallback();
}


// Commands the Wifi module to start a new scan for visible access points.
void Settings::WifiList::ListComponent::ScanTimer::startScan()
{
    Wifi::Device::Controller wifiController;
    wifiController.scanAccessPoints();
}


// Starts a new scan, and schedules the next scan.
void Settings::WifiList::ListComponent::ScanTimer::timerCallback()
{
    stopTimer();
    startScan();
    Config::MainFile mainConfig;
    int scanFrequency = mainConfig.getWifiScanFrequency();
    if (scanFrequency > 0)
    {
        startTimer(scanFrequency);
    }
}
