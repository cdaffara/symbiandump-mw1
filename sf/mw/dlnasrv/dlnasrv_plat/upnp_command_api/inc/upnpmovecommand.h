/** @file
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies  this distribution, and is available 
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Helper class to ease the use of the Upnp Framework's
*                UpnpCommand API when moving files via Upnp.
*
*/


#ifndef UPNP_MOVE_COMMAND_H
#define UPNP_MOVE_COMMAND_H

// INCLUDES
#include <badesca.h>                  // CDesCArrayFlat

// FORWARD DECLARATIONS
class CUpnpCommand;

// CLASS DEFINITION
class CUpnpMoveCommand: public CBase
    {

    public: // Construction/destruction methods

        /**
         * Creates a new UpnpCommand for file moving purposes. Does not
         * instantiate Upnp Framework services yet at this point.
         *
         * Leaves with KErrNotReady if Upnp dedicated IAP is not set, or with
         * using standard e32err.h error code if construction fails.
         *
         * @since S60 3.2
         * @return a new CUpnpMoveCommand object
         */
        IMPORT_C static CUpnpMoveCommand* NewL();

        /**
         * Destructor.
         *
         * @since S60 3.2
         */
        virtual ~CUpnpMoveCommand();

    public: // Business logic methods

        /**
         * Returns the availability information of the command.
         *
         * @since S60 3.2
         * @return TBool (ETrue if available)
         */
        IMPORT_C static TBool IsAvailableL();

        /**
         * Moves the given list of files to a remote Upnp Media Server. The
         * method is synchronous. The target server device will be queried
         * from the user using an UI component.
         *
         * The files that are successfully moved are removed from the array.
         * DRM protected files are skipped (not copied and left in the array)
         *
         * Leaves if the operation fails. Standard system wide (e32err.h)
         * error codes will be used.
         *
         * @since S60 3.2
         * @param aFiles (CDesCArrayFlat*) list of filenames (including path)
         */
        IMPORT_C void MoveFilesL( CDesCArrayFlat* aFiles );

    private: // Private construction methods

        /**
         * Constructor
         *
         * @since S60 3.2
         */
        CUpnpMoveCommand();

        /**
         * Second phase constructor
         *
         * @since S60 3.2
         */
        void ConstructL();

    private: // Data members

        /*
         * UpnpCommand instance. Owned.
         */
        CUpnpCommand*    iCommand;

    };

#endif // UPNP_MOVE_COMMAND_H

// End of File
