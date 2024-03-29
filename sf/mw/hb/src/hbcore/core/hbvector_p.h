/****************************************************************************
**
** Copyright (C) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (developer.feedback@nokia.com)
**
** This file is part of the HbCore module of the UI Extensions for Mobile.
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at developer.feedback@nokia.com.
**
****************************************************************************/

#ifndef HBVECTOR_P_H
#define HBVECTOR_P_H

#include <new>

#include <QAtomicInt>

#include "hbsmartoffset_p.h"
#include "hbsmartpointer_p.h"
#include "hbmemoryutils_p.h"

#ifdef HB_BIN_CSS
static const int HbVectorDefaultCapacity = 1;
#else
static const int HbVectorDefaultCapacity = 5;
#endif

#ifdef HB_BIN_CSS
#include "hbcssconverterutils_p.h"
#endif


/**
* const iterator class of HbVector
*/
template <typename T, typename Pointer>
class  const_vector_iterator
{
public:
    typedef const T value_type;
    typedef value_type & reference;
    typedef int difference_type;

public :
    Pointer ptr() const { return mPtr; }
    const_vector_iterator()
        : mPtr(0)
    {}

    explicit const_vector_iterator(Pointer ptr)
        : mPtr(ptr)
    {}

    reference operator*() const
    { return *mPtr; }

    const value_type * operator->() const
    { return mPtr.get(); }

    reference operator[](difference_type offset) const
    { return mPtr[offset]; }

    const_vector_iterator & operator++()
    { ++mPtr;  return *this; }

    const_vector_iterator operator++(int)
    {
        Pointer tmp = mPtr;
        ++*this;
        return const_vector_iterator(tmp);
    }

    const_vector_iterator operator+ (int offset) const
    {  return const_vector_iterator(mPtr + offset); }

     int operator- (const_vector_iterator &other) const
    { return mPtr - other.mPtr; }

     const_vector_iterator & operator-- ()
    { --mPtr; return *this; }

    const_vector_iterator & operator--(int)
    { Pointer temp = mPtr; --*this; return const_vector_iterator(temp); }

    bool operator!= (const const_vector_iterator &other) const
    { return mPtr != other.mPtr;  }

    int operator- (Pointer right) const
    { return mPtr - right; }

    bool operator< (const_vector_iterator & other)  const
    { return mPtr < other.mPtr;  }

    bool operator<= (const_vector_iterator & other)  const
    {  return mPtr <= other.mPtr;  }

    bool operator> (const_vector_iterator & other)  const
    {  return mPtr > other.mPtr;  }

    bool operator>= (const_vector_iterator & other)  const
   {  return mPtr >= other.mPtr;  }

    bool operator== (const_vector_iterator &right) const
    { return mPtr == right.mPtr; }

protected :
    Pointer mPtr;
};

/**
* iterator class of HbVector
*/
template <typename T, typename Pointer>
class  vector_iterator
    : public const_vector_iterator<T, Pointer>
{
public:
    typedef T value_type;
    typedef value_type & reference;
    typedef int difference_type;
public:

    vector_iterator()
    {}

    explicit vector_iterator(Pointer ptr)
      : const_vector_iterator<T,Pointer>(ptr)
    {}

    reference operator*() const
    { return *this->mPtr;}

    value_type * operator->() const
    { return this->mPtr.get(); }

    reference operator[](difference_type offset) const
    { return this->mPtr[offset]; }

    //Increment / Decrement
    vector_iterator & operator++()  
    {
        ++this->mPtr;
        return *this;
    }

    vector_iterator operator++(int)
    {  
        Pointer tmp = this->mPtr;
        ++*this;
        return vector_iterator(tmp);
    }

    vector_iterator & operator--()
    { 
        --this->mPtr;
        return *this;
    }

    vector_iterator operator--(int)
    {
        vector_iterator temp = *this;
        --*this;
        return vector_iterator(temp);
    }

    vector_iterator operator+(difference_type offset) const
    {  return vector_iterator(this->mPtr+offset); }

    vector_iterator operator-(difference_type offset) const
   {  return vector_iterator(this->mPtr-offset); }

    difference_type operator-(const_vector_iterator<T,Pointer>& right) const
    {  return static_cast<const const_vector_iterator<T,Pointer>&>(*this) - right; }
};

