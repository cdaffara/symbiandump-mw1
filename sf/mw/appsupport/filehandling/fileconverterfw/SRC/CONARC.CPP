// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <conarc.h>
#include <ecom/implementationinformation.h>
#include <ecom/ecom.h>

/** ROM patchable constant. Determines whether to perform Quoted Printable conversion for all the special
characters or not. The default value is 1.
When set to non zero, the conversion is performed for all the special characters.
When set to zero, special characters are not converted.

The constant can be changed at ROM build time using patchdata keyword in IBY file. 
To patch the value, add a line to an iby file that is included in the ROM being built using the following format:
"patchdata <dll> @ <symbol> <newvalue>"

@publishedPartner
@released */
EXPORT_C extern const TInt KEnableAllSplCharForQpCnv = 1;

//
// class CConverterBase
//

EXPORT_C TInt CConverterBase::Capabilities()
/** Gets the converter's capabilities.

The default is to return a bitmask of all the TCapability flags.

@return A bitmask of TCapability flags describing the capabilities. */
	{
	return EConvertsFiles|EConvertsObjects|EConvertsExtract;
	}

EXPORT_C void CConverterBase::ConvertL(const TFileName& aSourceFile, const TFileName& aTargetFile, MConverterUiObserver* aObserver)
/** Converts a file in a single step.

The caller can supply a MConverterUiObserver to be informed of progress in 
the conversion.

The default implementation calls ConvertAL() to prepare the object to perform 
the conversion, and then calls DoConvertL() in a loop until conversion is 
complete.

@param aSourceFile File to convert
@param aTargetFile File to which to write the converted output
@param aObserver Optional observer of the conversion process
@leave KErrNotSupported File conversion is not supported */
	{
	ConvertAL(aSourceFile,aTargetFile,aObserver);
	while (DoConvertL())
		;
	}

EXPORT_C void CConverterBase::ConvertObjectL(RReadStream& aReadStream, RWriteStream& aWriteStream, MConverterUiObserver* aObserver)
/** Converts a stream object in a single step.

The caller can supply a MConverterUiObserver to be informed of progress in 
the conversion.

The default implementation calls ConvertObjectAL() to prepare the object to 
perform the conversion, and then calls DoConvertL() in a loop until conversion 
is complete.

@param aReadStream Stream from which to read the data to convert
@param aWriteStream Stream to which to write the converted data
@param aObserver Optional observer of the conversion process */
	{
	ConvertObjectAL(aReadStream,aWriteStream,aObserver);
	while (DoConvertL())
		;
	}

EXPORT_C void CConverterBase::ConvertAL(const TFileName& /*aSourceFile*/, const TFileName& /*aTargetFile*/, MConverterUiObserver* /*aObserver*/)
/** Prepares for conversion in multiple steps of a file.

Clients must call this function before calling DoConvertL() one or more times 
to do the conversion. The function can call back the MaxSteps() function of 
the supplied MConverterUiObserver to tell the client the maximum number of 
calls to DoConvertL() that will be required.

The rest of this description describes how to implement this function.

The function should initialise the object before performing a conversion, 
but should not do the conversion itself. Such initialisation should include:

storing the MConverterUiObserver pointer (if supplied), so the client can 
later be notified of conversion progress

validating the integrity of the input data

optionally, creating the appropriate application engine, either for input 
or output, to enable access to the data

determining the number of steps (i.e. calls to DoConvertL()) required to perform 
the conversion and pass this to the client by calling MConverterUiObserver::MaxSteps()

The default implementation leaves with KErrNotSupported.

@param aSourceFile File to convert
@param aTargetFile File to which to write the converted output. The file can 
be overwritten if it already exists.
@param aObserver Observer of the conversion process
@leave KErrNotSupported File conversion is not supported */
	{
	User::Leave(KErrNotSupported);
	}

