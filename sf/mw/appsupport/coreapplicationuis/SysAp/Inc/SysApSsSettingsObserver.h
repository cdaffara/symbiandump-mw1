/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApSsSettingsObserver class definition. This class
*                implements methods needed for handling SsSettings functionality
*                that tells what is the currently used ALS line for outgoing
*                calls.
*
*/


#ifndef SYSAPSSSETTINGSOBSERVER_H
#define SYSAPSSSETTINGSOBSERVER_H

// INCLUDES
#include <e32base.h>
#include <msssettingsobserver.h>

// FORWARD DECLARATIONS
class CSysApAppUi;

// CLASS DECLARATION
/**
*  CSysApSsSettingsObserver
*
*  @lib   sysap
*  @since 1.0
*/

class CSysApSsSettingsObserver : public CBase, public MSSSettingsObserver
    {
    public: // Constructors and destructor
        /**
        * EPOC default constructor.
        */
        static CSysApSsSettingsObserver* NewL( CSysApAppUi& aSysApAppUi );

        /**
        * Destructor.
        */
        virtual ~CSysApSsSettingsObserver();
    
    public:     
        /**
        * CallBack function from MSSSettingsObserver. 
        * Gets called when SS settings are changed.
        * @param None
        * @return void
        */
        void PhoneSettingChanged( TSSSettingsSetting aSetting, TInt aNewValue );
    
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
        CSysApSsSettingsObserver( CSysApAppUi& aSysApAppUi );
    
        /**
        * C++ default constructor.
        * @param None
        * @return void
        */
        CSysApSsSettingsObserver();
        
    private:
        // By default, prohibit copy constructor
        CSysApSsSettingsObserver( const CSysApSsSettingsObserver& );
    
        // Prohibit assigment operator
        CSysApSsSettingsObserver& operator= ( const CSysApSsSettingsObserver& );
    
    private:
        /**
        * Method for setting the state of Second line indicator
        * @param None
        * @return void
        */
        void SetSecondLineIndicatorL( TInt aSsSettingsAlsLineValue ) const;
    
    private:
        RSSSettings         iSysApSsSettings;
        CSysApAppUi&        iSysApAppUi;
    };

#endif      // SYSAPSSSETTINGSOBSERVER_H
            
// End of File
