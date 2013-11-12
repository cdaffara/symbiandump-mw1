/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  COrientationActionPlugIn class declaration.
*
*/


#ifndef C_ORIENTATIONACTIONPLUGIN_H
#define C_ORIENTATIONACTIONPLUGIN_H

#include <cfactionplugin.h>

class COrientationAction;

/**
 * Orientation action plug-in.
 * 
 * @lib -
 * @since S60 4.0
 */
NONSHARABLE_CLASS( COrientationActionPlugIn ): public CCFActionPlugIn
    {
public:

    // Two phased constructors
    static COrientationActionPlugIn* NewL();
    static COrientationActionPlugIn* NewLC();

    // Destructor
    ~COrientationActionPlugIn();

private: // From base classes

    // @see CCFActionPlugIn
    void InitializeL();

    // @see CCFActionPlugIn
    TExecutionTime ExecuteL( CCFActionIndication* aActionIndication );

    // @see CCFActionPlugIn
    void GetActionsL( CDesCArray& aActionList ) const;

    // @see CCFActionPlugIn
    const TSecurityPolicy& SecurityPolicy() const;

private: // New methods

    // Handle 'Orientation' action
    CCFActionPlugIn::TExecutionTime HandleOrientationActionL(
        const CCFActionIndication& aAction );

private:

    COrientationActionPlugIn();

private: // Data

    /** Orientation controller */
    COrientationAction* iOrientationAction;
    };

#endif
