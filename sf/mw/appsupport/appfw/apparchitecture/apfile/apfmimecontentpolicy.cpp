// Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies).
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



// INCLUDE FILES
#include <apfmimecontentpolicy.h>
#include <f32file.h> // RFs
#include <caf/content.h>
#include <e32base.h>
#include <apgcli.h>    // For RApaLsSession 
#include <centralrepository.h>
#include <apmstd.h>


/* Closed content and extension information is stored in central repository with UID 0x10003A3F.
 * Keys of the Closed Content and Extension information repository is divided into two parts.
 * Most significant byte is used for identifying the type,i.e. whether it is Mimetype or extension, 
 * and the least significant 3 bytes are used for uniquely identifying the entry within that type. 
 * 
 * |-------------------- Key (32-bits) ---------------------|
 *  --------------------------------------------------------
 * | type (8-bits)  |            sequence number(24-bits)   |
 *  --------------------------------------------------------
 * 
 *  The type part is used for differentiating Content type and extension keys.
 *  The value can be 
 *     0x0 - For content type key
 *     0x1 - For extension key
 *     
 * Sequence number part is used to uniquely identifying the entry within that type. 
 *  
 *  Examples: 
 *  
 *  0x00000000 - Content type key with sequence number 0x0
 *  0x00000001 - Content type key with sequence number 0x1
 *  0x01000000 - Extension type key with sequence number 0x0
 *  0x01000001 - Extension type key with sequence number 0x1
 *  0x01000002 - Extension type key with sequence number 0x2
 */



//Partial key for finding MIME type keys in the repository
const TUint32 KClosedContentTypePartialKey=0x0;

//Partial key for finding extension type keys in the repository
const TUint32 KClosedExtensionTypePartialKey=0x01000000;

//Mask for finding the type (MIME or extension)of a key
const TUint32 KClosedTypeKeyMask=0xFF000000;


//Closed content and extension information repository UID
const TUid KClosedContentAndExtensionInfoRepositoryUID={0x10003A3F};


NONSHARABLE_CLASS(CApfMimeContentPolicyImpl) : public CBase
	{
public:	 // Constructors and destructor
	static CApfMimeContentPolicyImpl* NewL();
	static CApfMimeContentPolicyImpl* NewL(RFs& aFs);
	~CApfMimeContentPolicyImpl();	

public:  // New functions
	TBool IsClosedType(const TDesC& aMimeType) const;
	TBool IsClosedExtension(const TDesC& aFileExtension) const;
	TBool IsClosedFileL(const TDesC& aFileName) const;
	TBool IsDRMEnvelopeL(const TDesC& aFileName) const;	
	TBool IsClosedFileL(RFile& aFileHandle) const;
	TBool IsDRMEnvelopeL(RFile& aFileHandle) const;

private:	
	CApfMimeContentPolicyImpl();
	void ConstructL();
    void ConstructL(RFs& aFs);
	TBool IsClosedFileL(RFile& aFileHandle, const TDesC& aFileName) const;
	void ReadResourcesL();
	TBool IsValidExtension(TDesC& extension);	
    TBool IsValidMimeType(TDesC& extension);
private:
	CDesCArrayFlat* iCcl;	// Closed content list.
	CDesCArrayFlat* iExtList;	// Closed extensions list.
	RApaLsSession   iLs;	// A session to the Application Architecture server.
    mutable RFs iFs;	//	File session
    TBool   iFsConnected;	// ETrue if connected to file server, else EFalse
	};


//
// class CApfMimeContentPolicy
//

/**
C++ default constructor.
*/
CApfMimeContentPolicy::CApfMimeContentPolicy()
	{
	// Nothing to do here.
	}

/**
By default Symbian 2nd phase constructor is private.
*/
void CApfMimeContentPolicy::ConstructL()
	{
	iImpl = CApfMimeContentPolicyImpl::NewL();
	}

/**
By default Symbian 2nd phase constructor is private.
@param aFs  A handle to a shared file server session.
*/
void CApfMimeContentPolicy::ConstructL(RFs& aFs)
	{
	iImpl = CApfMimeContentPolicyImpl::NewL(aFs);
	}

