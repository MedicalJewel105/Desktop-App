#pragma once

#include "../../../posix_common/helper_commands.h"
#include "bound_route.h"
#include "routes.h"

// state-based route management, depends on parameters isConnected, isSplitTunnelActive,
// exclusive/inclusive split tunneling mode
class RoutesManager
{
public:
    RoutesManager();
    void updateState(const CMD_SEND_CONNECT_STATUS &connectStatus, bool isSplitTunnelActive, bool isExcludeMode);

private:
    CMD_SEND_CONNECT_STATUS connectStatus_;
    bool isSplitTunnelActive_;
    bool isExcludeMode_;

    BoundRoute boundRoute_;
    Routes dnsServersRoutes_;
    Routes vpnRoutes_;

    void addDnsRoutes(const CMD_SEND_CONNECT_STATUS &connectStatus);
    void addIkev2RoutesForInclusiveMode(const CMD_SEND_CONNECT_STATUS &connectStatus);
    void clearAllRoutes();
};
