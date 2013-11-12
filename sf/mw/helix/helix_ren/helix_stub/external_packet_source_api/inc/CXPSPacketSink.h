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





#ifndef __CXPSPACKETSUPPLIER_H__
#define __CXPSPACKETSUPPLIER_H__


#include <e32base.h>
#include "CXPSPacket.h"
#include "CXPSPktSinkObserver.h"





//
//	Class CXPSPacketSink
//	eXternal Packet Sink provides the functionality to rendering RTP media data 
//	along with the MMF controller. MMF Controller is used for control signals and
//  packet sink is used for RTP media data communication.
//
//  CXPSPacketSink provides APIs for media description and for sending packets
//	to the playback engine.
//
//
class CXPSPacketSink : public CBase
{
    
public:
    
    //
    // Creates packet sink
    //
    // Returns NULL on memory error
    // 
    static CXPSPacketSink* New();
    
    //
    // Virtual destructor
    //
    virtual ~CXPSPacketSink();
    
    //
    // Creates the symbian server with the specified name and 
    // waits for client connection.
    //
    // Parameters:
    //		ServerName	: Name of the server to be created.
    //		pObserver	: Observer for packet sink callbacks
    //
    // Returns KErrNone on success or system wide error otherwise
    // 
    //
    TInt Init(const TDesC& ServerName, MXPSPktSinkObserver* pObserver);
    
    //
    // AuthenticateClient sets the minimum capability that a client SHALL 
    // possess for connecting to the server.
    //
    // Note: 
    //		AuthenticateClient SHALL be invoked before the application
    //		issues a open URL on the MMF controller. OpenURL on controller 
    //		will result in a connect.
    //
    //		Init SHALL be called prior to this API invocation
    //
    // Parameters:
    //		aSecurityPolicy	: Security policy
    // 
    // Returns KErrNone on success or system wide error otherwise
    //
    //
    //
    TInt AuthenticateClient(TSecurityPolicy& aSecurityPolicy);
    
    //
    // Sets the SDP info into the server.
    // 
    // Optional xps related sdp attributes
    //      Session Level Attribute
    //          XpsStreamSyncOffset: Inter stream synchronization offset value 
    //                               in msec. This makes sure that XPS packet 
    //                               reading between streams can be offset only
    //                               by specified value. Default: 1000 msec
    //
    //		DisableVideoClock: Flag to disable clock source for video playback.
    // 	
    //      Media Level Attribute
    //          XpsHdrBufSize:  XPS transport Header buffer size between XPS server
    //                          & XPS client. Default: 512 bytes
    //          XpsPyldBufSize: XPS transport payload buffer size between XPS server
    //                          & XPS client. Default: 4096 bytes
    //
    //
    // Note: 
    //		SetSessionDescription SHALL be invoked before the application
    //		issues a open URL on the MMF controller. OpenURL on controller 
    //		will result in a connect and query for SDP by the client
    //
    //		Init SHALL be called prior to this API invocation
    //    
    //    The stream number is based on the order of the streams (m=mediatype) in the
    //    SDP file. Currently only m=audio and m=video would be valid streams and id 
    //    would be assigned in increasing order starting with zero.
    //
    // Parameters:
    //		aSDPDesc	: Session description protocol message
    //		unNumStreams: Number of streams
    // 
    // Returns KErrNone on success or system wide error otherwise
    //
    //
    //
    TInt SetSessionDescription(TDesC8& aSDPDesc, TUint unNumStreams);
    
    //
    // Sets the session's output device mode. 
    // Audio, Video or other streams will use the bit mask to make a decision
    // on routing to external devices (eg., Bluetooth, TV-Out,etc)
    //
    // Note: 
    //      SetSecureOutputMode is an optional API and SHALL be invoked while
    //      playing protected content. Default behaviour is allow all ie., 
    //      no restrictions applied in routing streams to output devices.
    //
    //		Init SHALL be called prior to this API invocation
    // 
    //      SetSecureOutputMode can be called anytime after Init. If it is called
    //      after playback started then the Secure output settings will take effect 
    //      at the presentation time of the first packet enqueued after SetSecureOutputMode
    //      is called.
    //
    // Parameters:
    //		ulSecureOutBitMask	: 32-Bit mask representing the secure output mode
    //                            All bits zero will result in BLOCKING of unsecure 
    //                            output devices. (0x0) (eg. TV-Out will be blocked)
    //                            All bits one will result in allowing streams to 
    //                            render to both secure and unsecure output devices. 
    //                            (0xFFFFFFFF) Rest of the bit mask combinations 
    //                            are reserved for future use.
    // 
    // Returns KErrNone on success or system wide error otherwise
    //
    //
    //
    TInt SetSecureOutputMode(TUint32 ulSecureOutBitMask);
    
    
    //
    // Enqueues the packet on the packet sink. 
    // 
    // Parameters:
    //		unStreamId	: Stream identifier
    //		CXPSPacket	: transferred packet, XPS takes over ownership
    //
    // Returns KErrNone on success or system wide error otherwise. 
    //			Packet overflow scenario will result in KErrOverflow.
    //		    Application SHALL stop supplying more packets on receiving KErrOverflow. 
    //			Packet supply can be resumed after the callback from the packet sink.
    //          MXPSPacketSinkObserver::ResumePacketSupply API will be used to notify. 
    //
    //  NOTE:
    //      KErrOverflow return indicates that the slots are filled out. Current packet 
    //      has been enqueued and there are no slots for more packets. Packet supply 
    //      SHALL be resumed after observer callback (MXPSPktSinkObserver::RestorePacketSupply).
    //      KErrNotReady will be returned if packets are enqueued after overflow and before
    //      resume callback
    //			
    // 
    TInt Enqueue(TUint unStreamNumber, CXPSPacket* pPacket);

