#pragma once
/**
 * @file  Wifi/Connection/ConnectionUpdateInterface.h
 *
 * @brief  The interface used by the Wifi::ConnectionTracker to notify all
 *         Wifi::ConnectionListener objects of new Wifi network connection 
 *         events.
 */

namespace Wifi { class ConnectionUpdateInterface; }
namespace Wifi { class AccessPoint; }

class Wifi::ConnectionUpdateInterface
{
public:
    ConnectionUpdateInterface() { }

    virtual ~ConnectionUpdateInterface() { }

private:
    /**
     * @brief  Signals that a new active Wifi connection is being opened.
     *
     * @param connectingAP  The Wifi access point the system is trying to 
     *                      connect to.
     */
    virtual void startedConnecting(const AccessPoint connectingAP) = 0;

    /**
     * @brief  Signals that an attempt to open a Wifi connection failed due to
     *         improper authentication, usually because of an incorrect Wifi
     *         password.
     *
     * @param connectingAP  The access point the system was attempting to use
     *                      to open the new connection.
     */
    virtual void connectionAuthFailed(const AccessPoint connectingAP) = 0;

    /**
     * @brief  Signals that a new Wifi connection was opened successfully.
     *
     * @param connectedAP  The access point used to open the connection.
     */
    virtual void connected(const AccessPoint connectedAP) = 0; 

    /**
     * @brief  Signals that an active Wifi connection was closed.
     *
     * @param disconnectedAP  The access point that was being used by the closed
     *                        connection.
     */
    virtual void disconnected(const AccessPoint disconnectedAP) = 0; 
};
