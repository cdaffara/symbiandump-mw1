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

#if !defined(__PRNSTD_H__)
#define __PRNSTD_H__

#if !defined(__E32STD_H__)
#include /**/ <e32std.h>
#endif

enum TPrintPanic
	{
	EPanicNoPrintObserver,
	EPanicNoPrinterDriver,
	EPanicNoPrinterPort,
	EPanicNoPrinterDirs,
	EPanicNoBodyPrinter,
	EPanicNoPreviewPageXyPos,
	EPanicNoModelList,
	EPanicInvalidPageNumbers,
	EPanicInvalidNumCopies,
	EPanicProcessHandleNull,
	EPanicPrintablePageTooSmall,
	EPanicBandRectTooSmall,
	EPanicPrintProcessOOM,
	EPanicNegativeIndex,
	EPanicNonPositivePageNumber,
	EPanicStartCalledTwice,
	//
	EDPanicInconsistentBandNum,
	EDPanicNoPrinterDrivers
	};
	//EPanicInvalidPrintablePageHeight,


GLREF_C void Panic(TPrintPanic aPanic);
GLREF_C const TDesC& Category();


#endif
