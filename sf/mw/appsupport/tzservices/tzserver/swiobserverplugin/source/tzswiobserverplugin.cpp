// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 
#include "tzswiobserverplugin.h"

using namespace Swi;

_LIT(KFilterTzPrivate,  "!:\\private\\1020383E");
_LIT(KFilterResourceTimezonelocalization,  "!:\\resource\\timezonelocalization");

#ifdef _DEBUG
//Logging data
_LIT(KTzDebugLogFile, "c:\\TzSwiObserver.log");
_LIT(KTzLogDir, "c:\\logs\\Tz");
_LIT(KTzLogFile, "c:\\logs\\Tz\\TzSwiObserver.log");
_LIT8(KTzCRLF, "\r\n");
_LIT8(KTzAdded, "Added ");
_LIT8(KTzDeleted, "Deleted ");
#endif

CTzSwiObserverPlugin* CTzSwiObserverPlugin::NewL()
	{
	CTzSwiObserverPlugin* self = new(ELeave) CTzSwiObserverPlugin();
	return self;
	}

CTzSwiObserverPlugin::~CTzSwiObserverPlugin()
	{
	//Should close the RTz
	iTz.Close();
#if defined(_DEBUG)
	delete iFileWriteStream;
#endif
	}

CTzSwiObserverPlugin::CTzSwiObserverPlugin()
	: CSwiObserverPlugin(), iTzConnected(EFalse)
	{
	}

/**
This function is called when the SWI Observer processor starts processing
an observation log file.

@param aFs File server session handle.
*/
void CTzSwiObserverPlugin::StartL(RFs& aFs)
	{
	iFs = &aFs;
	
#if defined(_DEBUG)
	TUint fileAtt;
	if (aFs.Att(KTzLogDir, fileAtt) == KErrNone)
		{
		iFileWriteStream = new(ELeave) RFileWriteStream;
		if (iFileWriteStream)
			{
			if (iFileWriteStream->Replace(aFs, KTzDebugLogFile, EFileShareAny | EFileWrite | EFileStreamText ) != KErrNone)
				{
				delete iFileWriteStream;
				iFileWriteStream = NULL;
				}
			}
		}
	
#endif
	}
 	
/**
Signals TZ Server that a new installation log is about to be processed.
	
@param aHeader Object containing all observation data related with a specific package.
@param aStatus The request status object.
 */ 		
void CTzSwiObserverPlugin::NextObservedHeaderL(const CObservationHeader& /*aHeader*/, TRequestStatus& aStatus)
	{
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}
 
/**
This function is called for each actual observation data (i.e. adding/deleting file).
	
@param aData   		Object for a single observation data.
@param aFilterIndex The index of the filter which matches with that notification.
@param aStatus 		The request status object.
 */
#if defined(_DEBUG)
void CTzSwiObserverPlugin::NextObservedDataL(const CObservationData& aData, TInt aFilterIndex, TRequestStatus& aStatus)
#else
void CTzSwiObserverPlugin::NextObservedDataL(const CObservationData& /*aData*/, TInt aFilterIndex, TRequestStatus& aStatus)
#endif
	{
	RTz::TSWIObserverFilterIndex idx = static_cast<RTz::TSWIObserverFilterIndex>(aFilterIndex);
	
	switch(idx)
		{
		case RTz::EFilterTzPrivate:
		case RTz::EFilterResourceTimezonelocalization:
			{
			GetTzClientL().SwiObsFileChangedL(idx);
			}
			
			break;
		default:
			// Do nothing
			break;
		}
	
#if defined(_DEBUG)
	if (iFileWriteStream)
		{
		if (aData.iFileFlag & Swi::EFileAdded)
			{
			iFileWriteStream->WriteL(KTzAdded());
			}
		else if (aData.iFileFlag & Swi::EFileDeleted)
			{
			iFileWriteStream->WriteL(KTzDeleted());
			}
		TBuf8<256> fileBuf;
		fileBuf.Copy(aData.iFileName->Des());
		iFileWriteStream->WriteL(fileBuf);
		iFileWriteStream->WriteL(KTzCRLF());
		}
#endif
	
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
	}
	
/**
This function is called when the SWI Observer processor is finished with
the observation log file.
	
@param aStatus The request status object.
 */	
void CTzSwiObserverPlugin::FinalizeL(TRequestStatus& aStatus)
 	{
 	GetTzClientL().SwiObsEndL();
 	
#if defined(_DEBUG)
 	if (iFileWriteStream)
 		{
 		iFileWriteStream->Close();
 		User::LeaveIfError(iFs->Rename(KTzDebugLogFile, KTzLogFile));
 		}
#endif
 		
 	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, KErrNone);
  	}
 
/**
Returns a list of filters which representing in what folders the tz observer
plugin is interested. 

The tz observer plugin needs to know any change in the \private\1020383E and \resource\timezonelocalization
directories of the system drive. 

@return A list of filters showing the interest of this plugin.
 */ 
CObservationFilter* CTzSwiObserverPlugin::GetFilterL() const
 	{
 	CObservationFilter* filterList = CObservationFilter::NewLC();
 	AddFilterL(*filterList, KFilterTzPrivate);
 	AddFilterL(*filterList, KFilterResourceTimezonelocalization);
 	CleanupStack::Pop(filterList);
 	return filterList;
 	}

/**
	Inserts a new filter into the filter list.
	Sets the first character of the new filter as the current system drive.
	
	@param aFilterList A filter list object.
	@param aFilter	   A filter data which will be inserted.
 */
void CTzSwiObserverPlugin::AddFilterL(CObservationFilter& aFilterList, const TDesC& aFilter) const
	{
	HBufC* filterData = aFilter.AllocLC();
	TPtr ptr(filterData->Des());
	ptr[0] = iFs->GetSystemDriveChar();
	aFilterList.AddFilterL(filterData);
	CleanupStack::Pop(filterData);
	}
	
void CTzSwiObserverPlugin::RunL()
	{
	// Do Nothing
	}
		
TInt CTzSwiObserverPlugin::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}

void CTzSwiObserverPlugin::DoCancel()
	{
	}

static const TUint KTzSwiObserverImplementationId = 0x1028649F;

static const TImplementationProxy ImplementationTable[] = 
	{
	IMPLEMENTATION_PROXY_ENTRY(KTzSwiObserverImplementationId, CTzSwiObserverPlugin::NewL)
	};

RTz& CTzSwiObserverPlugin::GetTzClientL()
	{
	if (!iTzConnected)
		{
		User::LeaveIfError(iTz.Connect());
		iTzConnected = ETrue;
		iTz.SwiObsBeginL();
		}
	return iTz;
	}

/**
Standard ECOM factory
*/	
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}
