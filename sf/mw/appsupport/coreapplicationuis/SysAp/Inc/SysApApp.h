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
* Description:  CSysApApp class definition. Application core.
*
*/


#ifndef SYSAPAPP_H
#define SYSAPAPP_H

// INCLUDES
//#include <aknapp.h>
#include <eikapp.h>

// CONSTANTS
const TUid KUidSysAp={0x100058F3};

// CLASS DECLARATION
/**
*  CSysApApp
*
*  @lib   sysap
*  @since 1.0
*/

class CSysApApp : public CEikApplication 
    {
    private: // from CApaApplication
        /**
        * Create CSysApDocument document object.
        */
        CApaDocument* CreateDocumentL();

        /**
        * Return KUidSysAp.
        */
        TUid AppDllUid() const;
    };

#endif      // SYSAPAPP_H
            
// End of File

