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
#include "afserializer.h"
#include <s32mem.h>
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
RBuf8 &operator <<(RBuf8 &dst, const QVariantHash &src)
{
    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::WriteOnly);

    QT_TRYCATCH_LEAVING(stream << src);
    const int dataLength(buffer.length());
    const unsigned char *dataPtr(reinterpret_cast<const unsigned char *>(buffer.constData()));
    if (dst.MaxLength() < dataLength) {
        dst.ReAllocL(dataLength);
    }
    dst.Copy(dataPtr, dataLength);
    return dst;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
QVariantHash &operator <<(QVariantHash &dst, const TDesC8 &src)
{
    QByteArray buffer(QByteArray::fromRawData(reinterpret_cast<const char *>(src.Ptr()),
                                               src.Length()) );

    QDataStream stream(&buffer, QIODevice::ReadOnly);
    stream >> dst;
    return dst;
}
