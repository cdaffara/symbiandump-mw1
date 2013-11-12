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
* Description:  CSysApKeySndHandler class definition.
*
*/


#ifndef SYSAPKEYSNDHANDLER_H
#define SYSAPKEYSNDHANDLER_H

// INCLUDES
#include <e32base.h>  
#include <w32std.h>


// FORWARD DECLARATIONS
class RWsSession;
class RAnimDll;
class RSysApKeySndAnim;

// CLASS DECLARATION

/**
*  CSysApKeySndHandler
*
*  @lib   sysap
*  @since 1.0
*/

class CSysApKeySndHandler : public CBase
	{
    public: // Constructors and destructors
        /** C'tor. 
        *   @param aAnimDll This is server side object.
        */
        static CSysApKeySndHandler* NewL( RWsSession& aWsSession );
        ~CSysApKeySndHandler();
       
    private:
        void ConstructL( RWsSession& aWsSession );
        CSysApKeySndHandler();

    private:
        RAnimDll*           iAnimDll;
        RSysApKeySndAnim*   iKeyPublisher;
        RWindowGroup        iOffScreenGroupWin;
    };

#endif // SYSAPKEYSNDHANDLER_H
// End of File
