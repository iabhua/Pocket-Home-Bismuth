#ifndef WIFI_IMPLEMENTATION
    #error File included directly outside of Wifi module implementation.
#endif
#pragma once
/**
 * @file  Wifi_APList_Module.h
 *
 * @brief  Creates and updates a list of visible Wifi access points.
 */

#include "Wifi_Module.h"

namespace Wifi
{
    namespace APList { class Module; }
    class Resource;
    class AccessPoint;
    namespace LibNM
    {
        class APHash;
        class AccessPoint;
    }
}

/**
 * @brief  Tracks all visible Wifi access points, using NetworkManager signals
 *         and data to construct and update AccessPoint objects.
 *
 *  All access point data is created externally by NetworkManager. The LibNM
 * library connects to NetworkManager to read this data, and share it in the
 * form of GLib object pointers. However, object data provided by LibNM is only
 * safe to access on the LibNM thread run by the LibNM::Thread::Module.
 *
 *  The APList::Module reads in all access point data provided by LibNM, and
 * uses it to create a list of AccessPoint objects. These objects can be safely
 * shared between threads, minimizing the need to interact directly with LibNM.
 *
 *  If multiple LibNM::AccessPoint objects have the same SSID, access point
 * mode, and security settings, it is assumed that they represent the same
 * network connection point, and a single AccessPoint object will be used to
 * represent all of them.
 *
 *  The APList::Module also ensures all updates to LibNM access point data are
 * properly processed and shared. Whenever an AccessPoint object's first
 * LibNM::AccessPoint is added, or its last LibNM::AccessPoint is removed, the
 * module will notify all APList::Listener objects. The module will also
 * directly update the saved signal strength of all equivalent AccessPoint
 * objects whenever that objects list of LibNM::AccessPoint objects changes.
 *
 *  Other modules that interact with LibNM may need access to the GLib object
 * pointers used to create the AccessPoint list. The APList module can provide
 * these object pointers, wrapped in LibNM::AccessPoint objects. These should
 * only be requested and used within the LibNM::Thread::Module thread.
 */
class Wifi::APList::Module : public Wifi::Module
{
public:
    /**
     * @brief  Creates the APList module. The constructed list module will be
     *         empty, and should be initialized with the updateAllAccessPoints
     *         function from within the LibNM thread.
     *
     * @param parentResource  The Wifi::Resource object instance.
     */
    Module(Resource& parentResource);

    /**
     * @brief  Clears all access point data on destruction.
     */
    virtual ~Module();

    /**
     * @brief  Gets AccessPoint objects for all visible access points.
     *
     * @return  The entire list of visible access points.
     */
    juce::Array<AccessPoint> getAccessPoints() const;

    /**
     * @brief  Finds a single AccessPoint using its hash value.
     *
     * @param apHash  An access point hash value.
     *
     * @return        The matching AccessPoint, or a null AccessPoint if no
     *                match is found.
     */
    AccessPoint getAccessPoint(LibNM::APHash apHash) const;

    /**
     * @brief  Gets the strongest visible LibNM::AccessPoint represented by an
     *         AccessPoint object.
     *
     *  This function should only be called on the thread created by the
     * Wifi::LibNM::Thread::Module object.
     *
     * @param accessPoint  A non-null AccessPoint object provided by the APList
     *                     module.
     *
     * @return             A LibNM::AccessPoint object described by the
     *                     accessPoint object. If multiple LibNM::AccessPoint
     *                     objects match accessPoint, the one with the
     *                     strongest signal strength is returned. If no match
     *                     is found, a null LibNM::AccessPoint is returned.
     */
    LibNM::AccessPoint getStrongestNMAccessPoint(const AccessPoint accessPoint)
        const;

    /**
     * @brief  Gets the list of all LibNM::AccessPoint objects provided by
     *         LibNM.
     *
     *  This function should only be called on the thread created by the
     * Wifi::LibNM::Thread::Module object.
     *
     * @return  All distinct, non-null LibNM::AccessPoint objects.
     */
    juce::Array<LibNM::AccessPoint> getNMAccessPoints() const;

    /**
     * @brief  Gets all LibNM::AccessPoint objects that are represented by an
     *         AccessPoint object.
     *
     *  This function should only be called on the thread created by the
     * Wifi::LibNM::Thread::Module object.
     *
     * @param accessPoint  An AccessPoint representing one or more
     *                     LibNM::AccessPoint objects.
     *
     * @return             All unique LibNM::AccessPoint objects represented by
     *                     accessPoint.
     */
    juce::Array<LibNM::AccessPoint> getNMAccessPoints
        (const AccessPoint accessPoint) const;

    /**
     * @brief  Adds a new LibNM::AccessPoint to the list, creating an
     *         AccessPoint object to represent it if necessary.
     *
     *  This function should only be called on the thread created by the
     * Wifi::LibNM::Thread::Module object.
     *
     * @param addedAP  A new LibNM::AccessPoint provided by LibNM.
     */
    void addAccessPoint(const LibNM::AccessPoint addedAP);

    /**
     * @brief  Removes a LibNM::AccessPoint from the list.
     *
     *  This function should only be called on the thread created by the
     * Wifi::LibNM::Thread::Module object.
     *
     *  If this was the only remaining LibNM::AccessPoint represented by an
     * AccessPoint object, that AccessPoint object's signal strength is set to
     * zero and it will no longer be shared outside this module.
     *
     * @param removedAP  The LibNM::AccessPoint that NetworkResource can no
     *                   longer find.
     */
    void removeAccessPoint(const LibNM::AccessPoint removedAP);

    /**
     * @brief  Updates the signal strength of an AccessPoint, setting it to the
     *         strongest signal strength of any matching LibNM::AccessPoint
     *         object.
     *
     * @param toUpdate  The AccessPoint to update.
     */
    void updateSignalStrength(AccessPoint toUpdate);

    /**
     * @brief  Clears the list of saved AccessPoint and LibNM::AccessPoint
     *         objects.
     */
    void clearAccessPoints();

    /**
     * @brief  Reloads all LibNM::AccessPoint objects from LibNM, updating the
     *         list of AccessPoint objects as necessary.
     */
    void updateAllAccessPoints();

    /**
     * @brief  Removes all LibNM::AccessPoint objects that are no longer valid.
     */
    void removeInvalidatedAccessPoints();

private:
    /**
     * @brief  Sets an AccessPoint object's signal strength to zero and
     *         notifies all listeners that it has been removed, if and only if
     *         all LibNM::AccessPoint objects represented by the access point
     *         have been removed.
     *
     *  This function should only be called on the thread created by the
     * Wifi::LibNM::Thread::Module object.
     *
     * @param toCheck  The AccessPoint object to check and potentially update.
     */
    void updateAPIfLost(AccessPoint toCheck);
};
