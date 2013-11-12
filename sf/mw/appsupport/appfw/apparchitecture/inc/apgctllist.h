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

#ifndef __APGCTL_LIST_H__
#define __APGCTL_LIST_H__

#if !defined(__E32STD_H__)
#include <e32std.h>
#endif
#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

// classes defined:
class CApaSystemControlList;

//
// classes referenced:
class CApaSystemControl;
class RFs;
//

class CApaSystemControlList : public CBase
   /** Provides a list of all available control panel applications present on the phone. 
   This class is implemented as a linked list of CApaSystemControl. Updates the control panel application list by removing 
   control panels that no longer exist, adding new control panels found to the control panel application list and replacing 
   a control panel if found in an earlier drive.
   
   @see CApaSystemControl
   @publishedPartner
   @released
   */
   	{
   public:
   	IMPORT_C static CApaSystemControlList* NewL(RFs& aFs);
   	IMPORT_C ~CApaSystemControlList();
   	
   	IMPORT_C TInt Count()const;
   	IMPORT_C TInt Index(TUid aType)const;
   	IMPORT_C CApaSystemControl* Control(TInt aIndex)const;
   	IMPORT_C CApaSystemControl* Control(TUid aType)const;
   	
   	IMPORT_C void UpdateL();
   	inline TInt UpdateCount()const;
   private:
   	CApaSystemControlList(RFs& aFs);
   	CApaSystemControl* PreviousControl(const CApaSystemControl* aControl) const;
   private:
   	TInt iUpdateCount;
   	RFs& iFs;
   	CApaSystemControl* iControl;
   	};
   
   
   //
   // inlines
   //
   
   
   inline TInt CApaSystemControlList::UpdateCount()const
   /** Gets the number of times the control panel application list has been 
   updated (by calling UpdateL()). It returns 1 for a newly created list.
   
   @return The number of times the control panel application list has been changed. */
   	{ return iUpdateCount; }
   
#endif