/**
* HbVector class
* HbVector is templatized vector which can store its internal data either in shared memory or heap.
* The class was written specially for sharing the output of CSS parser for subsequent lookup usage
* across processes, to reduce the overall memory usage and to improve performance.
*
* In case of out of memory situation, the vector is restored to the state it was before the exception occurred. 
* 
* The class uses smart pointers (smart_ptr<T> class objects) which internally contains offsets from
* a certain base address (which can either be shared memory or heap) instead of directly containing
* the virtual addresses. This way, the smart pointers are usable across processes, as the base 
* virtual addresses of the specific shared memory chunk may be different in different processes.
* 
* For iterating through the HbVector, special iterator classes are also provided (const_vector_iterator
* and vector_iterator)
* Important Note: By default implicit sharing happens. For the scenarios when source vector is in 
* shared memory and destination vector is in heap (or does not have write access to shared memory),
* implicit sharing is done by keeping a pointer to the shared internal data of source container inside 
* destination container. Here we assume that source container will not destroy its data as long as
* destination container is using it. (We can assume it safely for css sharing usecase when we do not
* remove stylesheet from shared memory as long as it is being used by client processes).
* 
* However, in normal scenario (places where you don't need sharing stuff), it's recommended to still
* use normal QVector.
*/

template <typename T>
class  HbVector
{
    friend class TestHbVector;
public :
    typedef int                                 size_type;
    typedef smart_ptr<T>                        pointer;
    typedef T                                   value_type;
    typedef const value_type                    const_value_type;
    typedef value_type &                        reference;
    typedef const_value_type &                  const_reference;
    typedef vector_iterator<T,pointer>          iterator;
    typedef const_vector_iterator<T,pointer>    const_iterator;

public:
    //Owing to OOM condition, the ctor of HbVector can throw, the exception is 
    // propagated to the caller.
    HbVector(HbMemoryManager::MemoryType type = HbMemoryManager::HeapMemory)
       : mData(0, type),
         mShared(false)
   {
       if (type == HbMemoryManager::HeapMemory) {
           static int heapNullOffset(createNullOffset(HbMemoryManager::HeapMemory));
           mData.setOffset(heapNullOffset);
           mData->mRef.ref();
       } else if (type == HbMemoryManager::SharedMemory) {
           static int sharedNullOffset(createNullOffset(HbMemoryManager::SharedMemory));
           mData.setOffset(sharedNullOffset);
           mData->mRef.ref();
#ifdef HB_BIN_CSS
           HbCssConverterUtils::registerOffsetHolder(mData.offsetPtr());
#endif
       } else {
           HbMemoryManager *manager = memoryManager();
           HbSmartOffset smartOffset(manager->alloc(sizeof(HbVectorData)), type );
           mData = new((char*)manager->base() + smartOffset.get()) HbVectorData(type);
           smartOffset.release();
       }
   }

    HbVector(const HbVector<T> &other) : mData(other.mData), mShared(other.mShared)
    {
       HbMemoryManager *manager = memoryManager();
       if(!manager->isWritable()){
           mShared = true;
       }
       if (!mShared) {
           mData->mRef.ref();
       }
#ifdef HB_BIN_CSS
       HbCssConverterUtils::registerOffsetHolder(mData.offsetPtr());
#endif
    }
    
    ~HbVector()
    {
       HbMemoryManager *manager = memoryManager();
       // if the memory where the vector is not writable it means it's client process, so do nothing
       if(!mData || !manager->isWritable()) 
           return;
       if(!mShared && !mData->mRef.deref()) {
           destroyData();
       }

#ifdef HB_BIN_CSS
       HbCssConverterUtils::unregisterOffsetHolder(mData.offsetPtr());
#endif
    }
    
    HbMemoryManager::MemoryType memoryType() const
    {
       return mShared ?  HbMemoryManager::HeapMemory : mData.memoryType();
    }
    
    #ifdef CSS_PARSER_TRACES
    void print() const
    {
    //qDebug() <<"==============HbVector::Print():Begin [size = "<< count() <<"] ==================";
       if(count())
       {
           const_iterator iter = begin();
           while(iter != end()) {
               iter->print();
               iter++;
           }
       }
       else
           qDebug() << "Empty HbVector::Nothing to print!";
     //qDebug() <<"==============HbVector::Print():End [size = "<< count() <<"] ==================";
    }
    
