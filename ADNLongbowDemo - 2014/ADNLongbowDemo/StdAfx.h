// (C) Copyright 2002-2012 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//- StdAfx.h : include file for standard system include files,
//-      or project specific include files that are used frequently,
//-      but are changed infrequently
//-----------------------------------------------------------------------------
#pragma once
#define ADNLONGBOWDEMO_MODULE

#pragma pack (push, 8)
#pragma warning(disable: 4786 4996)
//#pragma warning(disable: 4098)

//-----------------------------------------------------------------------------
#include <windows.h>

//- ObjectARX and OMF headers needs this
#include <map>

//-----------------------------------------------------------------------------
//- Include ObjectDBX/ObjectARX headers
//- Uncomment one of the following lines to bring a given library in your project.
//#define _BREP_SUPPORT_					//- Support for the BRep API
//#define _HLR_SUPPORT_						//- Support for the Hidden Line Removal API
//#define _AMODELER_SUPPORT_				//- Support for the AModeler API
//#define _ASE_SUPPORT_							//- Support for the ASI/ASE API
//#define _RENDER_SUPPORT_					//- Support for the AutoCAD Render API
//#define _ARX_CUSTOM_DRAG_N_DROP_	//- Support for the ObjectARX Drag'n Drop API
//#define _INC_LEAGACY_HEADERS_			//- Include legacy headers in this project
#include "arxHeaders.h"


//-----------------------------------------------------------------------------
#include "DocData.h" //- Your document specific data class holder

//- Declare it as an extern here so that it becomes available in all modules
extern AcApDataManager<CDocData> DocVars ;

#pragma pack (pop)

