/*
* Copyright (c) 2008-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Private Publish & Subscribe definitions of Startup app.
*
*/


#ifndef STARTUPAPPPRIVATEPSKEYS_H
#define STARTUPAPPPRIVATEPSKEYS_H


const TUid KPSUidStartupApp = { 0x100058F4 };


// =============================================================================
// Startup Application Synchronization API
// =============================================================================

const TUint32 KPSStartupAppState = 0x00000001;
enum TPSStartupAppState
    {
    EStartupAppStateWait = 1,         // Startup app is waiting for indication to start
                                      // playing animation(s).
    EStartupAppStateStartAnimations,  // Startup app may start playing the animation(s)
                                      // and showing country & time queries.
    EStartupAppStateFinished          // Startup app has finished playing the animation(s)
                                      // and finished showing country & time queries.
    };

const TUint32 KPSStartupAppStarted = 0x00000002;

#endif // STARTUPAPPPRIVATEPSKEYS_H
