// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// ServiceRegistry.h
// 
//

#include "ServiceRegistry.h"

/** Constructs a new CServiceRegistry object.

@return A new CServiceRegistry object.
@publishedPartner
@released
*/
EXPORT_C CServiceRegistry* CServiceRegistry::NewL()
	{
	CServiceRegistry* self = new(ELeave) CServiceRegistry();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/** Destructor 
@publishedPartner
@released
*/
EXPORT_C CServiceRegistry::~CServiceRegistry()
	{
	iApaLsSession.Close();	
	}

/** Constructor 
@publishedPartner
@released
*/		
CServiceRegistry::CServiceRegistry()
	{	
	}
	
/** Second phase constructor. 
@publishedPartner
@released
*/	
void CServiceRegistry::ConstructL()
	{	
	User::LeaveIfError(iApaLsSession.Connect());
	}
		
/** Adds a default association to the registry.
@param aServiceUid The uid of the service.
@param aDataType The datatype.
@param aDefaultAppUid The uid of the application that supports the
datatype and the service.
@return A standard error code.
@capability WriteDeviceData
@publishedPartner
@released
*/	
EXPORT_C TInt CServiceRegistry::SetDefault(TUid aServiceUid, const TDataType& aDataType, 
	TUid aDefaultAppUid)
	{
	return iApaLsSession.InsertDataMapping(aDataType, KDataTypePriorityNormal, aDefaultAppUid, aServiceUid);
	}
	
/** Deletes an association from the registry.
@param aServiceUid The uid of the service.
@param aDataType The datatype.
@return KErrNotFound if the association doesn't exist or any other standard error code.
@capability WriteDeviceData
@publishedPartner
@released
*/	
EXPORT_C TInt CServiceRegistry::RemoveEntry(TUid aServiceUid, const TDataType& aDataType)
	{	
	return iApaLsSession.DeleteDataMapping(aDataType, aServiceUid);
	}
	
/** Gets the default association from the registry.
@param aServiceUid The uid of the service.
@param aDataType The datatype.
@param aDefaultAppUid The uid of the default application, or KNullUid if the
the association doesn't exist.
@return KErrNotFound if the association doesn't exist or any other standard error code.
@publishedPartner
@released
*/	
EXPORT_C TInt CServiceRegistry::GetDefault(TUid aServiceUid, const TDataType& aDataType,
	TUid& aDefaultAppUid)
	{	
	return iApaLsSession.GetAppByDataType(aDataType, aServiceUid, aDefaultAppUid);
	}
	
