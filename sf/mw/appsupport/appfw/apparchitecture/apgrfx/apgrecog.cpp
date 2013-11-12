// Copyright (c) 1997-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// apgrecog.cpp
//


#include "../apserv/APSCLSV.H"
#include "APGCLI.H"
#include "APGPRIV.H"

class CAsyncFileRecognition;

_LIT8(KAllDataTypes,"*");


/**
This class is an extension class for RApaLsSession. It was added to reduce the need
of BC breaks in later releases.
@internalComponent
*/
NONSHARABLE_CLASS(CApaLsSessionExtension) : public CBase
	{
public:
	CApaLsSessionExtension(RApaLsSession& aSession);
	~CApaLsSessionExtension();

	void SendReceive(TInt aFunction,const TIpcArgs& aArgs,TRequestStatus& aStatus) const;
	TInt SendReceive(TInt aFunction,const TIpcArgs& aArgs) const;

	// file recognition functions
	void RecognizeFilesL(const TDesC& aPath, const TDesC8& aDataType, CDataRecognitionResultArray& aResult, TRequestStatus& aStatus);
	void CancelRecognizeFiles();
	void FilesRecognized(const TDesC& aPath, TInt aRequiredBufferSize, CDataRecognitionResultArray& aResult, TRequestStatus& aUsersRequestStatus, const TRequestStatus& aErrorCode);
	TBool RecognitionActive() const;
private:
	RApaLsSession& iSession;	// we are a friend of this class!
	CAsyncFileRecognition* iAsyncFileRecognition;
	};

/**
This class is used to simplify the usage of the asynchronous RApaLsSession::RecognizeFilesL() function.
@internalComponent
*/
NONSHARABLE_CLASS(CAsyncFileRecognition) : public CActive
	{
public:
	CAsyncFileRecognition(CDataRecognitionResultArray& aResult, TRequestStatus& aUsersRequestStatus, CApaLsSessionExtension& aSession);
	~CAsyncFileRecognition();
	void Start(const TDesC& aPath, const TDesC8& aDataType);
private:
	void RunL();
	void DoCancel();
private: // data
	TPckgBuf<TUint> iRequiredBufferSizePckg;
	CDataRecognitionResultArray& iResult;
	TRequestStatus& iUsersRequestStatus;
	CApaLsSessionExtension& iSession;
	const TDesC* iPath;
	const TDesC8* iDataType;
	};

/**
An entry of a CDataRecognitionResultArray object. Stores TDataRecognitionResult and the file name
in a compact format.
@internalComponent
*/
NONSHARABLE_CLASS(CDataRecognitionResultArray::CItem) : public CBase
	{
public:
	CItem(HBufC* aFileName, HBufC8* aDataType, TUid aUid, TInt aConfidence);
	~CItem();
	void GetDataRecognitionResult(TDataRecognitionResult& aResult) const;
	const TDesC& FileName() const;
private:
	HBufC* iFileName;
	HBufC8* iDataType;
	TUid iUid;
	TInt iConfidence;
	};


//
// CApaLsSessionExtension
//

CApaLsSessionExtension::CApaLsSessionExtension(RApaLsSession& aSession)
	: iSession(aSession),
	  iAsyncFileRecognition(NULL)
	{
	}

CApaLsSessionExtension::~CApaLsSessionExtension()
	{
	delete iAsyncFileRecognition;
	}

void CApaLsSessionExtension::SendReceive(TInt aFunction,const TIpcArgs& aArgs,TRequestStatus& aStatus) const
	{
	iSession.SendReceive(aFunction,aArgs,aStatus); //lint !e1060 Suppress protected member is not accessible to non-member non-friend
	}

TInt CApaLsSessionExtension::SendReceive(TInt aFunction,const TIpcArgs& aArgs) const
	{
	return iSession.SendReceive(aFunction,aArgs); //lint !e1060 Suppress protected member is not accessible to non-member non-friend
	}



//
// CAsyncFileRecognition
//

