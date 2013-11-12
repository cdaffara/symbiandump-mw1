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



#ifndef _SYMBIAN_METADATA_UTIL_IMP_H_
#define _SYMBIAN_METADATA_UTIL_IMP_H_

// Symbian includes
#include <f32file.h> // RFile 

#include "hxmetadatakeys.h"




// CHXMetaDataUtility is the entry point for metadata engine.
// This hides all the implmentation details of the actual metadata engine
// including Helix specfic details like GlobalManager, Location of the dlls etc.

class CHXMetaDataUtility : public CBase
{
public:
         ~CHXMetaDataUtility();
        static CHXMetaDataUtility* NewL();

        // Opens the file aFileName for metadata reading.
        // descriptor aFileName can be deleted after the OpenFileL returns.
         void OpenFileL( const TDesC& aFileName );

        // Opens the descriptor for metadata reading.
        // descriptor is required to be present till ResetL is being called.
         void OpenDesL(const TDesC8& aDes);

        // Opens the descriptor for metadata reading.
        // File Handle can be closed after the OpenFileL returns.
         void OpenFileL(RFile& aFile);

        // Resets the utility. Must be called before OpenFileL() or 
        // OpenDesL() can be called again.
        // 
         void ResetL();

        // returns the count of metadata entries.
         TInt GetMetaDataCount(TUint &aCount);

        // returns the Metadata entry at index a Index. 
        // pDes is not owned by the caller.
         TInt GetMetaDataAt(TUint aIndex, 
                    HXMetaDataKeys::EHXMetaDataId &id, HBufC*& pDes);

private:
        CHXMetaDataUtility();
        CHXMetaDataUtility* ConstructL();

        // utility functions
        void* GetSymbol(RLibrary &rlib, const char *symbolName);
        void  ConvertMetaDataL();

private:
        typedef int (*GetSym2Ord)(const char* pSymbolName);   
        RLibrary                    m_rlib;
        
        TBool                       m_bLibLoaded;
        TBool                       m_bCloseNeeded;

};

#endif _SYMBIAN_METADATA_UTIL_IMP_H_

