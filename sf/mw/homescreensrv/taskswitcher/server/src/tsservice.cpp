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
#include <QObject>
#include <QDateTime>
#include <QVariantHash>

#include <xqconversions.h>

#include "tsservice.h"
#include "tsmodelobserver.h"
#include "tsserviceobserver.h"
#include "tsresourcemanager.h"


// -----------------------------------------------------------------------------
LOCAL_C QVariantHash valueL( QObject *aModel, TInt aOffset )
    {
    QList<QVariantHash> items;
    QMetaObject::invokeMethod( aModel, 
                               "taskList", 
                               Q_RETURN_ARG(QList<QVariantHash>, 
                               items ) );
    if( aOffset >= items.count() ) 
        {
        User::Leave(KErrCorrupt);
        }
    return items.at(aOffset);
    }

// -----------------------------------------------------------------------------
CTsService* CTsService::NewLC(MTsResourceManager& resources, QObject* model)
    {
    CTsService *self = new (ELeave)CTsService();
    CleanupStack::PushL(self);
    self->ConstructL(resources, model);
    return self;
    }

// -----------------------------------------------------------------------------
CTsService::CTsService()
    {
    //No implementation required
    }

// -----------------------------------------------------------------------------
void CTsService::ConstructL( MTsResourceManager& resources, QObject* model )
    {
    iModel = model;
    User::LeaveIfNull(iModel);
    // @todo: add checking for all necessary methods
    
    iServiceObserver = new TsServiceObserver(*this);
    User::LeaveIfNull(iServiceObserver);
    QObject::connect(iModel, 
                     SIGNAL(dataChanged()),
                     iServiceObserver,
                     SLOT(dataChanged()));
    QMetaObject::invokeMethod(iModel,
                              "setResources",
                              Q_ARG(MTsResourceManager&, resources));
    }

// -----------------------------------------------------------------------------
CTsService::~CTsService()
    {
    delete iModel;
    delete iServiceObserver;
    delete iBuffer;
    }

// -----------------------------------------------------------------------------
void CTsService::DataChanged()
    {
    if(0 != iModelObserver) 
        {
        iModelObserver->DataChanged(*this);
        }
    }
// -----------------------------------------------------------------------------
TInt CTsService::Count() const
    {
    QList<QVariantHash> items;
    QMetaObject::invokeMethod(iModel, 
                              "taskList", 
                              Q_RETURN_ARG(QList<QVariantHash>, items));
    return items.count();
    }

// -----------------------------------------------------------------------------
void CTsService::SetObserver( MTsModelObserver *aObserver )
    {
    iModelObserver = aObserver;
    }

// -----------------------------------------------------------------------------
const TDesC& CTsService::DisplayNameL( TInt aOffset ) const
    {
    return StringValueL( aOffset, "TaskName" );
    }

// -----------------------------------------------------------------------------
TInt CTsService::IconHandleL( TInt aOffset ) const
    {
    return IntValueL( aOffset, "TaskScreenshot" );
    }

// -----------------------------------------------------------------------------
TTime CTsService::TimestampL( TInt aOffset ) const
    {
    return TimeValueL( aOffset, "TaskTimestamp" );
    }

// -----------------------------------------------------------------------------
TTime CTsService::TimestampUpdateL(TInt offset) const
{
    return TimeValueL(offset, "TaskUpdateTimestamp");
}

TTsModelItemKey CTsService::KeyL( TInt aOffset ) const
    {
    return TTsModelItemKey( IntValueL( aOffset, "TaskId" ), 
                            reinterpret_cast<TInt>( this ) );
    }

// -----------------------------------------------------------------------------
TBool CTsService::IsActiveL( TInt aOffset ) const
    {
    return IntValueL( aOffset, "TaskIsRunning" );
    }

// -----------------------------------------------------------------------------
TBool CTsService::IsClosableL( TInt aOffset ) const
    {
    return IntValueL( aOffset, "TaskCanBeClosed" );
    }

// -----------------------------------------------------------------------------
TBool CTsService::IsMandatoryL( TInt aOffset ) const
    {
    const QVariantHash values(valueL(iModel, aOffset));
    const QString key("TaskIsMandatory");
    return values.contains(key) ? values.value(key).toInt() : ETrue;
    }

// -----------------------------------------------------------------------------
TBool CTsService::CloseL( TTsModelItemKey aKey ) const
    {
    bool result(false);
    QMetaObject::invokeMethod( iModel, 
                               "closeTask", 
                               Q_RETURN_ARG(bool, result), 
                               Q_ARG(QVariant, aKey.Key() ) );
    return result;
    }

// -----------------------------------------------------------------------------
TBool CTsService::LaunchL( TTsModelItemKey aKey ) const
    {
    bool result(false);
    QMetaObject::invokeMethod( iModel, 
                               "openTask", 
                               Q_RETURN_ARG(bool, result), 
                               Q_ARG(QVariant, aKey.Key() ) );
    return result;
    }

// -----------------------------------------------------------------------------
TInt CTsService::IntValueL( TInt aOffset, const char* aKey) const
    {
    return valueL(iModel, aOffset).value( aKey ).toInt();
    }

// -----------------------------------------------------------------------------
TTime CTsService::TimeValueL(TInt aOffset, const char* aKey) const
    {
    // Conversion between TTime which counts from year 0, and QDateTime which uses unix epoch (1st Jan 1970)
    QDateTime timestamp = valueL( iModel, aOffset ).value( aKey ).toDateTime();
    
    return TTime( _L( "19700000:" ) ) + TTimeIntervalSeconds( timestamp.toTime_t() ) +
                         TTimeIntervalMicroSeconds( timestamp.time().msec() * 1000 );
    }

// -----------------------------------------------------------------------------
const TDesC& CTsService::StringValueL( TInt aOffset, const char* aKey ) const
    {
    delete iBuffer;
    const_cast<CTsService *>(this)->iBuffer = 0;
    
    const QVariantHash item(valueL(iModel, aOffset));
    QT_TRYCATCH_LEAVING(
    const_cast<CTsService *>(this)->iBuffer = 
    XQConversions::qStringToS60Desc(item.value(aKey).toString()));
    return *iBuffer;
    }
