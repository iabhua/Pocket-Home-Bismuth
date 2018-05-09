#include "SavedConnections.h"


#define BUS_NAME "org.freedesktop.NetworkManager"
#define PATH "/org/freedesktop/NetworkManager/Settings"
#define INTERFACE "org.freedesktop.NetworkManager.Settings"

//methods
#define LIST_CONNECTIONS "ListConnections"

SavedConnections::SavedConnections() :
GPPDBusProxy(BUS_NAME, PATH, INTERFACE) 
{ 
    StringArray paths = getConnectionPaths();
    for(const String& path : paths)
    {
        connectionList.add(SavedConnection(path.toRawUTF8()));
    }
    DBG("SavedConnections::" << __func__  << ": Found "
            << connectionList.size() << " connections.");
}

/*
 * Reads all connection paths from NetworkManager, and returns all the wifi
 * connections as SavedConnection objects.
 */
Array<SavedConnection> SavedConnections::getWifiConnections() const
{
    Array<SavedConnection> connections;
    for(const SavedConnection& con : connectionList)
    {
        if(con.isWifiConnection())
        {
            connections.add(con);
        }
    }
    DBG("SavedConnections::" << __func__  << ": Found "
            << connections.size() << " wifi connections.");
    return connections;
}

    
/*
 * Checks saved connection paths to see if one exists at the given path.
 */
bool SavedConnections::connectionExists(const String& connectionPath) const
{
    return connectionPaths.contains(connectionPath);
}
 
/*
 * Finds a saved connection from its path.  If no matching connection is
 * already loaded, the saved connection list will be updated in case the
 * requested connection was recently added.
 */
SavedConnection SavedConnections::getConnection(const String& connectionPath)
{
    if(!connectionExists(connectionPath))
    {
        updateSavedConnections();
    }
    if(connectionExists(connectionPath))
    {
        for(const SavedConnection& con : connectionList)
        {
            if(con.getPath() == connectionPath)
            {
                return con;
            }
        }
    }
    return SavedConnection();
}
  
/*
 * Finds all saved connections that are compatible with a given wifi
 * access point.
 */
Array<SavedConnection> SavedConnections::findConnectionsForAP
(const NMPPAccessPoint& accessPoint) const
{
    Array<SavedConnection> compatible;
    if(!isNull() && !accessPoint.isNull())
    {
        Array<SavedConnection> wifiCons = getWifiConnections();
        for(const SavedConnection& con : wifiCons)
        {
            if(accessPoint.isValidConnection(con.getNMConnection()))
            {
                compatible.add(con);
            }
        }
    }
    return compatible;
}
    
/*
 * Get the list of all available connection paths
 */
inline StringArray SavedConnections::getConnectionPaths()
{
    using namespace GVariantConverter;
    GVariant* conArrayVar = callMethod(LIST_CONNECTIONS);
    if(conArrayVar != nullptr)
    {
        StringArray paths = getValue<StringArray>(conArrayVar);
        g_variant_unref(conArrayVar);
        conArrayVar = nullptr;
        return paths;
    }
    return StringArray();
}

/*
 * Check the list of saved connections against an updated connection path
 * list, adding any new connections and removing any deleted connections.
 */
void SavedConnections::updateSavedConnections()
{
    connectionPaths = getConnectionPaths();
    Array<SavedConnection> toRemove;
    for(const SavedConnection& saved : connectionList)
    {
        if(!connectionPaths.contains(saved.getPath()))
        {
            toRemove.add(saved);
        }
        else
        {
            connectionPaths.removeString(saved.getPath());
        }
    }
    for(const SavedConnection& removing : toRemove)
    {
        connectionList.removeAllInstancesOf(removing);
    }
    for(const String& path : connectionPaths)
    {
        connectionList.add(SavedConnection(path.toRawUTF8()));
    }
}