    #endif
    
    bool isEmpty() const
    { return mData->mSize == 0; }
    
    void append(const_reference value);
    void insert ( int i, const_reference value )
    {
       Q_ASSERT_X(i >= 0 && i <= mData->mSize,"HbVector<T>::insert()","index out of range");
       insert(constBegin() + i, (size_type)1, value); 
    }
    
    void insert ( int i, int count, const_reference value )
    {
       Q_ASSERT_X(i >= 0 && i <= mData->mSize,"HbVector<T>::insert()","index out of range");
       insert(constBegin() + i, (size_type)count, value);
    }
    
    iterator insert ( const_iterator before, const_reference value )
    { return insert(before, (size_type)1, value); }
    
    iterator insert ( const_iterator before, int count, const_reference value );
    
    const_iterator constEnd() const
    { return const_iterator(mData->mStart + mData->mSize); }
    
    iterator end() 
    {
        detach(); 
        return iterator(mData->mStart + mData->mSize); 
    }

    const_iterator end() const
    { return const_iterator(mData->mStart + mData->mSize); }

    const_iterator constBegin() const
    { return const_iterator(mData->mStart); }

    iterator begin() 
    { detach() ; return iterator(mData->mStart); }

    const_iterator begin() const
    { return const_iterator(mData->mStart); }

    reference first() const
    { Q_ASSERT(!isEmpty()); return *this->mData->mStart; }

    const_reference last() const
    { Q_ASSERT(!isEmpty()); return this->mData->mStart[this->mData->mSize - 1]; } 

    reference at(size_type position) const
    {
       Q_ASSERT_X(position >= 0 && position < this->mData->mSize,"HbVector<T>::at", "index out of range"); 
       return this->mData->mStart[position];
    }

    const T *constData() const
    { return (T*)mData->mStart.get(); }

    value_type value(int i) const
    {
       if(i < 0 || i >= mData->mSize) {
           return value_type();
       }
       return mData->mStart[i];
    }

    value_type value(int i, const_reference defaultValue) const
    {
       return ((i < 0 || i >= mData->mSize) ? defaultValue : mData->mStart[i]); 
    }

    void resize(int newSize)
    {
       Q_ASSERT(newSize > -1);
       int oldSize = mData->mSize;
       if(newSize < oldSize) {
           // Destroy the last remaining elements
           erase(const_iterator(mData->mStart + newSize), constEnd());
       } else {
           reserve(newSize);
           Inserter it(mData->mStart + mData->mSize, mData->mStart + newSize);
           while(it.canInsert()) {
               it.insert(T());
           }
           it.release();
       }
       mData->mSize = newSize;
    }

    // reserve() can throw owing to OOM
    void reserve(int newSize)
    {
        if( newSize > 0
            && (mShared || newSize > mData->mCapacity || mData->mRef != 1)) {
            HbMemoryManager *manager = memoryManager();
            int offset = (char*)mData->mStart.get() - (char*)manager->base();
            int newOffset = -1;
            if(newSize > mData->mCapacity)
                newOffset = reAlloc(offset, newSize, mData->mSize);
            else
                newOffset = reAlloc(offset, mData->mCapacity, mData->mSize);
            mData->mStart.setOffset(newOffset);
            mData->mCapacity = newSize;
       }
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        int firstDiff = first - mData->mStart;
        int lastDiff = last - mData->mStart;
        int diff = lastDiff - firstDiff;
        detach();
        if(QTypeInfo<value_type>::isComplex) {
            copy(mData->mStart + lastDiff, mData->mStart + mData->mSize, mData->mStart + firstDiff);
            pointer i = mData->mStart + mData->mSize;
            pointer b = mData->mStart + mData->mSize - diff;
            while(i != b) {
                --i;
                i.get()->~T();
            }
        } else {
            ::memmove(mData->mStart + firstDiff,
                        mData->mStart + lastDiff, 
                        sizeof(value_type) * (mData->mSize -lastDiff));
        }

        mData->mSize -= diff;
        return iterator(mData->mStart + firstDiff);
    }