CAsyncFileRecognition::CAsyncFileRecognition(CDataRecognitionResultArray& aResult, TRequestStatus& aUsersRequestStatus, CApaLsSessionExtension& aSession)
	: CActive(EPriorityStandard),
	  iResult(aResult),
	  iUsersRequestStatus(aUsersRequestStatus),
	  iSession(aSession)
	{
	CActiveScheduler::Add(this);
	}

void CAsyncFileRecognition::Start(const TDesC& aPath, const TDesC8& aDataType)
	{
	iPath = &aPath;
	iDataType = &aDataType;
	iStatus = KRequestPending;
	iSession.SendReceive(EAppListServRecognizeFilesAsync,TIpcArgs(&aPath,&iRequiredBufferSizePckg,&aDataType),iStatus);
	SetActive();
	}

CAsyncFileRecognition::~CAsyncFileRecognition()
	{
	Cancel();
	iPath = NULL;
	iDataType = NULL;
	}

void CAsyncFileRecognition::RunL()
	{
	iSession.FilesRecognized(*iPath, iRequiredBufferSizePckg(), iResult, iUsersRequestStatus, iStatus);
	}

void CAsyncFileRecognition::DoCancel()
	{
	iSession.SendReceive(ECancelRecognizeFiles,TIpcArgs(TIpcArgs::ENothing));
	TRequestStatus* status = &iUsersRequestStatus;
	User::RequestComplete( status, KErrCancel );
	}


/**
Asynchronous recognition of a directory.
Function uses an internal CAsyncFileRecognition object to hide the 
second server message (transferring the data to the client) from the user.
When the path is not correct or the server is already busy with another 
recognition request, the function completes the request with an error.
@internalComponent
*/
void CApaLsSessionExtension::RecognizeFilesL(const TDesC& aPath, const TDesC8& aDataType, CDataRecognitionResultArray& aResult, TRequestStatus& aStatus)
	{
	_LIT(KBackslash,"\\");

	if(aPath.Right(1) != KBackslash)
		{
		TRequestStatus* status = &aStatus;
		User::RequestComplete(status,KErrPathNotFound);
		}
	
	aResult.SetPath(aPath);
	aStatus = KRequestPending;

	if(!iAsyncFileRecognition)
		{
		iAsyncFileRecognition = new (ELeave) CAsyncFileRecognition(aResult, aStatus, *this);
		iAsyncFileRecognition->Start(aPath, aDataType);
		}
	else
		{
		TRequestStatus* status = &aStatus;
		User::RequestComplete(status,KErrInUse);
		}
	}

TBool CApaLsSessionExtension::RecognitionActive() const
	{
	return iAsyncFileRecognition ? ETrue : EFalse;
	}

void CApaLsSessionExtension::CancelRecognizeFiles()
	{
	if(iAsyncFileRecognition)
		{
		iAsyncFileRecognition->Cancel();
		delete iAsyncFileRecognition;
		iAsyncFileRecognition = NULL;
		}
	}

/**
Callback function called by an CAsyncFileRecognition object, when an asynchronous
recognition has finished. If the recognition was successful, the data is transferred
to the client-side.
@internalComponent
*/
void CApaLsSessionExtension::FilesRecognized(const TDesC& aPath, TInt aRequiredBufferSize, CDataRecognitionResultArray& aResult, TRequestStatus& aUsersRequestStatus, const TRequestStatus& aErrorCode)
	{
	TRequestStatus* status = &aUsersRequestStatus;
	if(aErrorCode == KErrNone)
		{
		// transfer the result
		TRAPD(error,iSession.TransferAndInternalizeDataL(aPath, aRequiredBufferSize, aResult));
		User::RequestComplete(status, error);
		}
	else
		User::RequestComplete(status, aErrorCode.Int());

	delete iAsyncFileRecognition;
	iAsyncFileRecognition = 0;
	}


//
// CItem
//

CDataRecognitionResultArray::CItem::CItem(HBufC* aFileName, HBufC8* aDataType, TUid aUid, TInt aConfidence)
	: iFileName(aFileName), 
	  iDataType(aDataType), 
	  iUid(aUid), 
	  iConfidence(aConfidence)
	{
	}

