#pragma once

#include "types/enums.h"

#include <QDebug>
#include <QJsonObject>
#include <QSettings>

namespace types {

struct ShareProxyGateway
{
    ShareProxyGateway() = default;

    ShareProxyGateway(const QJsonObject &json)
    {
        if (json.contains(kJsonIsEnabledProp) && json[kJsonIsEnabledProp].isBool()) {
            isEnabled = json[kJsonIsEnabledProp].toBool();
        }

        if (json.contains(kJsonProxySharingModeProp) && json[kJsonProxySharingModeProp].isDouble()) {
            proxySharingMode = PROXY_SHARING_TYPE_fromInt(json[kJsonProxySharingModeProp].toInt());
        }

        if (json.contains(kJsonPortProp) && json[kJsonPortProp].isDouble()) {
            uint p = static_cast<uint>(json[kJsonPortProp].toInt());
            if (p < 65536) {
                port = p;
            }
        }
    }

    bool isEnabled = false;
    PROXY_SHARING_TYPE proxySharingMode = PROXY_SHARING_HTTP;
    uint port = 0;

    bool operator==(const ShareProxyGateway &other) const
    {
        return other.isEnabled == isEnabled &&
               other.proxySharingMode == proxySharingMode &&
               other.port == port;
    }

    bool operator!=(const ShareProxyGateway &other) const
    {
        return !(*this == other);
    }

    void fromIni(const QSettings &settings)
    {
        isEnabled = settings.value(kIniIsEnabledProp, isEnabled).toBool();
        proxySharingMode = PROXY_SHARING_TYPE_fromString(settings.value(kIniProxySharingModeProp, PROXY_SHARING_TYPE_toString(proxySharingMode)).toString());
        uint p = settings.value(kIniPortProp, port).toUInt();
        if (p < 65536) {
            port = p;
        }
    }

    void toIni(QSettings &settings) const
    {
        settings.setValue(kIniIsEnabledProp, isEnabled);
        settings.setValue(kIniProxySharingModeProp, PROXY_SHARING_TYPE_toString(proxySharingMode));
        settings.setValue(kIniPortProp, port);
    }

    QJsonObject toJson() const
    {
        QJsonObject json;
        json[kJsonIsEnabledProp] = isEnabled;
        json[kJsonProxySharingModeProp] = static_cast<int>(proxySharingMode);
        json[kJsonPortProp] = static_cast<int>(port);
        return json;
    }

    friend QDataStream& operator <<(QDataStream &stream, const ShareProxyGateway &o)
    {
        stream << versionForSerialization_;
        stream << o.isEnabled << o.proxySharingMode << o.port;
        return stream;
    }

    friend QDataStream& operator >>(QDataStream &stream, ShareProxyGateway &o)
    {
        quint32 version;
        stream >> version;
        if (version > o.versionForSerialization_)
        {
            stream.setStatus(QDataStream::ReadCorruptData);
            return stream;
        }
        stream >> o.isEnabled >> o.proxySharingMode;
        if (version >= 2) {
            stream >> o.port;
        }
        return stream;
    }

    friend QDebug operator<<(QDebug dbg, const ShareProxyGateway &s)
    {
        QDebugStateSaver saver(dbg);
        dbg.nospace();
        dbg << "{isEnabled:" << s.isEnabled << "; ";
        dbg << "proxySharingMode:" << PROXY_SHARING_TYPE_toString(s.proxySharingMode) << "; ";
        dbg << "port:" << s.port << "}";
        return dbg;
    }

private:
    static const inline QString kIniIsEnabledProp = "ShareProxyGatewayEnabled";
    static const inline QString kIniProxySharingModeProp = "ShareProxyGatewayMode";
    static const inline QString kIniPortProp = "ShareProxyGatewayPort";

    static const inline QString kJsonIsEnabledProp = "isEnabled";
    static const inline QString kJsonProxySharingModeProp = "proxySharingMode";
    static const inline QString kJsonPortProp = "port";

    static constexpr quint32 versionForSerialization_ = 2;  // should increment the version if the data format is changed
};

} // types namespace
