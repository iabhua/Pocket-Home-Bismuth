#include "LibNM/NMObjects/Connection.h"
#include "GLib/SmartPointers/ObjectPtr.h"

/* Rename smart pointers for brevity: */
typedef GLib::ObjectPtr<NMConnection*> NMConnectionPtr;

/*
 * Creates a Connection sharing a GObject with an existing Connection.
 */
LibNM::Connection::Connection(const Connection& toCopy) :
LibNM::Object(toCopy, NM_TYPE_CONNECTION) 
{
    ASSERT_CORRECT_CONTEXT;
}

/*
 * Creates a Connection to contain a NMConnection object.
 */
LibNM::Connection::Connection(NMConnection* toAssign) :
LibNM::Object(NM_OBJECT(toAssign), NM_TYPE_CONNECTION) 
{ 
    ASSERT_CORRECT_CONTEXT;
}
    
/*
 * Creates a null Connection.
 */
LibNM::Connection::Connection() : LibNM::Object(NM_TYPE_CONNECTION) { }

/*
 * Checks if this connection object and another could be describing the same 
 * network connection.
 */
bool LibNM::Connection::connectionMatches(const Connection& rhs) const
{
    ASSERT_CORRECT_CONTEXT;
    NMConnectionPtr self(NM_CONNECTION(getGObject()));
    NMConnectionPtr toCompare(NM_CONNECTION(rhs.getGObject()));
    if(self == toCompare)
    {
        return true;
    }
    else if(self == nullptr || toCompare == nullptr)
    {
        return false;
    }
    return nm_connection_compare(self, toCompare,
            NM_SETTING_COMPARE_FLAG_FUZZY);
}
   

/*
 * Adds a new connection setting to this connection.
 */
void LibNM::Connection::addSetting(NMSetting* setting)
{
    ASSERT_CORRECT_CONTEXT;
    if(isNull())
    {
        setGObject(G_OBJECT(nm_connection_new()));
    }
    NMConnectionPtr connection(NM_CONNECTION(getGObject()));
    jassert(connection != nullptr);
    nm_connection_add_setting(connection, setting);
}

/*
 * Removes one of the connection settings from this connection.
 */
void LibNM::Connection::removeSetting(GType settingType)
{
    ASSERT_CORRECT_CONTEXT;
    NMConnectionPtr connection(NM_CONNECTION(getGObject()));
    if(connection != nullptr)
    {
        nm_connection_remove_setting(connection, settingType);
    }
}
    
/*
 * Adds new wireless connection settings to this connection.
 */
void LibNM::Connection::addWifiSettings(const GByteArray* ssid, bool isHidden)
{
    ASSERT_CORRECT_CONTEXT;
    if(ssid != nullptr)
    {   
        NMSettingWireless* wifiSettings
                = (NMSettingWireless*) nm_setting_wireless_new();
        g_object_set(wifiSettings,
                NM_SETTING_WIRELESS_SSID,
                ssid,
                NM_SETTING_WIRELESS_HIDDEN,
                isHidden,
                nullptr);
        addSetting(NM_SETTING(wifiSettings));
    }
}

/*
 * Attempts to add WPA security settings to this connection.
 */
bool LibNM::Connection::addWPASettings(const juce::String& psk)
{
    ASSERT_CORRECT_CONTEXT;
    if(psk.length() < 8)
    {
        return false;
    }
    NMSettingWirelessSecurity* securitySettings
            = (NMSettingWirelessSecurity*) nm_setting_wireless_security_new();
    g_object_set(G_OBJECT(securitySettings),
            NM_SETTING_WIRELESS_SECURITY_PSK,
            psk.toRawUTF8(),
            NM_SETTING_WIRELESS_SECURITY_PSK_FLAGS,
            NM_SETTING_SECRET_FLAG_NONE, nullptr);
    addSetting(NM_SETTING(securitySettings));
    return true;
}

/*
 * Attempts to add WEP security settings to this connection.
 */