/**
Creates a new Mime Content Policy object, and puts a pointer to it onto the cleanup stack.
@return  The new Mime Content Policy object.
*/
EXPORT_C CApfMimeContentPolicy* CApfMimeContentPolicy::NewLC()
	{
	CApfMimeContentPolicy* self = new (ELeave) CApfMimeContentPolicy();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}
    
/**
Creates a new Mime Content Policy object, and puts a pointer to it onto the cleanup stack.
@param aFs  A handle to a shared file server session. 
@return  The new Mime Content Policy object.
*/
EXPORT_C CApfMimeContentPolicy* CApfMimeContentPolicy::NewLC(RFs& aFs)
	{
	CApfMimeContentPolicy* self = new (ELeave) CApfMimeContentPolicy();
	CleanupStack::PushL(self);
	self->ConstructL(aFs);
	return self;
	}
	
/**
Creates a new Mime Content Policy object.
@return  The new Mime Content Policy object.
*/
EXPORT_C CApfMimeContentPolicy* CApfMimeContentPolicy::NewL()
	{
	CApfMimeContentPolicy* self = NewLC();
	CleanupStack::Pop();
	return self;
	}

/**
Creates a new Mime Content Policy object.
@param aFs  A handle to a shared file server session.
@return  	The new Mime Content Policy object.
*/
EXPORT_C CApfMimeContentPolicy* CApfMimeContentPolicy::NewL(RFs& aFs)
	{
	CApfMimeContentPolicy* self = NewLC(aFs);
	CleanupStack::Pop();
	return self;
	}


/**
Destructor.
*/
EXPORT_C CApfMimeContentPolicy::~CApfMimeContentPolicy()
	{
	delete iImpl;
	}

/**
Checks if given MIME type is included in closed content list.
@param aMimeType  The mime type to be checked.   
@return  ETrue if given MIME type is in closed content list else returns EFalse.
*/
EXPORT_C TBool CApfMimeContentPolicy::IsClosedType(const TDesC& aMimeType)
	{	
	return iImpl->IsClosedType(aMimeType);
	}

/**
Checks the extension of given file against list of closed file extensions.
@param aFileExtension File extension to be checked.
@return ETrue if extension of given file name is in closed extensions list else returns EFalse.
*/
EXPORT_C TBool CApfMimeContentPolicy::IsClosedExtension(const TDesC& aFileExtension)
	{
	return iImpl->IsClosedExtension(aFileExtension);
	}

/**
Checks if given file is Closed or not.
This method checks for forward lock and superdistribution statuses of the file, in addition to IsClosedExtension 
and IsClosedType checks.

@param aFileName  A file to be checked   
@return  ETrue if given file is closed else returns EFalse.
@leave KErrNone, if successful; otherwise one of the other system-wide error codes
*/
EXPORT_C TBool CApfMimeContentPolicy::IsClosedFileL(const TDesC& aFileName)
	{
	return iImpl->IsClosedFileL(aFileName);
	}


/**
Checks if given file is a DRM envelope. Can leave if file handling fails.
@param aFileName  A file to be checked. 
@return ETrue if file is DRM envelope else returns EFalse.
@leave	KErrCANotSupported if the requested attribute does not exist. 
KErrPermissionDenied if the access to the protected content is not permitted 
by the CAF Agent. Otherwise one of the other CAF error codes defined in 
caferr.h or one of the other system-wide error codes for any other errors.  
*/
EXPORT_C TBool CApfMimeContentPolicy::IsDRMEnvelopeL(const TDesC& aFileName)
	{
	return iImpl->IsDRMEnvelopeL(aFileName);
	}

/**
Checks if given file is Closed or not. This method checks for forward lock and 
superdistribution statuses of the file, in addition to IsClosedExtension and IsClosedType checks.
Remember to make a file handle sharable. When a file handle is shared, the RFs handle has to be shared too. 
@param aFileHandle  Handle to the file to be checked.  
@return ETrue if given file is closed else returns EFalse.
@leave KErrNone, if successful; KErrBadHandle if an invalid handle has been passed as a parameter.
otherwise one of the other system-wide error codes
*/
EXPORT_C TBool CApfMimeContentPolicy::IsClosedFileL(RFile& aFileHandle)
	{
	return iImpl->IsClosedFileL(aFileHandle);
	}


