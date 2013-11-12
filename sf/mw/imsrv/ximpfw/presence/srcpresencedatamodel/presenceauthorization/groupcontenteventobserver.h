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
* Description:  Event observer interface for group content subscription.
*
*/

#ifndef MGROUPCONTENTEVENTOBSERVER_H
#define MGROUPCONTENTEVENTOBSERVER_H

#include "presencetypehelpers.h"

/**
 * Event observer interface for group content changes.
 *
 * @since S60 v3.2
 */
class MGroupContentEventObserver
    {
public: // Interface

    /** 
     * Handle event of added and removed items.
     */
    virtual void HandleChangeL( RPrGrpMemInfoImpArray& aAdded, RPrGrpMemInfoImpArray& aRemoved ) = 0;
    virtual void HandleSynthesiseL( RPrGrpMemInfoImpArray& aAdded ) = 0;

protected: // Destruction

    /**
     * Object cannot be deleted through this interface
     */
    virtual ~MGroupContentEventObserver(){}
    };

/* ======================================================================== */



#endif // MGROUPCONTENTEVENTOBSERVER_H
