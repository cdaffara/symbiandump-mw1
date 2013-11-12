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
* Description:  CSysApAnimKeySndControl class definition.
*
*/


#ifndef SYSAPANIMKEYSNDCONTROL_H
#define SYSAPANIMKEYSNDCONTROL_H

// INCLUDES
#include <coecntrl.h>     // CCoeControl
#include "SysAp.hrh"

// CONSTANTS
// MACROS
// DATA TYPES
// FUNCTION PROTOTYPES
// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
*  CSysApAnimKeySndControl
*
*  @since 1.0
*/
class CSysApAnimKeySndControl : public CCoeControl
	{
public:
    static CSysApAnimKeySndControl* NewL( RWindowGroup* aParent );
    ~CSysApAnimKeySndControl();
private:
	CSysApAnimKeySndControl();
	void ConstructL(RWindowGroup* aParent);
	};

#endif // SYSAPANIMKEYSNDCONTROL_H
// End of File
