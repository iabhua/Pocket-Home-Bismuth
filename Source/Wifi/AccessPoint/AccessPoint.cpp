#include "Wifi/AccessPoint/AccessPoint.h"
#include "Wifi/AccessPoint/APData.h"
#include "LibNM/NMObjects/AccessPoint.h"
#include "LibNM/APData/APHash.h"
#include "LibNM/APData/APMode.h"
#include "LibNM/APData/SecurityType.h"
#include "LibNM/ThreadHandler.h"


/*
 * Creates new access point data from a LibNM access point object.
 */
Wifi::AccessPoint::AccessPoint(const LibNM::AccessPoint nmAccessPoint)
{
    LibNM::ThreadHandler nmThread;
    nmThread.call([this, &nmAccessPoint]()
    {
        LibNM::APHash hash = nmAccessPoint.generateHash();
        getDataReference() = new APData(nmAccessPoint, hash);
    });
}

/*
 * Initializes the AccessPoint with another AccessPoint's data.
 */
 Wifi::AccessPoint::AccessPoint(const AccessPoint& rhs) : Nullable(rhs) { }

/*
 * Compares AccessPoint objects using their hash values.
 */
bool Wifi::AccessPoint::operator==(const AccessPoint& rhs) const
{
    if(isNull())
    {
        return rhs.isNull();
    }
    if(rhs.isNull())
    {
        return false;
    }
    return *getData() == *rhs.getData();
}

/*
 * Compares AccessPoint objects using their hash values.
 */
bool Wifi::AccessPoint::operator!=(const AccessPoint& rhs) const
{
    return !(*this == rhs);
}

/*
 * Compares AccessPoint objects using their hash values so they can be sorted.
 */
bool Wifi::AccessPoint::operator<(const AccessPoint& rhs) const
{
    if(isNull())
    {
        return false;
    }
    if(rhs.isNull())
    {
        return true;
    }
    return *getData() < *rhs.getData();
}

/*
 * Gets the access point's primary identifier.
 */
juce::String Wifi::AccessPoint::getSSID() const
{
    if(isNull())
    {
        return juce::String();
    }
    return getData()->getSSID();
}

/*
 * Gets the access point's hardware identifier.
 */
juce::String Wifi::AccessPoint::getBSSID() const
{
    if(isNull())
    {
        return juce::String();
    }
    return getData()->getBSSID();
}

/*
 * Gets the access point's signal strength.
 */
unsigned int Wifi::AccessPoint::getSignalStrength() const
{
    if(isNull())
    {
        return 0;
    }
    return getData()->getSignalStrength();
}

/*
 * Gets the access point's general security type.
 */
LibNM::SecurityType Wifi::AccessPoint::getSecurityType() const
{
    if(isNull())
    {
        return LibNM::SecurityType::unsecured;
    }
    return getData()->getSecurityType();
}

/*
 * Gets the hash value used to identify and sort the access point.
 */
LibNM::APHash Wifi::AccessPoint::getHashValue() const
{
    if(isNull())
    {
        return LibNM::APHash(nullptr, LibNM::APMode::unknown,
                LibNM::SecurityType::unsecured); 
    }
    return getData()->getHashValue();
}

/*
 * Updates the access point's signal strength.
 */
void Wifi::AccessPoint::setSignalStrength(const unsigned int newStrength)
{
    if(!isNull())
    {
        getData()->setSignalStrength(newStrength);
    }
}