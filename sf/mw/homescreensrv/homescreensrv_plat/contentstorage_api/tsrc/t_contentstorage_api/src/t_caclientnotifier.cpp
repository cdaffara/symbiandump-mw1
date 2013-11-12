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

#include "t_caclientnotifier.h"
#include "caentry.h"
#include "canotifier.h"
#include "canotifierfilter.h"
#include "caservice.h"

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
TestCaClientNotifier::TestCaClientNotifier():
    mEntryId(0),
    mGroupId(0),
    mChangeType(AddChangeType),
    mEntry(NULL),
    mCount(0),
    mEntryIds(new QList<int>()),
    mGroupIds(new QList<int>()),
    mChangeTypes(new QList<ChangeType>()),
    mEntries(new QList<const CaEntry *>()),
    mFilter(NULL),
    mNotifier(NULL)
{

}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
TestCaClientNotifier::TestCaClientNotifier(const CaService &service, CaNotifierFilter *filter):
    mEntryId(0),
    mGroupId(0),
    mChangeType(AddChangeType),
    mEntry(NULL),
    mCount(0),
    mEntryIds(new QList<int>()),
    mGroupIds(new QList<int>()),
    mChangeTypes(new QList<ChangeType>()),
    mEntries(new QList<const CaEntry *>())
{
    if (!filter) {
        mFilter = new CaNotifierFilter();
        mNotifier = service.createNotifier(*mFilter);
    } else {
        mFilter = NULL;
        mNotifier = service.createNotifier(*filter);
    }
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClientNotifier::clean()
{
    mEntryId = 0;
    mGroupId = 0;
    mChangeType = AddChangeType;
    mCount = 0;
    delete mEntryIds;
    mEntryIds = new QList<int>();
    delete mGroupIds;
    mGroupIds = new QList<int>();
    delete mChangeTypes;
    mChangeTypes = new QList<ChangeType>();
    delete mEntries;
    mEntries = new QList<const CaEntry *>();
    delete mEntry;
    mEntry = NULL;
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
TestCaClientNotifier::~TestCaClientNotifier()
{
    delete mEntry;
    delete mFilter;
    delete mNotifier;
    delete mEntryIds;
    delete mGroupIds;
    delete mChangeTypes;
    delete mEntries;

}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
CaNotifier *TestCaClientNotifier::getNotifier() const
{
    return mNotifier;
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
CaNotifierFilter *TestCaClientNotifier::getFilter() const
{
    return mFilter;
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClientNotifier::entryChanged(int entryId, ChangeType changeType)
{
    mEntryId = entryId;
    mChangeType = changeType;
    mEntryIds->append(entryId);
    mChangeTypes->append(changeType);
    mEntries->append(NULL);
    mGroupIds->append(0);
    mCount++;
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClientNotifier::entryChanged(const CaEntry &entry, ChangeType changeType)
{
    delete mEntry;
    mEntry = new CaEntry(entry);
    mEntryId = entry.id();
    mChangeType = changeType;
    mEntryIds->append(entry.id());
    mChangeTypes->append(changeType);
    mEntries->append(&entry);
    mGroupIds->append(0);
    mCount++;
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClientNotifier::entryTouched(int id)
{
    mEntryId = id;
    mEntryIds->append(id);
    mChangeTypes->append(AddChangeType);
    mEntries->append(NULL);
    mGroupIds->append(0);
    mCount++;
}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void TestCaClientNotifier::groupContentChanged(int groupId)
{
    mGroupId = groupId;
    mEntryIds->append(0);
    mChangeTypes->append(AddChangeType);
    mEntries->append(NULL);
    mGroupIds->append(groupId);
    mCount++;
}
