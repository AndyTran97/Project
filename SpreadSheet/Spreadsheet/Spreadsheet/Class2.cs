//// Written by Tuan Nguyen Tran
//// Course: CS 3500
//// Date: Sep 23, 2022
//// Assigment: PS4


//using SpreadsheetUtilities;
//using System;
//using System.Collections.Generic;
//using System.Linq;
//using System.Reflection.Metadata;
//using System.Text;
//using System.Text.RegularExpressions;
//using System.Threading.Tasks;
//using static System.Net.Mime.MediaTypeNames;

//namespace SS
//{

//    public class Spreadsheet : AbstractSpreadsheet
//    {
//        //field
//        private Dictionary<string, Cell> spreadsheet; // A dictionary to define a cell (cell's name -> cell's content)
//        private DependencyGraph graph; // dependency graph for cells' relationship

//        public Spreadsheet() : base(valid => true, s => s, "1.0")
//        {
//            spreadsheet = new Dictionary<string, Cell>();
//            graph = new DependencyGraph();
//        }

//        // ADDED FOR PS5
//        /// <summary>
//        /// True if this spreadsheet has been modified since it was created or saved                  
//        /// (whichever happened most recently); false otherwise.
//        /// </summary>
//        public override bool Changed { get => throw new NotImplementedException(); protected set => throw new NotImplementedException(); }

//        /// <summary>
//        /// If name is invalid, throws an InvalidNameException.
//        /// 
//        /// Otherwise, returns the contents (as opposed to the value) of the named cell.  The return
//        /// value should be either a string, a double, or a Formula.
//        /// </summary>
//        public override object GetCellContents(string name)
//        {
//            //Check for valid cell name
//            if (!IsValidCell(name))
//                throw new InvalidNameException();

//            //If spreadsheet contains this cell, return the content of it
//            if (spreadsheet.ContainsKey(name))
//            {
//                return spreadsheet[name].Content;
//            }

//            return string.Empty;
//        }

//        // ADDED FOR PS5
//        /// <summary>
//        /// If name is invalid, throws an InvalidNameException.
//        /// 
//        /// Otherwise, returns the value (as opposed to the contents) of the named cell.  The return
//        /// value should be either a string, a double, or a SpreadsheetUtilities.FormulaError.
//        /// </summary>
//        public override object GetCellValue(string name)
//        {
//            throw new NotImplementedException();
//        }

//        /// <summary>
//        /// Enumerates the names of all the non-empty cells in the spreadsheet.
//        /// </summary>
//        public override IEnumerable<string> GetNamesOfAllNonemptyCells()
//        {
//            return spreadsheet.Keys;
//        }

//        // ADDED FOR PS5
//        /// <summary>
//        /// Writes the contents of this spreadsheet to the named file using a JSON format.
//        /// The JSON object should have the following fields:
//        /// "Version" - the version of the spreadsheet software (a string)
//        /// "cells" - an object containing 0 or more cell objects
//        ///           Each cell object has a field named after the cell itself 
//        ///           The value of that field is another object representing the cell's contents
//        ///               The contents object has a single field called "stringForm",
//        ///               representing the string form of the cell's contents
//        ///               - If the contents is a string, the value of stringForm is that string
//        ///               - If the contents is a double d, the value of stringForm is d.ToString()
//        ///               - If the contents is a Formula f, the value of stringForm is "=" + f.ToString()
//        /// 
//        /// For example, if this spreadsheet has a version of "default" 
//        /// and contains a cell "A1" with contents being the double 5.0 
//        /// and a cell "B3" with contents being the Formula("A1+2"), 
//        /// a JSON string produced by this method would be:
//        /// 
//        /// {
//        ///   "cells": {
//        ///     "A1": {
//        ///       "stringForm": "5"
//        ///     },
//        ///     "B3": {
//        ///       "stringForm": "=A1+2"
//        ///     }
//        ///   },
//        ///   "Version": "default"
//        /// }
//        /// 
//        /// If there are any problems opening, writing, or closing the file, the method should throw a
//        /// SpreadsheetReadWriteException with an explanatory message.
//        /// </summary>
//        public override void Save(string filename)
//        {
//            throw new NotImplementedException();
//        }

//        /// <summary>
//        /// If name is invalid, throws an InvalidNameException.
//        /// 
//        /// Otherwise, the contents of the named cell becomes number.  The method returns a
//        /// list consisting of name plus the names of all other cells whose value depends, 
//        /// directly or indirectly, on the named cell.
//        /// 
//        /// For example, if name is A1, B1 contains A1*2, and C1 contains B1+A1, the
//        /// list {A1, B1, C1} is returned.
//        /// </summary>
//        protected override IList<string> SetCellContents(string name, double number)
//        {

//            //SetCellContentsHelper(name, number);
//            if (!IsValidCell(name))
//                throw new InvalidNameException();

//            //Set new content for the cell
//            SetCellContentsHelper(name, number);

//            //Replace all the variables that this cell depent on
//            graph.ReplaceDependees(name, new HashSet<string>());

//            //Recaculate for every cell related to this cell
//            return GetCellsToRecalculate(name).ToList();
//        }


//        /// <summary>
//        /// If name is invalid, throws an InvalidNameException.
//        /// 
//        /// Otherwise, the contents of the named cell becomes text.  The method returns a
//        /// list consisting of name plus the names of all other cells whose value depends, 
//        /// directly or indirectly, on the named cell.
//        /// 
//        /// For example, if name is A1, B1 contains A1*2, and C1 contains B1+A1, the
//        /// list {A1, B1, C1} is returned.
//        /// </summary>
//        protected override IList<string> SetCellContents(string name, string text)
//        {
//            if (text.Equals(""))
//                return new List<string>();

