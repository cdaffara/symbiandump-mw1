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

#ifndef __PRNIMP_H__
#define __PRNIMP_H__

#if !defined(__E32STD_H__)
#include /**/ <e32std.h> 
#endif
#if !defined(__E32BASE_H__)
#include /**/ <e32base.h> 
#endif
#if !defined(__PRNINF_H__)
#include <prninf.h>
#endif

// Classes defined
class CHeaderFooterPrinter;
class CPreviewProcess;
class CPrintProcess;		
//

// classes referenced
class CHeaderFooter;
class CTextLayout;
class CTextDraw;
//


class CHeaderFooterPrinter : public MPageRegionPrinter
	{
public:
	static CHeaderFooterPrinter* NewL(CHeaderFooter& aHeaderFooter,TRect aHeaderFooterRect);
	virtual ~CHeaderFooterPrinter();
	// from MPageRegionPrinter
	void PrintBandL(CGraphicsDevice* aDevice,TInt aPageNo,const TBandAttributes& aBandInPixels);
protected:
	CHeaderFooterPrinter(CHeaderFooter& aHeaderFooter,TRect aHeaderFooterRect);
	void ConstructL();
private:
	void DrawTextL(CGraphicsDevice* aDevice);
	void SetBandRect();
private:
	CHeaderFooter* iHeaderFooter;
	TRect iHdFtRect; // bounding rect of the header/footer area relative to the page (in twips)
	CTextLayout* iTextLayout;
	CTextDraw* iTextDraw;
	CGraphicsContext* iGc; // gc for drawing
	};


class CPreviewProcess : public CActive
	{
public:
	enum {EPriority=-75};
	// 2 phase ctor: automatically adds self to active scheduler
	static CPreviewProcess* NewL(TInt aPriority=EPriority);
	virtual ~CPreviewProcess();
	//
	void SetObserver(MPrintProcessObserver* anObserver);
	void SetHeaderFooter(CHeaderFooterPrinter* aHeaderPrinter,CHeaderFooterPrinter* aFooterPrinter);
	// Called to initiate previewing of a document
	void StartPreview(const TPrintParameters& aPrintParams,CGraphicsDevice& aPrinterDev,
				 MPageRegionPrinter& aBodyPrinter,const TPageSpec& aPageSpec,TInt aNumBands,void** aRef);
	void TerminatePrint(TInt aErrNum);
	//
	// function supporting the MFieldPageNumInfo interface of CPrintSetup
	TInt CurrentPageNum()const;
protected:
	CPreviewProcess(TInt aPriority);
	virtual void RunL(); // Active scheduler is friend - can access
	virtual void DoCancel();
	virtual TBool IncrementPrintState();
	virtual void PrintBandL(CGraphicsDevice* aDev); // Called by PrintBand to carry out actual printing
	virtual void InformObserver(); // Inform observer of current printing status (after each band)
//	virtual void HandlePrinterError(TInt anErrorCode);
	virtual void EndProcess(TInt aErrNum=KErrNone);
	void Init(const TPrintParameters& aPrintParams,MPageRegionPrinter& aBodyPrinter,void** aRef);
	void SetRequestStatus();
private:
	void SetBandRect();
	const CPreviewProcess& operator=(const CPreviewProcess& aProcess);
	//
protected:
	CHeaderFooterPrinter* iHeaderPrinter;
	CHeaderFooterPrinter* iFooterPrinter;
	MPrintProcessObserver* iObserver;
	MPageRegionPrinter* iBodyPrinter; // prints each page region
	void** iOwnerReferencePtr; // Pointer to PrinterSetup's CPrintProcess* - set to NULL on destruction
	TPrintParameters iPrintParams;
	TInt iCurrentPageNum; // page currently being processed
	TInt iCurrentBandNum;
	TBandAttributes iBandAttribs; // size & pos of band
private:
	enum { EEndOfProcessAlreadyActivated=0x01 };
private:
	TPageSpec iPageSpec;
	CGraphicsDevice* iPreviewDev;
	TInt iNumBands;
	TInt iFlags;

	__DECLARE_TEST;
	};


class CPrintProcess : public CPreviewProcess
	{
public:
	// 2 phase ctor: automatically adds self to active scheduler
	static CPrintProcess* NewL(TInt aPriority=EPriority);
	virtual ~CPrintProcess();
	// Called to initiate printing of a document
	void StartPrintL(const TPrintParameters& aPrintParams,CPrinterDevice& aPrinterDev,
		CPrinterDriverUI* aPrinterUi,MPageRegionPrinter& aBodyPrinter,CPrinterPort* aPort,void** aRef);
protected:
	CPrintProcess(TInt aPriority);
	virtual TBool IncrementPrintState();
	virtual void InformObserver();
	virtual void PrintBandL(CGraphicsDevice* aDev);
	virtual void DoCancel();
	virtual void EndProcess(TInt aErrNum=KErrNone);
private:
	const CPreviewProcess& operator=(const CPreviewProcess& aProcess);
	//
private:
	enum TPrintProcessStatus { EStartingBand, EPrintingBand, EEndPage, EEndPrint };
	TPrintProcessStatus iPrintStatus; // current position of PrintProcess in the print cycle
	CPrinterControl::TMoreOnPage iMoreOnPage;
	TInt iCurrentCopyNum; // copy currently being processed
	TInt iAreaPrinted; // area of the current page printed so far
	CPrinterDevice* iPrinterDev;
	CPrinterDriverUI* iPrinterUi;

	__DECLARE_TEST;
	};

#endif
