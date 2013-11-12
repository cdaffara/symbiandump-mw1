/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Contains CNcdNodeScreenshot class
*
*/


#ifndef NCD_NODE_SCREENSHOT_H
#define NCD_NODE_SCREENSHOT_H


// For streams
#include <s32mem.h>

#include "ncdcommunicable.h"
#include "ncdstoragedataitem.h"
#include "ncdnodeclassids.h"
#include "ncdutils.h"

class CNcdNodeManager;
class CNcdNodeMetaData;
class MNcdPreminetProtocolDataEntity;
class MNcdPreminetProtocolDownload;


/**
 *  This server side class contains the data and the functionality
 *  that the proxy objects will use to internalize itself.
 *
 *  This object should be added to the session. So, it will be usable
 *  in the proxy side by using the handle gotten during addition. 
 *  The handle is used to identify to what object the proxy directs 
 *  the function call.  When objects are added to sessions, 
 *  multiple handles may be gotten for the same object if addition is 
 *  done multiple times.
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
class CNcdNodeScreenshot : public CNcdCommunicable,
                           public MNcdStorageDataItem
    {

public:
    /**
     * NewL
     *
     * @return CNcdNodeScreenshot* Pointer to the created object 
     * of this class.
     */
    static CNcdNodeScreenshot* NewL(
        CNcdNodeManager& aNodeManager,
        const CNcdNodeMetaData& aParentMetaData );

    /**
     * NewLC
     *
     * @return CNcdNodeScreenshot* Pointer to the created object 
     * of this class.
     */
    static CNcdNodeScreenshot* NewLC(
        CNcdNodeManager& aNodeManager,
        const CNcdNodeMetaData& aParentMetaData );


    /**
     * Destructor
     *
     * @note Because this is CCatalogsCommunicable function the
     * session that owns this object should delete this class object.
     * So, instead of directly deleting this object from some other
     * class. Close-method should be used instead.
     */
    virtual ~CNcdNodeScreenshot();


    /**
     * Retrieves the data type that informs what class the data is for.
     * By checking the data type information, an InternalizeL function
     * of a right class can be called when the object data is set
     * from the storage.
     * The data type may be decided and set in a object that creates this 
     * class object.
     *
     * @return NcdNodeClassIds::TNcdNodeClassId Describes the data type. 
     */
    NcdNodeClassIds::TNcdNodeClassId ClassId() const;


    /**
     * Count of screenshot downloads. Use this to determine how many
     * screenshot downloads it is possible to get from ScreenshotDownload()
     * function.
     *
     * @return Screenshot download count.
     */
    TInt ScreenshotDownloadCount() const;


    /**
     * Get specific screenshot download URI
     *
     * @param aIndex Index of the screenshot download.
     * @return Download URI
     */
    const TDesC& ScreenshotDownloadUri( TInt aIndex ) const;


    /**
     * This function is called when the owner of this object
     * wants to internalize the content according to the data
     * that has been received from the parser.
     * 
     * @param aData The data is set in the protocol parser and can
     * be used to initialize this class object.
     */
    void InternalizeL( MNcdPreminetProtocolDataEntity& aData );
   
    
public: // MNcdStorageDataItem 

    // These functions are used to get the data from and to insert the data
    // into the database using by the given stream.

    /**
     * @see MNcdStorageDataItem::ExternalizeL
     */
    virtual void ExternalizeL( RWriteStream& aStream );


    /**
     * @see MNcdStorageDataItem::InternalizeL
     */
    virtual void InternalizeL( RReadStream& aStream );


public: // CCatalogsCommunicable

    /**
     * @see CCatalogsCommunicable::ReceiveMessage
     */
    virtual void ReceiveMessage( MCatalogsBaseMessage* aMessage,
                                 TInt aFunctionNumber );

    /**
     * @see CCatalogsCommunicable::CounterPartLost
     */
    virtual void CounterPartLost( const MCatalogsSession& aSession );


protected:

    /**
     * Constructor
     * @param aClassId Identifies this class. 
     * Is set in the NewLC function 
     */
    CNcdNodeScreenshot(
        NcdNodeClassIds::TNcdNodeClassId aClassId,
        CNcdNodeManager& aNodeManager,
        const CNcdNodeMetaData& aParentMetaData );

    /**
     * ConstructL
     */
    virtual void ConstructL();
    

    // These functions are called from the ReceiveMessage when
    // the given function id has matched to the function.

    /**
     * This function is called when the proxy wants to get the
     * data from the serverside. This function calls the
     * InternalizeDataForRequestL which may be overloaded in the
     * child classes
     * @param aMessage Contains data from the proxy and can be used
     * to send data back to proxy
     */
    void InternalizeRequestL( MCatalogsBaseMessage& aMessage );
        
    /**
     * This function writes the object data to the stream. 
     * The stream content will be sent to the proxy that requested the data.
     * Child classes should add their own data after this parent data.
     * @param aStream The data content of this class object will be written
     * into this stream.
     */
    virtual void ExternalizeDataForRequestL( RWriteStream& aStream );

    /**
     * This function is called when the proxy wants to get the
     * screenshot data from the serverside.
     * @param aMessage Contains data from the proxy and can be used
     * to send data back to proxy.
     */
    void ScreenshotDataRequestL( MCatalogsBaseMessage& aMessage );
        
    /**
     * This function is called from the proxy side. When the proxy
     * is deleted.
     * @param aMessage Contains data from the proxy and can be used
     * to send data back to proxy
     */
    void ReleaseRequest( MCatalogsBaseMessage& aMessage ) const;


private:

    // Prevent these two if they are not implemented
    CNcdNodeScreenshot( const CNcdNodeScreenshot& aObject );
    CNcdNodeScreenshot& operator =( const CNcdNodeScreenshot& aObject );


private: // data
    
    // The class id identifies this class. The id may be used to identify
    // what kind of class object is created when data is gotten from the db.
    NcdNodeClassIds::TNcdNodeClassId iClassId;

    // Node manager.
    CNcdNodeManager& iNodeManager;
    
    // Parent meta data.
    const CNcdNodeMetaData& iParentMetaData;


    // The message is set when ReceiveMessage is called. The message
    // is used in the CounterPartLost-function that informs the message
    // if the session has been lost.
    MCatalogsBaseMessage* iMessage;

    // Key = URI, Value = Mime type
    RPointerArray<CNcdKeyValuePair> iScreenshotDownload;
    };
    
#endif // NCD_NODE_SCREENSHOT_H