//            //SetCellContentsHelper(name, number);
//            if (!IsValidCell(name))
//                throw new InvalidNameException();

//            //Set new content for the cell
//            SetCellContentsHelper(name, text);

//            //Replace all the variables that this cell depent on
//            graph.ReplaceDependees(name, new HashSet<string>());

//            //Recaculate for every cell related to this cell
//            return GetCellsToRecalculate(name).ToList();
//        }

//        /// <summary>
//        /// If name is invalid, throws an InvalidNameException.
//        /// 
//        /// Otherwise, if changing the contents of the named cell to be the formula would cause a 
//        /// circular dependency, throws a CircularException, and no change is made to the spreadsheet.
//        /// 
//        /// Otherwise, the contents of the named cell becomes formula.  The method returns a
//        /// list consisting of name plus the names of all other cells whose value depends,
//        /// directly or indirectly, on the named cell.
//        /// 
//        /// For example, if name is A1, B1 contains A1*2, and C1 contains B1+A1, the
//        /// list {A1, B1, C1} is returned.
//        /// </summary>
//        protected override IList<string> SetCellContents(string name, Formula formula)
//        {

//            if (!IsValidCell(name))
//                throw new InvalidNameException();

//            //back up Denpendees' list of the cell
//            HashSet<string> backupDependees = (HashSet<string>)graph.GetDependees(name);

//            //Get all the variables of the formula
//            IEnumerable<string> tokens = formula.GetVariables();

//            //Replace all the variables that this cell depent on
//            graph.ReplaceDependees(name, tokens);


//            try
//            {
//                //Recaculate for every cell related to this cell
//                IEnumerable<string> cellRecalculate = GetCellsToRecalculate(name);

//                //Set new content for the cell
//                SetCellContentsHelper(name, formula);

//                return cellRecalculate.ToList();
//            }
//            catch (CircularException)
//            {
//                //If it throws CircularException, back up everything
//                graph.ReplaceDependees(name, backupDependees);
//                throw new CircularException();
//            }

//        }

//        /// <summary>
//        /// If name is invalid, throws an InvalidNameException.
//        /// 
//        /// Otherwise, if content parses as a double, the contents of the named
//        /// cell becomes that double.
//        /// 
//        /// Otherwise, if content begins with the character '=', an attempt is made
//        /// to parse the remainder of content into a Formula f using the Formula
//        /// constructor.  There are then three possibilities:
//        /// 
//        ///   (1) If the remainder of content cannot be parsed into a Formula, a 
//        ///       SpreadsheetUtilities.FormulaFormatException is thrown.
//        ///       
//        ///   (2) Otherwise, if changing the contents of the named cell to be f
//        ///       would cause a circular dependency, a CircularException is thrown,
//        ///       and no change is made to the spreadsheet.
//        ///       
//        ///   (3) Otherwise, the contents of the named cell becomes f.
//        /// 
//        /// Otherwise, the contents of the named cell becomes content.
//        /// 
//        /// If an exception is not thrown, the method returns a list consisting of
//        /// name plus the names of all other cells whose value depends, directly
//        /// or indirectly, on the named cell. The order of the list should be any
//        /// order such that if cells are re-evaluated in that order, their dependencies 
//        /// are satisfied by the time they are evaluated.
//        /// 
//        /// For example, if name is A1, B1 contains A1*2, and C1 contains B1+A1, the
//        /// list {A1, B1, C1} is returned.
//        /// </summary>
//        public override IList<string> SetContentsOfCell(string name, string content)
//        {
//            throw new NotImplementedException();
//        }

//        /// <summary>
//        /// Returns an enumeration, without duplicates, of the names of all cells whose
//        /// values depend directly on the value of the named cell.  In other words, returns
//        /// an enumeration, without duplicates, of the names of all cells that contain
//        /// formulas containing name.
//        /// 
//        /// For example, suppose that
//        /// A1 contains 3
//        /// B1 contains the formula A1 * A1
//        /// C1 contains the formula B1 + A1
//        /// D1 contains the formula B1 - C1
//        /// The direct dependents of A1 are B1 and C1
//        /// </summary>
//        protected override IEnumerable<string> GetDirectDependents(string name)
//        {
//            return graph.GetDependents(name);
//        }

//        /// <summary>
//        /// Check the pattern of what is the valid cell
//        /// </summary>
//        /// <param name="s">Cell's name</param>
//        /// <returns></returns>
//        private bool IsValidCell(string s)
//        {
//            String varPattern = @"^[a-zA-Z_](?:[a-zA-Z_]|\d)*$";
//            return Regex.IsMatch(s, varPattern);
//        }

//        /// <summary>
//        /// A helper for SetCellContents() to add or update the cell's content
//        /// </summary>
//        /// <param name="name">Cell's name</param>
//        /// <param name="content">Cell's content can be string, number, or Formula</param>
//        private void SetCellContentsHelper(string name, object content)
//        {
//            if (spreadsheet.ContainsKey(name))
//            {
//                spreadsheet[name].Content = content;
//            }
//            else //if don't, create new cell with new content
//            {
//                Cell cell = new Cell(content);
//                spreadsheet.Add(name, cell);
//            }
//        }


//        /// <summary>
//        /// Every Cell class contains the content(string, number, or Formula)
//        /// </summary>
//        private class Cell
//        {
//            public object Content { get; set; }

//            public Cell(object content)
//            {
//                Content = content;
//            }

//        }
//    }

//}
