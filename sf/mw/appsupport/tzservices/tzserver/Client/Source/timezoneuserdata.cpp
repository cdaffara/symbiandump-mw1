// Copyright (c) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
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
//

#include <tz.h>
#include <vtzrules.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <tzusernames.h>
#include <tzuserdefineddata.h>
#endif


const TInt KMaxYear = 9999;
const TInt KMaxStandardName = 255;
const TInt KMaxShortName = 10;


CTzUserNames::CTzUserNames()
	{
	}


/**
Destroys this CTzUserNames object.
*/
EXPORT_C CTzUserNames::~CTzUserNames()
	{
	delete iStandardName;
	delete iShortStandardName;
	delete iDaylightName;
	delete iShortDaylightName;	
	delete iCityName;
	delete iRegionName;
	}


void CTzUserNames::ConstructL(const TDesC& aStandardName, const TDesC& aShortStandardName, const TDesC& aDaylightName, const TDesC& aShortDaylightName, const TDesC& aCityName, const TDesC& aRegionName)
	{
	if(aStandardName.Length() > KMaxStandardName ||
	   aDaylightName.Length() > KMaxStandardName ||
	   	   aCityName.Length() > KMaxStandardName ||
	   	 aRegionName.Length() > KMaxStandardName ||
  aShortStandardName.Length() > KMaxShortName ||
  aShortDaylightName.Length() > KMaxShortName
			)
		{
		User::Leave(KErrArgument);
		}
	
	iStandardName  = aStandardName.AllocL();
	iShortStandardName = aShortStandardName.AllocL();
	iDaylightName = aDaylightName.AllocL();
    iShortDaylightName = aShortDaylightName.AllocL();
    iCityName = aCityName.AllocL();
    iRegionName = aRegionName.AllocL();
    }


/**
Creates a user-defined time zone names object from the given names.

@param aStandardName The standard time name.
@param aShortStandardName The short standard time name.
@param aDaylightName The daylight saving time name.
@param aShortDaylightName The short daylight saving time name.
@param aCityName The city name.
@param aRegionName The region name.

@return Pointer to a fully initialised instance of the CTzUserNames class.  The
caller takes ownership of this object.
*/	
EXPORT_C CTzUserNames* CTzUserNames::NewL(const TDesC& aStandardName, const TDesC& aShortStandardName, const TDesC& aDaylightName, const TDesC& aShortDaylightName, const TDesC& aCityName, const TDesC& aRegionName)
	{
	CTzUserNames* self = NewLC(aStandardName, aShortStandardName, aDaylightName, aShortDaylightName, aCityName, aRegionName);
	CleanupStack::Pop(self);
	return self;
	}


/**
Creates a user-defined time zone names object from the given names leaving the
newly created object on the cleanup stack.

@param aStandardName The standard time name.
@param aShortStandardName The short standard time name.
@param aDaylightName The daylight saving time name.
@param aShortDaylightName The short daylight saving time name.
@param aCityName The city name.
@param aRegionName The region name.

@return Pointer to a fully initialised instance of the CTzUserNames class.  The
caller takes ownership of this object.
*/	

EXPORT_C CTzUserNames* CTzUserNames::NewLC(const TDesC& aStandardName, const TDesC& aShortStandardName, const TDesC& aDaylightName, const TDesC& aShortDaylightName, const TDesC& aCityName, const TDesC& aRegionName)
	{
	CTzUserNames* self = new(ELeave) CTzUserNames();
	CleanupStack::PushL(self);
	self->ConstructL(aStandardName, aShortStandardName, aDaylightName, aShortDaylightName, aCityName, aRegionName);
	return self;
	}


/**
Returns the standard time name for this object.

@return The standard time name for this object
*/	
EXPORT_C const TDesC& CTzUserNames::StandardName() const
	{
	return *iStandardName;		
	}


/**
Returns the short standard time name for this object.

@return The short standard time name for this object.
*/		
EXPORT_C const TDesC& CTzUserNames::ShortStandardName() const
	{
	return *iShortStandardName;	
	}


/**
Returns the daylight saving time name for this object.

@return The daylight saving time name for this object.
*/	
EXPORT_C const TDesC& CTzUserNames::DaylightSaveName() const
	{
	return *iDaylightName;	
	}


/**
Returns the short daylight saving time name for this object.

@return The short daylight saving time name for this object.
*/		
EXPORT_C const TDesC& CTzUserNames::ShortDaylightSaveName() const
	{
	return *iShortDaylightName;	
	}


/**
Returns the city name for this object.

@return The city name for this object.
*/	
EXPORT_C const TDesC& CTzUserNames::CityName() const
	{
	return *iCityName;	
	}


/**
Returns the region name for this object.

@return The region name for this object.
*/		
EXPORT_C const TDesC& CTzUserNames::RegionName() const
	{
	return *iRegionName;	
	}


