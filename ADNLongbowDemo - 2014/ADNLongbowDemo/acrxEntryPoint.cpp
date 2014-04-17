// (C) Copyright 2002-2014 by Autodesk, Inc. 
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

//composed by Xiaodong Liang, Developer Technical Services, Autodesk

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CADNLongbowDemoApp : public AcRxArxApp {

public:
	CADNLongbowDemoApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}
	 

	 // multi-doc switching
	static void MyGroupTestGetUserInput() {
		 
		// get user input
		//while waiting, switch the document and switch back
		  AcGePoint3d  basePt = AcGePoint3d(0,0,0);
		 AcGePoint3d insPnt; 
         int res = acedGetPoint (asDblArray(basePt), L"\nSelect a Point : ", asDblArray (insPnt)); 
		 if(res == RTNORM)
		 {
			 acutPrintf(L"Corrdinate of selected points：（%f， %f，%f）",insPnt.x,insPnt.y,insPnt.z);
		 }  
	}

	// send complete command 
	static void MyGroupTestSendCommand1() {

		// send line command, provide all params,
		// generate the lines without user interaction
		acedCommand(RTSTR,_T("LINE"),
					RTSTR, _T("0,0,0"),
					RTSTR,_T("10,10,0"),
					RTSTR,_T("20,50,0"),
					RTSTR,_T(""),
					RTNONE);
	}

	//  help function for  send incomplete command
	// whether the line command is running
	static Adesk::Boolean isLineActive() 
	{ 
		struct resbuf rb; 
		acedGetVar(_T("CMDNAMES"),&rb); 
		if (_tcsstr(rb.resval.rstring, _T("LINE"))) 
			return Adesk::kTrue; 
		return Adesk::kFalse; 
	} 

	// send incomplete command:
	static void MyGroupTestSendCommand2() {
		 
		// id array  
		AcDbObjectIdArray oIdArray;
		// id of each line 
		AcDbObjectId lineId = AcDbObjectId::kNull;	 
		 
		//srart line command 
		AcDbObjectId lineId2 = AcDbObjectId::kNull;
		// wait for the user inputs start and end point of the first line 
		// there will be two PAUSE
		acedCommand(RTSTR, _T("_.LINE"), 
					RTSTR, PAUSE,
					RTSTR, PAUSE, 
					RTNONE);
		// get id of the first line 
		ads_name ename;
		acdbEntLast(ename);
		acdbGetObjectId(lineId, ename);
		oIdArray.append(lineId);
	 
		// loop, get more lines 
		while(isLineActive())
		{			
			// wait for the user inputs end point of the second line 
			int res = acedCommand(RTSTR, PAUSE, RTNONE);
			if(res != RTNORM)
				break;			
			{
				// get id of each line
				ads_name ename;
			    acdbEntLast(ename);
			    acdbGetObjectId(lineId, ename);
				oIdArray.append(lineId);				 
			} 
		}
		 
		// command ends
		//print the id of each line
		acutPrintf(L"ID of Lines \n");
		for(int i = 0;i <oIdArray.length();i++)
		{
			AcDbObjectId eachId = oIdArray[i];
			acutPrintf(L"%dLine： %I64d\n",i,eachId.asOldId());
		} 
	} 
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CADNLongbowDemoApp) 
 
ACED_ARXCOMMAND_ENTRY_AUTO(CADNLongbowDemoApp, MyGroup, TestGetUserInput,TestGetUserInput, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CADNLongbowDemoApp, MyGroup, TestSendCommand1,TestSendCommand1, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CADNLongbowDemoApp, MyGroup, TestSendCommand2,TestSendCommand2, ACRX_CMD_MODAL, NULL)
 

