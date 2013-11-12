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
* Description:  Declaration of CRfsTestDoc class
*
*/


#ifndef RFSTESTDOC_H
#define RFSTESTDOC_H

#include <akndoc.h>

/**
*  Document class for RfsTestApp application.
*
*  @since S60 S60 5.1
*/
class CRfsTestDoc : public CAknDocument
    {

public:

    /**
    * Constructs a CRfsTestDoc object.
    *
    * @param The application instance that is creating the document.
    * @return The new object
    */
    static CRfsTestDoc* NewL( CEikApplication& aApp );

    /**
    * Destructor.
    */
    virtual ~CRfsTestDoc();

private:

    /**
    * First phase constructor.
    *
    * @param The application instance that is creating the document.
    */
    CRfsTestDoc( CEikApplication& aApp );

private: // CEikDocument

    /**
    * @see CEikDocument
    */
    CEikAppUi* CreateAppUiL();

    };

#endif // RFSTESTDOC_H
