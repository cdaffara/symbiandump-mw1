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
* Description:  Test support file tool server message
 *
*/


#ifndef CPRFWTESTFILESRVMSG_H
#define CPRFWTESTFILESRVMSG_H 

#include <e32base.h>
#include <s32strm.h>

/**
 * Support for servermessage files.
 *
 * @since S60 v4.0
 */
class CXIMPTestFileSrvMsg : public CBase
    {
    public:
    
        enum TTestSrvMsgType
            {
            ETestSrvMsgPresenceBlockList,
            ETestSrvMsgPresenceBlocked,
            ETestSrvMsgPresenceBlockCanceled,
            ETestSrvMsgPresenceGrantRequestList,
            ETestSrvMsgPresenceGrantRequestReceived,
            ETestSrvMsgPresenceGrantRequestObsoleted,
            ETestSrvMsgPresentityGroupList,
            ETestSrvMsgPresentityGroupCreate,
            ETestSrvMsgPresentityGroupDelete,
            ETestSrvMsgPresentityGroupUpdate,
            ETestSrvMsgPresentityGroupMemberList,
            ETestCheckIfGroupMemberExists,
            ETestSrvMsgPresenceWatcherList,
            ETestSrvMsgPresentityPresence,
            ETestSrvMsgReqParamIdentity,
            };
    
    public:

        IMPORT_C static CXIMPTestFileSrvMsg* NewLC();
        
        IMPORT_C static CXIMPTestFileSrvMsg* NewL( 
                TInt aSrvMsgId,
                const TDesC8& aPayload );
        
        virtual ~CXIMPTestFileSrvMsg();

    private:

        CXIMPTestFileSrvMsg();

        void ConstructL();
        
        CXIMPTestFileSrvMsg( TInt aSrvMsgId );
        
        void ConstructL( const TDesC8& aPayload );

    public:

        /**
         * Extenalizes object data to given stream.
         * @param aStream Stream to write.
         */
        IMPORT_C void ExternalizeL( RWriteStream& aStream ) const;

        /**
         * Internalizes object data from given stream.
         * @param aStream Stream to read.
         */
        IMPORT_C void InternalizeL( RReadStream& aStream );

        /**
         * Getter for the data stored inside this class.
         * @return Reference to the data
         */
        IMPORT_C TDesC8& PayloadL();

        /**
         * Server message identifier specifies what kind of
         * server-originated message this is.
         * @return Server message identifier
         */
        IMPORT_C TInt SrvMsgId();

    private: // data

        TInt iSrvMsgId;
        HBufC8* iPayload;
    };

#endif // CPRFWTESTFILESRVMSG_H 
