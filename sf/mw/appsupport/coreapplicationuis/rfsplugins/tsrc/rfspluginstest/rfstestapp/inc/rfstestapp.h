/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declaration of CRfsTestApp class
*
*/


#ifndef RFSTESTAPP_H
#define RFSTESTAPP_H

#include <aknapp.h>

/**
*  Application class for RfsTestApp application.
*
*  @since S60 S60 5.1
*/
class CRfsTestApp : public CAknApplication
    {

public:

    /**
    * Destructor.
    */
    virtual ~CRfsTestApp();

private: // From CEikApplication

    /**
    * @see CEikApplication
    */
    CApaDocument* CreateDocumentL();

private: // From CApaApplication

    /**
    * @see CApaApplication
    */
    TUid AppDllUid() const;

    };

#endif // RFSTESTAPP_H
