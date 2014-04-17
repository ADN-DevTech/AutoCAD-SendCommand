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

#include "acedCmdNF.h"
#include "dbplanesurf.h"
#include "dbSubD.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("Adsk")

struct MyData1
{
	int mCount;

	AcDbLine *mpLine1Copy;
	AcDbLine *mpLine2Copy;
};

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CAsyncCommandDemoApp : public AcRxArxApp 
{

public:
	CAsyncCommandDemoApp () : AcRxArxApp () {}
	
	static AcArray<AcDbEntity*> _markers;
	static AcArray<int> viewportNumbers;
	static MyData1 _countData1;

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt)
	{
		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;		
		 
		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) 
	{
		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () 
	{
	}

   //test multi-doc switching
   //start the command, do not input the point, switch to the other
	//document. The command will be 'canceled' when switching
	static void MyGroupTestGetUserInput() {
		 
		// get user input 
		 AcGePoint3d  basePt = AcGePoint3d(0,0,0);
		 AcGePoint3d insPnt; 
         int res = acedGetPoint (asDblArray(basePt), 
				L"\nPlease select a point : ", 
				asDblArray (insPnt)); 

		 if(res == RTNORM)
		 {
			 acutPrintf(L"Coordinate of the selected point: （%f， %f，%f）",
				 insPnt.x,
				 insPnt.y,
				 insPnt.z);
		 }  
	}

	// send complete command 
	static void MyGroupTestSendCommand1(void)
	{
		// start line command. provide all params. 
		// the lines are generated without user interaction
		acedCommandS(RTSTR,_T("LINE"),
					 RTSTR, _T("0,0,0"),
					 RTSTR,_T("10,10,0"),
					 RTSTR,_T("20,50,0"),
					 RTSTR,_T(""),
					 RTNONE);
	}
 
 
	// help class for send incomplete command
	// param in callback function 
	struct callbackParam
	{
		// how many lines have been created
		int lineCount;
		// the array to store objectid of the lines 
		AcDbObjectIdArray oLineIdArray;
	};	

	 // help function for send incomplete command: 
	//  whether the line command is running
	static Adesk::Boolean isLineActive() 
	{ 
		struct resbuf rb; 
		acedGetVar(_T("CMDNAMES"),&rb); 
		if (_tcsstr(rb.resval.rstring, _T("LINE"))) 
			return Adesk::kTrue; 
		return Adesk::kFalse; 
	} 

	// send incomplete command
	static void MyGroupTestSendCommand2(void)
	{
		//declare the param class
		//it will be transferred in callback function
		static callbackParam MyData;
		MyData.lineCount = 0; 

		//start line command, 
		// execute callback function myTest1FLCallbac
		// input the param MyData
		// tells there will be two PAUSE to wait for 
		// user input
		acedCommandC(&myTest1FLCallback,
			reinterpret_cast<void *>(&MyData), 
			RTSTR, _T("_.LINE"),
			RTSTR, PAUSE, 
			RTSTR, PAUSE,
			RTNONE); 
	}

	//callback function
	static int myTest1FLCallback(void * pData)
	{
		//get the params input within the callback function
		struct callbackParam *pCountData = 
			reinterpret_cast<struct callbackParam *>(pData);

		// if the current running command is line
		if(isLineActive())
		{ 
			if(pCountData->lineCount == 0)
			{
				// get the objectId of the first line 
				AcDbObjectId lineId = AcDbObjectId::kNull;
				ads_name ename;
				acdbEntLast(ename);
				acdbGetObjectId(lineId, ename);			 
				pCountData->oLineIdArray.append(lineId);

				//keep passing the callback function until the command ends, 
				//there will be one PAUSE to get the next point 
				// to create the next line 				 
				int res = acedCommandC(&myTest1FLCallback,
					reinterpret_cast<void *>(pCountData), 
					RTSTR, PAUSE, 
					RTNONE);
				pCountData->lineCount++;
			}
			else if(pCountData->lineCount == 1)
			{
				// get the objectId of the second line 
				AcDbObjectId lineId = AcDbObjectId::kNull;
				ads_name ename;
				acdbEntLast(ename);
				acdbGetObjectId(lineId, ename);			 
				pCountData->oLineIdArray.append(lineId);

				//keep passing the callback function until the command ends,  
				//there will be one PAUSE to get the next point 
				// to create the next line 	
				int res = acedCommandC(&myTest1FLCallback, 
					reinterpret_cast<void *>(pCountData), 
					RTSTR, PAUSE, 
					RTNONE);
				pCountData->lineCount++;
			}
		}
		else
		{			
			//the user ends the line command
			 acutPrintf(L"\nline command ends");			  
			//print the id of each line 			 
			for(int i = 0;i <pCountData->oLineIdArray.length();i++)
			{
				AcDbObjectId eachId = pCountData->oLineIdArray[i];
				acutPrintf(L"\n%d line： %I64d\n",i,eachId.asOldId());
			} 
		}
		return 0;
	}

	 
};

AcArray<AcDbEntity*> CAsyncCommandDemoApp::_markers;
AcArray<int> CAsyncCommandDemoApp::viewportNumbers;
MyData1 CAsyncCommandDemoApp::_countData1;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CAsyncCommandDemoApp)
 
ACED_ARXCOMMAND_ENTRY_AUTO(CAsyncCommandDemoApp, MyGroup, TestGetUserInput,TestGetUserInput, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CAsyncCommandDemoApp, MyGroup, TestSendCommand1, TestSendCommand1, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CAsyncCommandDemoApp, MyGroup, TestSendCommand2, TestSendCommand2, ACRX_CMD_TRANSPARENT, NULL)