/**
Checks if given file is a DRM envelope. Can leave if file handling fails.
@param aFileHandle  Handle to the file to be checked. 
@return ETrue if file is DRM envelope else returns EFalse.
@leave	KErrCANotSupported if the requested attribute does not exist. 
KErrPermissionDenied if the access to the protected content is not permitted 
by the CAF Agent. Otherwise one of the other CAF error codes defined in 
caferr.h or one of the other system-wide error codes for any other errors.  
*/
EXPORT_C TBool CApfMimeContentPolicy::IsDRMEnvelopeL(RFile& aFileHandle)
	{
	return iImpl->IsDRMEnvelopeL(aFileHandle);
	}

//
// class CApfMimeContentPolicyImpl
//

/**
C++ default constructor.
*/
CApfMimeContentPolicyImpl::CApfMimeContentPolicyImpl()
	{
	// C++ default constructor can NOT contain any code, that
	}

/**
By default Symbian 2nd phase constructor is private.
*/
void CApfMimeContentPolicyImpl::ConstructL()
	{
	// Resource reading is done without coe & eikon env.
	User::LeaveIfError(iFs.Connect());
	iFsConnected = ETrue;
	
	User::LeaveIfError(iFs.ShareProtected());
	ReadResourcesL();
	}
	
/**
By default Symbian 2nd phase constructor is private.
@param aFs A handle to a shared file server session. 
*/
void CApfMimeContentPolicyImpl::ConstructL(RFs& aFs)
	{
	iFsConnected = EFalse;
	iFs = aFs;	
	ReadResourcesL();
	}

/**
Two-phased constructor.
*/
CApfMimeContentPolicyImpl* CApfMimeContentPolicyImpl::NewL()
	{
	CApfMimeContentPolicyImpl* self = new (ELeave) CApfMimeContentPolicyImpl();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	
	return self;
	}

/**
Two-phased constructor.
*/
CApfMimeContentPolicyImpl* CApfMimeContentPolicyImpl::NewL(RFs& aFs)
	{
	CApfMimeContentPolicyImpl* self = new (ELeave) CApfMimeContentPolicyImpl();
	CleanupStack::PushL(self);
	self->ConstructL(aFs);
	CleanupStack::Pop(self);

	return self;
	}
	
/**
Destructor.
*/
CApfMimeContentPolicyImpl::~CApfMimeContentPolicyImpl()
	{
	iLs.Close();
	
	if(iFsConnected)
        iFs.Close();

	delete iCcl;
	delete iExtList;
	}

/**
Checks if given MIME type is included in closed content list.
@param aMimeType  The mime type to be checked.
@return ETrue if given MIME type is in closed content list else returns EFalse.
*/
TBool CApfMimeContentPolicyImpl::IsClosedType(const TDesC& aMimeType) const 
	{
	TInt dummy = 0;
	 
	// Check if given descriptor is in closed content list.
	// Find() returns 0 if found from array, non-zero if not.
	const TBool found = (iCcl->FindIsq(aMimeType, dummy) == 0);
	return found;
	}

/**
Checks the extension of given file against list of closed file extensions.
@param aFileExtension File extension to be checked.
@return  ETrue  if extension of given file name is in closed extensions list else returns EFalse.
*/
TBool CApfMimeContentPolicyImpl::IsClosedExtension(const TDesC& aFileExtension) const
	{
	TInt dummy = 0;

	// Check if given descriptor is in closed content list.
	// Find() returns 0 if found from array, non-zero if not.
	const TBool found = (iExtList->FindIsq(aFileExtension, dummy) == 0);
	return found;
	}
	
/**
Checks if given file is Closed or not. This method checks for forward lock and superdistribution statuses 
of the file, in addition to IsClosedExtension and IsClosedType checks.
@param aFileName  A file to be checked.
@return ETrue if given file is closed else returns EFalse. 
@leave KErrNone, if successful; otherwise one of the other system-wide error codes.
*/
TBool CApfMimeContentPolicyImpl::IsClosedFileL(const TDesC& aFileName) const
	{
	RFile file;
    TInt protectionError;

    // open file to be checked for protection
    protectionError = file.Open(iFs, aFileName, EFileRead | EFileShareReadersOnly);
    
    // If the file is already opened using EFileShareAny (which is against best practices), we have to use it also.
    if (protectionError == KErrInUse) 
        protectionError = file.Open(iFs, aFileName, EFileRead | EFileShareAny);
    
    User::LeaveIfError(protectionError);

    CleanupClosePushL(file);
    const TBool retVal = IsClosedFileL(file, aFileName);
    CleanupStack::PopAndDestroy(); // file
    return retVal;
	}
	