CDataRecognitionResultArray::CItem::~CItem()
	{
	delete iFileName;
	delete iDataType;
	}

void CDataRecognitionResultArray::CItem::GetDataRecognitionResult(TDataRecognitionResult& aResult) const
	{
	TDataRecognitionResult result;
	if(iDataType->Length() != 0)
		{
		TDataType dataType(*iDataType);
		result.iDataType = dataType;
		}
	else
		{
		TDataType dataType(iUid);
		result.iDataType = dataType;
		}

	result.iConfidence = iConfidence;
	aResult = result;
	}

const TDesC& CDataRecognitionResultArray::CItem::FileName() const
	{
	if (iFileName == NULL)
		return KNullDesC;
	else
		return *iFileName;
	}


//
// CDataRecognitionResultArray
//

/**
Constructor
@publishedAll
@released
*/
EXPORT_C CDataRecognitionResultArray::CDataRecognitionResultArray()
	{
	}

/**
Destructor
@publishedAll
@released
*/
EXPORT_C CDataRecognitionResultArray::~CDataRecognitionResultArray()
	{
	iItems.ResetAndDestroy();
	}

/**
Returns the path of the recognition results.
@publishedAll
@released
@return The path of the recognition results
*/
EXPORT_C const TFileName& CDataRecognitionResultArray::Path() const
	{
	return iPath;
	}

/**
Returns the number of entries in the CDataRecognitionResultArray.
@publishedAll
@released
@return The number of entries in the CDataRecognitionResultArray.
*/
EXPORT_C TUint CDataRecognitionResultArray::Count() const
	{
	return iItems.Count();
	}

/**
Returns the recognition result of the given index.
@publishedAll
@released
@param aResult On successful completion, this parameter contains the recognition result at the given index.
@param aIndex The index of the recognition result to be returned. An invalid index causes
the function to leave with KErrNotFound.
@return The requested TDataRecognitionResult object
@leave KErrNotFound An invalid index is passed
*/
EXPORT_C void CDataRecognitionResultArray::GetDataRecognitionResultL(TDataRecognitionResult& aResult, const TUint aIndex) const
	{
	if( aIndex >= iItems.Count() )
		User::Leave(KErrNotFound);

	iItems[aIndex]->GetDataRecognitionResult(aResult);
	}

/**
Returns the file name of the object at the given index.
@publishedAll
@released
@param aFileName On successful completion, this parameter contains the file name of the object at the given index. 
@param aIndex Specifies which file name to be returned. An invalid index causes
the function to leave with KErrNotFound.
@return The requested file name
@leave KErrNotFound An invalid index is passed
*/
EXPORT_C void CDataRecognitionResultArray::GetFileNameL(TFileName& aFileName, const TUint aIndex) const
	{
	if( aIndex >= iItems.Count() )
		User::Leave(KErrNotFound);

	aFileName.Zero();
	aFileName.Copy( (iItems[aIndex]->FileName()) );
	}

TUint CDataRecognitionResultArray::InternalizeL(const CBufFlat& aBuffer)
	{
	RBufReadStream readStream;
	
	readStream.Open(aBuffer);
	const TInt count=readStream.ReadUint32L();
	for (TInt i=0; i<count; ++i)
		{
		const TInt fileNameLength = readStream.ReadUint8L();
		HBufC* const fileName = HBufC::NewLC(fileNameLength);
		TPtr fileNamePtr = fileName->Des();
		readStream.ReadL(fileNamePtr,fileNameLength);

		const TInt dataTypeLength = readStream.ReadUint8L();
		HBufC8* const dataType = HBufC8::NewLC(dataTypeLength);
		TPtr8 dataTypePtr = dataType->Des();
		readStream.ReadL(dataTypePtr,dataTypeLength);

		TUid uid;
		uid.iUid = readStream.ReadInt32L();
		
		TInt const confidence = readStream.ReadInt32L();
		
		CItem* entry = new (ELeave) CItem(fileName,dataType,uid,confidence);
		iItems.Append(entry);
		
		CleanupStack::Pop(dataType);	// ownership was transferred to "entry"
		CleanupStack::Pop(fileName);	// ownership was transferred to "entry"
		}

	readStream.Close();
		
	return KErrNone;
	}

