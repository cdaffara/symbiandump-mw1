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

#ifndef CAOBJECTADAPTER_H
#define CAOBJECTADAPTER_H

// INCLUDES
#include <QPixmap>

#include <cadefs.h>

class QSize;
class CaEntry;
class CaIconDescription;
class HbIcon;

class CaObjectAdapter
{
public:

    static HbIcon makeIcon(const CaEntry &entry, const QSizeF &size);

    static void setId(CaEntry &entry, int id);

    static void setId(CaIconDescription &iconDescription, int id);
                      
    static ErrorCode convertErrorCode(int internalErrorCode);

};

#endif // CAOBJECTADAPTER_H
