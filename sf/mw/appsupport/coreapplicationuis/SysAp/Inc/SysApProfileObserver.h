/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApProfileObserver class definition. 
*
*/


#ifndef SYSAPPROFILEOBSERVER_H
#define SYSAPPROFILEOBSERVER_H

// INCLUDES
#include <e32base.h>
#include <MProfileChangeObserver.h>
#include <CProfileChangeNotifyHandler.h>
#include "SysAp.hrh"

// FORWARD DECLARATIONS
class CSysApAppUi;

// CLASS DECLARATION

/**
*  CSysApProfileObserver
*
*  @lib   sysap
*  @since 3.0
*/

class CSysApProfileObserver : public CBase, public MProfileChangeObserver
    {
    public: // Constructors and destructor
        /**
        * Default constructor
        */
        static CSysApProfileObserver* NewL( CSysApAppUi& aSysApAppUi );

        /**
        * Destructor.
        */
        virtual ~CSysApProfileObserver();

    private:

        /**
        * EPOC default constructor.
        * @param None
        * @return void
        */
        void ConstructL( );
        
        /**
        * Constructor
        * @param CSysApAppUi& aSysApAppUi
        * @return void
        */         
        CSysApProfileObserver( CSysApAppUi& aSysApAppUi );
    
        /**
        * C++ default constructor.
        * @param None
        * @return void
        */
        CSysApProfileObserver();
        
    public: // From MProfileChangeObserver

        /**
        * This is a callback function which is called when a active profile event
        * completes. User must derive his class from MProfileChangeObserver
        * and implement this method if he is to use notify services.
        * @param aProfileEvent Profile event
        * @param aProfileId Active profile id
        */
        void HandleActiveProfileEventL( TProfileEvent aProfileEvent, TInt aProfileId );

    private:
        // By default, prohibit copy constructor
        CSysApProfileObserver( const CSysApProfileObserver& );
    
        // Prohibit assigment operator
        CSysApProfileObserver& operator= ( const CSysApProfileObserver& );
    
    private:
        CSysApAppUi&                 iSysApAppUi;
        CProfileChangeNotifyHandler* iHandler;
    };

#endif      // SYSAPPROFILEOBSERVER_H
            
// End of File
