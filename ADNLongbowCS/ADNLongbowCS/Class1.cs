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

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.Geometry;

//composed by Xiaodong Liang, Developer Technical Services, Autodesk

namespace ADNLongbowCS
{
    public class Class1
    {

        #region "send complete command"
        [CommandMethod("TestSimple")]
        public static void TestSimple()
        {
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            //call Editor.Command to send command directly 
            ed.Command(new object[] { "Line", "0,0,0", "10,10,0", "20,50,0", "" });
        }
        #endregion

        #region "send incomplete command by callback function"

        //count of callback
        private static int _Count = 0;
        //store the Id of each line
        private static ObjectIdCollection oIdArray = new ObjectIdCollection();
        

        // declare the callback delegation
        delegate void Del();
        private static Del _actionCompletedDelegate;

        // help function，check if Line command is running
        public static bool isLineActive()
        {
            String str = (String)Application.GetSystemVariable("CMDNAMES");
            if (str.Contains("LINE") == true)
            {
                return true;
            }
            return false;
        }

        // send incomplete command by callback function
        [CommandMethod("TestCallback")]
        public static void TestCallback()
        {
            _Count = 0;           

            var ed = Application.DocumentManager.MdiActiveDocument.Editor;

            // start Line command, the first line 
            Editor.CommandResult cmdResult1 = ed.CommandAsync(new object[] { "_.LINE", Editor.PauseToken, Editor.PauseToken });

            // delegate callback function, wait for interaction ends 
            _actionCompletedDelegate = new Del(CreateLinesAsyncCallback);
            cmdResult1.OnCompleted(new Action(_actionCompletedDelegate));
        }

        // callback function 
        public static void CreateLinesAsyncCallback()
        {
            // AutoCAD hands over to the callback function 
            var ed = Application.DocumentManager.MdiActiveDocument.Editor;

            //if Line command is running 
            if (isLineActive())
            { 
                if (_Count == 0)
                {
                    // get the first line ID 
                    PromptSelectionResult LastEnt = ed.SelectLast();
                    oIdArray.Add(LastEnt.Value[0].ObjectId) ;

                    // increase count 
                    _Count++;

                    // hand over to AutoCAD to execute the next interaction 
                    Editor.CommandResult cmdResult = ed.CommandAsync(Editor.PauseToken);
                    // delegate callback function, wait for interaction ends
                    _actionCompletedDelegate = new Del(CreateLinesAsyncCallback);
                    cmdResult.OnCompleted(new Action(_actionCompletedDelegate));
                }
                else if (_Count == 1)
                {
                    // get the second line ID 
                    PromptSelectionResult LastEnt = ed.SelectLast();
                    oIdArray.Add(LastEnt.Value[0].ObjectId);

                    // increase count 
                    _Count++;

                    // hand over to AutoCAD to execute the next interaction 
                    Editor.CommandResult cmdResult = ed.CommandAsync(Editor.PauseToken);
                    // delegate callback function, wait for interaction ends
                    _actionCompletedDelegate = new Del(CreateLinesAsyncCallback);
                    cmdResult.OnCompleted(new Action(_actionCompletedDelegate));
                }
            }
            else
            {
                //the end user ends the send command
                // display each id of the line
                foreach(ObjectId eachId in oIdArray)
                   ed.WriteMessage("\nline object ID: {0}", eachId.ToString());                 

            }
        }

        #endregion

        #region "send incomplete command by awaithe async"

        [CommandMethod("TestAwait")]
        public static async void TestAwait()
        { 

            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;         

            try
            {
                // start Line command.  
                //after this line, the code hands over to AutoCAD, wait for user interaction
                await ed.CommandAsync(new Object[] { "_.LINE", Editor.PauseToken, Editor.PauseToken });

                //After user completes the last interaction, this command TestAwait will be called again, but
                // begins from here: right after the last   await ed.CommandAsync
                // now we get the id of the first line
                {
                    PromptSelectionResult LastEnt = ed.SelectLast();
                    oIdArray.Add(LastEnt.Value[0].ObjectId);
                }


                // continue the next  interaction
                await ed.CommandAsync(new Object[] { Editor.PauseToken });


                //After user completes the last interaction, this command TestAwait will be called again, but
                // begins from here: right after the last   await ed.CommandAsync
                // now we get the id of the first line

                {
                    PromptSelectionResult LastEnt = ed.SelectLast();
                    oIdArray.Add(LastEnt.Value[0].ObjectId);
                }

                // end Line command
                await ed.CommandAsync(new Object[] { "" });

                // print id of each line
                foreach (ObjectId eachId in oIdArray)
                    ed.WriteMessage("line object ID: {0}\n", eachId.ToString());                 
    

                ed.WriteMessage("Done");
            }
            catch (Autodesk.AutoCAD.Runtime.Exception ex)
            {
                ed.WriteMessage(ex.Message);
            }
        }
        #endregion


    }
}
