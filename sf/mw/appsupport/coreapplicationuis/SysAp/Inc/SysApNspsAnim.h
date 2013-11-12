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
* Description:  RSysApNspsAnim class definition. A Window server Plug-in.
*
*/


#ifndef SysApNspsHandler_H
#define SysApNspsHandler_H

// INCLUDES
#include <w32std.h>     // RAnim

// CONSTANTS
// MACROS
// DATA TYPES
// FUNCTION PROTOTYPES
// FORWARD DECLARATIONS
class CCoeControl;

// CLASS DECLARATION
/**
*  RSysApNspsAnim
*
*  @lib   sysap
*  @since 1.0
*/

class RSysApNspsAnim : public RAnim
	{
    public: // Constructors and destructors
        
        /** C'tor. 
        *   @param aAnimDll This is server side object.
        */
        RSysApNspsAnim( RAnimDll& aAnimDll );
        
        /** Construction.
        *   @param aDevice
        */
        void ConstructL( const RWindowBase& aDevice );
        
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
    };

#endif
// End of File
