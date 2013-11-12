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

#include "afactivation.h"
#include "afactivation_p.h"

#include "afstorageproxy.h"

AfActivation::AfActivation(QObject *parent) : QObject(parent), d_ptr(0)
{
    QSharedPointer<AfStorageProxy> connection(new AfStorageProxy());    
    QT_TRAP_THROWING(
        if(!connection->waitActivity())
        {
            User::Leave(KErrGeneral);
        }
    )
    
    d_ptr = new AfActivationPrivate(connection, this);
    connect(d_ptr, SIGNAL(activated(Af::ActivationReason,QString,QVariantHash)), this, SIGNAL(activated(Af::ActivationReason,QString,QVariantHash)));
}
    
QVariantHash AfActivation::parameters() const
{
    return d_ptr->parameters();
}

Af::ActivationReason AfActivation::reason() const
{
    return d_ptr->reason();
}

QString AfActivation::name() const
{
    return d_ptr->name();
}
