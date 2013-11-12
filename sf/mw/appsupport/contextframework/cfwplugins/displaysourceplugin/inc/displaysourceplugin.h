/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CDisplaySourcePlugin class declaration.
*
*/


#ifndef C_DISPLAYSOURCEPLUGIN_H
#define C_DISPLAYSOURCEPLUGIN_H

#include <cfcontextsourceplugin.h>

class CDisplayServiceBase;

/**
 * Display source.
 * Display source interprets events related to display state to context objects.
 *
 * @lib -
 */
NONSHARABLE_CLASS( CDisplaySourcePlugIn ): public CCFContextSourcePlugIn
    {
public:

    // Two phased constructors
    static CDisplaySourcePlugIn* NewL( TContextSourceInitParams* aParams );
    static CDisplaySourcePlugIn* NewLC( TContextSourceInitParams* aParams );

    // Destructor
    ~CDisplaySourcePlugIn();

private: // From base classes

    // @see CCFContextSourcePlugIn
    void HandleSettingL( CCFContextSourceSettingArray* aSettingList );

    // @see CCFContextSourcePlugIn
    void DefineContextsL();

    // @see CCFContextSourcePlugIn
    void InitializeL();

private:

    CDisplaySourcePlugIn( TContextSourceInitParams* aParams );

private:

    /** Display services */
    RPointerArray<CDisplayServiceBase> iServices;
    };

#endif // C_DISPLAYSOURCEPLUGIN_H
