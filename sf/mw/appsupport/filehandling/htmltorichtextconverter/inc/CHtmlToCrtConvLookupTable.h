// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

// HTML version 3.2 supported

#ifndef __CHTMLTOCRTCONVLOOKUPTABLE_H__
#define __CHTMLTOCRTCONVLOOKUPTABLE_H__

#include "CHtmlToCrtConvHash.h"

struct TTagHashTableEntry
	{
	THtmlToCrtConvTagType			iTagType;
	const TText16*					iText;
	};

struct TAttributeHashTableEntry
	{
	THtmlToCrtConvAttributeType		iAttributeType;
	const TText16*					iText;
	};

struct TEntityHashTableEntry
	{
	const TDesC16*					iEntity;
	const TText16*					iText;
	};

static const TTagHashTableEntry KTagHashTable[] =
	{
	{EHtmlTagH1,						_S16("H1")},
	{EHtmlTagH2,						_S16("H2")},
	{EHtmlTagH3,						_S16("H3")},
	{EHtmlTagH4,						_S16("H4")},
	{EHtmlTagH5,						_S16("H5")},
	{EHtmlTagH6,						_S16("H6")},
	{EHtmlTagParagraph,					_S16("P")},
	{EHtmlTagUnorderedList,				_S16("UL")},
	{EHtmlTagCenter,					_S16("CENTER")},
	{EHtmlTagBlockquote,				_S16("BLOCKQUOTE")},
	{EHtmlTagItalic,					_S16("I")},
	{EHtmlTagBold,						_S16("B")},
	{EHtmlTagUnderline,					_S16("U")},
	{EHtmlTagStrikethrough,				_S16("STRIKE")},
	{EHtmlTagSubscript,					_S16("SUB")},
	{EHtmlTagSuperscript,				_S16("SUP")},
	{EHtmlTagEmphasis,					_S16("EM")},
	{EHtmlTagStrong,					_S16("STRONG")},
	{EHtmlTagBreak,						_S16("BR")},
	{EHtmlTagHtml,						_S16("HTML")},
	{EHtmlTagHead,						_S16("HEAD")},
	{EHtmlTagTitle,						_S16("TITLE")},
	{EHtmlTagStyle,						_S16("STYLE")},
	{EHtmlTagScript,					_S16("SCRIPT")},
	{EHtmlTagBase,						_S16("BASE")},
	{EHtmlTagMeta,						_S16("META")},
	{EHtmlTagLink,						_S16("LINK")},
	{EHtmlTagBody,						_S16("BODY")},
	{EHtmlTagAddress,					_S16("ADDRESS")},
	{EHtmlTagOrderedList,				_S16("OL")},
	{EHtmlTagDefinitionList,			_S16("DL")},
	{EHtmlTagListItem,					_S16("LI")},
	{EHtmlTagDefinitionTerm,			_S16("DT")},
	{EHtmlTagDefinitionDefinition,		_S16("DD")},
	{EHtmlTagPreformatted,				_S16("PRE")},
	{EHtmlTagDivision,					_S16("DIV")},
	{EHtmlTagForm,						_S16("FORM")},
	{EHtmlTagIsindex,					_S16("ISINDEX")},
	{EHtmlTagHorizontalRule,			_S16("HR")},
	{EHtmlTagTable,						_S16("TABLE")},
	{EHtmlTagCaption,					_S16("CAPTION")},
	{EHtmlTagTableRow,					_S16("TR")},
	{EHtmlTagTableHeader,				_S16("TH")},
	{EHtmlTagTableData,					_S16("TD")},
	{EHtmlTagDirectory,					_S16("DIR")},
	{EHtmlTagMenu,						_S16("MENU")},
	{EHtmlTagTeletype,					_S16("TT")},
	{EHtmlTagBig,						_S16("BIG")},
	{EHtmlTagSmall,						_S16("SMALL")},
	{EHtmlTagDefinition,				_S16("DFN")},
	{EHtmlTagCode,						_S16("CODE")},
	{EHtmlTagSample,					_S16("SAMP")},
	{EHtmlTagKeyboard,					_S16("KBD")},
	{EHtmlTagVariable,					_S16("VAR")},
	{EHtmlTagCitation,					_S16("CITE")},
	{EHtmlTagInput,						_S16("INPUT")},
	{EHtmlTagSelect,					_S16("SELECT")},
	{EHtmlTagOption,					_S16("OPTION")},
	{EHtmlTagTextarea,					_S16("TEXTAREA")},
	{EHtmlTagAnchor,					_S16("A")},
	{EHtmlTagImage,						_S16("IMG")},
	{EHtmlTagApplet,					_S16("APPLET")},
	{EHtmlTagFont,						_S16("FONT")},
	{EHtmlTagBasefont,					_S16("BASEFONT")},
	{EHtmlTagMap,						_S16("MAP")},
	{EHtmlTagArea,						_S16("AREA")}
	};

