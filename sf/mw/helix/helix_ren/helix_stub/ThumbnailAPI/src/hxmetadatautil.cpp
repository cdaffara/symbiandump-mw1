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

// MetaData includes

#include "hxmetadatautil.h"
#include "hxmetadatakeys.h"



CHXMetaDataUtility::CHXMetaDataUtility()
   
{

//do nothing
}

CHXMetaDataUtility::~CHXMetaDataUtility()
{
    //do nothing

}

CHXMetaDataUtility* CHXMetaDataUtility::NewL()
{
    return NULL;
}

CHXMetaDataUtility* CHXMetaDataUtility::ConstructL()
{
    return NULL;
}


void CHXMetaDataUtility::OpenFileL( const TDesC& /*aFileName*/ )
{
    User::Leave(KErrNotSupported);
   
}

void CHXMetaDataUtility::OpenFileL(RFile& /*aFile*/)
{
     User::Leave(KErrNotSupported);
}

void CHXMetaDataUtility::OpenDesL(const TDesC8& /*aDes*/)
{
   
    User::Leave(KErrNotSupported);

}

void CHXMetaDataUtility::ResetL()
{
   User::Leave(KErrNotSupported);
}


TInt CHXMetaDataUtility::GetMetaDataCount(TUint& /*aCount*/)
{
     
   return 0;
}



TInt CHXMetaDataUtility::GetMetaDataAt(TUint /*aIndex*/,
             HXMetaDataKeys::EHXMetaDataId& /*id*/, HBufC*& /*pDes*/)
{
   
   
   return KErrNotSupported;
    
}


// utility functions

void* CHXMetaDataUtility::GetSymbol(RLibrary& /* rlib */, const char* /* symbolName*/)
{
  
 return NULL;   
}

