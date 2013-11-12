/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Application class of the module.
*
*/



#ifndef STARTUPAPPLICATION_H
#define STARTUPAPPLICATION_H

// INCLUDES


#include <eikapp.h>


// CONSTANTS
const TUid KUidStartUp = { 0x100058F4 };

// CLASS DECLARATION

/**
* CStartupApp application class.
*/
class CStartupApplication : public CEikApplication 
    {
    private: // from CApaApplication
        /**
        * Create CStartupDocument document object.
        */
        CApaDocument* CreateDocumentL();

        /**
        * Return KUidStartUp.
        */
        TUid AppDllUid() const;
    };

#endif // STARTUPAPPLICATION_H

// End of file
