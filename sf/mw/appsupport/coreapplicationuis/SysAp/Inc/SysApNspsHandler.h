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
* Description:  CSysApNspsHandler class definition.
*
*/


#ifndef SYSAPNSPSHANDLER_H
#define SYSAPNSPSHANDLER_H

// INCLUDES
#include <e32base.h>  


// FORWARD DECLARATIONS
class RWsSession;
class RWindowBase;
class RAnimDll;
class RSysApNspsAnim;

// CLASS DECLARATION

/**
*  CSysApNspsHandler
*
*  @lib   sysap
*  @since 1.0
*/

class CSysApNspsHandler : public CBase
	{
    public: // Constructors and destructors
        /** C'tor. 
        *   @param aAnimDll This is server side object.
        */
        static CSysApNspsHandler* NewL( RWsSession& aWsSession, const RWindowBase& aDevice );
        ~CSysApNspsHandler();

    public: // New functions
        
        /**
        * Checks from current Epoc swstate if enos sync message is already sent to MTC or not
        * @param None
        * @return const TBool
        */
        void SetNspsOn();
        
        /**
        * Checks from current Epoc swstate if enos sync message is already sent to MTC or not
        * @param None
        * @return const TBool
        */
        void SetNspsOff();
        void SetMessageTonePlaying();
        void SetMessageToneNotPlaying();
        void ActivateKeyeventForwardingForLights();
        void DeActivateKeyeventForwardingForLights();
        
    private:
        void ConstructL( RWsSession& aWsSession, const RWindowBase& aDevice );
        CSysApNspsHandler();

    private:
        RAnimDll*           iAnimDll;
        RSysApNspsAnim*     iSysApAnim;
    };

#endif
// End of File
