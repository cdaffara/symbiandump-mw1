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
* Description:  CSysApCenRepHacSettingObserver class definition
*
*/


#ifndef SYSAPCENREPHACSETTINGOBSERVER_H
#define SYSAPCENREPHACSETTINGOBSERVER_H

// INCLUDES
#include <e32base.h>
#include <centralrepository.h>
#include <cenrepnotifyhandler.h>

// FORWARD DECLARATIONS
class CSysApAppUi;

// CLASS DECLARATION

/**
*  CSysApCenRepHacSettingObserver
*
*  A class for receiving notifactions about HAC (Hearing Aid Compatibility)
*  CenRep-key changes
*
*  @lib   sysap
*  @since 3.1
*/
class CSysApCenRepHacSettingObserver : public CBase, public MCenRepNotifyHandlerCallback
    {
    public: // Constructors and destructor
        /**
        * Two-phased constructor
        *
        * @since S60 3.1
        * @param    aSysApAppUi    reference to SysAp's application UI class
        * @return   CSysApCenRepHacSettingObserver* a new CSysApCenRepHacSettingObserver instance
        */
        static CSysApCenRepHacSettingObserver* NewL( CSysApAppUi& aSysApAppUi );

        /**
        * Destructor.
        */
        virtual ~CSysApCenRepHacSettingObserver();

    private:

        /**
        * Symbian default constructor.
        * 
        * @since S60 3.1
        * @param None
        * @return None
        */
        void ConstructL( );
        
        /**
        * Constructor
        *
        * @since S60 3.1
        * @param aSysApAppUi reference to SysAp's application UI class
        * @return None
        */         
        CSysApCenRepHacSettingObserver( CSysApAppUi& aSysApAppUi );
    
        /**
        * C++ default constructor.
        *
        * @since S60 3.1
        * @param None
        * @return void
        */
        CSysApCenRepHacSettingObserver();
        
    public: // From MCenRepNotifyHandlerCallback
        void HandleNotifyInt( TUint32 aId, TInt aNewValue );
        void HandleNotifyError( TUint32 aId, TInt error, CCenRepNotifyHandler* aHandler );
        
    public: // Other functions
        
        /**
        * Returns the current HAC mode.
        *
        * @since S60 3.1
        * @param None
        * @return TInt
        */               
        TInt HacMode() const;

    private:
        // By default, prohibit copy constructor
        CSysApCenRepHacSettingObserver( const CSysApCenRepHacSettingObserver& );
    
        // Prohibit assigment operator
        CSysApCenRepHacSettingObserver& operator= ( const CSysApCenRepHacSettingObserver& );
    
    private:
        /**
        * Reference to SysAp's application UI class
        */
        CSysApAppUi& iSysApAppUi;
        
        /**
        * Central Repository session object which is used for accessing the
        * HAC setting value
        * Own.
        */
        CRepository* iSession;
        
        /**
        * Central Repository notify handler which is used for receiving notifications
        * on HAC value changes.
        * Own.
        */
        CCenRepNotifyHandler* iHacSettingHandler;
    };

#endif      // SYSAPCENREPHACSETTINGOBSERVER_H
            
// End of File
