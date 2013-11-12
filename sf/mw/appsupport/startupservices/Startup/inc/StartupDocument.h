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
* Description:  Document class of the module.
*
*/



#ifndef STARTUPDOCUMENT_H
#define STARTUPDOCUMENT_H

// INCLUDES
#include <eikdoc.h>

// FORWARD DECLARATIONS
class   CEikAppUi;

// CLASS DECLARATION

/**
*  CStartupDocument application class.
*/
class CStartupDocument : public CEikDocument
{
    public:

        /**
        *   C++ default constructor.
        */
        CStartupDocument(CEikApplication& aApp): CEikDocument(aApp) { }

        /**
        *   Two-phased constructor.
        */
        static CStartupDocument* NewL(CEikApplication& aApp);

        /**
        *   Destructor.
        */
        virtual ~CStartupDocument();

    private:
        /**
        *   EPOC default constructor.
        */
        void ConstructL();

        /**
        * This method makes an application hidden so that it is not visible
        * for example in Applications list and FastSwap window.
        * @param CApaWindowGroupName* aWgName
        * @return void        
        */
        void UpdateTaskNameL( CApaWindowGroupName* aWgName );

    private:// from CEikDocument

        /**
        * Create CStartupAppUi object.
        */
        CEikAppUi* CreateAppUiL();
        
    private:// data

};

#endif

//End of file
