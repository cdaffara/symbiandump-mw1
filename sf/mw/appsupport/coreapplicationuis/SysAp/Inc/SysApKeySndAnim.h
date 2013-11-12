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
* Description:  RSysApKeySndAnim class definition. A Window server Plug-in.
*
*/


#ifndef SYSAPKEYSNDANIM_H
#define SYSAPKEYSNDANIM_H

// INCLUDES
#include <w32std.h>    

// CONSTANTS
// MACROS
// DATA TYPES
// FUNCTION PROTOTYPES
// FORWARD DECLARATIONS
class CSysApAnimKeySndControl;

// CLASS DECLARATION
/**
*  RSysApKeySndAnim
*
*  @since 1.0
*/
class RSysApKeySndAnim : public RAnim
	{
    public: // Constructors and destructors
        
        /** C'tor. 
        *   @param aAnimDll This is server side object.
        */
        RSysApKeySndAnim( RAnimDll& aAnimDll );
        
        /** Construction.
        *   @param aParent
        */
        void ConstructL(RWindowGroup* aParent);

        /**
        * Closes the animation object
        */
    	void Close();

    private:        
	    CSysApAnimKeySndControl* iKeySoundControl;
        
    };

#endif // SYSAPKEYSNDANIM_H
// End of File
