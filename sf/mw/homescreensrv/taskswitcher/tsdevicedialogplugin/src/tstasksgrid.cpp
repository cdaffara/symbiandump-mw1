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

#include "tstasksgrid.h"

void TsTasksGrid::polish(HbStyleParameters &params)
{
    HbGridView::polish(params);

    // center second item when dialog is shown
    if (model()) {
        scrollTo(model()->index(1, 0), PositionAtCenter);
    }
}
