// Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#ifndef __APGCOMMONUTILS_H__
#define __APGCOMMONUTILS_H__

#include <e32base.h>
#include <s32strm.h>
#include <s32buf.h>
#include<usif/scr/appregentries.h>


/**
Implementation of the MStreamBuf interface that throws away all
data written to it but keeps track of how many bytes have been
written to it.  It does not support reading.
*/
class TNullBuf : public MStreamBuf
    {
public:
    inline TNullBuf();
    inline TUint BytesWritten();
private:
    inline virtual void DoWriteL(const TAny* aPtr,TInt aLength);
private:
    TUint iBytesWritten;
    };

/**
A write stream that throws away all its input, but keeps track of how many
bytes have been written to it.  It is used for determining the amount of
memory needed to store externalised objects.
*/
class RNullWriteStream : public RWriteStream
    {
public:
    inline RNullWriteStream();
    inline TUint BytesWritten();
private:
    TNullBuf iSink;
    };

inline TNullBuf::TNullBuf() : iBytesWritten(0) 
    {
    }

inline TUint TNullBuf::BytesWritten() 
    {
    return iBytesWritten;
    }

inline void TNullBuf::DoWriteL(const TAny*,TInt aLength)
    {
    iBytesWritten += aLength;
    }

inline RNullWriteStream::RNullWriteStream()
    {
    Attach(&iSink);
    }

inline TUint RNullWriteStream::BytesWritten()
    {
    return iSink.BytesWritten();
    }

template <class T>
TInt GetObjectSizeL(T* aObject)
    {
    TInt size(0);
    if(aObject)
        {
        RNullWriteStream nullstream;
        CleanupClosePushL(nullstream);
        nullstream << *aObject;
        nullstream.CommitL();
        size = nullstream.BytesWritten();
        CleanupStack::PopAndDestroy(&nullstream);
        return size;
        }
    return -1;
    }

#endif //__APGCOMMONUTILS_H__
