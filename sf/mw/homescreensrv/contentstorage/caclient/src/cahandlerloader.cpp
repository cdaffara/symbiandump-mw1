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
 * Description:
 *
 */

#include <QString>
#include "cahandlerloader.h"

/*!
    \class CaHandlerLoader
    \ingroup 
    \brief Interface for class of objects providing command handlers.

    \sa CaHandlerLoader
*/

/*!
    Destructor.
*/
CaHandlerLoader::~CaHandlerLoader()
{
}

/*!
    \fn virtual CaHandler *loadHandler(const QString &entryTypeName,
                                   const QString &commandName) = 0;
    Loads handler implementations appropriate for the requested entry type name and command.

    The caller takes ownership of the returned pointer.

    \param entryTypeName an entry type name.
    \param commandName a name of the command to be handled.
    \retval a pointer to the handler serving the entry type and command if found, NULL otherwise.
*/