bool LibNM::Connection::addWEPSettings(const juce::String& psk)
{
    ASSERT_CORRECT_CONTEXT;
    const char* keyType = nullptr;
    if (psk.length() == 10 || psk.length() == 26)
    {
        keyType = (const char*) NM_WEP_KEY_TYPE_KEY;
    }
    //valid passphrase format: length 5 or length 14
    else if (psk.length() == 5 || psk.length() == 13)
    {
        keyType = (const char*) NM_WEP_KEY_TYPE_PASSPHRASE;
    }
    else
    {
        DBG("Connection::" << __func__
                << ": Invalid WEP Key type, "
                << "psk.length() = " << psk.length()
                << ", not in [5,10,13,26]");
        return false;
    }
    NMSettingWirelessSecurity* securitySettings
            = (NMSettingWirelessSecurity*) nm_setting_wireless_security_new();

    g_object_set(G_OBJECT(securitySettings),
            NM_SETTING_WIRELESS_SECURITY_WEP_KEY_TYPE,
            keyType,
            NM_SETTING_WIRELESS_SECURITY_PSK_FLAGS,
            NM_SETTING_SECRET_FLAG_NONE,  nullptr);

    nm_setting_wireless_security_set_wep_key
            (securitySettings, 0, psk.toRawUTF8());

    addSetting(NM_SETTING(securitySettings));
    return true;
}

/*
 * Gets one of this connection's setting objects.
 */
NMSetting* LibNM::Connection::getSetting(GType settingType) const
{
    ASSERT_CORRECT_CONTEXT;
    NMConnectionPtr connection(NM_CONNECTION(getGObject()));
    if(connection != nullptr)
    {
        return nm_connection_get_setting(connection, settingType);
    }
    return nullptr;
}

/*
 * Checks the validity of this connection.
 */
bool LibNM::Connection::verify(GError** error) const
{
    ASSERT_CORRECT_CONTEXT;
    NMConnectionPtr connection(NM_CONNECTION(getGObject()));
    if(connection != nullptr)
    {
        return nm_connection_verify(connection, error);
    }
    return false;
}

/*
 * Sets the connection path stored by this object.
 */
void LibNM::Connection::setPath(const char* path)
{
    ASSERT_CORRECT_CONTEXT;
    if(isNull())
    {
        setGObject(G_OBJECT(nm_connection_new()));
    }
    NMConnectionPtr connection(NM_CONNECTION(getGObject()));
    if(connection != nullptr)
    {
        nm_connection_set_path(connection, path);
    }
}

/*
 * Gets the connection path stored by this object.
 */
const char* LibNM::Connection::getPath() const
{
    ASSERT_CORRECT_CONTEXT;
    const char* path = "";
    NMConnectionPtr connection(NM_CONNECTION(getGObject()));
    if(connection != nullptr)
    {
        path = nm_connection_get_path(connection);
        if(path == nullptr)
        {
            path = "";
        }
    }
    return path;
}

/*
 * Gets a unique ID string for this connection.
 */
const char* LibNM::Connection::getUUID() const
{
    ASSERT_CORRECT_CONTEXT;
    const char* uuid = "";
    NMConnectionPtr connection(NM_CONNECTION(getGObject()));
    if(connection != nullptr)
    {
        uuid = nm_connection_get_uuid(connection);
        if(uuid == nullptr)
        {
            uuid = "";
        }
    }
    return uuid;
}

/*
 * Gets the connection's NetworkManager ID string.
 */
const char* LibNM::Connection::getID() const
{
    ASSERT_CORRECT_CONTEXT;
    const char* conId = "";
    NMConnectionPtr connection(NM_CONNECTION(getGObject()));
    if(connection != nullptr)
    {
        conId = nm_connection_get_id(connection);
        if(conId == nullptr)
        {
            conId = "";
        }
    }
    return conId;
}


#ifdef JUCE_DEBUG
/*
 * Prints all stored connection data to stdout.
 */
void LibNM::Connection::printDebugOutput() const
{
    ASSERT_CORRECT_CONTEXT;
    NMConnectionPtr connection(NM_CONNECTION(getGObject()));
    if(connection != nullptr)
    {
        nm_connection_dump(connection);
    }
}
#endif