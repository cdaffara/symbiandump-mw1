/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  GSServerEngine is responsible for initializing the TV
*                driver during the boot and always when the cable is connected.
*
*/



#ifndef GSSERVERCLIENT_H
#define GSSERVERCLIENT_H

//  INCLUDES
#include <e32std.h>
#include <e32base.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Client side API for starting the server which initializes the driver. There's
*  no other methods in this class except for connecting and disconnecting. That is
*  because the server handles everything automatically, someone just has to
*  start it and keep it running.
*
*  @lib GSServerModel.lib
*  @since Series60 3.1
*/
class RGSServerClient : public RSessionBase
    {
    public:
        /**
        * C++ default constructor.
        */
        IMPORT_C RGSServerClient();

        /**
        * Connects to the CGSServer server.
        * @since Series60 3.1
        * @return KErrNone if all goes well
        */
        IMPORT_C TInt Connect();

        /**
        * Closes the connection to the CGSServer server.
        * @since Series60 3.1
        */
        IMPORT_C void Close();
    };

#endif      // GSSERVERCLIENT_H

// End of File
