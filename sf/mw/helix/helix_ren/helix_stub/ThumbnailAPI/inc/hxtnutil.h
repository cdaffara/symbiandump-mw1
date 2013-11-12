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



#ifndef _SYMBIAN_TE_UTIL_H_
#define _SYMBIAN_TE_UTIL_H_

// Symbian includes
#include <f32file.h> // RFile 

#include "hxmetadatakeys.h"
#include "hxtnutil_obs.h"




// CHXThumbnailUtility is the entry point for TE engine.
// It enables access to Helix Thumbnail engine for reading
// the decoded YUV frames for a given file.

class CHXThumbnailUtility : public CBase         
{
public:
    ~CHXThumbnailUtility();
    
     /**
     * Constructs a new CHXThumbnailUtility object. This method
     * may leave if no resources are available to construct 
     * a new object.    
     * 
     * Possible leave codes:
     *  - <code>KErrNoMemory</code> if memory allocation fails
     *     
     * @param aObserver  observer to notify when info is ready for reading
     *
     * @return  pointer to a new CHXThumbnailUtility instance
     */

    static CHXThumbnailUtility* NewL(MHXThumbnailUtilityImplObserver& aObserver);
                                                                  
    /**
     * OpenFileL reads data from aFileHandle to get information
     * about the specified video clip file.
     * Possible leave codes:
     *  - <code>KErrNoMemory</code> if memory allocation fails
     *  - <code>KErrNotSupported</code> if can't support this clip for thumbnail.   
     *  - <code>KErrNotFound</code> if can't find a good thumbnail.   
     *
     * @param aFileHandle  File handle of file to get the thumbnail.
     * @param uStartPositingMS  Starting position in clip time from which to 
                                read the thumbnail.     
     */

    void    OpenFileL(const RFile& aFileHandle, TUint uStartPositingMS = 0);

    
    /**
     * Cancels frame generation. If no frame is currently being 
     * generated, the function does nothing. Can be called multiple times
     * without any side effects.
     */
    
    void CancelThumb();
    

    /** returns the count of metadata entries.
      * @param aCount  Number of metadata entries returned.
      * @return  System wide error code if there is any error.
     */
    
    TInt GetMetaDataCount(TUint &aCount);

    /** returns the metadata entry at the given index
      * @param aIndex  aIndex of the metadata entry.
      * @param id returned id or key of the metadata entry.
      * @param pDes returned value of the metadata entry. It is not 
                    owned by the caller.
      * @return  System wide error code if there is any error.             
     */
        
    TInt GetMetaDataAt(TUint aIndex, 
            HXMetaDataKeys::EHXMetaDataId &id, HBufC*& pDes);

protected:
    CHXThumbnailUtility();

private:
    void    ConstructL(MHXThumbnailUtilityImplObserver& aObserver);    
    
         
};


#endif _SYMBIAN_TE_UTIL_H_

