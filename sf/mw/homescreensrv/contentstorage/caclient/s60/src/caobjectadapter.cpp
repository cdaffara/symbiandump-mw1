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

#include <e32std.h>
#include <exception>
#include <e32base.h>
#include <badesca.h>
#include <AknIconUtils.h>
#include <fbs.h>
#include <bitdev.h>
#include <QPixmap>
#include <HbIcon>
#include <QBitmap>
#include <QDebug>
#include <XQConversions>

#include "cadef.h"
#include "caobjectadapter.h"
#include "cainnerentry.h"
#include "caentry_p.h"
#include "cainnerquery.h"
#include "caquery.h"

#include "canotifierfilter.h"
#include "cainnernotifierfilter.h"
#include "caclient_defines.h"

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
void CaObjectAdapter::convertL(const CaEntry &fromEntry,
                               CCaInnerEntry &toEntry)
{
    toEntry.SetId(fromEntry.id());

    toEntry.SetTextL(
        XQConversions::qStringToS60Desc(fromEntry.text())->Des(),
        fromEntry.isLocalized(NameLocalized));

    toEntry.SetDescriptionL(
        XQConversions::qStringToS60Desc(fromEntry.description())->Des(),
        fromEntry.isLocalized(DescriptionLocalized));

    toEntry.SetEntryTypeNameL(
        XQConversions::qStringToS60Desc(fromEntry.entryTypeName())->Des());

    toEntry.SetFlags(static_cast<TUint>(fromEntry.flags()));

    toEntry.SetRole(static_cast<TUint>(fromEntry.role()));

    const CaIconDescription fromIconDescription =
        fromEntry.iconDescription();

    toEntry.SetIconDataL(
        XQConversions::qStringToS60Desc(
            fromIconDescription.filename())->Des(),
        XQConversions::qStringToS60Desc(
            fromIconDescription.skinId())->Des(),
        XQConversions::qStringToS60Desc(
            fromIconDescription.applicationId())->Des());

    const QMap<QString, QString> attributesMap = fromEntry.attributes();

    foreach(QString key, attributesMap.keys()) {
        if (key == APPLICATION_UID_ATTRIBUTE_NAME) {
            const TInt32 uid = attributesMap.value(key).toInt();
            toEntry.SetUid(uid);
        } else {
            toEntry.AddAttributeL(
                XQConversions::qStringToS60Desc(key)->Des(),
                XQConversions::qStringToS60Desc(
                    attributesMap.value(key))->Des());
        }
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaObjectAdapter::convertL(const CaQuery &fromQuery,
                               CCaInnerQuery &toQuery)
{
    toQuery.SetParentId(fromQuery.parentId());
    
    toQuery.SetChildId(fromQuery.childId());

    toQuery.SetFlagsOn(static_cast<TUint>(fromQuery.flagsOn()));

    toQuery.SetFlagsOff(static_cast<TUint>(fromQuery.flagsOff()));

    toQuery.SetRole(static_cast<TInt>(fromQuery.entryRoles()));

    toQuery.SetCount(fromQuery.count());

    const QStringList list = fromQuery.entryTypeNames();

    CDesC16ArrayFlat *sourceList =
        new(ELeave)CDesC16ArrayFlat(KDefaultGranularity);
    CleanupStack::PushL(sourceList);

    foreach(const QString str, list) {
        sourceList->AppendL(XQConversions::qStringToS60Desc(str)->Des());
    }

    toQuery.SetEntryTypeNames(sourceList);

    CleanupStack::Pop(sourceList);

    // mapping sort enums
    SortAttribute sortAttr;
    Qt::SortOrder sortOrder;
    fromQuery.getSort(sortAttr, sortOrder);
    toQuery.SetSort(CaObjectAdapter::getSortCode(sortAttr, sortOrder));

    const QMap<QString, QString> attributesMap = fromQuery.attributes();

    foreach(QString key, attributesMap.keys()) {
        if (key == APPLICATION_UID_ATTRIBUTE_NAME) {
            const TInt32 uid = attributesMap.value(key).toInt();
            toQuery.SetUid(uid);
        }
        else {
            toQuery.AddAttributeL(
                XQConversions::qStringToS60Desc(key)->Des(),
                XQConversions::qStringToS60Desc(
                    attributesMap.value(key))->Des());
        }
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaObjectAdapter::convert(
        const CCaInnerEntry &fromEntry, CaEntry &toEntry)
{
    toEntry.setId(fromEntry.GetId());

    toEntry.setText(XQConversions::s60DescToQString(fromEntry.GetText()),
            fromEntry.isLocalized(CCaInnerEntry::ENameLocalized));

    toEntry.setDescription(
        XQConversions::s60DescToQString(fromEntry.GetDescription()),
        fromEntry.isLocalized(CCaInnerEntry::EDescriptionLocalized));
    toEntry.setEntryTypeName(
        XQConversions::s60DescToQString(fromEntry.GetEntryTypeName()));

    toEntry.setFlags(static_cast<EntryFlag>(fromEntry.GetFlags()));

    // take care of converting icon attributes
    const CCaInnerIconDescription* innerIcon = fromEntry.Icon();
    CaIconDescription iconDescription;

    iconDescription.setId(innerIcon->Id());
    iconDescription.setFilename(
        XQConversions::s60DescToQString(innerIcon->FileName()));
    iconDescription.setSkinId(
        XQConversions::s60DescToQString(innerIcon->SkinId()));
    iconDescription.setApplicationId(
        XQConversions::s60DescToQString(innerIcon->ApplicationId()));

    toEntry.setIconDescription(iconDescription);

    const RCaEntryAttrArray &attributesArray = fromEntry.GetAttributes();

    if (toEntry.entryTypeName() == APPLICATION_ENTRY_TYPE_NAME
            || toEntry.entryTypeName() == WIDGET_ENTRY_TYPE_NAME) {
        toEntry.setAttribute(APPLICATION_UID_ATTRIBUTE_NAME,
            QString::number(fromEntry.GetUid()));
    }

    const int attributesArrayCount = attributesArray.Count();

    for (int i = 0; i < attributesArrayCount; ++i) {
        const CCaEntryAttribute *const attribute = attributesArray[i];
        toEntry.setAttribute(
            XQConversions::s60DescToQString(attribute->Name()),
            XQConversions::s60DescToQString(attribute->Value()));
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaObjectAdapter::convertL(const CaNotifierFilter &notifierFilter,
                               CCaInnerNotifierFilter &source)
{
    source.SetParentId(static_cast<TInt>(notifierFilter.getParentId()));

    RArray<TInt> idsList;
    CleanupClosePushL(idsList);
    CaObjectAdapter::convertL(notifierFilter.getIds(), idsList);
    source.SetIdsL(idsList);

    CleanupStack::PopAndDestroy(&idsList);

    switch (notifierFilter.getEntryRole()) {
    case static_cast<TUint>(ItemEntryRole):
        source.SetEntryRole(EItemEntryRole);
        break;
    case static_cast<TUint>(GroupEntryRole):
        source.SetEntryRole(EGroupEntryRole);
        break;
    default:
        source.SetEntryRole(ENoneEntryRole);
        break;
    }
    CDesC16ArrayFlat *typeNames = new(ELeave) CDesC16ArrayFlat(
        KDefaultGranularity);
    CleanupStack::PushL(typeNames);

    const QStringList stringList = notifierFilter.getTypeNames();

    foreach(QString str, stringList) {
        typeNames->AppendL(XQConversions::qStringToS60Desc(str)->Des());
    }

    source.SetTypeNames(typeNames);

    CleanupStack::Pop(typeNames);
}
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaObjectAdapter::convertL(
    const RPointerArray<CCaInnerEntry> &fromEntriesArray,
    QList< QSharedPointer<CaEntry> > &toEntryList)
{
    for (int i = 0; i < fromEntriesArray.Count(); ++i) {
        const QSharedPointer<CaEntry> toEntry (new CaEntry(
            static_cast<EntryRole>(fromEntriesArray[i]->GetRole())));

        CaObjectAdapter::convert(*fromEntriesArray[i], *toEntry);

        toEntryList << toEntry;
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaObjectAdapter::convertL(const QList<int> &entryIdList,
                               RArray<TInt> &source)
{
    foreach(int id, entryIdList) {
        source.AppendL(static_cast<TInt>(id));
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaObjectAdapter::convertL(const RArray<TInt> &innerEntryIdList,
                               QList<int> &idList)
{
    for (TInt i = 0; i < innerEntryIdList.Count(); ++i) {
        idList.append(innerEntryIdList[i]);
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
ErrorCode CaObjectAdapter::convertErrorCode(int internalErrorCode)
{
    ErrorCode error(NoErrorCode);
    switch (internalErrorCode) {
    case KErrNone:
        error = NoErrorCode;
        break;
    case KErrNotFound:
        error = NotFoundErrorCode;
        break;
    case KErrNoMemory:
        error = OutOfMemoryErrorCode;
        break;
    case KErrArgument:
        error = BadArgumentErrorCode;
        break;
    case KErrServerTerminated:
        error = ServerTerminated;
        break;
    default:
        error = UnknownErrorCode;
        break;
    }
    return error;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaObjectAdapter::convert(
    CaNotifierPrivate::NotifierType src,
    CCaInnerNotifierFilter::TNotifierType &dest)
{
    switch (src) {
    case CaNotifierPrivate::EntryChangedWithIdNotifierType:
        dest = CCaInnerNotifierFilter::EEntryChangedWithId;
        break;
    case CaNotifierPrivate::EntryChangedWithEntryNotifierType:
        dest = CCaInnerNotifierFilter::EEntryChangedWithEntry;
        break;
    case CaNotifierPrivate::EntryTouchedNotifierType:
        dest = CCaInnerNotifierFilter::EEntryTouched;
        break;
    case CaNotifierPrivate::GroupContentChangedNotifierType:
        dest = CCaInnerNotifierFilter::EGroupContentChanged;
        break;
    }
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaObjectAdapter::convert(
    TChangeType src, ChangeType &dest)
{
    switch (src) {
    case EAddChangeType:
        dest = AddChangeType;
        break;
    case ERemoveChangeType:
        dest = RemoveChangeType;
        break;
    case EUpdateChangeType:
        dest = UpdateChangeType;
        break;
    }
}

CCaInnerQuery::TSortAttribute CaObjectAdapter::getSortCode(
    SortAttribute sortAttribute, Qt::SortOrder sortOrder)
{
    CCaInnerQuery::TSortAttribute sortCode(CCaInnerQuery::Default);

    switch (sortAttribute) {
    case NameSortAttribute: {
        sortCode = (sortOrder == Qt::AscendingOrder) ? CCaInnerQuery::Name
                   : CCaInnerQuery::NameDesc;
        break;
    }
    case CreatedTimestampSortAttribute: {
        sortCode = (sortOrder == Qt::AscendingOrder)
                   ? CCaInnerQuery::CreatedTimestamp
                   : CCaInnerQuery::CreatedTimestampDesc;
        break;
    }
    case MostUsedSortAttribute: {
        sortCode = (sortOrder == Qt::AscendingOrder)
                   ? CCaInnerQuery::MostUsed : CCaInnerQuery::MostUsedDesc;
        break;
    }
    case LastUsedSortAttribute: {
        sortCode = (sortOrder == Qt::AscendingOrder)
                   ? CCaInnerQuery::LastUsed : CCaInnerQuery::LastUsedDesc;
        break;
    }
    case DefaultSortAttribute: {
        sortCode = (sortOrder == Qt::AscendingOrder)
                   ? CCaInnerQuery::Default : CCaInnerQuery::DefaultDesc;
        break;
    }
    }

    return sortCode;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CaObjectAdapter::setId(CaEntry &entry,
                            int id)
{
    entry.setId(id);
}
