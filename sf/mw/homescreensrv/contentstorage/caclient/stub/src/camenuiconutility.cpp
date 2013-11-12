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

#include <HbIcon>
#include <QDir>
#include <cadefs.h>

#include "camenuiconutility.h"
#include "caicondescription.h"
#include "caentry.h"

/*!
 Get icon when uid is defined.
 \param entry const reference to CaEntry.
 \param sie const reference to icon size.
 \retval icon.
 */
HbIcon CaMenuIconUtility::getApplicationIcon(int uid,
        const QSizeF &size)
{
    Q_UNUSED(size);
    HbIcon icon;
    return icon;
}

/*!
 Get icon.
 \param entry const reference to CaEntry.
 \param sie const reference to icon size.
 \retval icon.
 */
HbIcon CaMenuIconUtility::getEntryIcon(const CaEntry& entry,
        const QSizeF &size)
{
    HbIcon icon;
    QString filename(entry.iconDescription().filename());
    if (!filename.isEmpty()) {
        icon = HbIcon(filename);
    }
    if (icon.isNull() || !(icon.size().isValid())) {
        icon = HbIcon(QDir(".").absoluteFilePath("resource/application.png"));
    }
    if (entry.entryTypeName() == "widget") {
        icon.addBadge(Qt::AlignBottom | Qt::AlignLeft,
                    HbIcon("qtg_small_homescreen"));
    }
    return icon;
}