/**
Checks if given file is Closed or not. This method checks for forward lock and superdistribution statuses 
of the file, in addition to IsClosedExtension and IsClosedType checks.
@param aFileHandle  Handle to the file to be checked.   
@return ETrue if given file is closed else returns EFalse.
@leave KErrNone, if successful; KErrBadHandle if an invalid handle has been passed as a parameter.
otherwise one of the other system-wide error codes.
*/
TBool CApfMimeContentPolicyImpl::IsClosedFileL(RFile& aFileHandle) const
	{
	if (aFileHandle.SubSessionHandle())
	    {
	    TFileName name;
	    aFileHandle.Name(name);
        return IsClosedFileL(aFileHandle, name);
    	}

    User::Leave(KErrBadHandle);
    return EFalse;	// to keep compiler happy
	}

/**
Checks if given file is Closed or not.
@param aFileHandle  Handle to the file to be checked.
@param aFileName  File to be checked.
@return ETrue if given file is closed else returns EFalse.
@leave KErrNone, if successful; KErrBadHandle if an invalid handle has been passed as a parameter.
otherwise one of the other system-wide error codes.
*/
TBool CApfMimeContentPolicyImpl::IsClosedFileL(RFile& aFileHandle, const TDesC& aFileName) const
	{
    TInt value = 0; 
    // allocate a content object
    ContentAccess::CContent* content = ContentAccess::CContent::NewL(aFileHandle);
    
    // Check if file is protected
    // If the file type can not be determined just forget it.
    if (content->GetAttribute(ContentAccess::EIsProtected, value) == KErrNone)
        {
        if (value)
 		    {
        	// File is DRM protected
        	value = 0;
        	// Check if file is forwardable
        	if (content->GetAttribute(ContentAccess::EIsForwardable, value) == KErrNone)
        	    {
        	    delete content;
        	    if (!value)
                    {
                    // EIsProtected == ETrue &&  EIsForwardable == EFalse => EForwardLocked
                    // If forwardlocked, sending never allowed, so return without further ado.
			        return ETrue;	
                    }
                else
                    {
                    // EIsProtected == ETrue &&  EIsForwardable == ETrue => ESuperDistributable
                    // No need to check extension or mime type for files that are superdistributable,
			        // they must not be blocked in any case.
			        return EFalse;
                    }
        	    }
 		    }
        }
    
    delete content;
    content = NULL;
    
    // Check file extension.	
	TParse parser;
	parser.Set(aFileName, NULL, NULL);
	if (IsClosedExtension(parser.Ext()))
		return ETrue;

	// Recognize and check MIME type.	
	TDataType recData;
	TUid uid;
	const TInt err(iLs.AppForDocument(aFileHandle, uid, recData));
	User::LeaveIfError(err);
	    
	return IsClosedType(recData.Des());   // Check MIME type.
	}

/**
Checks if given file is a DRM envelope. Can leave if file handling fails.
@param aFileName  A file to be checked.
@return ETrue if file is DRM envelope else returns EFalse.
@leave	KErrCANotSupported if the requested attribute does not exist.
KErrPermissionDenied if the access to the protected content is not permitted 
by the CAF Agent. Otherwise one of the other CAF error codes defined in 
caferr.h or one of the other system-wide error codes for any other errors.
*/
TBool CApfMimeContentPolicyImpl::IsDRMEnvelopeL(const TDesC& aFileName) const
	{
    // allocate a content object
    ContentAccess::CContent* content = ContentAccess::CContent::NewLC(aFileName, ContentAccess::EContentShareReadOnly);
    
    TInt value = 0;
    User::LeaveIfError(content->GetAttribute(ContentAccess::EIsProtected, value));
    CleanupStack::PopAndDestroy(content);
    
    return (value != 0);    // File is DRM protected if value != 0
	}
	
