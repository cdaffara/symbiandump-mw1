/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApAccessoryObserver class definition. This class listens to
*                changes in Accessory Mode and updates accessory specific Status
*                Pane indicators and profiles.
*
*/


#ifndef SYSAPACCESSORYOBSERVER_H
#define SYSAPACCESSORYOBSERVER_H

// INCLUDES
#include <e32base.h>
#include <AccessoryMode.h>
#include <AccessoryServer.h>
#include <AccessoryConnection.h>

// FORWARD DECLARATIONS
class CSysApAppUi;

// CLASS DECLARATION
/**
*  CSysApAccessoryObserver class. This class observer changes in Accessory Server.
*
*  @lib   sysap
*  @since 3.1 
*/

class CSysApAccessoryObserver : public CActive 
    {
    public: // Constructors and destructor
        /**
        * EPOC default constructor.
        */
        static CSysApAccessoryObserver* NewL( CSysApAppUi& aSysApAppUi );
        
        /**
        * Destructor.
        */
        virtual ~CSysApAccessoryObserver();
      
        /**
        * Returns current accessory mode
        * 
        * @return Current accessory mode
        */
        TAccMode GetAccessoryMode() const;
        TInt GetAccessoryConnectionType() const;

    private:
        /**
        * Symbian OS default constructor.
        * @param None
        * @return void
        */
        void ConstructL( );
        
        /**
        * C++ default constructor.
        * @param None
        * @return void
        */
        CSysApAccessoryObserver();

        /**
        * Constructor
        * @param CSysApAppUi& aSysApAppUi
        * @return void
        */         
        CSysApAccessoryObserver( CSysApAppUi& aSysApAppUi );
    
        // By default, prohibit copy constructor
        CSysApAccessoryObserver( const CSysApAccessoryObserver& );
    
        // Prohibit assigment operator
        CSysApAccessoryObserver& operator= ( const CSysApAccessoryObserver& );
       
    private:
        /**
        * From CActive. Cancels the active object from observing Accessory Mode events.
        * @param None
        * @return void
        */
        void DoCancel();
        
        /**
        * From CActive. Gets called by active object when an alarm event occurs.
        * @param None
        * @return void
        */
        void RunL();

        /**
        * From CActive. Gets called by active object when leave happens inside RunL
        * @param TInt aError
        * @return void
        */
        TInt RunError( TInt aError );

    private:
        CSysApAppUi&         iSysApAppUi;
        RAccessoryServer     iAccessoryServer;
        RAccessoryMode       iAccessoryMode;
        TAccPolAccessoryMode iAccMode;
        
        /**
        * Previous accessory mode is stored for filtering out
        * notifications due change in audio output status.
        */
        TAccMode iPreviousMode;
        RAccessoryConnection iAccessoryConnection;
        TInt iPhysicalConnectionType;
        
    };

#endif      // SYSAPACCESSORYOBSERVER_H
            
// End of File
