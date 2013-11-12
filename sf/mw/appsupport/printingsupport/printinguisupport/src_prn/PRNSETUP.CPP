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

#include <e32std.h>
#include <e32base.h>

#include <prninf.h>
#include <prnsetup.h>
#include "PRNIMP.H"
#include "PRNSTD.H"


//
// CPrintSetup
//

EXPORT_C CPrintSetup* CPrintSetup::NewL()
/** Allocates and constructs an uninitialised CPrintSetup object. 

After construction, the print settings, including the target printer device 
and the page specification must be set before printing can begin.

@return The print setup object */
	{
	CPrintSetup* self=new(ELeave) CPrintSetup();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


CPrintSetup::CPrintSetup()
	{__DECLARE_NAME(_S("CPrintSetup"));}


void CPrintSetup::ConstructL()
	{
	iDriverDirList = new(ELeave) CArrayFixSeg<TFileName>(1);
	// create header and footer
	iHeader = CHeaderFooter::NewL();
	iFooter = CHeaderFooter::NewL();
	iHeader->SetPageNumInfo(*this);
	iFooter->SetPageNumInfo(*this);
	}

EXPORT_C CPrintSetup::~CPrintSetup()
/** Destructor.

This frees all resources owned by the CPrintSetup object, prior to its destruction. */
	{
	delete iPrintProcess;
	delete iModelList;
	delete iPrinterDriver;
	delete iHeader;
	delete iFooter;
	delete iDriverDirList;
	}

EXPORT_C void CPrintSetup::AddPrinterDriverDirL(const TDesC& aDriverDir)
/** Adds a search path for printer drivers.

This function must be called before a model name list can be created. It can 
be called repeatedly to add a number of paths to the search list. 

When a printer model name list is requested (using ModelNameListL()), the 
directories specified in each of the search paths are scanned on all non-remote 
drives for .pdr files, indicating the models supported. If any path contains 
a drive, that drive alone is searched.

@param aDriverDir Path which specifies a directory in which to search for 
printer drivers. Any filename in the path is ignored. If the path is already 
in the list, it is not added again.
@leave KErrNoMemory There is insufficient memory to perform the operation. */
	{
	TParse parser;
	parser.Set(aDriverDir,NULL,NULL);
	for (TInt i=iDriverDirList->Count()-1 ; i>=0 ; i--)
		{
		if ( (*iDriverDirList)[i].CompareF(parser.DriveAndPath())==0 )
			return; // its already on the list, so dont add it again
		}
	iDriverDirList->AppendL(parser.DriveAndPath());
	}

EXPORT_C CPrinterModelList* CPrintSetup::ModelNameListL(RFs& aFs)
/** Gets the names of all printer models supported.

The function scans all directories in the search path list which were added 
using AddPrinterDriverDirL(). It returns a list of the printer models supported 
by the .pdr files found in those directories.

@param aFs A connection to a file server session.
@leave KErrNoMemory There is insufficient memory to perform the operation. 
@return The list of printer models supported. */
	{
	__ASSERT_ALWAYS(iDriverDirList->Count()>0,Panic(EPanicNoPrinterDirs));
	//
	if (!iModelList)
		{
		CPdrModelList* modelList = CPdrModelList::NewL();
		CleanupStack::PushL(modelList);
		// define some variables that are used in the loops, so it only gets done once
		TDriveList driveList;
		TDriveInfo driveInfo;
		TEntry entry;
		TParse parser;
		// get a list of available drives
		User::LeaveIfError(aFs.DriveList(driveList));
		for (TInt i=iDriverDirList->Count()-1 ; i>=0 ; i--)
			{
			TInt err = parser.Set((*iDriverDirList)[i],NULL,NULL);
			if (err==KErrNone)
				{
				if (parser.DrivePresent())
					{// if there is a drive present add the path straight
					err = aFs.Entry(parser.DriveAndPath(),entry);
					if (err==KErrNone)
						modelList->AddDirectoryL(parser.DriveAndPath()); // add dir
					}
				else
					{
					for (TInt n=0 ; n<KMaxDrives ; n++)
						{// if there's no drive scan all drives for matching directories
						if (driveList[n]!=0)
							{
							aFs.Drive(driveInfo,n);
							if (driveInfo.iType!=EMediaNotPresent && driveInfo.iType!=EMediaRemote)
								{
								TDriveUnit drive(n);
								err = parser.Set(drive.Name(),&(*iDriverDirList)[i],NULL);
								if (err==KErrNone)
									err = aFs.Entry(parser.DriveAndPath(),entry);
								if (err==KErrNone)
									modelList->AddDirectoryL(parser.DriveAndPath()); // add dir
								}
							}
						}
					}
				}
			}
		CleanupStack::Pop(); // modelList
		iModelList = modelList;
		}
	return iModelList->ScanForModelsL();
	}

EXPORT_C void CPrintSetup::FreeModelList()
/** Deletes and sets the printer model list to NULL. */
	{
	delete iModelList;
	iModelList = NULL;
	}
	

EXPORT_C void CPrintSetup::CreatePrinterDeviceL(TUid aModelUid,RFs& aFs)
/** Selects the target printer device, from the list of printer models.

If no printer model name list has previously been created by a call to ModelNameListL(), 
this function will temporarily create one, freeing it before exiting. 

If no printer model in the list has the UID specified, the first printer model 
in the list is selected by default. If the list contains no printer models 
at all, a leave occurs.

If a print process is taking place when this function is called, it is aborted.

The existing printer device's page specification is preserved.

@param aModelUid A UID which identifies a printer device.
@param aFs A connection to a file server session.
@leave KErrNoMemory There is insufficient memory to perform the operation.
@leave KErrNotFound The printer model list contains no printer devices. */
	{
	TBool listCreated=EFalse;
	if (!iModelList)
		{
		ModelNameListL(aFs);
		listCreated = ETrue;
		}
	TInt modelNum=iModelList->UidToNum(aModelUid);
	if (modelNum==KErrNotFound)
		{
		if (iModelList->ModelCount()>0)
			modelNum = 0;
		else
			User::Leave(KErrNotFound);
		}
	CreatePrinterDeviceL(modelNum);
	if (listCreated)
		FreeModelList();
	}

EXPORT_C void CPrintSetup::CreatePrinterDeviceL(TInt aModelIndex)
/** Selects the target printer device by its index into the list of printer models.

Unlike the other overload of this function, a panic occurs if no printer model 
list has previously been created using ModelNameListL().

If a print process is taking place when this function is called, it is aborted.

The existing printer device's page specification is preserved.

@param aModelIndex Index of the printer device into the list of printer devices.
@leave KErrNoMemory There is insufficient memory to perform the operation. */
	{
	__ASSERT_ALWAYS(iModelList,Panic(EPanicNoModelList));
	//
	CPrinterDriver* printerDriver = iModelList->CreatePrinterDriverL(aModelIndex);
	if (iPrinterDriver)
		{
		EndPrint(); // just in case we're printing at the time
		printerDriver->PrinterDevice()->SelectPageSpecInTwips(iPrinterDriver->PrinterDevice()->CurrentPageSpecInTwips());
		delete iPrinterDriver;
		}
	iPrinterDriver = printerDriver;
	}

EXPORT_C TInt CPrintSetup::StartPrintL(const TPrintParameters& aPrintParams,MPageRegionPrinter& aBodyPrinter,
									   CPrinterPort* aPort,MPrintProcessObserver* aObserver)
/** Starts a new print job.

If a print process observer is specified, this function calls its NotifyPrintStarted() 
function.

@param aPrintParams The parameters for the print job.
@param aBodyPrinter An object which implements the page region printer interface.
@param aPort The printer port. Must be provided if the selected printer device 
requires one.
@param anObserver An optional object which implements the print process observer 
interface.
@return KErrNone if the operation completed successfully, KErrAlreadyExists 
if a print process is already taking place, or another of the system-wide 
error codes. 
@panic PRINT 1 No printer device has been selected. Use CreatePrinterDevice() 
to select one. */
	// Instigates printing of document
	// I take ownership of the port passed in, deleting it on error
	//
	{
	__ASSERT_ALWAYS(PrinterDevice(),Panic(EPanicNoPrinterDriver));

	if (!iPrintProcess)
		{
		CleanupStack::PushL(aPort);
		iPrintProcess = CPrintProcess::NewL();
		CleanupStack::Pop(); // aPort
		TRAPD(ret,DoStartPrintL(aPrintParams,aBodyPrinter,aObserver,aPort));
		if (ret!=KErrNone)
			{// there's been an error in print startup
			delete iPrintProcess;
			iPrintProcess = NULL;
			User::Leave(ret);
			}
		return KErrNone;
		}
	else
		{
		delete aPort;
		return KErrAlreadyExists;
		}
	}


void CPrintSetup::DoStartPrintL(const TPrintParameters& aPrintParams,MPageRegionPrinter& aBodyPrinter,
									   MPrintProcessObserver* aObserver,CPrinterPort* aPort)
	{
	CleanupStack::PushL(aPort);
	InitializePrintingL(iPrinterDriver->PrinterDevice(),aPrintParams,aObserver);
	CPrinterDriverUI* printerUi=iPrinterDriver->CreatePrinterDriverUIL(); // process takes ownership of this
	CleanupStack::Pop(); // aPort
	// start the print
	((CPrintProcess*)iPrintProcess)->StartPrintL(aPrintParams,*iPrinterDriver->PrinterDevice(),printerUi,aBodyPrinter,aPort,(void**)&iPrintProcess);
	}

EXPORT_C TInt CPrintSetup::StartPreviewPrintL(const TPrintParameters& aPrintParams,MPageRegionPrinter& aBodyPrinter,
				MPrintProcessObserver* aObserver,CGraphicsDevice& aPrinterDev,const TRect& aHeader,const TRect& aFooter,TInt aNumBands)
/** Starts a new print preview.

If a print process observer is specified, this function calls its NotifyPrintStarted() 
function.

@param aPrintParams The parameters for the print preview operation. 
@param aBodyPrinter An object which implements the page region printer interface. 
@param aObserver An optional object which implements the print process observer 
interface. 
@param aPrinterDev The graphics device to print to. Must not be NULL 
@param aHeader The rectangle within the top page margin to contain 
the header.
@param aFooter The rectangle within the bottom page margin to contain 
the footer.
@param aNumBands The number of bands per page.
@return KErrNone if the operation completed successfully, KErrAlreadyExists 
if a print preview process is already taking place, or another of the system-wide 
error codes. 
@panic PRINT 1 No printer device has been selected. Use CreatePrinterDevice() 
to select one. */
	// Instigates print preview
	//
	{
	__ASSERT_ALWAYS(PrinterDevice(),Panic(EPanicNoPrinterDriver));

	if (!iPrintProcess)
		{
		iPrintProcess = CPreviewProcess::NewL();
		// Set up the printer device
		TRAPD(ret,InitializePrintingL(aPrintParams,aObserver,aHeader,aFooter));
		if (ret!=KErrNone)
			{// there's been an error in print startup
			delete iPrintProcess;
			iPrintProcess = NULL;
			User::Leave(ret);
			}
		// start the print
		iPrintProcess->StartPreview(aPrintParams,aPrinterDev,aBodyPrinter,iPrinterDriver->PrinterDevice()->CurrentPageSpecInTwips(),aNumBands,(void**)&iPrintProcess);
		return KErrNone;
		}
	else
		return KErrAlreadyExists;
	}

void CPrintSetup::InitializePrintingL(CGraphicsDevice* aPrinterDev,const TPrintParameters& aPrintParams,MPrintProcessObserver* aObserver)
	{
	TRect footer(iPrinterDriver->PrinterDevice()->CurrentPageSpecInTwips().OrientedPageSize());
	TRect printablePage=iPrinterDriver->PrinterDevice()->PrintablePageInPixels();
	printablePage=iPrinterDriver->PrinterDevice()->PixelsToTwips(printablePage);
	footer.iTl.iX += iPageMarginsInTwips.iMargins.iLeft;
	if (footer.iTl.iX<printablePage.iTl.iX)
		footer.iTl.iX=printablePage.iTl.iX;
	footer.iBr.iX -= iPageMarginsInTwips.iMargins.iRight;
	if (footer.iBr.iX>printablePage.iBr.iX)
		footer.iBr.iX=printablePage.iBr.iX;
	footer.iTl.iY = footer.iBr.iY-iPageMarginsInTwips.iMargins.iBottom+iPageMarginsInTwips.iFooterOffset;
	if (footer.iTl.iY>printablePage.iBr.iY)
		footer.iTl.iY=printablePage.iBr.iY;
	footer.iBr.iY=printablePage.iBr.iY;
	TRect header(footer);
	header.iTl.iY = iPageMarginsInTwips.iHeaderOffset;
	if (printablePage.iTl.iY>iPageMarginsInTwips.iHeaderOffset)
		iPageMarginsInTwips.iHeaderOffset=printablePage.iTl.iY;
	header.iBr.iY = iPageMarginsInTwips.iMargins.iTop;
	if (header.iBr.iY<header.iTl.iY)
		header.iBr.iY=header.iTl.iY;
	InitializePrintingL(aPrintParams,aObserver,aPrinterDev->TwipsToPixels(header),aPrinterDev->TwipsToPixels(footer));
	}

void CPrintSetup::InitializePrintingL(const TPrintParameters& aPrintParams,MPrintProcessObserver* aObserver,
									 const TRect& aHeaderRectInPixels,const TRect& aFooterRectInPixels)
	{
	iPrintObserver = aObserver;
	iPrintParams = aPrintParams;
	// Set observer
	iPrintProcess->SetObserver(aObserver);
	// create header and footer printers if necessary
	CHeaderFooterPrinter* footerPrinter=NULL;
	if (iFooter->Text())
		{
		footerPrinter = CHeaderFooterPrinter::NewL(*iFooter,aFooterRectInPixels);
		if (iHeader->Text())
			CleanupStack::PushL(footerPrinter);
		}

	CHeaderFooterPrinter* headerPrinter=NULL;
	if (iHeader->Text())
		{
		headerPrinter = CHeaderFooterPrinter::NewL(*iHeader,aHeaderRectInPixels);
		if (iFooter->Text())
			CleanupStack::Pop(); //footerPrinter
		}

	// Set header and footer printers
	iPrintProcess->SetHeaderFooter(headerPrinter,footerPrinter);
	}

EXPORT_C void CPrintSetup::EndPrint()
// Aborts printing if it is currently taking place, else does nothing.
// Will also abort preview when this is integrated
//
/** Aborts the print operation, if one is currently taking place.

If a print process observer exists, its NotifyPrintEnded() function is called 
with KErrCancel.

@see MPrintProcessObserver */
	{
	if (iPrintProcess)
		// printing is taking place
		iPrintProcess->TerminatePrint(KErrCancel);
	}

EXPORT_C TStreamId CPrintSetup::StoreL(CStreamStore& aStore)const
/** Stores a CPrintSetup object, including its components (e.g. header and footer) 
to a stream store.

A panic occurs if no printer device has been created. Use CreatePrinterDevice() 
to create one.

@param aStore Store to which the CPrintSetup object should be stored. 
@return ID of the stream containing the external representation of the CPrintSetup 
object. */
	{
	CStoreMap* map=CStoreMap::NewLC(aStore);
	StoreComponentsL(aStore,*map);
//
	RStoreWriteStream stream(*map);
	TStreamId id=stream.CreateLC(aStore);
	stream<< *this;
	stream.CommitL();
//
	map->Reset();
	CleanupStack::PopAndDestroy(2);
	return id;
	}

EXPORT_C void CPrintSetup::StoreComponentsL(CStreamStore& aStore,CStoreMap& aMap)const
/** Stores the rich text components of the print setup object's header and footer.

This function does not store any other print setup information, so may need 
to be accompanied by a call to ExternalizeL().

@param aStore The store to which the rich text components of the header and 
footer are stored. 
@param aMap Table of swizzles. Supports the deferred loading from the stream 
store of the components. */
	{
	iHeader->StoreComponentsL(aStore,aMap);
	iFooter->StoreComponentsL(aStore,aMap);
	}

EXPORT_C void CPrintSetup::ExternalizeL(RWriteStream& aStream) const
/** Externalises a CPrintSetup object to a write stream.

The presence of this function means that the standard templated operator<<() 
(defined in s32strm.h) is available to externalise objects of this class.

This function does not externalise the rich text components of the header 
and footer (e.g. fields, pictures and rich text-specific formatting), so it 
may need to be accompanied by a call to StoreComponentsL().

@param aStream Stream to which the object should be externalised.
@panic PRINT 1 No printer device has been selected. Use CreatePrinterDevice() 
to select one. */
	{
	__ASSERT_ALWAYS(iPrinterDriver,Panic(EPanicNoPrinterDriver));

	aStream.WriteInt32L(iNumOfFirstPage);
	aStream << iPageMarginsInTwips;
	aStream << *iHeader;
	aStream << *iFooter;
	aStream << iPrinterDriver->PrinterDevice()->Model().iUid;
	aStream << iPrinterDriver->PrinterDevice()->CurrentPageSpecInTwips();
	}

EXPORT_C void CPrintSetup::RestoreL(const CStreamStore& aStore,TStreamId aStreamId,const MFieldFileNameInfo* aFileNameInfo,const MFieldNumPagesInfo* aNumPagesInfo,MPictureFactory* aFactory)
/** Restores a CPrintSetup object, including its components from a stream store.

@param aStore Store from which to restore the CPrintSetup object 
@param aStreamId ID of the stream containing the external representation of 
the CPrintSetup object 
@param aFileNameInfo An optional object implementing the MFieldFileNameInfo 
interface. This provides the file name for insertion into a field in the header 
or footer.
@param aNumPagesInfo An optional object implementing the MFieldNumPagesInfo 
interface. This provides the total number of pages for insertion into a field 
in the header or footer.
@param aFactory Picture factory. Must be supplied if the header or footer contains 
pictures which should be restored. */
	{
	// retrieve the headstream from the store
	RStoreReadStream stream;
	stream.OpenLC(aStore,aStreamId);
	// internalize the headstream, then fetch the components from their own streams
	InternalizeL(stream);
	CleanupStack::PopAndDestroy(); // stream
	RestoreComponentsL(aStore,aFileNameInfo,aNumPagesInfo,aFactory);
	}

EXPORT_C void CPrintSetup::RestoreComponentsL(const CStreamStore& aStore,const MFieldFileNameInfo* aFileNameInfo,const MFieldNumPagesInfo* aNumPagesInfo,MPictureFactory* aFactory)
/** Restores the rich text components of the print setup object's header and footer.

This function only restores the rich text components of the header and footer 
(e.g. fields, pictures and rich text-specific formatting), so it may need 
to be accompanied by a call to InternalizeL().

@param aStore The store from which the components should be restored.
@param aFileNameInfo An optional object implementing the MFieldFileNameInfo 
interface. This provides the file name for insertion into a field in the header 
or footer.
@param aNumPagesInfo An optional object implementing the MFieldNumPagesInfo 
interface. This provides the total number of pages for insertion into a field 
in the header or footer.
@param aFactory Picture factory. Must be supplied if the header or footer contains 
pictures which should be restored. */
	{
	if (aFileNameInfo)
		{
		iHeader->SetFileNameInfo(*aFileNameInfo);
		iFooter->SetFileNameInfo(*aFileNameInfo);
		}
	if (aNumPagesInfo)
		{
		iHeader->SetNumPagesInfo(*aNumPagesInfo);
		iFooter->SetNumPagesInfo(*aNumPagesInfo);
		}
	iHeader->RestoreComponentsL(aStore,aFactory);
	iFooter->RestoreComponentsL(aStore,aFactory);
	}

EXPORT_C void CPrintSetup::InternalizeL(RReadStream& aStream)
/** Internalises a CPrintSetup object from a read stream.

The presence of this function means that the standard templated operator>>() 
(defined in s32strm.h) is available to internalise objects of this class.

This function does not restore the rich text components of the header and 
footer, (e.g. fields, pictures and rich text-specific formatting), so it may 
need to be accompanied by a call to RestoreComponentsL().

@param aStream Stream from which the object should be internalised. */
	{
	iNumOfFirstPage = aStream.ReadInt32L();
	aStream>> iPageMarginsInTwips;
	// read in the new header and footer, setting them up as necessary
	delete iHeader;
	delete iFooter;
	iHeader = NULL;
	iFooter = NULL;
	iHeader = CHeaderFooter::NewL();
	iFooter = CHeaderFooter::NewL();
	aStream>> *iHeader;
	aStream>> *iFooter;
	iHeader->SetPageNumInfo(*this);
	iFooter->SetPageNumInfo(*this);
	// do the remainder of the internalize
	TUid modelUid;
	TPageSpec spec;
	aStream>> modelUid;
	aStream>> spec;
	// create device of correct type
	RFs fs;
	User::LeaveIfError(fs.Connect());
	TInt err;
	TRAP(err, CreatePrinterDeviceL(modelUid, fs));
	fs.Close();
	if(err == KErrNone)
		{
		if(iPrinterDriver)
			iPrinterDriver->PrinterDevice()->SelectPageSpecInTwips(spec);
		}
	else
		{
		User::Leave(err);
		}
	}


TInt CPrintSetup::UpdateFieldPageNum()const
	{
	if (iPrintProcess)
		return iPrintProcess->CurrentPageNum()+iNumOfFirstPage;
	else
		return 1;
	}

EXPORT_C CPrinterDevice* CPrintSetup::PrinterDevice()const
/** Gets the selected printer device.

@return The selected printer device.
@panic PRINT 1 No printer device has been selected. Use CreatePrinterDevice() 
to select one. */
	{
	__ASSERT_ALWAYS(iPrinterDriver,Panic(EPanicNoPrinterDriver));
	//
	return iPrinterDriver->PrinterDevice(); 
	}

EXPORT_C CPrinterDriverUI* CPrintSetup::CreatePrinterDriverUIL()
/** Creates a user interface for the target printer device, if the current printer 
driver has a matching .udl file.

Returns a NULL pointer if no matching .udl file was found. The UI can provide 
such things as custom print setup dialogs.

@return The UI for the selected printer device.
@panic PRINT 1 No printer device has been selected. Use CreatePrinterDevice() 
to select one. */
	{ 
	__ASSERT_ALWAYS(iPrinterDriver,Panic(EPanicNoPrinterDriver));
	//
	return iPrinterDriver->CreatePrinterDriverUIL(); 
	}
