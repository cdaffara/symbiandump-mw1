/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/


// symbian includes
#include <E32Base.h>
#include <e32std.h>

#include "hxtnutil.h"


CHXThumbnailUtility::CHXThumbnailUtility()    
{
// do nothing

}

CHXThumbnailUtility::~CHXThumbnailUtility()
{
    //do nothing   
}

CHXThumbnailUtility* CHXThumbnailUtility::NewL(MHXThumbnailUtilityImplObserver& /*aObserver*/)
{
return NULL;  
}

void CHXThumbnailUtility::ConstructL(MHXThumbnailUtilityImplObserver& /*aObserver*/)
{
     User::Leave(KErrNotSupported);
}

void CHXThumbnailUtility::OpenFileL(const RFile& /*aFileHandle*/, TUint /*uStartPositonMS*/)
{
  User::Leave(KErrNotSupported);
}

void CHXThumbnailUtility::CancelThumb()
{
   //do nothing   
}

TInt CHXThumbnailUtility::GetMetaDataCount(TUint& /*aCount*/)
{
    return 0;              
}


TInt CHXThumbnailUtility::GetMetaDataAt(TUint /*aIndex*/,
             HXMetaDataKeys::EHXMetaDataId& /*id*/, HBufC*& /*pDes*/)
{
  return KErrNotSupported ;
}


