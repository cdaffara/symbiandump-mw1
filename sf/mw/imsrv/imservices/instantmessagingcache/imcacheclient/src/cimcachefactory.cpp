/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  factory class to create access and updateer class implementation
*
*/



// INCLUDE FILES
#include    "cimcachefactory.h"

#include	"cimcacheupdater.h"
#include	"mimcacheupdater.h"
#include	"cimcacheaccessor.h"
#include	"mimcacheaccessor.h"
#include	"cimcacheclient.h"
// logs
#include    "imcachedebugtrace.h"

// ============================ MEMBER FUNCTIONS ===============================
 // -----------------------------------------------------------------------------
// CIMCacheFactory::Instance
//see whether there is an instance in the TLS
//if not create the instance and set the same in TLS
// -----------------------------------------------------------------------------
//
 EXPORT_C CIMCacheFactory* CIMCacheFactory::InstanceL()
    {
     TRACE( T_LIT("CIMCacheFactory::InstanceL begin") );
     CIMCacheFactory *factory = static_cast<CIMCacheFactory*>( Dll::Tls() );
    
     if ( ! factory )
        {
        // no existing instance, create a new one
        factory = CIMCacheFactory::NewL();
        CleanupStack::PushL( factory );
        User::LeaveIfError( Dll::SetTls( static_cast<TAny*>( factory ) ) );
        CleanupStack::Pop( factory );
        }
     TRACE( T_LIT("CIMCacheFactory::InstanceL end") );
     factory->IncrementReference();
     return factory;
    }    
 // -----------------------------------------------------------------------------
// CIMCacheFactory::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CIMCacheFactory* CIMCacheFactory::NewL()
	{
    TRACE( T_LIT("CIMCacheFactory::NewL begin") );
    CIMCacheFactory* self = new ( ELeave ) CIMCacheFactory() ;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );  //self
    TRACE( T_LIT("CIMCacheFactory::NewL end") );
    return self;
	}

// -----------------------------------------------------------------------------
// CIMCacheFactory::Release()
// -----------------------------------------------------------------------------
// 	       
EXPORT_C void CIMCacheFactory::Release()
    {
    TRACE( T_LIT("CIMCacheFactory::Release start") );
    //if any instance in TLS get the same and delete it    
    CIMCacheFactory *factory = static_cast<CIMCacheFactory*>( Dll::Tls() );
    if ( factory &&  !(factory->DecrementReference()))
    if ( factory )
        {
        delete factory;
        Dll::SetTls( NULL );
        }
    TRACE( T_LIT("CIMCacheFactory::Release end") );
    }
// -----------------------------------------------------------------------------
// CIMCacheFactory::IncrementReference()
// -----------------------------------------------------------------------------
//
void CIMCacheFactory::IncrementReference()
    {
    TRACE( T_LIT("CIMCacheFactory::IncrementReference ") );
    iReferenceCount++;   
    }

// -----------------------------------------------------------------------------
// CPresenceCacheServer::DecrementReference()
// -----------------------------------------------------------------------------
//
TInt CIMCacheFactory::DecrementReference()
    {
    TRACE( T_LIT("CIMCacheFactory::DecrementReference ") );
    iReferenceCount--; 
    return iReferenceCount;    
    }
    
// -----------------------------------------------------------------------------
// CIMCacheFactory::CIMCacheFactory()
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
// 
CIMCacheFactory::CIMCacheFactory()
	{
	}

// -----------------------------------------------------------------------------
// CIMCacheFactory::~CIMCacheFactory()
// Symbian OS desstructor 
// -----------------------------------------------------------------------------
//
CIMCacheFactory::~CIMCacheFactory()
	{
	TRACE( T_LIT("CIMCacheFactory::~CIMCacheFactory start") );
	iUpdaterArray.ResetAndDestroy();
	iAccessorArray.ResetAndDestroy();
	delete iServerKeepAlive;
	TRACE( T_LIT("CIMCacheFactory::~CIMCacheFactory end") );
	}
// -----------------------------------------------------------------------------
// CIMCacheFactory::ConstructL()
// Symbian OS default constructor can leave.
// -----------------------------------------------------------------------------
//
void CIMCacheFactory::ConstructL( )
	{
	TRACE( T_LIT("CIMCacheFactory::ConstructL start") );
	iServerKeepAlive = new(ELeave) CIMCacheClient(); // CSI: 74 # this needs to be like this
	TRACE( T_LIT("CIMCacheFactory::ConstructL end") );
	}

// -----------------------------------------------------------------------------
// CIMCacheFactory::CreateUpdaterL()
// -----------------------------------------------------------------------------
//
EXPORT_C MIMCacheUpdater* CIMCacheFactory::CreateUpdaterL(TInt aServiceId,
															const TDesC& aUserId ,
															TBool aRegistrationNeeded )
    {
    TRACE( T_LIT("CIMCacheFactory::CreateAccessL begin") );
    
    CIMCacheUpdater* updater = NULL;

	TInt count = iUpdaterArray.Count();
	for( TInt i = 0; i < count; ++i )
		{
		if( iUpdaterArray[i]->ServiceId() == aServiceId )
			{
			updater = iUpdaterArray[i];
			break;
			}
		}
	if( !updater ) 
		{
		updater = CIMCacheUpdater::NewL( aServiceId, aUserId, aRegistrationNeeded );
		iUpdaterArray.Append(updater);
		}
		
    TRACE( T_LIT("CIMCacheFactory::CreateAccessL end") );
    return updater;
    }

// -----------------------------------------------------------------------------
// CIMCacheFactory::CreateAccessorL()
// -----------------------------------------------------------------------------
//
EXPORT_C MIMCacheAccessor* CIMCacheFactory::CreateAccessorL(TInt aServiceId,
															const TDesC& aUserId  )
    {
    TRACE( T_LIT("CIMCacheFactory::CreateAccessL begin") );
    
    CIMCacheAccessor* accessor = NULL;

	TInt count = iAccessorArray.Count();
	for( TInt i = 0; i < count; ++i )
		{
		if( iAccessorArray[i]->ServiceId() == aServiceId )
			{
			accessor = iAccessorArray[i];
			break;
			}
		}
	if( !accessor ) 
		{
		accessor = CIMCacheAccessor::NewL( aServiceId, aUserId );
		iAccessorArray.Append(accessor);
		}
		
    TRACE( T_LIT("CIMCacheFactory::CreateAccessL end") );
    return accessor;
    }
//  End of File

