/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Class for resetting Logs and a few SIM specific settings in Shared Data
*
*/


#ifndef SYSAPSIMCHANGED_H
#define SYSAPSIMCHANGED_H

// INCLUDES
#include <e32base.h>

// CONSTANTS

// FORWARD DECLARATIONS
class CSysApAppUi;
class RFs;

// CLASS DECLARATION
/**
*  A class for resetting Logs and SIM settings in cenrep when
*  a new SIM card has been inserted in phone.
*
*  @lib   SysAp
*  @since 1.0 
*/

class CSysApSimChanged : public CBase
    { 
    public:  // Constructors and destructor

        /**
        * two-phase constructor.
        */      
        static CSysApSimChanged* NewL( CSysApAppUi& aSysApAppUi, RFs& aFs );

        /**
        * destructor
        */
        virtual ~CSysApSimChanged();

        /**
        * Clears repositories if needed, depending on SIM
        * owned/changed state.
        *
        * @since S60 3.2
        */
        void HandleSimChangedL();

private:
        void ClearRepositoriesL();
        void ClearLogsL();
    
        /**
        * Communicates SIM event to SS-settings.
        *
        * @since S60 3.2
        */
        void ClearSsSettingsL();        
    
        /**
        * Constructor
        * @param CSysApAppUi& aSysApAppUi
        * @param aFs reference to FileServer session
        * @return void
        */         
        CSysApSimChanged( CSysApAppUi& aSysApAppUi, RFs& aFs );

    private:        
        CSysApAppUi&        iSysApAppUi;
        
        /**
        * Reference to FileServer session.
        */
        RFs& iFs;
    };
// CLASS DECLARATION
/**
*  Required by CLogClient
*
*  @lib   SysAp
*  @since 1.0 
*/

class CShareActive : public CActive
	{
public:
	CShareActive();
	~CShareActive();
	void StartL();
protected:
	void DoCancel();
	void RunL();
	};

#endif // SYSAPSIMCHANGED_H

// End of File






