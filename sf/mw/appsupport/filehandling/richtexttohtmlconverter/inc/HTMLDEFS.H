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

#ifndef __RICH_TEXT_2_HTML_TAGS_H_
#define __RICH_TEXT_2_HTML_TAGS_H_

// consts for HTML tags
const TInt KTwipsToBlockQuote			= 1440;	//
const TInt KHtmlTwipsToHeight			= 72;	// 
const TInt KHtmlTwipsToHeightBaseAdjust	= 30;	// Assigned by trial & error
const TInt KHtmlMaxFontSize				= 7;

// codes for HTML reserved characters
#define KLessThan					'<'
#define KGreaterThan				'>'
#define KAmpersand					'&'

_LIT8(KHtmlLessThan, "&lt;");
_LIT8(KHtmlGreaterThan, "&gt;");
_LIT8(KHtmlAmpersand, "&amp;");

#define KCRLF						"\x0d\x0a"

// Tagsville...
// Meta markup tags
_LIT8(KHTMLDocType32,		"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\">\x0d\x0a");
_LIT8(KHtmlStartTag,			"<HTML> \x0d\x0a");
_LIT8(KHtmlEndTag,			"</HTML> \x0d\x0a");
_LIT8(KHtmlHeadStartTag,		"<HEAD> \x0d\x0a");
_LIT8(KHtmlHeadEndTag,		"</HEAD> \x0d\x0a");
_LIT8(KHTMLHeadMetaGenTag,	"<META NAME=\"generator\" CONTENT=\"rt2html converter\">\x0d\x0a");
#ifdef _UNICODE
	_LIT8(KHTMLMetaCharSetUTF8,	"<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=UTF-8\">");
#endif // _UNICODE
_LIT8(KHTMLHeadTitleStartTag, "<TITLE>");
_LIT8(KHTMLHeadTitleEndTag,	 "</TITLE>\x0d\x0a");
_LIT8(KHtmlBodyStartTag,		 "<BODY BGCOLOR=\"#%+02x%+02x%+02x\" TEXT=\"#%+02x%+02x%+02x\">\x0d\x0a");
_LIT8(KHtmlBodyEndTag,		 "</BODY>\x0d\x0a");

_LIT8(KHtmlFontSizeStart,	 "<FONT SIZE=%d>");
_LIT8(KHtmlFontColourStart,	 "<FONT COLOR=\"#%+02x%+02x%+02x\">");
//
_LIT8(KHtmlFontStartClrNSize, "<FONT SIZE=%d COLOR=\"#%+02x%+02x%+02x\">");
//
_LIT8(KHtmlFontEnd,			 "</FONT>");
_LIT8(KHtmlBoldStart,		 "<B>");
_LIT8(KHtmlBoldEnd,			 "</B>");
_LIT8(KHtmlUnderlineStart,	 "<U>");
_LIT8(KHtmlUnderlineEnd,		 "</U>");
_LIT8(KHtmlItalicStart,		 "<I>");
_LIT8(KHtmlItalicEnd,		 "</I>");
_LIT8(KHtmlStrikeoutStart,	 "<S>") ;
_LIT8(KHtmlStrikeoutEnd,		 "</S>");
_LIT8(KHtmlSuperscriptStart,	 "<SUP>");
_LIT8(KHtmlSuperscriptEnd,	 "</SUP>");
_LIT8(KHtmlSubscriptStart,	 "<SUB>");
_LIT8(KHtmlSubscriptEnd,		 "</SUB>");
_LIT8(KHtmlTeletypeStart,     "<TT>");
_LIT8(KHtmlTeletypeEnd,	     "</TT>");

// Paragraph level formatting
_LIT8(KHtmlDivAlignNoneStart,		"<DIV>");
_LIT8(KHtmlDivAlignCentreStart,		"<DIV ALIGN=CENTER>");
_LIT8(KHtmlDivAlignLeftStart,		"<DIV ALIGN=LEFT>");
_LIT8(KHtmlDivAlignRightStart,		"<DIV ALIGN=RIGHT>");
_LIT8(KHtmlDivAlignJustifyStart,	"<DIV ALIGN=JUSTIFIED>");
_LIT8(KHtmlDivEnd,			"</DIV> \x0d\x0a");
_LIT8(KHtmlDivBlankEnd,		"&nbsp;</DIV>");
_LIT8(KHtmlDivBlank,			"<DIV>&nbsp;</DIV>");
_LIT8(KHtmlParagraphEnd,			"</P> \x0d\x0a");
_LIT8(KHtmlBulletListStart,		"<UL>");
_LIT8(KHtmlBulletListEnd,		"\x0d\x0a</UL>\x0d\x0a");
_LIT8(KHtmlBulletListPointStart, "\x0d\x0a<LI>");
_LIT8(KHtmlBulletListPointEnd, "</DIV></LI>");
_LIT8(KHtmlBlockquoteStart,		"<BLOCKQUOTE>");
_LIT8(KHtmlBlockquoteEnd,		"</BLOCKQUOTE>");

_LIT8(KHtmlLineBreak,		 "<BR>");
_LIT8(KHtmlFailedEmbedTag,	 "<FONT COLOR=\"red\" size=7>X</FONT>");

#endif // __RICH_TEXT_2_HTML_TAGS_H_
