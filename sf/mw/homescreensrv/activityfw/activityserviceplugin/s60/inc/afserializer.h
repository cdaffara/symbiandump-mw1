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
#ifndef HSSERIALIZER_H
#define HSSERIALIZER_H

#include <qvariant.h>

/**
 * Operator serialize VarinatHash to RBuf8
 */
RBuf8 &operator <<(RBuf8 &dst, const QVariantHash &src);

/**
 * Operator deserialize RBuf8 to VarinatHash
 */
QVariantHash &operator <<(QVariantHash &dst, const TDesC8 &src);

#endif
