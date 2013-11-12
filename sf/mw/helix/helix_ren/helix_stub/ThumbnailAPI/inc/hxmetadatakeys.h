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





#ifndef _SYMBIAN_METADATA_KEYS_H_
#define _SYMBIAN_METADATA_KEYS_H_

struct HXMetaDataKeys
{
public:
    enum EHXMetaDataKeyType {
        EInteger=0,
        EBoolean,
        EBuffer,        
        EString
    };

    enum EHXMetaDataId {
        EHXTitle      = 0,
        EHXCopyright, 
        EHXAuthor,   
        EHXGenre,  
        EHXPerformer,
        EHXDescription,
        EHXLiveStream,
        EHXSeekable, 
        EHXContentType,  
        EHXFormat,       
        EHXQuality,      
        EHXAbstract,     
        EHXDuration,         
        EHXClipBitRate,      
        EHXFramesPerSecond, 
        EHXFrameSize,      
        EHXStreamCount,   
        EHXVideoBitRate,
        EHXAudioBitRate,  
        EHXMimeType,  
        EHXCodec,
        EHXIconURI,
        EHXEPreviewURI,
        EHXContentURI,
        EHXContentID,
        EHXInfoURL 
    };

    EHXMetaDataId        m_id;
    const char*          m_pHXKey;
    EHXMetaDataKeyType   m_eType;
};


#define HXTitle        "Title"      
#define HXCopyright    "Copyright" 
#define HXAuthor       "Author"   
#define HXAbstract     "Abstract" 
#define HXGenre        "Genre"  
#define HXPerformer    "Performer"  
#define HXContentType  "ContentType"
#define HXLiveStream   "LiveStream"
#define HXSeekable     "Seekable" 

static const HXMetaDataKeys HXMetaDataKeyTab[] = {
    {  HXMetaDataKeys::EHXTitle        , HXTitle         ,HXMetaDataKeys::EBuffer  }
    ,{ HXMetaDataKeys::EHXCopyright    , HXCopyright     ,HXMetaDataKeys::EBuffer  }
    ,{ HXMetaDataKeys::EHXAuthor       , HXAuthor        ,HXMetaDataKeys::EBuffer  }
    ,{ HXMetaDataKeys::EHXAbstract     , HXAbstract      ,HXMetaDataKeys::EBuffer  }
    ,{ HXMetaDataKeys::EHXGenre        , HXGenre         ,HXMetaDataKeys::EBuffer  }
    ,{ HXMetaDataKeys::EHXPerformer    , HXPerformer     ,HXMetaDataKeys::EBuffer  }
    ,{ HXMetaDataKeys::EHXContentType  , HXContentType   ,HXMetaDataKeys::EBuffer  }
    ,{ HXMetaDataKeys::EHXLiveStream   , HXLiveStream    ,HXMetaDataKeys::EInteger }
    ,{ HXMetaDataKeys::EHXSeekable     , HXSeekable      ,HXMetaDataKeys::EInteger }
};

static const HXMetaDataKeys HXStreamMetaDataKeyTab[] = {
    { HXMetaDataKeys::EHXMimeType      , "MimeType"      ,HXMetaDataKeys::EString  }
    ,{ HXMetaDataKeys::EHXCodec        , "Codec"         ,HXMetaDataKeys::EString  }
};


#endif _SYMBIAN_METADATA_KEYS_H_

