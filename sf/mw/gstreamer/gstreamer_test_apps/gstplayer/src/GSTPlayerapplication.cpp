/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the
* Free Software Foundation, Inc., 59 Temple Place - Suite 330,
* Boston, MA 02111-1307, USA.
*
* Description:
*
*/
/*
* ==============================================================================
*  Name        : gstplayerapplication.cpp
*  Part of     : gstplayer
*  Interface   : 
*  Description : 
*  Version     : 
*

* ==============================================================================
*/

// INCLUDE FILES
#include "gstplayerdocument.h"
#include "gstplayerapplication.h"

// ============================ MEMBER FUNCTIONS ===============================

// UID for the application;
// this should correspond to the uid defined in the mmp file
const TUid KUidGSTPlayerApp = { 0xA000017F };

// -----------------------------------------------------------------------------
// CGSTPlayerApplication::CreateDocumentL()
// Creates CApaDocument object
// -----------------------------------------------------------------------------
//
CApaDocument* CGSTPlayerApplication::CreateDocumentL()
    {
    // Create an GSTPlayer document, and return a pointer to it
    return (static_cast<CApaDocument*>
                    ( CGSTPlayerDocument::NewL( *this ) ) );
    }

// -----------------------------------------------------------------------------
// CGSTPlayerApplication::AppDllUid()
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid CGSTPlayerApplication::AppDllUid() const
    {
    // Return the UID for the GSTPlayer application
    return KUidGSTPlayerApp;
    }

// End of File

