#pragma once
#include <nm-connection.h>
#include "GPPObject.h"

/**
 * @file NMPPConnection.h
 * 
 * @brief A RAII container and C++ interface for LibNM NMConnection objects.
 */

class NMPPConnection : public GPPObject
{
public:
    /**
     * Creates a NMPPConnection sharing a GObject with an existing
     * NMPPConnection.
     * 
     * @toCopy  An existing connection object.
     */
    NMPPConnection(const NMPPConnection& toCopy);
    
    /**
     * Creates a NMPPConnection to contain a NMConnection object.
     * 
     * @toAssign  A valid NMConnection for this NMPPConnection to hold.
     */
    NMPPConnection(NMConnection* toAssign);
    
    /**
     * Creates a null NMPPConnection.
     */
    NMPPConnection() { }
    
    virtual ~NMPPConnection() { }
    
    /**
     * Check if this connection object and another could be describing the 
     * same network connection.
     * 
     * @param rhs  Another connection to compare with this one.
     * 
     * @return  true iff this connection object and rhs could be describing the
     *          same network connection.
     */
    bool connectionMatches(const NMPPConnection& rhs) const;
    
    /**
     * Add a new connection setting to this connection.  If the connection is
     * null, this will create a new NMConnection object.
     * 
     * @param setting  A valid NMSetting object.  Ownership of this setting
     *                 will be transferred to the NMConnection object.
     */
    void addSetting(NMSetting* setting);
    
    /**
     * Remove one of the connection settings from this connection.
     * 
     * @param settingType  A setting type to search for in the connection.  If a
     *                     matching NMSetting object is found, it will be
     *                     removed and unreferenced.
     */
    void removeSetting(GType settingType);
    
    /**
     * Get one of this connection's setting objects.
     * 
     * @param settingType  A setting type to search for in the connection.
     * 
     * @return  the requested NMSetting object if one is stored with this type,
     *          or nullptr if no matching setting is found.
     */
    NMSetting* getSetting(GType settingType) const;
    
    /**
     * Check the validity of this connection.
     * 
     * @param error  The address of a GError*, set to nullptr.  If an error 
     *               occurs, a GError will be created and *error will be set to
     *               its address.  If this occurs, the error will need to be
     *               cleared with g_error_clear()
     * 
     * @return  true iff this connection's settings define a valid network
     *          connection.
     */
    bool verify(GError** error) const;
    
    /**
     * Set the connection path stored by this object.  If the connection is
     * null, this will create a new NMConnection object.
     * 
     * @path  The DBus path where this connection is saved.  This value has no
     *        effect on the actual DBus path, and the caller is entirely
     *        responsible for making sure that this is the correct path value.
     */
    void setPath(const char* path);
    
    /**
     * Get the connection path stored by this object.
     * 
     * @return  the connection's stored path, or the empty string if no path is 
     *          stored.
     */
    const char* getPath() const;

    /**
     * Get a unique ID string for this connection.
     * 
     * @return the connection's UUID string, or the empty string if the
     *         connection is null.
     */
    const char* getUUID() const;
    
    /**
     * Get the connection's NetworkManager ID string.
     * 
     * @return the connection's ID string, or the empty string if the connection
     *         is null.
     */
    const char* getID() const;
    
private:
    /**
     * Get the GType of the stored NMConnection object.
     * 
     * @return NM_TYPE_CONNECTION
     */
    GType getType() const override;
    
    /**
     * Check if a GObject's type allows it to be held by this object. 
     * 
     * @param toCheck  Any valid GObject, or nullptr.
     * 
     * @return  true iff toCheck is a NMConnection, a NMActiveConnection,
     *          or is null. 
     */
    virtual bool isValidType(GObject* toCheck) const override;
};