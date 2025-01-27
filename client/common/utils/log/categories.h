#pragma once

#include <QLoggingCategory>

// log categories
Q_DECLARE_LOGGING_CATEGORY(LOG_BASIC)
Q_DECLARE_LOGGING_CATEGORY(LOG_CLI_IPC)
Q_DECLARE_LOGGING_CATEGORY(LOG_IPC)
Q_DECLARE_LOGGING_CATEGORY(LOG_CONNECTION)
Q_DECLARE_LOGGING_CATEGORY(LOG_FAILOVER)
Q_DECLARE_LOGGING_CATEGORY(LOG_NETWORK)
Q_DECLARE_LOGGING_CATEGORY(LOG_PING)
Q_DECLARE_LOGGING_CATEGORY(LOG_OPENVPN)
Q_DECLARE_LOGGING_CATEGORY(LOG_IKEV2)
Q_DECLARE_LOGGING_CATEGORY(LOG_LOCAL_WEBSERVER)
Q_DECLARE_LOGGING_CATEGORY(LOG_SOCKS_SERVER)
Q_DECLARE_LOGGING_CATEGORY(LOG_HTTP_SERVER)
Q_DECLARE_LOGGING_CATEGORY(LOG_WIFI_SHARED)
Q_DECLARE_LOGGING_CATEGORY(LOG_NETWORK_EXTENSION_MAC)
Q_DECLARE_LOGGING_CATEGORY(LOG_EMERGENCY_CONNECT)
Q_DECLARE_LOGGING_CATEGORY(LOG_FIREWALL_CONTROLLER)
Q_DECLARE_LOGGING_CATEGORY(LOG_BEST_LOCATION)
Q_DECLARE_LOGGING_CATEGORY(LOG_WSTUNNEL)
Q_DECLARE_LOGGING_CATEGORY(LOG_CTRLD)
Q_DECLARE_LOGGING_CATEGORY(LOG_CUSTOM_OVPN)
Q_DECLARE_LOGGING_CATEGORY(LOG_WIREGUARD)
Q_DECLARE_LOGGING_CATEGORY(LOG_PACKET_SIZE)
Q_DECLARE_LOGGING_CATEGORY(LOG_DOWNLOADER)
Q_DECLARE_LOGGING_CATEGORY(LOG_AUTO_UPDATER)
Q_DECLARE_LOGGING_CATEGORY(LOG_LAUNCH_ON_STARTUP)
Q_DECLARE_LOGGING_CATEGORY(LOG_CONNECTED_DNS)
Q_DECLARE_LOGGING_CATEGORY(LOG_AUTH_HELPER)
Q_DECLARE_LOGGING_CATEGORY(LOG_ASSERT)
Q_DECLARE_LOGGING_CATEGORY(LOG_SPLIT_TUNNEL_EXTENSION)

// for GUI
Q_DECLARE_LOGGING_CATEGORY(LOG_USER)
Q_DECLARE_LOGGING_CATEGORY(LOG_LOCATION_LIST)
Q_DECLARE_LOGGING_CATEGORY(LOG_PREFERENCES)

// common for CLI
Q_DECLARE_LOGGING_CATEGORY(LOG_CLI)