void CDataRecognitionResultArray::SetPath(const TFileName& aPath)
	{
	iPath.Copy(aPath);
	}

//
//RApaLsSession
//

/** Gets the data (MIME) type of data passed by buffer.

@param aBuffer A buffer containing data; Provide preferred size of buffer. 
If MIME type could not be recognized using this buffer, provide a buffer of 
larger size.
@param aDataType On return, contains the result of the attempt to recognize 
data. 
@return KErrNone, if successful; otherwise one of the other system-wide error 
codes. 

@see RApaLsSession::GetPreferredBufSize()
*/
EXPORT_C TInt RApaLsSession::RecognizeData(const TDesC8& aBuffer, TDataRecognitionResult& aDataType) const
    {
    return RecognizeData(KNullDesC,aBuffer,aDataType);
    }

/** Gets the data (MIME) type for data taken from a file with a specified name.

@param aName The full filename, including drive and path, of the file containing the data.
@param aBuffer A buffer containing data taken from the specified file; Provide preferred size of buffer 
from beginning of the file. If MIME type could not be recognized using this buffer, provide a buffer of 
larger size.
@param aDataType On return, contains the result of the attempt to recognize 
data. 
@return KErrNone, if successful; otherwise one of the other system-wide error 
codes. 

@see RApaLsSession::GetPreferredBufSize()
*/
EXPORT_C TInt RApaLsSession::RecognizeData(const TDesC& aName, const TDesC8& aBuffer, TDataRecognitionResult& aDataType) const
	{
	TPckg<TDataRecognitionResult> result(aDataType);
	return SendReceiveWithReconnect(EAppListServRecognizeData,TIpcArgs(&result, &aName, &aBuffer));
	} //lint !e1764: Suppress reference parameter could be declared const ref

/** Gets the data (MIME) type for data in a file passed by handle.

@param aFile The file containing the data. Before this function can be called,
the file server session which owns this file handle must first be marked as shareable by 
calling RFs::ShareProtected().
@param aDataType On return, contains the result of the attempt to recognize 
data. 
@return KErrNone, if successful; otherwise one of the other system-wide error 
codes. */
EXPORT_C TInt RApaLsSession::RecognizeData(const RFile& aFile, TDataRecognitionResult& aDataType) const
	{
	TPckg<TDataRecognitionResult> result(aDataType);
	TIpcArgs ipcArgs(&result);
	const TInt error = aFile.TransferToServer(ipcArgs, 1, 2);
	if (error)
		return error;

	return SendReceiveWithReconnect(EAppListServRecognizeDataPassedByFileHandle,ipcArgs);
	} //lint !e1764 Suppress reference parameter 'aDataType' could be declared const ref



/** Tests whether data taken from a named file has the specified 
data (MIME) type.

@param aName The name of the file containing the data.
@param aBuffer A buffer containing data taken from the specified file; Provide preferred size of buffer 
from beginning of the file. If MIME type could not be recognized using this buffer, provide a buffer of 
larger size.
@param aDataType The data (MIME) type.
@param aResult On return, contains the result of the test.
@return KErrNone, if successful; otherwise one of the other system-wide error 
codes. 

@see RApaLsSession::GetPreferredBufSize()
*/
EXPORT_C TInt RApaLsSession::RecognizeSpecificData(const TDesC& aName, const TDesC8& aBuffer, const TDataType& aDataType, TBool& aResult) const
	{
	const TPckgC<TDataType> dataType(aDataType);
	const TInt returnValue=SendReceiveWithReconnect(EAppListServRecognizeSpecificData,TIpcArgs(&dataType, &aName, &aBuffer));
	if (returnValue<0)
		return returnValue;

	aResult = returnValue;
	return KErrNone;
	}
	
