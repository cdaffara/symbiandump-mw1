/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observer class for remote control keys
 *
*/


#ifndef SYSAPREMCONOBSERVER_H
#define SYSAPREMCONOBSERVER_H

//  INCLUDES
#include <remconcoreapitargetobserver.h>

// FORWARD DECLARATIONS
class CRemConInterfaceSelector;  
class CRemConCoreApiTarget;
class CSysApAppUi;


// CLASS DECLARATION

/**
*  Class for monitoring side volume key events.
*
*  When FM TX is active the user is not allowed to adjust the volume. This class
*  monitors volume key events and displays a note to the user, informing that
*  it is not possible to adjust the volume.
*
*  @lib None.
*  @since 3.2
*/
NONSHARABLE_CLASS( CSysApRemConObserver ) : public CBase,
                                         public MRemConCoreApiTargetObserver
    {
    public:  // Constructor and destructor
        
        /**
        * Two-phased constructor.
        */
        static CSysApRemConObserver* NewL( CSysApAppUi& aSysApAppUi );
        
        /**
        * Destructor.
        */
        virtual ~CSysApRemConObserver();

    protected: // Functions from base classes

        /**
        * @see SysApemConCoreApiTargetObserver.
        *
        * A command has been received. 
        * @param aOperationId The operation ID of the command.
        * @param aButtonAct The button action associated with the command.
        */
        void MrccatoCommand( TRemConCoreApiOperationId aOperationId, 
                             TRemConCoreApiButtonAction aButtonAct);


    private: // Functions
        /**
        * C++ default constructor.
        * Not accessible. 
        */
        CSysApRemConObserver();
        
        /**
        * Constructor.
        */
        CSysApRemConObserver( CSysApAppUi& aSysApAppUi );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        
    private: // Data

		/**
		* Remote Controller interface selector.
		* Own.
		*/
        CRemConInterfaceSelector* iInterfaceSelector;  
        
        /** 
        * Remote Controller target. 
        * Not own.
        */
        CRemConCoreApiTarget* iCoreTarget;
        
        /**
        * SysAp application UI class. 
        * Used for showign the UI note.
        */
        CSysApAppUi& iSysApAppUi;
    };

#endif // SYSAPREMCONOBSERVER_H

// End of File
