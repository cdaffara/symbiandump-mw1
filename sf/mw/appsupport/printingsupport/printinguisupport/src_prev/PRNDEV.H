// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __PRNDEV_H__
#define __PRNDEV_H__

#ifdef SYMBIAN_REMOVE_FONT_METRICS_V1
#undef SYMBIAN_HIDE_FONT_METRICS_V1
#define SYMBIAN_HIDE_FONT_METRICS_V1	// forcibly hide the API
#undef SYMBIAN_SUPPORT_FONT_METRICS_V1	// forcibly turn of support
#endif

#ifdef SYMBIAN_SUPPORT_FONT_METRICS_V1
#undef SYMBIAN_HIDE_FONT_METRICS_V1
#endif

#include <e32std.h>
#include <gdi.h>
#include <w32std.h>


class CPrintPreviewDevice : public CGraphicsDevice
	{
public:
	static CPrintPreviewDevice* NewL(CWsScreenDevice* aWinDevice);
	void SetTwipToPixelMapping(const TSize& aImageSizeInPixels,const TSize& aPageSizeInTwips);
	inline void SetWindow(RWindow* aWin);
	inline void SetPanelPos(const TPoint& aPoint);
	inline void SetBandRect(const TRect& aRect);
#ifndef SYMBIAN_HIDE_FONT_METRICS_V1
	TInt GetNearestFontInPixels(CFont*& aFont, const TFontSpec& aFontSpec);
#endif
	TInt GetNearestFontToDesignHeightInPixels(CFont*& aFont, const TFontSpec& aFontSpec);
	TInt GetNearestFontToMaxHeightInPixels(CFont*& aFont, const TFontSpec& aFontSpec, TInt aMaxHeight);
	//
	// from CGraphicsDevice
	virtual TDisplayMode DisplayMode() const;
	virtual TSize SizeInPixels() const;
	virtual TSize SizeInTwips() const;
	virtual TInt CreateContext(CGraphicsContext*& aGC);
	virtual TInt NumTypefaces() const;
	virtual void TypefaceSupport(TTypefaceSupport& aTypefaceSupport,TInt aTypefaceIndex) const;
	virtual TInt FontHeightInTwips(TInt aTypefaceIndex,TInt aHeightIndex) const;
	virtual void PaletteAttributes(TBool& aModifiable,TInt& aNumEntries) const;
	virtual void SetPalette(CPalette* aPalette);
	virtual TInt GetPalette(CPalette*& aPalette) const;
	// from MGraphicsDeviceMap
	virtual TInt HorizontalTwipsToPixels(TInt aTwips) const;
	virtual TInt VerticalTwipsToPixels(TInt aTwips) const;
	virtual TInt HorizontalPixelsToTwips(TInt aPixels) const;
	virtual TInt VerticalPixelsToTwips(TInt aPixels) const;
#ifndef SYMBIAN_HIDE_FONT_METRICS_V1
	virtual TInt GetNearestFontInTwips(CFont*& aFont, const TFontSpec& aFontSpec);
#endif
	virtual TInt GetNearestFontToDesignHeightInTwips(CFont*& aFont, const TFontSpec& aFontSpec);
	virtual TInt GetNearestFontToMaxHeightInTwips(CFont*& aFont, const TFontSpec& aFontSpec, TInt aMaxHeight);
	virtual void ReleaseFont(CFont* aFont);
	//
	~CPrintPreviewDevice();
private:
	CPrintPreviewDevice(CWsScreenDevice* aWinDevice);
	void DoCreateContextL(CGraphicsContext*& aGc);
private:
	enum { EAccFac = 1000 };
	RWindow* iWin;
	CWsScreenDevice* iWinDevice; // owns
	TPoint iPanelPos;
	TSize iPageSizeInTwips;
	TInt iVZoom;
	TInt iHZoom;
	TRect iBandRect;
	};


class CPrintPreviewGc : public CWindowGc
	{
public:
	CPrintPreviewGc(CWsScreenDevice* aDevice);
	virtual ~CPrintPreviewGc();
	void SetOriginOffset(const TPoint& aOffset);
	void SetBandRect(const TRect& aRect);
	// from CGraphicsContext
	virtual void SetOrigin(const TPoint &aPos=TPoint(0,0));
	virtual void SetClippingRect(const TRect& aRect);
	virtual void CancelClippingRect();
private:
	TPoint iOriginOffset;
	TRect iBandRect;
	};


// inlines //

inline void CPrintPreviewDevice::SetBandRect(const TRect& aRect)
	{ iBandRect = aRect; }

inline void CPrintPreviewDevice::SetPanelPos(const TPoint& aPoint)
	{ iPanelPos = aPoint; }

inline void CPrintPreviewDevice::SetWindow(RWindow* aWin)
	{ iWin = aWin; }


#endif