/**
Checks if given file is a DRM envelope. Can leave if file handling fails.
@param aFileHandle  Handle to the file to be checked.
@return ETrue if file is DRM envelope else returns EFalse.
@leave	KErrCANotSupported if the requested attribute does not exist. 
KErrPermissionDenied if the access to the protected content is not permitted 
by the CAF Agent. Otherwise one of the other CAF error codes defined in 
caferr.h or one of the other system-wide error codes for any other errors. 
*/
TBool CApfMimeContentPolicyImpl::IsDRMEnvelopeL(RFile& aFileHandle) const
	{
    // allocate a content object
    ContentAccess::CContent* content = ContentAccess::CContent::NewLC(aFileHandle);
    
    TInt value = 0;
    User::LeaveIfError(content->GetAttribute(ContentAccess::EIsProtected, value));
    CleanupStack::PopAndDestroy(content);
       
    return (value != 0);    // File is DRM protected if value != 0
	}

	
/**
Reads closed content list and closed extensions list. Connects to RApaLsSession.
Called by constructor.
@param aFs A handle to a shared file server session. 
*/
void CApfMimeContentPolicyImpl::ReadResourcesL()
	{
    ASSERT(!iCcl);
    ASSERT(!iExtList);
    
	CRepository *cenrep=CRepository::NewL(KClosedContentAndExtensionInfoRepositoryUID);	
	CleanupStack::PushL(cenrep);
	
	RArray<TUint32> keyArray;
	CleanupClosePushL(keyArray);
	
    TBuf<KMaxDataTypeLength> keyData;
    //Find the extenstion type keys in the repository 
	cenrep->FindL(KClosedExtensionTypePartialKey, KClosedTypeKeyMask, keyArray);
	int keyCount=keyArray.Count();

	iExtList=new (ELeave) CDesCArrayFlat(keyCount);

	TInt valid;
	TInt index;
	//Get each extension type key value and store in iExtList array
	for(index=0; index<keyCount; index++)
	    {
	    cenrep->Get(keyArray[index], keyData);
        //Check validity of the extension. If its invalid it will not be added to list.	    
	    valid=IsValidExtension(keyData);
	    if(valid)
	        iExtList->AppendL(keyData);
	    }
	
	keyArray.Reset();
	
    //Find the content type keys in the repository 	
    cenrep->FindL(KClosedContentTypePartialKey, KClosedTypeKeyMask, keyArray);
    keyCount=keyArray.Count();
    
    iCcl=new (ELeave) CDesCArrayFlat(keyCount);
    
    //Get each content type key value and store in iCcl array
    for(index=0; index<keyCount; index++)
        {
        cenrep->Get(keyArray[index], keyData);  
        //Check validity of the mime type. If its invalid it will not be added to list.
        valid=IsValidMimeType(keyData);
        if(valid)        
            iCcl->AppendL(keyData);
        }
    
    CleanupStack::PopAndDestroy(2, cenrep);
	
    // Sort lists to enable binary find
    iCcl->Sort();
    iExtList->Sort();

	// Connect RApaLsSession and leave it open for the whole
	// lifetime of the object.
	User::LeaveIfError(iLs.Connect());
	}


//Checks the given extension is valid or invalid. The extension should start with a ".".
TBool CApfMimeContentPolicyImpl::IsValidExtension(TDesC& extension)
    {
     TChar dot='.';
     //Check whether extension should start with "."
     return(extension.Locate(dot)==0);
    }

//Checks the given mime type is valid or not.
//The mime type will be in the following format type/subtype. Ex: "application/vnd.oma.drm.message"
//Mime type should posses the following properties. Otherewise those are considered as invalid.
//1. Only one front slash should exist. That should be followed by the type field.
//2. There should not be any backslashes.

TBool CApfMimeContentPolicyImpl::IsValidMimeType(TDesC& mimeType)
    {
    TChar backslash='\\';            
    TChar forwardslash='/';
    
    //Check any backslash is used
    TBool found=mimeType.Locate(backslash);
    if(found!=KErrNotFound)
        return(EFalse);

    //Locate forward slash position
    found=mimeType.Locate(forwardslash);
    
    //There should be at least one forward slash
    if(found==KErrNotFound)
        {
        return EFalse;
        }
    else
        {
        //Forward slash position should not at first or last position of the mime type
        if(found==0||(found==mimeType.Length()-1))
            return EFalse;
        
        //There should not be more than one forward slash
        found=mimeType.Mid(found+1).Locate(forwardslash);
        if(found!=KErrNotFound)
            {
            return(EFalse);       
            }
        else
            {
            //MIME format is valid
            return(ETrue);
            }
        }
    }