    void remove(int i)
    {
       Q_ASSERT_X(i >= 0 && i < mData->mSize, "HbVector<T>::remove", "index out of range");
       erase(constBegin() + i , constBegin() + i + 1);
    }

    void remove(int i, int count)
    {
       Q_ASSERT_X(i >= 0 && i + count <= mData->mSize, "HbVector<T>::remove", "index out of range");
       erase(constBegin() + i , constBegin() + i + count);
    }

    void clear()
    {
       resize(0);
    }

    size_type count() const
    { return mData->mSize; } 

    size_type size() const
    { return count(); } 

    size_type capacity() const
    { return mData->mCapacity;}

    bool contains(const_reference value) const
    {
       pointer first = mData->mStart;
       pointer last = mData->mStart + mData->mSize;
       while( last != first ) {
           if( *--last == value) {
               return true;
           }
       }
      return false;
    }

    const_reference operator[](int position) const
    {
       Q_ASSERT_X(position >= 0 && position < this->mData->mSize,"HbVector<T>::at", "index out of range");
       return this->mData->mStart[position];
    }
    
    reference operator[](int position)
    {
       Q_ASSERT_X(position >= 0 && position < this->mData->mSize,"HbVector<T>::at", "index out of range");
       return this->mData->mStart[position];
    }

    bool operator!=(const HbVector<T> &other) const
    {
        return !(*this == other);
    }

    bool operator==(const HbVector<T> &other) const
    {
        if (size() != other.size())
            return false;
        pointer start = mData->mStart;
        pointer end = start + mData->mSize;
        pointer otherEnd  = other.mData->mStart + other.mData->mSize;
        while (end != start) {
            if (!(*--end == *--otherEnd)) {
                return false;
            }
        }
        return true;
    }

    HbVector<T> & operator+= (const HbVector<T>& other)
    {
        int newSize = mData->mSize + other.mData->mSize;
        reserve(newSize);
        Inserter it(mData->mStart + mData->mSize, mData->mStart + newSize);
        pointer otherFirst = other.mData->mStart;

        for(;it.canInsert(); ++otherFirst ) {
            it.insert(*otherFirst);
        }
        mData->mSize = newSize;
        it.release();
        return *this;
    }

    HbVector<T> & operator+= (const_reference value)
    {
        append(value); 
        return *this;
    }

    
    HbVector<T> & operator<< (const_reference value)
    {
        append(value); 
        return *this;
    }

    HbVector<T> &operator << ( const HbVector<T> &other)
    {
        *this += other; 
        return *this;
    }

    HbVector<T> & operator=(const HbVector<T> &other)
    {
        Q_ASSERT(memoryType() == HbMemoryManager::SharedMemory
                 || memoryType() == HbMemoryManager::HeapMemory);
        // if the Current vector and other Vector memory type is not same then do Deep Copy.
        //To Do: mShared == true is just Quick fix for unit-test where the other vector is in heap but 
        // its data shared to shared memory also the current vector is same state. this needs to fix
        // and decision making is required very clear for all the scenarios.
        if (other.memoryType() != memoryType() || other.mShared || mShared)
        {
            if(!mShared && !mData->mRef.deref()) {
                destroyData();
            }
            mShared = true;
        } else {
            if(!mData->mRef.deref() ) {
               destroyData();
            }
            other.mData->mRef.ref();
            mShared = other.mShared;
        }
        mData = other.mData;
        return *this;
    }

    HbVector<T> & operator=(const QVector<T> &other)
    {
        clear();
        foreach (T obj, other) {
            append(obj);
        }
        return *this;
    }

    operator QVector<T>() const
    {
        QVector<T> v;
        v += *this;
        return v;
    }

private:
    void detach()
    {
        if(mData->mRef != 1 || mShared) {
            copyData(size(), size());
            // Here assumption is the new copy of data is created in heap.
            // so disabling the shared flag.
            mShared = false;
        }
    }

