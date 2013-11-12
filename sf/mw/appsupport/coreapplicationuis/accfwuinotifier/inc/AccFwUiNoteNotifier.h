/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of accessory UI note notifier.
*
*/


#ifndef __CACCFWUINOTENOTIFIER_H
#define __CACCFWUINOTENOTIFIER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <eiknotapi.h>
#include <eikenv.h>


// CONSTANTS
const TUid KAccFwUiNoteNotifierUid = { 0x10205061 };

// FUNCTION PROTOTYPES
CArrayPtr<MEikSrvNotifierBase2>* NotifierArray();

// FORWARD DECLARATIONS
class CCoeEnv;


// CLASS DECLARATION
/**
*  Displays information note about connected accessory
*  
*  @lib accfwuinotifier.lib
*  @since S60 3.1
*/
class CAccFwUiNoteNotifier : public CActive, public MEikSrvNotifierBase2
    {
    public:  // Constructors and destructor

        static CAccFwUiNoteNotifier* NewL();	                    
        
        ~CAccFwUiNoteNotifier();
        
    public: 
            
        // From MEikSrvNotifierBase2
            
        void Release();

        TNotifierInfo RegisterL();

        TNotifierInfo Info() const;

        TPtrC8 StartL( const TDesC8& aBuffer );

        void StartL( const TDesC8& aBuffer, TInt aReplySlot, const RMessagePtr2& aMessage );

        void Cancel();

        TPtrC8 UpdateL( const TDesC8& aBuffer );

		// From CActive
	
		void RunL();
        TInt RunError(TInt aError);
		void DoCancel();
                
    private: // Functions

        /**
        * C++ default constructor.
        */
        CAccFwUiNoteNotifier();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
	
    private:    // Data
	
	    // Notifier info    
        TNotifierInfo iInfo;
        // not owned
        CCoeEnv* iCoeEnv;
        //resource file offset
        TInt iResourceFileOffset;
		// Message to complete
		RMessagePtr2 iMessage;
        // Indication whether notifier cancelled
        TBool iIsCancelled;
	    
	    // Text to show in note
        HBufC* iNoteText;
        
       
    };

#endif      // __CACCFWUINOTENOTIFIER_H

// End of File
