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
* Description:    
*
*/



//  INCLUDES
#ifndef MCCFILESOURCE_H
#define MCCFILESOURCE_H

#include <e32std.h>
#include "mccdatasource.h"


class CMccFileSourceImpl;
class TMccFileSourceSetting;

/**
*  
*/
class CMccFileSource: public CMccDataSource
    {

    public: // Methods called internally or by the controller

        /**
        * Two-phased constructor.
        */
        static MDataSource* NewSourceL( TUid aImplementationUid, 
                                        const TDesC8& aInitData );
                                        
		void ConstructSourceL ( const TDesC8& aInitData );
		
		CMccFileSource();
		
		virtual ~CMccFileSource();

    public: // From CMccDataSource
    
        virtual void GetParameterL( TUint32 aParam, TDes8& aVal );
        
        virtual void SetParameterL( TUint32 aParam, const TDesC8& aVal );
        
    public:
    
	    /**
	    * Opens a 3GP file for streaming
	    * @since SWIS Lite
	    * @param TFileName aFileName
	    */
	    virtual void OpenFileL( TFileName aFileName, TBool aFileChangeMode = EFalse );

	    /**
	    * Closes the 3GP file
	    * @since SWIS Lite
	    */
	    virtual void CloseFileL();

        /**
	    * Gets filename
	    */
        TFileName& FileNameL();
        
	    /**
	    * Seeks to given position in a file
	    * @since SWIS Lite
	    * @param TTimeIntervalMicroSeconds aPosition
	    */
	    virtual void SetPositionL( TTimeIntervalMicroSeconds aPosition );
	    
	    /**
	    * Returns the current video framerate
	    * @since SWIS Lite
	    * @return TReal32
	    */
	    virtual TReal VideoFrameRateL();

	    /**
	    * Returns the current audio bitrate
	    * @since SWIS Lite
	    * @return TUint
	    */
	    virtual TUint32 AudioBitRateL();

	    /**
	    * Returns the duration of media clip
	    * @since SWIS Lite
	    * @return TTimeIntervalMicroSeconds
	    */
	    virtual TTimeIntervalMicroSeconds DurationL();

	    /**
	    * Returns the video framesize
	    * @since SWIS Lite
	    * @return TSize
	    */
	    virtual TSize VideoFrameSizeL();

	    /**
	    * Returns the video bitrate
	    * @since SWIS Lite
	    * @return TUint
	    */
	    virtual TUint32 VideoBitRateL();

	    /**
	    * Returns the current reading position
	    * @since SWIS Lite
	    * @return TTimeIntervalMicroSeconds
	    */
	    virtual TTimeIntervalMicroSeconds PositionL();
	    
	    /**
	    * Returns the audio fourCC
	    * @return TFourCC
	    */
	    virtual TFourCC AudioFourCCL();
	    
	    /**
	    * Returns the video fourCC
	    * @return TFourCC
	    */
	    virtual TFourCC VideoFourCCL();
	    
    private: // From MDataSource
	    
	    /**
	    * Reads audio frames from file
	    * @since SWIS Lite
	    * @param 
	    * @return
	    */
	    void ReadAudioFrameL();

	    /**
	    * Reads one video frame from file
	    * @since SWIS Lite
	    * @param 
	    * @return
	    */
	    void ReadVideoFrameL();
	    
	    //\// Callback functions for timers
	    /**
	    * Callback function for periodic timer
	    * @since SWIS Lite
	    * @param TAny*
	    * @return TInt
	    */
	    static TInt TickAudio( TAny* aObject ); 

	    /**
	    * Callback function for periodic timer
	    * @since SWIS Lite
	    * @param TAny*
	    * @return TInt
	    */
	    static TInt TickVideo( TAny* aObject );
	
    public: // From MDataSource

        /**
        * From MDataSource. Returns the data type as a fourCC code of the data source.
        *
        * @since    Series 60 3.0
        * @param    "aMediaId"
        *           This identifies the type of media eg audio or video and the stream id.
        *           Only KUidMediaTypeAudio -mediatype is supported.
        * @return    The fourCC code identifying the source datatype for the specified aMediaId.
        */
        TFourCC SourceDataTypeCode( TMediaId /*aMediaId*/ );

        /**
        * From MDataSource. Sets the data type as a fourCC code for the data source
        *
        * @since    Series 60 3.0
        * @param    "aSourceFourCC"
        *            This specifies the data type as a fourCC code to set the source to.
        *
        * @param    "aMediaId"
        *            This identifies the type of media eg audio or video and the stream id.
        *           Only KUidMediaTypeAudio -mediatype is supported.
        *
        * @return    KErrNone if successful, KErrNotSupported if the source does not support having
        *            it's data type set, otherwise a system wide error code.
        */
        TInt SetSourceDataTypeCode( TFourCC aCodec, 
                                    TMediaId aMedia );

        /**
        * From MDataSource. Not used. Leaves with KErrNotSupported.
        * 
        * @since    Series 60 3.0
        * @param    "aBuffer"
        *            The buffer that has been emptied by a data sink and is now available for reuse
        * @return   None
        */
        void BufferEmptiedL( CMMFBuffer* aBuffer );
        
        /**
        * From MDataSource. Method to indicate whether the data source can create a buffer.
        *
        * @since    Series 60 3.0
        * 
        * @return    EFalse
        */
        TBool CanCreateSourceBuffer();
        
        /**
        * From MDataSource. Not used. Leaves with KErrNotSupported.
        *
        * @since    Series 60 3.0
        * @param    "aMediaId"
        *           This identifies the type of media eg audio or video and the stream id.
        *           This parameter is required in cases where the source can supply data
        *           of more than one media type and/or multiple strams of data eg a multimedia file.
        *
        * @param    "aReference"
        *           This must be written to by the method to indicate whether the created buffer is
        *           a 'reference' buffer.  A 'reference' buffer is a buffer that is owned by the source
        *           and should be used in preference to the sink buffer provided the sink buffer
        *           is also not a reference buffer.
        * .  
        * @return   The created buffer
        */
        CMMFBuffer* CreateSourceBufferL( TMediaId, 
                                         TBool& aReference );
        
        /**
        * From MDataSource. Method to 'logon' the data source to the same thread that source will be supplying data in.
        *
        * Jitterbuffer for incoming packets and downlink timeout timer are created here.
        * 
        * @param    "aEventHandler"
        *           This is an MAsyncEventHandler to handle asynchronous events that occur during the
        *           transfer of multimedia data.  The event handler must be in the same thread as the data transfer
        *           thread - hence the reason it is passed in the SourceThreadLogon as opposed to say the constructor.
        *
        *
        * @return   KErrNone if successful, otherwise a system wide error code.
        */
        TInt SourceThreadLogon( MAsyncEventHandler& aEventHandler );
        
        /**
        * From MDataSource. Method to 'logoff' the data source from the same thread that source supplies data in.
        *
        * Jitterbuffer for incoming packets and downlink timeout timer are deleted. Datasource deletes the 
        * stream(s) and unregisters itself as an observer for the RTP session for incoming RTP and RTCP packets.
        *
        * @return None
        */
        void SourceThreadLogoff();

        /**
        * From MDataSource. Method to 'prime' the data source.
        *
        * Datasource is set to primed state.
        *
        * @since    Series 60 3.0
        * @return   None
        */
        void SourcePrimeL();
        
        /**
        * From MDataSource. Method to 'play' the data source.
        *
        * Downlink timeout timer is started and datasource is set to playing state.
        *
        * @since    Series 60 3.0
        * @return   None
        */
        void SourcePlayL();
        
        /**
        * From MDataSource. Method to 'pause' the data source.
        *
        * Downlink timeout timer is stopped and datasource is set to paused state.
        *
        * @since    Series 60 3.0
        * @return   None
        */
        void SourcePauseL();
        
        /**
        * From MDataSource. Method to 'stop' the data source.
        *
        * Downlink timeout timer is stopped and datasource is set to stopped state.
        *
        * @since    Series 60 3.0
        * @return   None
        */
        void SourceStopL();

        /**
        * From MDataSource. Method called by a MDataSink to request the data source to fill aBuffer with data.
        *
        * Datasource gets packet from jitterbuffer and forwards it to the payloadformatdecoder
        * for further processing. Datasource works synchronously; consumers BufferFilledL
        * is called when RTP packet is got succesfully from jitterbuffer.
        * 
        * @since    Series 60 3.0
        * @param    "aBuffer"
        *           The buffer that needs filling with data
        *
        * @param    "aConsumer" 
        *           The data sink that consumes the data. The data source needs this to make the BufferFilledL
        *           callback on aConsumer when the data source has completed filling the aBuffer.
        *
        * @param    "aMediaId"
        *           This identifies the type of media eg audio or video and the stream id.
        *           This parameter is required in cases where the source can supply data
        *           of more than one media type and/or multiple strams of data eg a multimedia file.
        * @return   None
        */
        void FillBufferL( CMMFBuffer* aBuffer,
                          MDataSink* aConsumer,
                          TMediaId aMediaId );
                          
        /**
        * From MDataSource
        */
        void NegotiateSourceL( MDataSink& aDataSink );	

    private:

		/**
	    * Updates the current reading position
	    * @since SWIS Lite
	    * @param 
	    */
	    void UpdatePositionL();

		/**
	    * Returns the frame lenght from given audio sample
	    * @since SWIS Lite
	    */
		TUint GetFrameLength( TPtrC8 aData );
		
		void GetFileInfoL( TMccFileSourceSetting& aSettings );

    private:
	
		CMccFileSourceImpl*   iSourceImpl;
	#ifdef EUNIT_TEST
        friend class UT_CMccFileSource;
        friend class UT_CMccMultiplexer;
    #endif	
	
	};


#endif 


// End of file