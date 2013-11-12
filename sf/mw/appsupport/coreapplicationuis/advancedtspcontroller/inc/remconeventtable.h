/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Idle state listener
*
*/



#ifndef REMCONEVENTTABLE_H
#define REMCONEVENTTABLE_H

// INCLUDES
#include <e32base.h>
#include <e32property.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Idle state listener implementation.
*
*  RemTspController.lib
*  @since S60 3.1
*/
NONSHARABLE_CLASS( CRemConEventTable )
    {
    public: // Constructors and destructor
        
        enum RemConEventType
            {
            ERemConEvent = 0,
            ERemConExtEventCall,
            ERemConExtEventPoc,
            ERemConExtEventSync,
            };
            
        enum RemConTableRules
            {
            EActiveAudio = 0,
            EForegroundApplication,
            EDefaultApplicationGroup,
            ELastPlayedMediaApplication,
            ELaunchDefaultApp
            };
        
        /**
        * Destructor.
        */
        ~CRemConEventTable();

        /**
        * Two-phased constructor.
        */
        static CRemConEventTable* NewL( TInt aTableNum );
        
        TInt TableNum();
        
        void ClearTable();
        
        void AddRemConEventToTable( TInt aEvent, RemConEventType aEventType );
        
        void RemoveRemConEventFromTable( TInt aEvent, RemConEventType aEventType );
        
        TBool IsRemConEventInTable( TInt aEvent, RemConEventType aEventType );
        
        void GetRulesForThisTable( RArray<TInt>& aArrayToRules );
        
        void AddRuleForThisTable( TInt aRule );
        
        void RemoveRuleFromThisTable( TInt aRule );
        
        void AddDefaultApplicationForThisTable( TInt aDefaultApplication );
        
        void RemoveDefaultApplicationFromThisTable( TInt aDefaultApplication );
        
        void GetDefaultApplicationsForTable( RArray<TInt>& aArrayToRules );
                
        void AddDefaultLaunchAppForThisTable( const TInt aUid );

        void GetDefaultLaunchAppForTable( TUid& aUid );

        void AddTransparentApplicationForThisTable( TInt aRule );

        void GetTransparentApplicationsForTable( RArray<TInt>& aArrayToRules );

    public: // New functions

    public: // Functions from base classes

    protected: // New functions

        /**
        * C++ default constructor.
        */
        CRemConEventTable();

        /**
        * Symbian 2nd phase constructor.
        */
        void ConstructL( TInt aTableNum );
        
    	//CRemConIdleListener();        
    protected:  // Functions from base classes

    public:     // Friend classes

    protected:  // Friend classes
    
    private:
    

    private:
    
        TInt iTableNum;
           
        RArray<TInt> iRemConEventArray;
        
        RArray<TInt> iRemConExtCallEventArray;
        
        RArray<TInt> iRemConExtPocEventArray;
        
        RArray<TInt> iRemConExtSyncEventArray;
        
        RArray<TInt> iRulesForThisTable;
        
        RArray<TInt> iDefaultApplicationsForThisTable;

        RArray<TInt> iTransparentApplicationsForThisTable;

        TUid iUid;
    };

#endif // REMCONEVENTTABLE_H

// End of File