/**
Externalises user-defined time zone names to the given stream.

@param aStream Stream to which the object should be externalised. 

@internalComponent
*/	
EXPORT_C void CTzUserNames::ExternalizeL(RWriteStream& aStream) const
	{
	TInt count = iStandardName->Length();
	aStream.WriteInt32L(count); 
	if(count>0)	
		{
		aStream << *iStandardName;
		}		

	count = iShortStandardName->Length();
	aStream.WriteInt32L(count); 
	if(count>0)
		{
		aStream << *iShortStandardName;
		}		
	
	count = iDaylightName->Length();
	aStream.WriteInt32L(count); 
	if(count>0)	
		{
		aStream << *iDaylightName;
		}
	
	count = iShortDaylightName->Length();
	aStream.WriteInt32L(count); 
	if(count>0)	
		{
		aStream << *iShortDaylightName;
		}
	
	count = iCityName->Length();
	aStream.WriteInt32L(count); 
	if(count>0)
		{
		aStream <<*iCityName;
		}
	
	count = iRegionName->Length();
	aStream.WriteInt32L(count); 
	if(count>0)
		{
		aStream << *iRegionName;
		}
	}


/**
Internalizes user-defined time zone names from the given read stream.

@param aStream Stream from which the object should be internalised. 

@internalComponent
*/ 
EXPORT_C void CTzUserNames::InternalizeL(RReadStream& aStream)
	{
	__ASSERT_DEBUG(!iStandardName && !iDaylightName && !iShortDaylightName &&
			!iCityName && !iRegionName, User::Invariant());
	
	TInt count = aStream.ReadInt32L();
	if(count)
		{
		iStandardName = HBufC::NewL(aStream, KMaxTInt);	
		}
	else
		{
		iStandardName = KNullDesC().AllocL();
		}	
			
	count = aStream.ReadInt32L();
	if(count)
		{
		iShortStandardName = HBufC::NewL(aStream, KMaxTInt);	
		}
	else
		{
		iShortStandardName = KNullDesC().AllocL();
		}	

	count = aStream.ReadInt32L();
	if(count)
		{
		iDaylightName = HBufC::NewL(aStream, KMaxTInt);
		}
	else
		{
		iDaylightName = KNullDesC().AllocL();
		}
			
	count = aStream.ReadInt32L();
	if(count)
		{
		iShortDaylightName = HBufC::NewL(aStream, KMaxTInt);
		}
	else
		{
		iShortDaylightName = KNullDesC().AllocL();
		}
			
	count = aStream.ReadInt32L();
	if(count)
		{
		iCityName = HBufC::NewL(aStream, KMaxTInt);;
		}
	else
		{
		iCityName = KNullDesC().AllocL();
		}	
	
	count = aStream.ReadInt32L();
	if(count)
		{
		iRegionName = HBufC::NewL(aStream, KMaxTInt);;
		}
	else
		{
		iRegionName = KNullDesC().AllocL();
		}
	
	__ASSERT_DEBUG(iStandardName->Length() <= KMaxStandardName &&
				   iDaylightName->Length() <= KMaxStandardName &&
					   iCityName->Length() <= KMaxStandardName &&
					 iRegionName->Length() <= KMaxStandardName &&
			  iShortStandardName->Length() <= KMaxShortName &&
	   	      iShortDaylightName->Length() <= KMaxShortName, User::Invariant()); 
	}


/**
Creates a new user-defined time zone names object from the given stream.

@param aStream Stream with the time zone rules to be used to create a CTzRules
object.

@return Pointer to a fully initialised instance of the CTzUserNames class.  The
caller takes ownership of this object.

@internalComponent
*/
EXPORT_C CTzUserNames* CTzUserNames::NewL(RReadStream& aStream)
	{
	CTzUserNames* self = new(ELeave) CTzUserNames();
	CleanupStack::PushL(self);
	self->InternalizeL(aStream);
	CleanupStack::Pop(self);
	return self;
	}


/**
Determines the size of this object. 

@return The size of CTzUserNames object.

@internalComponent
*/	
EXPORT_C TInt CTzUserNames::SizeOfObject() const
	{
	return 6*sizeof(TInt) + iStandardName->Size() + iShortStandardName->Size() +
		iDaylightName->Size() + iShortDaylightName->Size() + iCityName->Size() +
		iRegionName->Size();	
	}
	

/**
Creates a new CTzUserData object.

@param aTzServer A connection to the time zone server.

@return A fully initialised instance of the CTzUserData class.  The caller takes
ownership of this object.

@leave KErrNotReady The given connection to the time zone server is not
connected.
*/	
EXPORT_C CTzUserData* CTzUserData::NewL(RTz& aTzServer)
    {
	__ASSERT_ALWAYS(aTzServer.Handle()!= 0, User::Leave(KErrNotReady));

  	CTzUserData* self = new(ELeave) CTzUserData(aTzServer);
	return self;		
    };


/**
Destroys this CTzUserData object
*/  
EXPORT_C CTzUserData::~CTzUserData()
	{
	}


CTzUserData::CTzUserData(RTz& aTzServer)
	:iTzServer(aTzServer)
	{
	}