/** 
Gets the data (MIME) type for files in a specified directory. Because this call may take a long
time to complete, the asynchronous version is strongly recommended. Furthermore, it is not possible
to use this synchronous function while an asynchronous request is still active.
@publishedAll
@released
@param aPath A valid path. Note that the path must end with a backslash.
@param aResult If the call was successful, this parameter contains the recognition result.
@return KErrNone, if successful; otherwise one of the other system-wide error codes.
*/
EXPORT_C TInt RApaLsSession::RecognizeFilesL(const TDesC& aPath, CDataRecognitionResultArray& aResult) const
	{
	return RecognizeFilesL(aPath, KAllDataTypes, aResult);
	}

/** 
Gets the data (MIME) type for files in a specified directory. Because this call may take a long
time to complete, the asynchronous version is strongly recommended. Furthermore, it is not possible
to use this synchronous function while an asynchronous request is still active.
@publishedAll
@released
@param aPath A valid path. Note that the path must end with a backslash.
@param aDataType A data type filter. Wildcards are allowed. For example, "text*" would also
add "text/plain" data types to the result.
@param aResult If the call was successful, this parameter contains the recognition result.
@return KErrNone, if successful; otherwise one of the other system-wide error codes.
*/
EXPORT_C TInt RApaLsSession::RecognizeFilesL(const TDesC& aPath, const TDesC8& aDataType, CDataRecognitionResultArray& aResult) const
	{
	if(iExtension && iExtension->RecognitionActive())
		return KErrInUse;

	_LIT(KBackslash,"\\");
	if(aPath.Right(1) != KBackslash)
		return KErrPathNotFound;

	TPckgBuf<TUint> requiredBufferSizePckg;
	aResult.SetPath(aPath);
	TInt error = SendReceiveWithReconnect(EAppListServRecognizeFiles,TIpcArgs(&aPath,&requiredBufferSizePckg,&aDataType));
	if(!error)
		error=TransferAndInternalizeDataL(aPath, requiredBufferSizePckg(), aResult);

	return error;

	}

/** 
Gets the data (MIME) type for files in a specified directory.
@publishedAll
@released
@param aPath A valid path. Note that the path must end with a backslash.
@param aResult If the call was successful, this parameter contains the recognition result.
@param aStatus A request status object.
*/
EXPORT_C void RApaLsSession::RecognizeFilesL(const TDesC& aPath, CDataRecognitionResultArray& aResult, TRequestStatus& aStatus)
	{
	RecognizeFilesL(aPath,KAllDataTypes,aResult,aStatus);
	}

/** 
Gets the data (MIME) type for files in a specified directory.
@publishedAll
@released
@param aPath A valid path. Note that the path must end with a backslash.
@param aDataType A data type filter. Wildcards are allowed. For example, "text*" would also
add "text/plain" data types to the result.
@param aResult If the call was successful, this parameter contains the recognition result.
@param aStatus A request status object
*/
EXPORT_C void RApaLsSession::RecognizeFilesL(const TDesC& aPath, const TDesC8& aDataType, CDataRecognitionResultArray& aResult, TRequestStatus& aStatus)
	{
	if(!iExtension)
		iExtension = new (ELeave) CApaLsSessionExtension(*this);

	iExtension->RecognizeFilesL(aPath, aDataType, aResult, aStatus);
	}

/**
This function transfers the data to the client-side and "fills" the CDataRecognitionResultArray object. 
@internalComponent
*/
TInt RApaLsSession::TransferAndInternalizeDataL(const TDesC& aPath, const TInt aRequiredBufferSize, CDataRecognitionResultArray& aResult) const
	{
	CBufFlat* const buffer = CBufFlat::NewL(aRequiredBufferSize);
	CleanupStack::PushL(buffer);
	buffer->ExpandL(0,aRequiredBufferSize);
	TPtr8 bufPtr=buffer->Ptr(0);

	// transfer recognition buffer
	TInt error = SendReceiveWithReconnect(EAppListServTransferRecognitionResult,TIpcArgs(&aPath,&bufPtr,aRequiredBufferSize));
	if(!error)
		error = aResult.InternalizeL(*buffer);

	CleanupStack::PopAndDestroy(buffer);
	return error;
	}

