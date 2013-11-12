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
#include <xqconversions.h>
#include <qservicemanager.h>
#include "tsservicesproviderconfig.h"

const char ServicesInterface [] = "com.nokia.qt.taskswitcher.dataprovider";
// -----------------------------------------------------------------------------
CTsServiceProviderConfig* CTsServiceProviderConfig::NewLC(QObject* aEnv)
    {
    CTsServiceProviderConfig *self = new (ELeave)CTsServiceProviderConfig(aEnv);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
CTsServiceProviderConfig::CTsServiceProviderConfig(QObject* aEnv)
:
        iEnv(aEnv)
{
    //No implementation required
}

// -----------------------------------------------------------------------------
void CTsServiceProviderConfig::ConstructL()
    {
    QTM_NAMESPACE::QServiceInterfaceDescriptor *ptr(0);
    QT_TRYCATCH_LEAVING(
       QTM_NAMESPACE::QServiceManager serviceManager;
       const QList<QTM_NAMESPACE::QServiceInterfaceDescriptor> serivces(serviceManager.findInterfaces(QTM_NAMESPACE::QServiceFilter(ServicesInterface)));
       foreach(QTM_NAMESPACE::QServiceInterfaceDescriptor serviceImplementation, serivces)
           {
           ptr = new QTM_NAMESPACE::QServiceInterfaceDescriptor(serviceImplementation);
           if( 0 != ptr ) 
               {
               if( KErrNone != iServices.Append( ptr ) )
                   {
                   delete ptr;
                   }
               }
           }
       )
    }

// -----------------------------------------------------------------------------
CTsServiceProviderConfig::~CTsServiceProviderConfig()
    {
    for( TInt iter(0); iter < iServices.Count(); ++iter )
        {
        delete static_cast<QTM_NAMESPACE::QServiceInterfaceDescriptor *>(iServices[iter]);
        }
    iServices.Close();
    }

// -----------------------------------------------------------------------------
TInt CTsServiceProviderConfig::Count() const
    {
    return iServices.Count();
    }

// -----------------------------------------------------------------------------
QObject* CTsServiceProviderConfig::LoadL(TInt aOffset)const
    {
    const QTM_NAMESPACE::QServiceInterfaceDescriptor *service
        = static_cast<QTM_NAMESPACE::QServiceInterfaceDescriptor *>(iServices[aOffset]);
    QObject *retVal(0);
    QT_TRYCATCH_LEAVING(
    QTM_NAMESPACE::QServiceManager serviceManager;
    retVal = serviceManager.loadInterface(*service); )
    User::LeaveIfNull( retVal );
    QObject::connect(retVal, 
                     SIGNAL(createThumbnail(QPixmap,int,const void*)),
                     iEnv,
                     SLOT(createThumbnail(QPixmap,int,const void*)));
    
    QObject::connect(iEnv, 
                     SIGNAL(thumbnailCreated(QPixmap, const void*)),
                     retVal,
                     SLOT(thumbnailCreated(QPixmap, const void*)));

    return retVal;
    }
