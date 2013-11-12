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
 
/*!
    \class CaHandler
    \ingroup
    \brief Interface for command handlers
*/

/*!
    \fn int CaHandler::execute(const CaEntry &entry, const QString &commandName) = 0
    \param entry the entry for which the command is executed.
    \param commandName the name of the command to execute.
    \retval an error code.
    \sa e32err.h for error code descriptions.
*/
