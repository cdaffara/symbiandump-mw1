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
* Description:  The registrar setting view
*
*/


#ifndef SIP_SETT_LIST_SIP_REG_SET_VIEW_H
#define SIP_SETT_LIST_SIP_REG_SET_VIEW_H

//  INCLUDES

#include <aknview.h>
#include "tsipsettingsdata.h"
#include "sipsettlistsipsrvsetview.h"

// FORWARD DECLARATIONS

class MGSSIPProfileHandler;

// CLASS DECLARATION

/**
*  CSIPSettListSIPRegSetView view class
*  @since 3.0
*  view class for SIP Registration settings
*/
class CSIPSettListSIPRegSetView : public CSIPSettListSIPSrvSetView
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor. Instance is placed on the cleanup stack
        * @param aHandler   Profile handler
        */
        static CSIPSettListSIPRegSetView* NewLC( 
            MGSSIPProfileHandler* aHandler );
        
        /**
        * Destructor.
        */
        virtual ~CSIPSettListSIPRegSetView();        

    public: // Functions from base classes

        /**
        * From CAknView.
        * Returns the UID of the view
        * @return UID of the view
        */
        TUid Id() const;        
    
    protected:  // Functions from base classes

        /**
        * From CSIPSettListSIPSrvSetView
        * Returns used data structure
        * @return Pointer to used server data structure
        */
        TGSSIPServerData* DataStruct();

        /**        
        * Returns used item list resource id        
        * @return Resource ID
        */
        TInt ResourceID();

        /**
        * From CGSSettListSIPSrvSetView
        * Returns used service profile class server view
        * @return used service profile class 
        */
        TSIPProfileTypeInfo::TSIPProfileClass ProfileType();        

        /**
        * Checks is currently shown profile locked.
        * @return ETrue if profile is locked
        */
        TBool ProfileLocked();

    private:

        /**
        * C++ default constructor.
        * @param aHandler   Profile handler
        */
        CSIPSettListSIPRegSetView( MGSSIPProfileHandler* aHandler );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:

        // Profile handler
        MGSSIPProfileHandler* iHandler;
        
    friend class UT_SIPSettListSIPRegSetView;
    };

#endif      // SIP_SETT_LIST_SIP_REG_SET_VIEW_H
            
// End of File
