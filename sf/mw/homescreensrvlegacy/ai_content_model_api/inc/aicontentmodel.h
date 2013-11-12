/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef M_AICONTENTMODEL_H
#define M_AICONTENTMODEL_H

#include <e32base.h>
#include <libc/stddef.h>  // For wchar_t

/**
 * Maximum length for a content item textual id.
 *
 * @see TAiContentItem::id
 */
const TInt KAiContentIdMaxLength = 255;

/**
 * Maximum length for content type.
 *
 * @see TAiContentItem::type
 */
const TInt KAiContentTypeMaxLength = 255;

/**
 * Null value for content id. Plug-in content model should never use this id
 * value in its content model.
 */
const TInt KAiNullContentId = KErrNotFound;

/**
 *  Encapsulates metadata of single plug-ins content item. The class is used
 *  as building block for the plug-ins content model, abstracts content
 *  selector, content reference and event.
 */
struct TAiContentItem
{
    /**
     * Plug-in specific id of this content item.
     */
    TInt id;

    /**
     * Textual id of this content item. Used for binding content items to
     * UI elements.
     *
     * @see KAiContentIdMaxLength
     */
    const wchar_t* cid;

    /**
     * Content item data MIME type. For example "text/plain" for textual 
     * content.
     *
     * @see KAiContentTypeMaxLength
     */
    const char* type;
};

/**
 * Helper function for accessing Content item textual id TAiContentItem::cid.
 *
 * This function's performance is relative to O(N) where N is the length of 
 * the textual id in characters. If the id of the same content item is accessed 
 * repeatedly (for example in a loop) store the returned id in a local TPtrC 
 * variable instead of calling this function repeatedly.
 *
 * @param aContentItem  content item whose textual id to return.
 * @return textual id of aContentItem as a descriptor.
 */
inline TPtrC16 ContentCid( const TAiContentItem& aContentItem )
    {
    return TPtrC16( (const TText16*) aContentItem.cid );
    }

/**
 * Helper function for accessing Content item data type TAiContentItem::type.
 *
 * This function's performance is relative to O(N) where N is the length of 
 * the type name in characters. If the type of the same content item is accessed 
 * repeatedly (for example in a loop) store the returned type in a local TPtrC8 
 * variable instead of calling this function repeatedly.
 *
 * @param aContentItem  content item whose type to return.
 * @return data type of aContentItem as a descriptor.
 */
inline TPtrC8 ContentType( const TAiContentItem& aContentItem )
    {
    return TPtrC8( (const TText8*) aContentItem.type );
    }


/**
 * Mime type for passing bitmap content data.
 * The content data should contain a packaged CGulIcon object pointer when this
 * MIME tyoe is used.
 *
 * @see CGulIcon
 * @see MAiContentObserver::PublishPtr
 */
const char KAiContentTypeBitmap[] = "image/x-s60-bitmap";

/**
 * MIME type for passing textual data.
 *
 * @see MAiContentObserver::Publish
 */
const char KAiContentTypeText[] = "text/plain";
 

/**
 *  Abstract interface which provides services to iterate content items
 *  supported by the plug-in. Only used by the Active Idle Framework.
 *  Each plug-in must provide implementation of interface to access:
 *  content selectors, content references, and events. Instances of interface
 *  are accessed through method GetProperty in interface CAiContentPublisher.
 *
 *  @since S60 3.2
 */
class MAiContentItemIterator
{
public:
    /**
     * Tests if this enumeration contains more elements.
     *
     * @return ETrue if this iterator object contains at least one more
     *         element to provide; EFalse otherwise.
     */
    virtual TBool HasNext() const = 0;

    /**
     * Returns the next element of this iterator if this enumeration object
     * has at least one more element to provide.
     *
     * @return The next element.
     * @leave KErrOverflow if iterator is at the end of range.
     */
    virtual const TAiContentItem& NextL() = 0;

    /**
     * Returns the first element of this iterator which matches aId.
     *
     * @param  aId - unique identification of the content item, corresponds
     *         to TAiContentItem::id.
     * @return The first element matching aId.
     * @leave KErrNotFound if element matching aId is not found from the
     *            complete iterator range.
     */
    virtual const TAiContentItem& ItemL( TInt aId ) const = 0;

    /**
     * Returns the first element of this iterator which matches aCid.
     *
     * @param  aCid - textual identification of the content item, corresponds
     *         to TAiContentItem::cid.
     * @return The first element matching aCid.
     * @leave KErrNotFound if element matching aCid is not found from the
     *            complete iterator range.
     */
    virtual const TAiContentItem& ItemL( const TDesC& aCid ) const = 0;

    /**
     * Resets iterator to the first item in the list.
     */
    virtual void Reset() = 0;

    /**
     * Release the iterator.
     */
    virtual void Release() = 0;
    
protected:
    /**
     * Protected destructor prevents deletion through this interface.
     */
    ~MAiContentItemIterator() { }

private:
    /**
     * Required to implement CleanupReleasePushL(MAiContentItemIterator*).
     */
    static void Cleanup(TAny* aSelf);
    friend void CleanupReleasePushL(MAiContentItemIterator*);
};

/**
 * Helper function which calls MAiContentItemIterator::Release() with NULL 
 * checking. Especially useful in destructors.
 */
inline void Release(MAiContentItemIterator* aObj)
    {
    if (aObj) aObj->Release();
    }

/**
 * 
 */
inline void CleanupReleasePushL(MAiContentItemIterator* aObj)
    {
    CleanupStack::PushL(
        TCleanupItem(&MAiContentItemIterator::Cleanup, aObj) );
    }

/**
 * Required to implement CleanupReleasePushL(MAiContentItemIterator*).
 * Inline to avoid problems with multiple definitions.
 */
inline void MAiContentItemIterator::Cleanup(TAny* aSelf)
    {
    ::Release(static_cast<MAiContentItemIterator*>(aSelf));
    }



#endif // M_AICONTENTMODEL_H