/**
Creates a new user-defined time zone from the given time zone rules and time
zone names.

@param aTzUserRules The time zone rules to use for the newly created
user-defined time zone.
@param aTzUserNames The time zone names to use for the newly created
user-defined time zone.

@return A time zone identifier that identifies the newly created user-defined
time zone.  The caller takes ownership of this object.

@leave KErrLocked The time zone Server Backup or Restore are in progress.
@leave KErrOverflow All the time zone user IDs have been used.
To resolve this the user must delete one or more user defined time zones.


@capability WriteDeviceData
*/			
EXPORT_C CTzId* CTzUserData::CreateL(const CTzRules& aTzUserRules,
	const CTzUserNames& aTzUserNames)
	{
	return iTzServer.CreateUserTimeZoneL(aTzUserRules, aTzUserNames);
	}


/**
Reads the time zone rules for the given time zone identifier.

@param aTzId The time zone identifier that identifies the user-defined time zone
that is to be read.

@return The time zone rules for the given time zone identifier.  The caller
takes ownership of this object.

@leave KErrNotFound The time zone names is not found for given time zone
identifier.
@leave KErrArgument The given time zone identifier does not identify a
user-defined time zone.
*/				
EXPORT_C CTzRules* CTzUserData::ReadRulesL(const CTzId& aTzId) const
	{
	if (!aTzId.IsUserTzId())
		{
		User::Leave(KErrArgument);	
		}
		
	return iTzServer.GetTimeZoneRulesL(aTzId, 0, KMaxYear, ETzUtcTimeReference);
	}


/**
Reads the time zone names for the given time zone identifier.

@param aTzId The time zone identifier that identifies the user-defined time zone
that is to be read.

@return The time zone names for the given time zone identifier.  The caller
takes ownership of this object.

@leave KErrArgument The given time zone identifier does not identify a
user-defined time zone.
@leave KErrNotFound The time zone names is not found for given time zone
identifier.
*/		
EXPORT_C CTzUserNames* CTzUserData::ReadNamesL(const CTzId& aTzId) const
	{
	if (!aTzId.IsUserTzId())
		{
		User::Leave(KErrArgument);	
		}

	return iTzServer.GetUserTimeZoneNamesL(aTzId);
	}


/**
Updates a user-defined time zone using the given time zone rules and time zone
names.  The user-defined time zone to be updated is identified by the given time
zone identifier.

@param aTzId The time zone identifier that identifies the user-defined time zone
that is to be read.
@param aTzUserRules The time zone rules which are used to update the
user-defined time zone.  The user-defined time zone is identified by the given
time zone identifier.
@param aTzUserNames The time zone names which are used to update the
user-defined time zone.  The user-defined time zone is identified by the given
time zone identifier. 
 
@leave KErrArgument The given time zone identifier does not identify a
user-defined time zone.

@capability WriteDeviceData
*/			
EXPORT_C void CTzUserData::UpdateL(const CTzId& aTzId, const CTzRules& aTzUserRules, const CTzUserNames& aTzUserNames)
	{
	if (!aTzId.IsUserTzId())
		{
		User::Leave(KErrArgument);	
		}

    iTzServer.UpdateUserTimeZoneL(aTzId, aTzUserRules, aTzUserNames);
	}


/**
Deletes the user-defined time zone identified by the given time zone identifier.

The current time zone is maintained through the CTzLocalizer class.  If the
given time zone identifier is referred to by the current time zone then the
current time zone will revert to the default time zone specified by the system
TZ rules database.

Frequently used time zones are maintained through the CTzLocalizer class.  If
the given time zone identifier is referred to by a frequently used time zone
then the frequently used time zone will revert to the default time zone
specified by the system TZ rules database.

If any Calendar entries are associated with the deleted user-defined time zone
then they will not be affected.  The Calendar entries maintain a copy of the
associated time zone rules.

@param aTzId The time zone identifier that identifies the user-defined time zone
that is to be deleted.
 
@leave KErrArgument The given time zone identifier does not identify a
user-defined time zone.

@capability WriteDeviceData
*/			
EXPORT_C void CTzUserData::DeleteL(const CTzId& aTzId)
	{
	if (!aTzId.IsUserTzId())
		{
		User::Leave(KErrArgument);	
		}

	iTzServer.DeleteUserTimeZoneL(aTzId);
	}

/**
Returns the time zone identifiers for all existing user-defined time zones.

@param aTzIds On return the array will be populated with the time zone
identifiers for all existing user-defined time zones.  If no user-defined time
zones exist the array will be empty.
*/	
EXPORT_C void CTzUserData::GetTzIdsL(RPointerArray<CTzId>& aTzIds) const
	{
	CleanupStack::PushL( TCleanupItem(CleanupPointerArray, &aTzIds) );
	iTzServer.GetUserTimeZoneIdsL(aTzIds);
	CleanupStack::Pop();
	}

void CTzUserData::CleanupPointerArray(TAny* aArray)
    {
    RPointerArray<CTzId>* array = static_cast<RPointerArray<CTzId>* >(aArray);
    if (array)
        {
        array->ResetAndDestroy();
        array->Close();
        }
    }
 

