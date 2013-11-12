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

#include <caentry.h>

#include "cahandler.h"
#include "cahandlerproxy.h"
#include "cahandlerloader.h"
#include "caobjectadapter.h"

/*!
    \class CaHandlerProxy
    \ingroup
    \brief Forwards execute request to an implemenation provided by specific handler loader.

    \sa CaHandlerLoader
*/

/*!
  Destructor.
*/
CaHandlerProxy::~CaHandlerProxy()
{
}


/*!
  Constructs handler proxy.
  \param loader Provides handler implementations. It has to be pointer to a valid object.
*/
CaHandlerProxy::CaHandlerProxy(const QSharedPointer<CaHandlerLoader> &loader):
    mLoader(loader)
{
    Q_ASSERT(!mLoader.isNull());
}

/*!
  Forwards execute request to an appropriate handler if found otherwise ignores the request.
  \param entry Subject for the requested command.
  \param commandName Name of the command to be executed.
  \param receiver QObject class with slot.
  \param member Slot from QObject class.
  \retval KErrNone on succes, error code otherwise.
  \sa e32err.h for KErrNone definition.
*/
int CaHandlerProxy::execute(const CaEntry &entry, const QString &commandName,
        QObject* receiver, const char* member)
{
    CaHandler *const handler = getHandler(entry, commandName);
    int result = -1;

    if (handler) {
            result = handler->execute(entry, commandName, receiver, member);
    }
    return result;
}

/*!
  Looks for handler implementation in local cache or if not found, request it from
  handler loader.
  \param entry The entry being a subject for the requested command.
  \param commandName Name of the command to be executed.
  \return Pointer to a handler instance if available, NULL otherwise.
*/
CaHandler *CaHandlerProxy::getHandler(const CaEntry &entry,
                                      const QString &commandName)
{
    CaHandler *implementation(0);

    const QString entryTypeName(entry.entryTypeName());

    const ImplementationMapIterator it(
        mImplementationMap.find(entryTypeName));

    if (it != mImplementationMap.end()) {
        implementation = it->data();
    } else {
        implementation = mLoader->loadHandler(entryTypeName, commandName);
        mImplementationMap[entryTypeName] = QSharedPointer<CaHandler>(implementation);
    }

    return implementation;
}
