/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:         Document
*
*/









#ifndef C_BCTESTBCTESRUTILITIESDOCUMENT_H
#define C_BCTESTBCTESRUTILITIESDOCUMENT_H

#include <eikdoc.h>

class  CEikAppUi;

/**
 * CBCTestUtilitiesDocument application class.
 */
class CBCTestUtilitiesDocument : public CEikDocument
    {
public: // Constructors and destructor

    /**
     * Symbian OS two-phased constructor.
     * @return Pointer to created Document class object.
     * @param aApp Reference to Application class object.
     */
    static CBCTestUtilitiesDocument* NewL( CEikApplication& aApp );

    /**
     * Destructor.
     */
    virtual ~CBCTestUtilitiesDocument();

private: 

// Constructors

    /**
     * Overload constructor.
     * @param aApp Reference to Application class object.
     */
    CBCTestUtilitiesDocument( CEikApplication& aApp );

// From CEikDocument

    /**
     * From CEikDocument, CreateAppUiL.
     * Creates CBCTestUtilitiesAppUi "App UI" object.
     * @return Pointer to created AppUi class object.
     */
    CEikAppUi* CreateAppUiL();

    };

#endif  // C_BCTESTTEMPLATEDOCUMENT_H

