/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Writes RTP payload format containing ILBC data.
*
*/



#ifndef ILBCPAYLOADFORMATWRITE_H
#define ILBCPAYLOADFORMATWRITE_H

//  INCLUDES
#include <e32base.h>
#include <mmf/server/mmfformat.h>
#include <mmf/server/mmfdatabuffer.h>
#include "rtpheader.h"
#include "formatstatemachine.h"
#include "mccinternalcodecs.h"

// FORWARD DECLARATIONS
class MMccRtpDataSink;
class CMccRtpMediaClock;

// CLASS DECLARATION
/**
*  Packetizes RTP payload containing ILBC data.
*
*  @lib Mccilbcplformat.dll
*  @since Series 60 3.0
*/
class CIlbcPayloadFormatWrite : public CPayloadFormatWrite, 
                                public MAsyncEventHandler, 
                                public MPayloadFormatWrite
    {
    public:     // Constructors and destructor
        /**
        * Two-phased constructor.
        * @param aSource    Data source configuration parameters
        */
        static CIlbcPayloadFormatWrite* NewL( MDataSink* aSink );

        /**
        * Destructor.
        */
        virtual ~CIlbcPayloadFormatWrite();

    public: // New functions

        /**
        * Configures payload format with format spesific parameters.
        * Leaves with KErrArgument if parameters are invalid.
        *
        * @since    Series 60 3.0 
        * @param    aConfigParams       Configuration parameters
        * @return   void
        */
        virtual void ConfigurePayloadFormatL( const TDesC8& aConfigParams,
                                              CMccRtpMediaClock& aClock );
    
    public: // From MPayloadFormatWrite

        /**
        * Empty the source buffer by formatting the iLBC frames into RTP payload.
        * Source buffer is given in "iBuffer".
        * Called by the state machine.
        *
        * @since    Series 60 3.0
        * @param    None
        * @return   void
        */
        void EmptySourceBufferL();

        /**
        * Handle the event that source buffer has been emptied.
        * Source buffer is given in "iBuffer".
        * Called by the state machine.
        *
        * @since    Series 60 3.0
        * @param    None
        * @return   void
        */
        void SourceBufferEmptiedL();

    public: // From CMMFFormatEncode

        /**
        * Funtion to return time interval for the complete frame
        *
        * @param aMediaType KUidMediaTypeAudio or KUidMediaTypeVideo
        *
        * @return           time interval in micro seconds
        */
        TTimeIntervalMicroSeconds FrameTimeInterval( TMediaId aMediaType ) const;
                    
        /**   *** NOT SUPPORTED ***
        * Function to return the clip duration
        *
        * @param aMediaType KUidMediaTypeAudio or KUidMediaTypeVideo
        *
        * @return           clip duration in micro seconds
        */
        TTimeIntervalMicroSeconds Duration( TMediaId aMediaType ) const;

        /**
        * Create a sink buffer for the given media
        *
        * @param aMediaId   contains the media type KUidMediaTypeAudio or 
        *                   KUidMediaTypeVideo 
        * @param aReference value of False is returned to caller if sink 
        *                   buffer is created
        * @return           Pointer to sink buffer created
        */
        CMMFBuffer* CreateSinkBufferL( TMediaId aMediaId, 
                                       TBool& aReference );

        /**
        * Return the sink four CC code for the given media
        *
        * @param aMediaId    contains the media type KUidMediaTypeAudio or
        *                    KUidMediaTypeVideo
        * @return            FourCC code for the mediaId
        */
        TFourCC SinkDataTypeCode( TMediaId aMediaId );

        /**
        * Set the sink data type to the given four CC code for the given media
        *
        * @param aSourceFourCC    fourCC code
        * @param aMediaId         contains the media type KUidMediaTypeAudio or
        *                         KUidMediaTypeVideo
        * @return                 error code KErrNotSupported if invalid media 
        *                         ID, else return KErrNone
        */
        TInt SetSinkDataTypeCode( TFourCC aSinkFourCC, 
                                  TMediaId aMediaId );

        /**
        * Log in to the sink thread - this funtion merely passes the command to 
        * its sink clip data source object. The sink clip object will handle 
        * the thread log on procedures.
        *
        * @param aEventHandler    address of event handler
        *
        * @return                 error code returned by source clip
        */
        TInt SinkThreadLogon( MAsyncEventHandler& aEventHandler );

        /**
        * Log out of the sink thread - this funtion merely passes the command to 
        * its sink clip data source object. The sink clip object will handle 
        * the thread log off procedures.
        *
        * @param
        *
        * @return
        */
        void SinkThreadLogoff();

        /**
        * Empty the given source buffer.
        *
        * @param aBuffer    data buffer containing G.711 frames
        * @param aSupplier  data source pointer
        * @param aMediaId   contains the media type KUidMediaTypeAudio or 
        *                   KUidMediaTypeVideo
        * @return
        */
        void EmptyBufferL( CMMFBuffer* aBuffer, 
                           MDataSource* aSupplier, 
                           TMediaId aMediaId );
        
        /**
        * Called after the data buffer is written. Update the number of bytes 
        * written and the current write position for the next write operation. 
        *
        * @param aBuffer       data buffer emptied
        *
        * @return
        */
        void BufferEmptiedL( CMMFBuffer* aBuffer );

        /**
        * Accessor funtion to return the number of channels
        *
        * @param    
        *
        * @return  Number of channels
        */
        TUint NumChannels();

        /**
        * Accessor funtion to return the sampling rate
        *
        * @param    
        *
        * @return Sampling rate
        */
        TUint SampleRate();

        /**
        * Set the samplerate to the given value.
        *
        * @param aSampleRate
        *
        * @return error code KErrNotSupported if invalid samplerate, else 
        *         return KErrNone
        */
        TInt SetSampleRate( TUint aSampleRate );

        /**
        * No implementation required for encoder.
        *
        * @param   Not used
        *
        * @return  KErrNone
        */
        virtual TInt SendEventToClient( const TMMFEvent& /*aEvent*/ )
            {return KErrNone;}

        
    public: // From MDataSink

        /**
        * From MDataSink Primes the source.
        * @since 
        * @param
        * @return
        */
        void SinkPrimeL();

        /**
        * From MDataSink Plays the source.
        * @since 
        * @param
        * @return
        */
        void SinkPlayL();

        /**
        * From MDataSink Pauses the source.
        * @since 
        * @param
        * @return
        */
        void SinkPauseL();

        /**
        * From MDataSink Stops the source.
        * @since 
        * @param
        * @return
        */
        void SinkStopL();
        

    private: // New functions

        /**
        * Prepare packet header and deliver the packet to the datasink.
        * @since Series 60 3.0
        * @param aPayload Payload to deliver to the datasink.
        * @return
        */        
        void DeliverPacketL( CMMFDataBuffer& aPayload );
        
        /**
        * Create a sink buffer of the given size.
        *
        * @param   aSize     size of sink buffer to create
        *
        * @return  Pointer to sink buffer created
        */
        CMMFDataBuffer* CreateSinkBufferOfSizeL( TUint aSize ); 
        
        /**
         * Update payload format with format spesific parameters.
         *
         * @since    Series 60 3.0 
         * @param    aCodecInfo       Codec info
         * @return   void
         */
        void UpdateConfigurationL( const TMccCodecInfo& aCodecInfo );
        
        /**
         * Configures the formatter based on input parameters.
         *
         * @since    Series 60 3.0 
         * @param    aCodecInfo       Codec info
         * @return   void
         */
        void DoConfigurePayloadFormatL( const TMccCodecInfo& aCodecInfo );

    private:
        /**
        * C++ default constructor.
        */
        CIlbcPayloadFormatWrite();

        /**
        * By default Symbian 2nd phase constructor is private.
        *
        * @param aConf    Decoder Configuration params
        */
        void ConstructL( MDataSink* aSource );

    private: // Data

        // FourCC
        TFourCC iFourCC;

        // Format encoding state machine
        CFormatEncodeStateMachine* iStateMachine;
        
        // Pointer to source data buffer, passed from MCPDataPath
        CMMFDataBuffer* iSourceBuffer;

        // Sink buffer to hold Ilbc/RTP payload
        CMMFDataBuffer* iSinkBuffer; 

        // RTP header used for sending RTP packet
        TRtpSendHeader iRtpSendHeader;

        // Frames per RTP packet (packetization rate)
        TInt iFramesPerPacket;
        
        // Counter for determining are all frames/samples got for RTP packet
        TInt iFrameIndex;

        // Frame time interval in micro seconds
        TTimeIntervalMicroSeconds iFrameTimeInterval;

        // Current media type
        TMediaId iMediaId;
        
        // Flag to indicate whether to set MARK field to be 1 (the first 
        // packet in a talk spurt should have MARK field set)
        TBool iFirstPacketFinished;
       
        // Indicates are we sending comfort noise payload
        TBool iCNModeON;

        // Data sink capable to receive RTP data
        MMccRtpDataSink* iRtpDataSink;
        
        // Parameters relating to RTP data encoding
        TMccCodecInfo iCInfo; 
        
        // Boolen for rtp sink
        TBool iIsRtpSink;
        
        // Key for Rtp media clock
        TUint32 iKey;
        
        /**
         * Rtp media clock instance
         * Not own.
         */
        CMccRtpMediaClock* iRtpMediaClock;
        
        // Friend class for testing
        #ifdef TEST_EUNIT
            friend class UT_CIlbcPayloadFormatWrite;
        #endif
    };

#endif //ILBCPAYLOADFORMATWRITE_H
