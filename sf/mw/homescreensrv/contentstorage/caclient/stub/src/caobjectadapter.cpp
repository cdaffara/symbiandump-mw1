/*
 * Copyright (c)2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  ?Description
 *
 */

#include <QDir>
#include <HbIcon>


#include "caobjectadapter.h"
#include "caentry.h"
#include "caicondescription.h"
#include <cadefs.h>

/*!
 * Create an icon.
 * \param entry a CaEntry instance.
 * \param size Currently ignored.
 * \retval a HbIcon instance.
 */
HbIcon CaObjectAdapter::makeIcon(const CaEntry &entry, const QSizeF &size)
{
    Q_UNUSED(size);
    HbIcon icon;
    QString filename(entry.iconDescription().filename());
    if (!filename.isEmpty()) {
        icon = HbIcon(filename);
        //icon.addBadge( Qt::AlignTop | Qt::AlignRight,  HbIcon("qtg_small_hs_widget"));
    }
    if (icon.isNull() || !(icon.size().isValid())) {
        icon = HbIcon("qtg_large_application");
    }
    return icon;
}


/*!
 * Set entry id.
 * \param entry entry.
 * \param id entry id.
 */
void CaObjectAdapter::setId(CaEntry &entry, int id)
{
    entry.setId(id);
}

/*!
 * Set icon description id.
 * \param iconDescription icon description.
 * \param id icon description id.
 */
void CaObjectAdapter::setId(CaIconDescription &iconDescription, int id)
{
    iconDescription.setId(id);
}

/**
 * Convert Symbian error code to error code (used in QT code).
 * \param internalErrorCode Symbian error code.
 * \retval converted error code (ErrorCode enum).
 */
ErrorCode CaObjectAdapter::convertErrorCode(int internalErrorCode)
{
    ErrorCode error(NoErrorCode);
    switch (internalErrorCode) {
    case 0:
        error = NoErrorCode;
        break;
    default:
        error = UnknownErrorCode;
        break;
    }
    return error;
}

