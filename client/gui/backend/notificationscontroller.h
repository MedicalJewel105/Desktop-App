#ifndef NOTIFICATIONSCONTROLLER_H
#define NOTIFICATIONSCONTROLLER_H

#include <QObject>
#include <QSet>
#include <QVector>
#include "types/notification.h"

class NotificationsController : public QObject
{
    Q_OBJECT
public:
    explicit NotificationsController(QObject *parent = nullptr);
    virtual ~NotificationsController() {}
    void shutdown();

    int totalMessages() const;
    int unreadMessages() const;

    QVector<types::Notification> messages() const;
    const QSet<qint64> &shownIds() const;

public slots:
    void updateNotifications(const QVector<types::Notification> &arr);
    void setNotificationReaded(qint64 notificationId);

signals:
    void stateChanged(int totalMessages, int unread);
    void newPopupMessage(int messageId);

private:
    QVector<types::Notification> notifications_;
    QSet<qint64> idOfShownNotifications_;
    QVector<int> unreadPopupNotificationIds_;

    int latestTotal_;
    int latestUnreadCnt_;

    void saveToSettings();
    void readFromSettings();
    void updateState();
    void checkForUnreadPopup();
};

#endif // NOTIFICATIONSCONTROLLER_H
