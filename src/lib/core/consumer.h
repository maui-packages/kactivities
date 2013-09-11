/*
 * Copyright (c) 2010, 2011, 2012 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License version 2 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef ACTIVITIES_CONSUMER_H
#define ACTIVITIES_CONSUMER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QScopedPointer>

#include "info.h"

#include "kactivities_export.h"

namespace KActivities {

class ConsumerPrivate;

/**
 * Contextual information can be, from the user's point of view, divided
 * into three aspects - "who am I?", "where am I?" (what are my surroundings?)
 * and "what am I doing?".
 *
 * Activities deal with the last one - "what am I doing?". The current activity
 * refers to what the user is doing at the moment, while the other activities
 * represent things that he/she was doing before, and probably will be doing
 * again.
 *
 * Activity is an abstract concept whose meaning can differ from one user to
 * another. Typical examples of activities are "developing a KDE project",
 * "studying the 19th century art", "composing music", "lazing on a Sunday
 * afternoon" etc.
 *
 * Consumer provides read-only information about activities.
 *
 * Before relying on the values retrieved by the class, make sure that the
 * serviceStatus is set to Running. Otherwise, you can get invalid data either
 * because the service is not functioning properly (or at all) or because
 * the class did not have enough time to synchronize with it.
 *
 * For example, if this is the only existing instance of the Consumer class,
 * the listActivities method will return an empty list.
 *
 * @code
 * void someMethod() {
 *     Consumer c;
 *     doSomethingWith(c.listActivities());
 * }
 * @endcode
 *
 * Instances of the Consumer class should be long-lived. For example, members
 * of the classes that use them.
 *
 * @since 4.5
 */
class KACTIVITIES_EXPORT Consumer : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString currentActivity READ currentActivity NOTIFY currentActivityChanged)
    Q_PROPERTY(QStringList activities READ activities NOTIFY activitiesChanged)
    Q_PROPERTY(ServiceStatus serviceStatus READ serviceStatus NOTIFY serviceStatusChanged)

public:
    /**
     * Different states of the activities service
     */
    enum ServiceStatus {
        NotRunning, ///< Service is not running
        Unknown,    ///< Unable to determine the status of the service
        Running     ///< Service is running properly
    };

    explicit Consumer(QObject *parent = Q_NULLPTR);

    ~Consumer();

    /**
     * @returns the id of the current activity
     * @note Activity ID is a UUID-formatted string. If the activities
     *     service is not running, or there was some error, the
     *     method will return null UUID. The ID can also be an empty
     *     string in the case there is no current activity.
     * @note This method is <b>pre-fetched and cached</b>
     */
    QString currentActivity() const;

    /**
     * @returns the list of activities filtered by state
     * @param state state of the activity
     * @note If the activities service is not running, only a null
     *     activity will be returned.
     * @note This method is <b>pre-fetched and cached only for the Info::Running state</b>
     */
    QStringList activities(Info::State state) const;

    /**
     * @returns the list of all existing activities
     * @note If the activities service is not running, only a null
     *     activity will be returned.
     * @note This method is <b>pre-fetched and cached</b>
     */
    QStringList activities() const;

    /**
     * @returns status of the activities service
     */
    ServiceStatus serviceStatus();

Q_SIGNALS:
    /**
     * This signal is emitted when the global
     * activity is changed
     * @param id id of the new current activity
     */
    void currentActivityChanged(const QString &id);

    /**
     * This signal is emitted when the activity service
     * goes online or offline
     * @param status new status of the service
     */
    void serviceStatusChanged(Consumer::ServiceStatus status);

    /**
     * This signal is emitted when a new activity is added
     * @param id id of the new activity
     */
    void activityAdded(const QString &id);

    /**
     * This signal is emitted when the activity
     * is removed
     * @param id id of the removed activity
     */
    void activityRemoved(const QString &id);

    /**
     * This signal is emitted when the activity list changes
     * @param activities list of activities
     */
    void activitiesChanged(const QStringList & activities);

private:
    const QScopedPointer<ConsumerPrivate> d;
};

} // namespace KActivities

#endif // ACTIVITIES_CONSUMER_H
