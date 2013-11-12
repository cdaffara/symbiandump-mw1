/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  ?Description
*
*/


#ifndef C_TESTACTIONPLUGIN_H
#define C_TESTACTIONPLUGIN_H

// INCLUDE FILES
#include <e32cmn.h>
#include <w32std.h>
#include <aknglobalnote.h>

#include <CFActionPlugin.h>

// FORWARD DECLARATIONS
class CVwsSessionWrapper;

// P&S definitions
const TUid KActionPluginTestCategory = { 0x0100FFFF };
const TUint32 KLeaveInitializationFunction = 0x00000100;


// CLASS DECLARATION

/**
 * Class CTestActions
 *
 * Provides a couple of (test) actions for triggered rules:
 * Starting an application, and opening an URL address.
 * Implemented as an Activator plugin for Context Framework.
 */
class CTestActionPlugIn : public CCFActionPlugIn
    {
    public: // Constructors and destructor

        static CTestActionPlugIn* NewL();
        static CTestActionPlugIn* NewLC();
            
        virtual ~CTestActionPlugIn();

    public: // Functions from CActionPlugIn
    
        // From CActionPlugIn
        void InitializeL();

        // From CActionPlugIn
        TExecutionTime ExecuteL( CCFActionIndication* aActionIndication );
        
        // From CActionPlugIn
        void GetActionsL( CDesCArray& aActionList ) const;

        // From CActionPlugIn
        const TSecurityPolicy& SecurityPolicy() const;

    protected: // Constructors

        // C++ constrcutor
        CTestActionPlugIn();

        // 2nd phase constructor
        void ConstructL();  

    private: // New functions

        /**
        *
        */
        void StartTimer();
        
        /**
        *
        */
        static TInt CallBack( TAny* aSelf );

    private: // Data

        // Own:
        TSecurityPolicy iSecurityPolicy;

        //
        CPeriodic* iPeriodic;
    };

#endif // C_TESTACTIONPLUGIN_H
