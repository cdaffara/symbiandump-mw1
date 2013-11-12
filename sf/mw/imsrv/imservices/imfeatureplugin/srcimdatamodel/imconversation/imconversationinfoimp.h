/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MImConversationInfo API object implementation.
*
*/

#ifndef CIMCONVERSATIONINFOIMP_H
#define CIMCONVERSATIONINFOIMP_H


#include <s32strm.h>
#include <badesca.h>
#include <imconversationinfo.h>
#include "ximpapidataobjbase.h"
#include "imapiobjbase.h"


class CImConversationInfoImp;
class CXIMPIdentityImp;
/**
 * MImConversationInfo API object implementation.
 *
 * @lib Imdatamodel.dll
 *
 */
NONSHARABLE_CLASS( CImConversationInfoImp ): public CXIMPApiDataObjBase,public MImConversationInfo
    {
    public:
    /** The class ID. */
    enum { KClassId = IMIMP_CLSID_CIMCONVERSATIONINFOIMP };

public:
    IMPORT_C static CImConversationInfoImp* NewLC();
    IMPORT_C static CImConversationInfoImp* NewL();
    virtual ~CImConversationInfoImp();

private:

    CImConversationInfoImp();
    void ConstructL();
    
public: // From CXIMPApiDataObjBase

    /**
     * @see CXIMPApiDataObjBase
     */
    XIMPIMP_DECLARE_DATAOBJ_BASE_METHODS
    XIMPIMP_DECLARE_IF_BASE_METHODS
    
public: // From MImConversationInfo

	const MXIMPIdentity& MessageId() const;  	
	const TDesC16& TextMessage() const;
	const MDesCArray&  RecipientL() const;
/*	const TDesC16& TextMessageSubject() const;
	const TMessageTypes& MessageType() const;
	const TDesC16& ContentMessage() const;
	const TInt DeliveryReportResult() const;
	const TDesC16& DeliveryReportDescription() const;
	const MXIMPIdentity& ExtConvId() const;
	const TDesC16& ExtConvGroupId() const;
	const MXIMPIdentity& BlockedEntityId() const;
	const TDesC16& BlockedEntityDisplayName() const;
*/	
	
	void SetMessageIdL( MXIMPIdentity* aIdentity );
	void SetTextMessageL( const TDesC16& aMessage );
	void SetRecipientL(const MDesCArray* aRecipients);
	
	/*void SetMessageSubjectL( const TDesC16& aMessageSubject );	
	void SetMessageTypeL( const TDesC16& aMessageType );
	void SetContentMessageL( const TDesC16& aContMessage );
	void SetExtConvIdL( MXIMPIdentity* aExIdentity );
  	void SetDeliveryReportResultL( const TInt aResult );
  	void SetDeliveryReportDescriptionL( const TDesC16& aDescription );
	void SetExtConvGroupIdL( const TDesC16& aGroupId );
  	void SetBlockedEntityIdL( MXIMPIdentity* aBlkIdentity );
  	void SetBlockedEntityDisplayNameL( const TDesC16& aDisplayName );            
      */                          
public: // New functions

    /**
     * Internalizes object data from given stream.
     * @param aStream Stream to read.
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );

    /**
     * Set identity
     */
    IMPORT_C void SetIdentity( CXIMPIdentityImp* aIdentity );

    /**
     * TLinearOrder for ordering based on stored group id,
     * when this class is used within an RPointerArray or derivatives.
     */
    IMPORT_C static TInt IdLinearOrder(
            const CImConversationInfoImp& aA,
            const CImConversationInfoImp& aB );

    /**
     * Access to identity object
     * @return Identity
     */
    IMPORT_C const CXIMPIdentityImp& IdentityImp() const;

private: // data

    /**
     * Identity
     */
    CXIMPIdentityImp* iIdentity;

    /**
     * TextMessage
     */
    RBuf16 iTextMessage;
    
    /**
     * Receipient(s) of the Message
     */
    CDesCArray* iRecipients ;
    

    };


#endif // CIMCONVERSATIONINFOIMP_H