    //
    // Enqueues the packet on the packet sink. 
    // 
    // Parameters:
    //		unStreamId	: Stream identifier
    //		aHeaderInfo	: RTP header of the packet
    //		aPayloadData: RTP payload
    //
    // Returns KErrNone on success or system wide error otherwise. 
    //			Packet overflow scenario will result in KErrOverflow.
    //		    Application SHALL stop supplying more packets on receiving KErrOverflow. 
    //			Packet supply can be resumed after the callback from the packet sink.
    //          MXPSPacketSinkObserver::ResumePacketSupply API will be used to notify. 
    //
    //  NOTE:
    //      KErrOverflow return indicates that the slots are filled out. Current packet 
    //      has been enqueued and there are no slots for more packets. Packet supply 
    //      SHALL be resumed after observer callback (MXPSPktSinkObserver::RestorePacketSupply).
    //      KErrNotReady will be returned if packets are enqueued after overflow and before
    //      resume callback
    //			
    // 
    TInt Enqueue(TUint unStreamId, const TRtpRecvHeader& aHeaderInfo, const TDesC8& aPayloadData);
    
    //
    // StreamEnd indicates the end of given stream
    //
    // Paramaters:
    //		unStreamId	: Stream identifier
    //
    // Returns KErrNone on success or system wide error otherwise
    //
    // 
    TInt StreamEnd(TUint unStreamId);
    
    //
    // Configures the stream's buffering capability. Sets the 
    // queue to the size specified.
    //
    // Note:
    //		This API is optional. Packet sink will start with default values
    //
    // Paramaters:
    //		unStreamId	: Stream identifier
    //		unNumSlots	: Maximum number of packets that may be bufferred in
    //					  packet sink for the given stream
    //
    // Returns KErrNone on success or system wide error otherwise
    //
    // Warning: It is strongly recommended to use ConfigStreamBySize()
    // instead of ConfigStream()
    //
    TInt ConfigStream(TUint unStreamId, TUint unNumSlots);
    
    //
    // Configures the stream's buffering capability. Sets the 
    // queue to the heap size specified.
    //
    // Note:
    //		This API is optional. Packet sink will start with default values
    //      This API could be called during runtime to re-adjust the queue size
    //
    // Paramaters:
    //		unStreamId	        : Stream identifier
    //		unQueueSiezInBytes	: Maximum number of heap size that may be 
    //                    bufferred in packet sink for the given stream
    //
    // Returns KErrNone on success or system wide error otherwise
    //
    TInt ConfigStreamBySize(TUint unStreamId, TUint32 unQueueSiezInBytes);

    //
    // Resets the packet sink. Releases all the packets stored in packet sink
    // Does not affect the SDP or intialization attributes. It only cleans up the
    // bufferred packets on the packet sink
    //
    // Returns KErrNone on success or system wide error otherwise
    //
    // 
    TInt Reset();

    //
    // SetOverflowAutoManage instructs XPS whether to do buffer overflow control or not
    //
    // Paramaters:
    //		bIsOverflowAutoManaged: 
    //        ETrue: if XPS has no packet for one stream, and Helix is demanding 
    //               for it, XPS will drop 1 packet from any stream whose queue
    //               is full. It allows XPS client to push further packets into XPS 
    //               in the hope that required stream packet could come to XPS 
    //               eventually. This process repeats till XPS receives deired packet.
    //        EFalse:XPS dose not do buffer management described above
    //
    //      Note: If XPS client has single queue implementation, it is better to
    //            let XPS handle the overflow situation.
    //      
    //      Default: ETrue
    //
    // Returns: the previous status of Overflow Management
    //
    // 
    TBool SetOverflowAutoManage(TBool bIsOverflowAutoManaged);
    
private:
    // private member functions
    
    CXPSPacketSink();
    
    
    
private:
    // private data members
    
    
    
};

#endif // End of __CXPSPACKETSUPPLIER_H__

