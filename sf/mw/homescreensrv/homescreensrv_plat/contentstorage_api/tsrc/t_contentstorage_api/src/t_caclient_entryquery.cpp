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
* Description:  Main test class for hspluginmodel library.
*
*/

#include <HbIcon>
#include "t_caclient.h"
#include "caservice.h"
#include "caquery.h"
#include "calocalizationentry.h"
#include "camenuiconutility.h"


// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_getEntryIcon()
{
    {
    CaEntry entry;
    entry.setEntryTypeName(QString("application"));
    QSizeF size(50,50);
    
    HbIcon icon = CaMenuIconUtility::getEntryIcon(entry, size);
    QVERIFY(icon.size().isValid());
    QVERIFY(icon.height() > 0);
    QVERIFY(icon.width() > 0);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClient::Test_getApplicationIcon()
{
    {
    int uid = 270495019; // contest mbm icon
    QSizeF size(50,50);
    
    HbIcon icon = CaMenuIconUtility::getApplicationIcon(uid,size);
    
    QVERIFY(icon.size().isValid());
    QVERIFY(icon.height() > 0);
    QVERIFY(icon.width() > 0);
    
    int uid2 = 537006483; // AknConf mif icon
    HbIcon icon2 = CaMenuIconUtility::getApplicationIcon(uid2,size);
    QVERIFY(icon2.size().isValid());
    }
}