EXPORT_C void CConverterBase::ConvertObjectAL(RReadStream& /*aReadStream*/, RWriteStream& /*aWriteStream*/, MConverterUiObserver* /*aObserver*/)
/** Prepares for conversion in multiple steps of a stream object.

Clients must call this function before calling DoConvertL() one or more times 
to do the conversion. The function can call back the MaxSteps() function of 
the supplied MConverterUiObserver to tell the client the maximum number of 
calls to DoConvertL() that will be required.

For a description of how to implement this function, see ConvertAL().

@param aReadStream Stream from which to read the data to convert
@param aWriteStream Stream to which to write the converted data
@param aObserver Optional observer of the conversion process
@leave KErrNotSupported Stream object conversion is not supported */
	{
	User::Leave(KErrNotSupported);
	}

EXPORT_C TBool CConverterBase::DoConvertL()
/** Performs a step in converting the data.

The function advances a step in converting the data each time that it is called. 
When conversion is complete, the function returns EFalse.

@return EFalse if conversion is complete, else ETrue */
	{
	User::Leave(KErrNotSupported);
	return EFalse;
	}

EXPORT_C void CConverterBase::CancelConvert()
/** Cleans up a conversion that has been prepared or is in progress.

It should free any resources and reset the converter object to an initial 
state.

The default is to do nothing. */
	{
	}

EXPORT_C CConverterBase* CConverterBase::EmbeddedObjectL(TDataType& /*aType*/)
/** Gets a converter for an embedded object of the specified type.

The default is to return NULL.

@param aType Type of the embedded object
@return Converter for the embedded object */
	{
	return NULL;
	}

EXPORT_C TBool CConverterBase::GetEmbeddedFileName(TFileName& /*aFileName*/)
/** Gets a filename embedded in the object.

@param aFileName The file name found
@return True if a filename was found, false otherwise */
	{
	return EFalse;
	}

EXPORT_C void CConverterBase::ExtendedInterfaceL(TUid /*aInterfaceUid*/, CBase*& /*aInterface*/)
/** Allows licensees to extend the Converter Architecture API.

This function replaces the private Reserved_1() function, so the change is 
binary compatible with v7.0.

If overridden by the converter, it allows third party code to request the 
extended interface object by UID. If not overridden, the default implementation 
does nothing.

@param aInterfaceUid A UID that identifies the required extended interface 
object.
@param aInterface On return, an object that extends the Converter Architecture 
interface, or NULL, if the UID specified is not recognised. */
	{
	}

//
// class MConverterUiObserver
//

EXPORT_C void MConverterUiObserver::Reserved1_Conv_Obs()
	{
	}

//
// class CConverterBase2
//

EXPORT_C CConverterBase2::~CConverterBase2() 
	{
	if (iDestructionKey.iUid!=0)
		{
		REComSession::DestroyedImplementation(iDestructionKey);
		}
	}

CConverterBase2*  CConverterBase2:: CreateConverterL(TUid aImplUid)
	{
	return ((CConverterBase2*)REComSession::CreateImplementationL(aImplUid, _FOFF(CConverterBase2, iDestructionKey)));
	}

EXPORT_C void CConverterBase2::ConvertL(const TFileName& aSourceFile, const TFileName& aTargetFile, MConverterUiObserver* aObserver)
/** Converts a file in a single step.

The caller can supply a MConverterUiObserver to be informed of progress in 
the conversion.

The default implementation calls ConvertAL() to prepare the object to perform 
the conversion, and then calls DoConvertL() in a loop until conversion is 
complete.

@param aSourceFile File to convert
@param aTargetFile File to which to write the converted output
@param aObserver Optional observer of the conversion process
@leave KErrNotSupported File conversion is not supported */
	{
	ConvertAL(aSourceFile,aTargetFile,aObserver);
	while (DoConvertL())
		;
	}

EXPORT_C  void CConverterBase2::ConvertObjectL(RReadStream& aReadStream, RWriteStream& aWriteStream, MConverterUiObserver* aObserver)
/** Converts a stream object in a single step.

The caller can supply a MConverterUiObserver to be informed of progress in 
the conversion.

The default implementation calls ConvertObjectAL() to prepare the object to 
perform the conversion, and then calls DoConvertL() in a loop until conversion 
is complete.

@param aReadStream Stream from which to read the data to convert
@param aWriteStream Stream to which to write the converted data
@param aObserver Optional observer of the conversion process */
	{
	CConverterBase::ConvertObjectL(aReadStream,aWriteStream,aObserver);
	}


