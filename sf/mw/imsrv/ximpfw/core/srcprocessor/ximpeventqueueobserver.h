/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: Observer for Event queue 
*
*/

#ifndef MXIMPEVENTQUEUEOBSERVER_H
#define MXIMPEVENTQUEUEOBSERVER_H

#include <e32std.h>

/**
 * Event queue observer.
 *
 * @since S60 v3.2
 */
class MXIMPEventQueueObserver
    {
protected:

    /**
     * Protected destructor. Observer instancies
     * can't be destroyed via this interface.
     */
    virtual ~MXIMPEventQueueObserver() {}

public:

    /**
     * Notifies observer that new event was
     * added to event queue and it is
     * available for observer consumption.
     */
    virtual void NewEventAvailable() = 0;

    };

#endif // MXIMPEVENTQUEUEOBSERVER_H