static const TAttributeHashTableEntry KAttributeHashTable[] =
	{
	{EHtmlAttributeAlign,				_S16("ALIGN")},
	{EHtmlAttributePrompt,				_S16("PROMPT")},
	{EHtmlAttributeHref,				_S16("HREF")},
	{EHtmlAttributeName,				_S16("NAME")},
	{EHtmlAttributeContent,				_S16("CONTENT")},
	{EHtmlAttributeRel,					_S16("REL")},
	{EHtmlAttributeRev,					_S16("REV")},
	{EHtmlAttributeTitle,				_S16("TITLE")},
	{EHtmlAttributeBackground,			_S16("BACKGROUND")},
	{EHtmlAttributeBgcolor,				_S16("BGCOLOR")},
	{EHtmlAttributeText,				_S16("TEXT")},
	{EHtmlAttributeLink,				_S16("LINK")},
	{EHtmlAttributeVlink,				_S16("VLINK")},
	{EHtmlAttributeAlink,				_S16("ALINK")},
	{EHtmlAttributeCompact,				_S16("COMPACT")},
	{EHtmlAttributeType,				_S16("TYPE")},
	{EHtmlAttributeStart,				_S16("START")},
	{EHtmlAttributeValue,				_S16("VALUE")},
	{EHtmlAttributeWidth,				_S16("WIDTH")},
	{EHtmlAttributeAction,				_S16("ACTION")},
	{EHtmlAttributeMethod,				_S16("METHOD")},
	{EHtmlAttributeEnctype,				_S16("ENCTYPE")},
	{EHtmlAttributeNoshade,				_S16("NOSHADE")},
	{EHtmlAttributeSize,				_S16("SIZE")},
	{EHtmlAttributeBorder,				_S16("BORDER")},
	{EHtmlAttributeCellspacing,			_S16("CELLSPACING")},
	{EHtmlAttributeCellpadding,			_S16("CELLPADDING")},
	{EHtmlAttributeValign,				_S16("VALIGN")},
	{EHtmlAttributeNowrap,				_S16("NOWRAP")},
	{EHtmlAttributeRowspan,				_S16("ROWSPAN")},
	{EHtmlAttributeColspan,				_S16("COLSPAN")},
	{EHtmlAttributeHeight,				_S16("HEIGHT")},
	{EHtmlAttributeChecked,				_S16("CHECKED")},
	{EHtmlAttributeMaxlength,			_S16("MAXLENGTH")},
	{EHtmlAttributeSrc,					_S16("SRC")},
	{EHtmlAttributeMultiple,			_S16("MULTIPLE")},
	{EHtmlAttributeSelected,			_S16("SELECTED")},
	{EHtmlAttributeRows,				_S16("ROWS")},
	{EHtmlAttributeCols,				_S16("COLS")},
	{EHtmlAttributeAlt,					_S16("ALT")},
	{EHtmlAttributeHspace,				_S16("HSPACE")},
	{EHtmlAttributeVspace,				_S16("VSPACE")},
	{EHtmlAttributeUsemap,				_S16("USEMAP")},
	{EHtmlAttributeIsmap,				_S16("ISMAP")},
	{EHtmlAttributeCodebase,			_S16("CODEBASE")},
	{EHtmlAttributeCode,				_S16("CODE")},
	{EHtmlAttributeColor,				_S16("COLOR")},
	{EHtmlAttributeClear,				_S16("CLEAR")},
	{EHtmlAttributeShape,				_S16("SHAPE")},
	{EHtmlAttributeCoords,				_S16("COORDS")},
	{EHtmlAttributeNohref,				_S16("NOHREF")}
	};

#endif
