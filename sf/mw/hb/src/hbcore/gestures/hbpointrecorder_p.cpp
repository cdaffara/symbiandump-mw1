/****************************************************************************
**
** Copyright (C) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (developer.feedback@nokia.com)
**
** This file is part of the HbCore module of the UI Extensions for Mobile.
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at developer.feedback@nokia.com.
**
****************************************************************************/

#include "hbpointrecorder_p.h"

#include <QPair>
#include <QTime>
#include <QDebug>

//#define POINT_DEBUG
#ifndef POINT_DEBUG
# define DEBUG if (0) qDebug
#else
# define DEBUG qDebug
#endif

/*!
   @hbcore
   \internal
   \class HbPointRecorder

   \brief Class to store and hold list of points and timestamps.

*/

/*!
    \internal
    \brief Constructor for HbPointRecorder
    \return

*/
HbPointRecorder::HbPointRecorder()
    :
    mThreshold(0)
{
}

/*!
    \internal
    \brief Destructor for HbPointRecorder
    \return

*/
HbPointRecorder::~HbPointRecorder()
{
}

/*!
    \internal
    \brief Clears all data from the recorder
    \param threshold set used threshold to detect movement and stops
*/
void HbPointRecorder::resetRecorder(qreal threshold)
{
    mThreshold = threshold;
    clear();
}

/*!
    \internal
    \brief Records point to list with timestamp.
    \param point Point to be recorded.
    \param time Time to be recorded.
    \return Nothing.

*/
void HbPointRecorder::record(qreal point, const QTime &time)
{
    // Empty list always accepts first point without tests.
    if ( !isEmpty() ) {
        // No point to record a point, if timestamp is less or equal with previous.
        if ( lastTime().msecsTo(time) == 0 ) {
            DEBUG() << "Ignoring point, because no difference in time stamps.";
            return;
        }

        // Don't tolerate points, which are too close to previously recorded point.
        if ( qAbs(lastPoint() - point) < mThreshold ) {
            DEBUG() << "Ignoring point, because it is withing threshold of previous point";
            return;
        }
    }

    // In case the list contains two or more points, direction can be
    // determined. Each new point added needs to be checked for direction
    // change.
    if ( length() > 1 ) {
        // Clear list, on direction change. Leave the last recorded point
        // to the list, as it can be considered as first point for new direction.
        if ( dirChanged( point ) ) {
            HbPointTime temp = last();
            clear();
            append(temp);
        }
    }

    // Finally check, if the position has changed. Don't record point, when no position
    // change.
    if ( isEmpty() || point != lastPoint() ) {
        // Add point and time to list.
        append(HbPointTime(point, time));
    } else {
        DEBUG() << "Ignoring point, because it equals previous.";
    }
}

/*!
    \internal
    \brief
    \return Last recorded point.

*/
qreal HbPointRecorder::lastPoint() const
{
    Q_ASSERT(!isEmpty());
    return last().first;
}

/*!
    \internal
    \brief
    \return Last recorded timestamp.

*/
const QTime& HbPointRecorder::lastTime() const
{
    Q_ASSERT(!isEmpty());
    return last().second;
}

/*!
    \internal
    \brief Checks if new point causes direction change.
    \param point The point suspected cause direction change.
    \return True, when direction changes.
*/
bool HbPointRecorder::dirChanged( qreal point ) const
{
    // Without direction, the direction cannot really change
    if ( length() < 2 ) { return false; }

    qreal pos0 = (*this)[-2].first;
    qreal pos1 = (*this)[-1].first;
    qreal dir0 = pos1 - pos0;
    qreal dir1 = point - pos1;

    // Don't react on movement, when changes happen within threshold
    if ( qAbs(dir1) <= mThreshold ) { return false; }

    // Check for '+' and '-' -signs in directions. Opposite signs means
    // direction change.
    return ( dir0 < 0 && dir1 >= 0 ) || ( dir0 > 0 && dir1 <= 0 );
}

/*!
    \internal
    \brief Operator to access the list.

    Supports negative values, which will define order from the end.
*/
HbPointTime HbPointRecorder::operator[]( int number ) const
{
    // Let's trust, that QList::at() takes care of validity checks.
    if ( number < 0 ) {
        number = count() + number;
    }
    return at(number);
}


/*!
    \internal
    \copydoc HbPointRecorder::getLastRecords
*/
QList<HbPointTime> HbPointRecorder::getLastRecords( int number ) const
{
    QList<HbPointTime> recordings;
    number = number > count() ? count() : number;
    for (int i=count()-number; i < count(); i++) {
        recordings.append((*this)[i]);
    }

    return recordings;
}
