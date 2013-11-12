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

#ifndef CACLIENTTEST_GLOBAL_H
#define CACLIENTTEST_GLOBAL_H

#include <QDebug>
#include <QtGlobal>

/*!
 To enable logging of function entry and exit use the following flag for qmake:
 -config nft
 To include in logs extra information about RAM and heap usage, define an additional constant e.g. in ProductVariant.hrh:
 #define NFT_RAM
 */

#ifdef NFT
#ifdef Q_OS_SYMBIAN
#include <hal.h>
#include <e32std.h>
#endif
#endif

#ifdef NFT

#define CACLIENTTEST(aText) qDebug() << QString(aText)

#ifdef Q_OS_SYMBIAN
#ifdef NFT_RAM
#define CACLIENTTEST_FREERAM_ENTRY(function) \
    TInt CACLIENTTEST_ENTRY_RAM(0); \
    TInt CACLIENTTEST_ENTRY_HEAP(0); \
    TInt CACLIENTTEST_ENTRY_HEAP_SIZE(0); \
    { \
    TInt allRAM(0); \
    HAL::Get(HAL::EMemoryRAM, allRAM); \
    HAL::Get(HAL::EMemoryRAMFree, CACLIENTTEST_ENTRY_RAM); \
    RHeap &heap = User::Heap(); \
    TInt biggestBlock(0); \
    CACLIENTTEST_ENTRY_HEAP = heap.Available(biggestBlock); \
    CACLIENTTEST_ENTRY_HEAP_SIZE = heap.Size(); \
    qDebug("(nft) " function " - Memory (kB) - Free RAM: %d, Heap size: %d, Free heap: %d", \
        CACLIENTTEST_ENTRY_RAM >> 10, CACLIENTTEST_ENTRY_HEAP_SIZE >> 10, \
        CACLIENTTEST_ENTRY_HEAP >> 10); \
    }

#define CACLIENTTEST_FREERAM_EXIT(function) \
    { \
    TInt allRAM(0); \
    TInt freeRAM(0); \
    HAL::Get(HAL::EMemoryRAM, allRAM); \
    HAL::Get(HAL::EMemoryRAMFree, freeRAM); \
    RHeap &heap = User::Heap(); \
    TInt biggestBlock(0); \
    TInt heapFree = heap.Available(biggestBlock); \
    TInt heapSize = heap.Size(); \
    qDebug("(nft) " function " - Memory (kB) - Free RAM: %d, Heap size: %d, Free heap: %d", \
        freeRAM >> 10, heapSize >> 10, heapFree >> 10); \
    qDebug("(nft) " function " - Memory (kB) - RAM diff: %d, Heap size diff: %d, Free heap diff: %d", \
        (freeRAM-CACLIENTTEST_ENTRY_RAM) >> 10, (heapSize-CACLIENTTEST_ENTRY_HEAP_SIZE) >> 10, \
        (heapFree-CACLIENTTEST_ENTRY_HEAP) >> 10); \
    }

#else

#define CACLIENTTEST_FREERAM_ENTRY(function)
#define CACLIENTTEST_FREERAM_EXIT(function)

#endif

#define CACLIENTTESTTIME_ENTRY(function) \
    TInt64 CACLIENTTEST_ENTRY_TIME(0); \
    { \
        TTime t; \
        t.UniversalTime(); \
        qDebug("\n" function "      entry:%20lld", t.Int64()); \
        CACLIENTTEST_ENTRY_TIME = t.Int64(); \
    }

#define CACLIENTTESTTIME_EXIT(function) { \
        TTime t; \
        t.UniversalTime(); \
        qDebug("\n" function "      entry:%20lld\n" function " difference:%20lld", t.Int64(), \
            t.Int64()-CACLIENTTEST_ENTRY_TIME); \
        }

#define CACLIENTTEST_FUNC_ENTRY(function)   CACLIENTTEST_FREERAM_ENTRY(function) \
        CACLIENTTESTTIME_ENTRY(function)
#define CACLIENTTEST_FUNC_EXIT(function)    CACLIENTTESTTIME_EXIT(function) \
        CACLIENTTEST_FREERAM_EXIT(function)

#else //Q_OS_SYMBIAN
#define CACLIENTTEST_FUNC_ENTRY(function)   qDebug()<< function << " entry";
#define CACLIENTTEST_FUNC_EXIT(function)    qDebug()<< function << " exit";
#endif //Q_OS_SYMBIAN
#else //NFT
#define CACLIENTTEST_FUNC_ENTRY(function)
#define CACLIENTTEST_FUNC_EXIT(function)
#endif //NFT

#endif //CACLIENTTEST_GLOBAL_H