/** 
Cancels any outstanding asynchronous recognition requests.
@publishedAll
@released
*/
EXPORT_C void RApaLsSession::CancelRecognizeFiles()
	{
	if(iExtension)
		iExtension->CancelRecognizeFiles();
	
	delete iExtension;
	iExtension = NULL;
	}

/** Tests whether data taken from a file passed by handle has the specified 
data (MIME) type.

@param aFile The file containing the data. Before this function can be called,
the file server session which owns this file handle must first be marked as shareable by 
calling RFs::ShareProtected().
@param aDataType The data (MIME) type.
@param aResult On return, contains the result of the test.
@return KErrNone, if successful; otherwise one of the other system-wide error 
codes. */
EXPORT_C TInt RApaLsSession::RecognizeSpecificData(const RFile& aFile, const TDataType& aDataType, TBool& aResult) const
	{
	const TPckgC<TDataType> dataType(aDataType);
	TIpcArgs ipcArgs(&dataType);
	TInt returnValue = aFile.TransferToServer(ipcArgs, 1, 2);
	if (returnValue == KErrNone)
		returnValue = SendReceiveWithReconnect(EAppListServRecognizeSpecificDataPassedByFileHandle,ipcArgs);

	if (returnValue < KErrNone)
		return returnValue;
		
	aResult = returnValue;
	return KErrNone;
	}



/** Gets the confidence threshold for successful data recognition.

This is the minimum acceptable confidence level that must be reported by a 
data recognizer for data to be accepted as of a given type.

@param aConfidence On return, the confidence threshold. 
@return KErrNone, if successful; otherwise one of the other system-wide error 
codes. 
*/
EXPORT_C TInt RApaLsSession::GetAcceptedConfidence(TInt& aConfidence) const
	{
	TPckg<TInt> confidence(aConfidence);
	return SendReceiveWithReconnect(EAppListServGetConfidence,TIpcArgs(&confidence));
	} //lint !e1764 Suppress reference parameter could be declared const ref



/** Sets the confidence threshold for successful data recognition.

This is the minimum acceptable confidence level that must be reported by a 
data recognizer for data to be accepted as of a given type.

@param aConfidence The confidence threshold. Although this is an integer value, 
data recognizers use the discrete values defined by the CApaDataRecognizerType::TRecognitionConfidence 
enumeration.
@return KErrNone, if successful; otherwise one of the other system-wide error 
codes. 
@capability WriteDeviceData 
*/
EXPORT_C TInt RApaLsSession::SetAcceptedConfidence(TInt aConfidence)
	{
	return SendReceiveWithReconnect(EAppListServSetConfidence,TIpcArgs(aConfidence));
	} //lint !e1762 Suppress member function could be made const


/** Gets a list of recognized data(MIME) types by all recognizers.

@param aDataTypes The array of data (MIME) types.
@return KErrNone, if successful; otherwise one of the other system-wide error 
codes. 
*/
EXPORT_C TInt RApaLsSession::GetSupportedDataTypesL(CDataTypeArray& aDataTypes) const
	{
	// gets the datatype count in terms of buffer length; negative value means one of the system-wide error
	TInt ret = SendReceiveWithReconnect(EAppListServGetDataTypesPhase1,TIpcArgs());
	if (ret>0)
		{
		CBufFlat* const buf=CBufFlat::NewL(ret);
		CleanupStack::PushL(buf);
		buf->ExpandL(0,ret);
		TPtr8 ptr=buf->Ptr(0);
		ret=SendReceiveWithReconnect(EAppListServGetDataTypesPhase2,TIpcArgs(&ptr));
		if (ret==KErrNone)
			{
			RBufReadStream readStream(*buf);
			readStream >> aDataTypes;
			}
			
		CleanupStack::PopAndDestroy(buf);
		}
		
	return ret;
	}