    pointer copy(pointer begin, pointer end, pointer dest)
    {
        while(begin != end) {
            *dest++ = *begin++;
        }
        return dest;
    }    

// copy the Data , this will be called in Implicit sharing whenever vector State is going to change.
// mData is not freed, because it is assumed, that it is owned by another HbVector (mData->mRef > 1)
    void copyData(int newSize, int oldSize)
    {
        HbMemoryManager::MemoryType memType = memoryType();
        DataPointer tempData(mData);
        DataPointer newData(0, memType);
        HbMemoryManager *manager = memoryManager();
        HbSmartOffset offset(manager->alloc(sizeof(HbVectorData)), memType);
        newData = new ((char*)manager->base() + offset.get())
                        HbVectorData(memType, oldSize, newSize);
        mData = newData;
        offset.release();
        if(!mShared && !tempData->mRef.deref()) {
            destroyData();
        }

        if(QTypeInfo<value_type>::isComplex) {
            Inserter it(mData->mStart, mData->mStart + oldSize);
            pointer sourceStart = tempData->mStart;
            for(;it.canInsert(); ++sourceStart) {
                it.insert(*sourceStart);
            }
            it.release();
        } else {
            ::memcpy(mData->mStart, tempData->mStart, oldSize * sizeof(T));
        }
    }

// reallocate the data with new size , in implicit sharing we have new copy of data.
// This function can throw because it calls a throwing function (manager->realloc),
// the exception is propagated to the caller
    int reAlloc(int oldOffset, int newSize , int oldSize)
    {   
        HbMemoryManager *manager = memoryManager();
        if(mData->mRef != 1 || mShared) {
            copyData( newSize, oldSize );
            // Here assumption is the new copy of data is created in heap.
            // so disabling the shared flag.
            mShared = false;
            return (char*)mData->mStart.get() - (char*)manager->base();
        } else {
            // this statement can throw
            int offset = -1;
            if (capacity() > 0) {
                offset = manager->realloc(oldOffset, newSize * sizeof(T));
            } else {
                offset = manager->alloc(newSize * sizeof(T));
            }

            return offset;
        }
   }

   void destroyData()
   {
       mData->~HbVectorData();
       deAllocateData();
   }

private : // Data
    static int createNullOffset(HbMemoryManager::MemoryType type)
    {
        GET_MEMORY_MANAGER(type);
        int nullOffset(manager->alloc(sizeof(HbVectorData)));
        HbVectorData* data = HbMemoryUtils::getAddress<HbVectorData>(type, nullOffset);
        new(data) HbVectorData(type);
        return nullOffset;
    }
    void deAllocateData()
    {
       HbMemoryManager *manager = memoryManager();
       mData->deAllocateAll(manager);
       int dataOffset = (char*) mData.get() - (char*)manager->base();
       manager->free(dataOffset);
       mData = 0;
    }
    HbMemoryManager *memoryManager()
    {
        Q_ASSERT(memoryType() == HbMemoryManager::SharedMemory
                 || memoryType() == HbMemoryManager::HeapMemory);
        return HbMemoryManager::instance(memoryType());
    }

    struct HbVectorData
    {
        // The ctor of HbVectorData can throw owing to manager->alloc, we're not catching the exception
        // if any and simply allow to propagate it to caller.
        HbVectorData(HbMemoryManager::MemoryType type, unsigned int size = 0, unsigned int capacity = 0)
            : mStart(0,type),
              mSize(size), 
              mCapacity(capacity),
              mRef(1)
        {
#ifdef HB_BIN_CSS

            HbCssConverterUtils::registerOffsetHolder(mStart.offsetPtr());
#endif
            GET_MEMORY_MANAGER(type);
            if (capacity > 0) {
                mStart = (T*) ((char*)manager->base() + manager->alloc(capacity*sizeof(T)));
            }
        }

        ~HbVectorData()
        { 
#ifdef HB_BIN_CSS	
            HbCssConverterUtils::unregisterOffsetHolder(mStart.offsetPtr());
#endif //HB_BIN_CSS
            destroyAll();
        }

        void deAllocateAll(HbMemoryManager *manager)
        {
            if(!mCapacity) return;
            int offset = (char*) mStart.get() - (char*) manager->base();
            manager->free(offset);
            mStart = 0;
            mSize = 0;
            mCapacity = 0;
        }

        void destroyAll()
        {
            if(QTypeInfo<value_type>::isComplex) {
                pointer start(mStart);
                pointer last(start + mSize);
                
                while(last-- != start) {
                    last.get()->~value_type();
                }
            }
        }
        
