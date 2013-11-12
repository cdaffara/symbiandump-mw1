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



#include <e32math.h>
#include <RtpHeader.h>

#include "CXPSPacketSink.h"

#include "CXPSPacket.h"



//
// Creates packet sink
//
// Returns NULL on memory error
// 
CXPSPacketSink* CXPSPacketSink::New()
{
    return NULL ;
}


//
//  Constructor
//
CXPSPacketSink::CXPSPacketSink() 
                    
{
   //do nothing
}

//
//  Destructor
//
CXPSPacketSink::~CXPSPacketSink()
{
    // do nothing
}



//
//  Creates the symbian server with the specified name and 
//  waits for client connection.
//
//  Parameters:
//		ServerName	: Name of the server to be created.
//		pObserver	: Observer for packet sink callbacks
//
//  Returns KErrNone on success or system wide error otherwise
// 
//
TInt CXPSPacketSink::Init(const TDesC& /*ServerName*/, MXPSPktSinkObserver* /*pObserver*/)
{
   
    return KErrNotSupported;
}


//
// Sets the SDP info into the server.
//
// Note: 
//		SetSessionDescription SHALL be invoked before the application
//		issues a open URL on the MMF controller. OpenURL on controller 
//		will result in a connect and query for SDP by the client
//
//		Init SHALL be called prior to this API invocation
//
// Parameters:
//		aSDPDesc	: Session description protocol message
//		unNumStreams: Number of streams
// 
// Returns KErrNone on success or system wide error otherwise
//
//
//
TInt CXPSPacketSink::SetSessionDescription(TDesC8& /*aSDPDesc*/, TUint /* uNumStreams*/)
{
    
    return KErrNotSupported;
}

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
//		SetSecureOutputMode SHALL be invoked before the application
//		issues a OpenURL on the MMF controller. 
//
//		Init SHALL be called prior to this API invocation
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
TInt CXPSPacketSink::SetSecureOutputMode(TUint32 /* ulSecureOutBitMask */)
{
    return KErrNotSupported;
}

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
//		aSDPDesc	: Session description protocol message
// 
// Returns KErrNone on success or system wide error otherwise
//
//
//
TInt CXPSPacketSink::AuthenticateClient(TSecurityPolicy& /* aSecurityPolicy*/)
{
    return KErrNotSupported;
}

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
TInt CXPSPacketSink::Enqueue(TUint /* unStreamNumber*/, CXPSPacket* /*pPacket*/)
{
    return KErrNotSupported;
}

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
// 
TInt CXPSPacketSink::Enqueue(TUint /*unStreamId*/, const TRtpRecvHeader& /*aHeaderInfo*/, 
                             const TDesC8& /*aPayloadData*/)
{
    return KErrNotSupported;
}

//
// StreamEnd indicates the end of given stream
//
// Paramaters:
//		unStreamId	: Stream identifier
//
// Returns KErrNone on success or system wide error otherwise
//
// 
TInt CXPSPacketSink::StreamEnd(TUint /*unStreamNumber*/)
{
   return KErrNotSupported;
}



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
TInt CXPSPacketSink::ConfigStream(TUint /*unStreamId*/, TUint /*unNumSlots*/)
{
   return KErrNotSupported;
}


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
TInt CXPSPacketSink::ConfigStreamBySize(TUint /*unStreamId*/, TUint32 /*unQueueSiezInBytes*/)
{
    return KErrNotSupported;
}


//
// Resets the packet sink. Releases all the packets stored in packet sink
// Does not affect the SDP or intialization attributes. It only cleans up the
// bufferred packets on the packet sink
//
// Returns KErrNone on success or system wide error otherwise
//
// 
TInt CXPSPacketSink::Reset()
{
    return KErrNotSupported;
}

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
TBool CXPSPacketSink::SetOverflowAutoManage(TBool /*bIsOverflowAutoManaged*/)
{
    
    return ETrue;
}