EXPORT_C  void CConverterBase2::ConvertAL(const TFileName& /*aSourceFile*/, const TFileName& /*aTargetFile*/, MConverterUiObserver* /*aObserver*/)
/** Prepares for conversion in multiple steps of a file.

Clients must call this function before calling DoConvertL() one or more times 
to do the conversion. The function can call back the MaxSteps() function of 
the supplied MConverterUiObserver to tell the client the maximum number of 
calls to DoConvertL() that will be required.

The rest of this description describes how to implement this function.

The function should initialise the object before performing a conversion, 
but should not do the conversion itself. Such initialisation should include:

storing the MConverterUiObserver pointer (if supplied), so the client can 
later be notified of conversion progress

validating the integrity of the input data

optionally, creating the appropriate application engine, either for input 
or output, to enable access to the data

determining the number of steps (i.e. calls to DoConvertL()) required to perform 
the conversion and pass this to the client by calling MConverterUiObserver::MaxSteps()

The default implementation leaves with KErrNotSupported.

@param aSourceFile File to convert
@param aTargetFile File to which to write the converted output. The file can 
be overwritten if it already exists.
@param aObserver Observer of the conversion process
@leave KErrNotSupported File conversion is not supported */
	{
	User::Leave(KErrNotSupported);
	}

EXPORT_C  void CConverterBase2::ConvertObjectAL(RReadStream& /*aReadStream*/, RWriteStream& /*aWriteStream*/, MConverterUiObserver* /*aObserver*/)
/** Prepares for conversion in multiple steps of a stream object.

Clients must call this function before calling DoConvertL() one or more times 
to do the conversion. The function can call back the MaxSteps() function of 
the supplied MConverterUiObserver to tell the client the maximum number of 
calls to DoConvertL() that will be required.

For a description of how to implement this function, see ConvertAL().

@param aReadStream Stream from which to read the data to convert
@param aWriteStream Stream to which to write the converted data
@param aObserver Optional observer of the conversion process
@leave KErrNotSupported Stream object conversion is not supported */
	{
	User::Leave(KErrNotSupported);
	}

EXPORT_C TBool CConverterBase2::DoConvertL()
/** Performs a step in converting the data.

The function advances a step in converting the data each time that it is called. 
When conversion is complete, the function returns EFalse.

@return EFalse if conversion is complete, else ETrue */
	{
	User::Leave(KErrNotSupported);
	return EFalse;
	}

EXPORT_C TInt CConverterBase2::Capabilities()
/** Gets the converter's capabilities.

The default is to return a bitmask of all the TCapability flags.

@return A bitmask of TCapability flags describing the capabilities. */
	{
	return EConvertsFiles|EConvertsObjects|EConvertsExtract;
	}

EXPORT_C void CConverterBase2::CancelConvert()
/** Cleans up a conversion that has been prepared or is in progress.

It should free any resources and reset the converter object to an initial 
state.

The default is to do nothing. */
	{
	}

EXPORT_C CConverterBase* CConverterBase2::EmbeddedObjectL(TDataType& /*aType*/) 
/** Gets a converter for an embedded object of the specified type.

The default is to return NULL.

@param aType Type of the embedded object
@return Converter for the embedded object */
	{
	return NULL;
	}

EXPORT_C TBool CConverterBase2::GetEmbeddedFileName(TFileName& /*aFileName*/) 
/** Gets a filename embedded in the object.

@param aFileName The file name found
@return True if a filename was found, false otherwise */
	{
	return EFalse;
	}

EXPORT_C void CConverterBase2::ExtendedInterfaceL(TUid /*aInterfaceUid*/, CBase*& /*aInterface*/)
/** Allows licensees to extend the Converter Architecture API.


If overridden by the converter, it allows third party code to request the 
extended interface object by UID. If not overridden, the default implementation 
does nothing.

@param aInterfaceUid A UID that identifies the required extended interface 
object.
@param aInterface On return, an object that extends the Converter Architecture 
interface, or NULL, if the UID specified is not recognised. */
	{
	}

EXPORT_C void CConverterBase2::Reserved_1()
	{
	}

EXPORT_C void CConverterBase2::Reserved_2()
	{
	}