        pointer mStart;
        size_type mSize;
        size_type mCapacity;
        QAtomicInt mRef;
    };

    struct Inserter {
        Inserter(pointer begin, pointer end) : begin(begin), it(begin), end(end) {

        }
        void release() {
            it = begin;
        }
        bool canInsert() const {
            return (it != end);
        }
        void insert(const T& obj) {
            new (it) T(obj);
            ++it;
        }

        ~Inserter() {
            if(QTypeInfo<value_type>::isComplex) {
                while(it != begin) {
                    begin.get()->~T();
                    ++begin;
                }
            }
        }
    private:
        pointer begin;
        pointer it;
        pointer end;
    };
    typedef smart_ptr<HbVectorData> DataPointer;
    DataPointer mData;
    bool mShared;

};


template <typename T>
Q_TYPENAME HbVector<T>::iterator
HbVector<T>::insert(const_iterator before, int count, const_reference value)
{
    int offset = before - mData->mStart;
    if(count != 0) {
        if(mShared || mData->mRef != 1 || mData->mSize + count > mData->mCapacity) {
            HbMemoryManager *manager = memoryManager();
            int offset = (char*)mData->mStart.get() - (char*)manager->base();
            int newCapacity = mData->mSize + count;
            if(newCapacity < mData->mCapacity) {
                newCapacity = mData->mCapacity;
            }
            mData->mStart.setOffset(reAlloc(offset, newCapacity, mData->mSize));
            mData->mCapacity = newCapacity;
        }
        if(QTypeInfo<value_type>::isStatic) {
            pointer b = mData->mStart + mData->mSize;
            pointer i = b + count;
            HbMemoryManager::MemoryType memType = memoryType();
            while(i != b)
                new(--i) T(memType);
            i = mData->mStart + mData->mSize;
            pointer j = i + count;
            b = mData->mStart + offset;
            while( i != b )
                *--j = *--i;
            i = b + count;
            while( i != b )
                *--i = value;
        } else {
            pointer b = mData->mStart + offset;
            pointer i = b + count;
            ::memmove(i, b, (mData->mSize - offset) * sizeof(T));
            Inserter it(b, i);
            while(it.canInsert()) {
                it.insert(value);
            }
            it.release();
        }

        mData->mSize += count;
    }
    return iterator(mData->mStart + offset);
}

template <typename T>
void HbVector<T>::append(const value_type &value)
{
    if(!mShared && mData->mRef == 1 && mData->mSize < mData->mCapacity ) {
        //There is enough space, construct a new object at the end.
        if(QTypeInfo<value_type>::isComplex) {
            new ((void*)(mData->mStart + mData->mSize)) value_type(value);
        } else {
            mData->mStart[mData->mSize] = value;
        }
    } else {
        HbMemoryManager *manager = memoryManager();
        int offset = (char*)mData->mStart.get() - (char*)manager->base();
#ifdef HB_BIN_CSS
        int capacity = (mData->mCapacity == 0) ? HbVectorDefaultCapacity : mData->mCapacity + 1;
        int newOffset = reAlloc(offset, capacity, mData->mSize);
#else
        int capacity = (mData->mCapacity == 0) ? HbVectorDefaultCapacity : mData->mCapacity * 2;
        int newOffset = reAlloc(offset, capacity, mData->mSize);
#endif
        mData->mCapacity = capacity;
        mData->mStart.setOffset(newOffset);


        if(QTypeInfo<value_type>::isComplex) {
            Inserter it(mData->mStart + mData->mSize, mData->mStart + mData->mSize + 1);
            it.insert(value_type(value));
            it.release();
        } else {
            mData->mStart[mData->mSize] = value;
        }
    }
    ++mData->mSize;
}

template <typename T>
QVector<T>& operator += (QVector<T>& v1, const HbVector<T>& v2)
{
    v1.reserve(v1.size()+v2.size());
    typename HbVector<T>::const_iterator iter = v2.constBegin();
    typename HbVector<T>::const_iterator end = v2.constEnd();
    while(iter != end) {
        v1.append(*iter);
        ++iter;
    }
    return v1;
}

#endif // HBVECTOR_P_H
