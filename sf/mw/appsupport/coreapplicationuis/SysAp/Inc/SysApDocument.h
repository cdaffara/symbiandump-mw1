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
* Description:  CSysApDocument class definition.
*
*/


#ifndef SYSAPDOCUMENT_H
#define SYSAPDOCUMENT_H

// INCLUDES
//#include <AknDoc.h>
#include <eikdoc.h>

// FORWARD DECLARATIONS
class  CEikAppUi;

// CLASS DECLARATION
/**
*  CSysApDocument application class.
*  @lib   sysap
*  @since 1.0
*/

class CSysApDocument : public CEikDocument
    {
    public:
        /**
        * C++ constructor.
        * @param CEikApplication& aApp
        * @return void
        */
        CSysApDocument(CEikApplication& aApp): CEikDocument(aApp) { }
        
        /**
        *   Two-phased constructor.
        */
//        static CSysApDocument* NewL(CEikApplication& aApp);

        /**
        * Destructor.
        * @param None
        * @return void
        */
        virtual ~CSysApDocument();

    private:
        
        /**
        *   EPOC default constructor.
        */
//        void ConstructL();
        
        /**
        * C++ default constructor.
        * @param None
        * @return void
        */
        CSysApDocument();

        /**
        * This method makes an application hidden so that it is not visible
        * for example in Applications list and FastSwap window.
        * @param CApaWindowGroupName* aWgName
        * @return void        
        */
        void UpdateTaskNameL( CApaWindowGroupName* aWgName );
        
    private:// from CAknDocument
        /**
        * Create CSysApUi "App UI" object.
        * @param None
        * @return CEikAppUi*        
        */
        CEikAppUi* CreateAppUiL();
    };

#endif      // SYSAPDOCUMENT_H
            
// End of File
