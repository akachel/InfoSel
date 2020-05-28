 // //////////////////////////////////////////////////////////
 // //////////////////////////////////////////////////////////
     // //////                                        ////////
     // //////          INFOrmation Theoretic         ////////
     // //////        Feature SELection Project       ////////
     // //////                                        ////////
     // //////          Library User Interface        ////////
     // //////                                        ////////
     // //////          //////////////////////////////////////
     // //////          //////////////////////////////////////
     // //////
     // ////// Copyright (C) 2010
     // //////
     // ////// Design & Programming: Adam Kachel
     // ////// Theoretical Analysis: Jacek Biesiada
     // //////
     // ////// Silesian University of Technology
     // ////// Department of M. & Computer Science
     // //////
// //////////////////////
// //////////////////////
///
/// Code Of Control Script Processing
/**
 * @file       script.cpp
 * @version    1.00
 * @author     Adam Kachel
 * @since      2002.11.12
 * @date       2009.03.20
 *
 * @contents  
 *             This file contains code for processing script text of input
 *             files used to configure, start and in general control 
 *             any job with the InfoSel++ library, which is based
 *             on an application specific scripting language.
 */

#include "comm.h"
#include "pcbas.h"
#include "data.h"
#include "dcol.h"
#include "dvec.h"
#include "dset.h"
#include "dfile.h"
#include "albas.h"
#include "aluse.h"
#include "pcuse.h"

namespace InfoSelUI {

/////////////////////////////
////
//// Helper Documentation Routine 
//// for Printing Formal Grammar
//// of Scripting Language Used 
//// in Control Files
////

void ctlfile::print_script_grammar_(ostream &out)
{
 out << "                                                                           " << endl;
 out << "The infosel++ library control scripting language is defined as follows:    " << endl;
 out << "                                                                           " << endl;
 out << "  >>> Text characters:                                                     " << endl;
 out << "                                                                           " << endl;
 out << "    text_char:                                                             " << endl;
 out << "      control_char                                                         " << endl;
 out << "      symbol_char                                                          " << endl;
 out << "      alphadigit_char                                                      " << endl;
 out << "      SPACE                                                                " << endl;
 out << "                                                                           " << endl;
 out << "    control_char: one of                                                   " << endl;
 out << "      CR LF EOL HTAB VTAB FF EOF                                           " << endl;
 out << "                                                                           " << endl;
 out << "    symbol_char: one of                                                    " << endl;
 out << "      # \\ ' \" + - * / = > | ; , .. : ( ) { } [ ]                         " << endl;
 out << "                                                                           " << endl;
 out << "    alphadigit_char:                                                       " << endl;
 out << "      alpha_char                                                           " << endl;
 out << "      digit_char                                                           " << endl;
 out << "                                                                           " << endl;
 out << "    alpha_char:                                                            " << endl;
 out << "      _                                                                    " << endl;
 out << "      lower_char                                                           " << endl;
 out << "      upper_char                                                           " << endl;
 out << "                                                                           " << endl;
 out << "    lower_char: one of                                                     " << endl;
 out << "      a..z                                                                 " << endl;
 out << "                                                                           " << endl;
 out << "    upper_char: one of                                                     " << endl;
 out << "      A..Z                                                                 " << endl;
 out << "                                                                           " << endl;
 out << "    digit_char: one of                                                     " << endl;
 out << "      0..9                                                                 " << endl;
 out << "                                                                           " << endl;
 out << "    textchar_sequence:                                                     " << endl;
 out << "      text_char                                                            " << endl;
 out << "      textchar_sequence text_char                                          " << endl;
 out << "                                                                           " << endl;
 out << "  >>> White spaces:                                                        " << endl;
 out << "                                                                           " << endl;
 out << "    white_space:                                                           " << endl;
 out << "      SPACE                                                                " << endl;
 out << "      control_char                                                         " << endl;
 out << "      comment                                                              " << endl;
 out << "                                                                           " << endl;
 out << "    comment:                                                               " << endl;
 out << "      #textchar_sequence EOL                                               " << endl;
 out << "      \\#textchar_sequence#\\                                              " << endl;
 out << "                                                                           " << endl;
 out << "  >>> Tokens:                                                              " << endl;
 out << "                                                                           " << endl;
 out << "    token:                                                                 " << endl;
 out << "      literal                                                              " << endl;
 out << "      identifier                                                           " << endl;
 out << "      keyword                                                              " << endl;
 out << "      operator                                                             " << endl;
 out << "      punctuator                                                           " << endl;
 out << "                                                                           " << endl;
 out << "    literal:                                                               " << endl;
 out << "      symbol_literal                                                       " << endl;
 out << "      character_literal                                                    " << endl;
 out << "      arithmetical_literal                                                 " << endl;
 out << "      logical_literal                                                      " << endl;
 out << "      string_literal                                                       " << endl;
 out << "                                                                           " << endl;
 out << "    symbol_literal:                                                        " << endl;
 out << "      missing_value_symbol                                                 " << endl;
 out << "      default_value_symbol                                                 " << endl;
 out << "      void_values_symbol                                                   " << endl;
 out << "                                                                           " << endl;
 out << "    missing_value_symbol:                                                  " << endl;
 out << "      missing                                                              " << endl;
 out << "                                                                           " << endl;
 out << "    default_value_symbol:                                                  " << endl;
 out << "      default                                                              " << endl;
 out << "                                                                           " << endl;
 out << "    void_values_symbol:                                                    " << endl;
 out << "      void                                                                 " << endl;
 out << "                                                                           " << endl;
 out << "    character_literal:                                                     " << endl;
 out << "      'text_char'                                                          " << endl;
 out << "                                                                           " << endl;
 out << "    arithmetical_literal:                                                  " << endl;
 out << "      integer_literal                                                      " << endl;
 out << "      real_literal                                                         " << endl;
 out << "                                                                           " << endl;
 out << "    integer_literal:                                                       " << endl;
 out << "      digit_char                                                           " << endl;
 out << "      integer_literal digit_char                                           " << endl;
 out << "                                                                           " << endl;
 out << "    real_literal:                                                          " << endl;
 out << "      fractional_literal                                                   " << endl;
 out << "      fractional_literal exponent_part                                     " << endl;
 out << "                                                                           " << endl;
 out << "    fractional_literal:                                                    " << endl;
 out << "      integer_literal.integer_literal                                      " << endl;
 out << "                                                                           " << endl;
 out << "    exponent_part:                                                         " << endl;
 out << "      exponent_prefix integer_literal                                      " << endl;
 out << "      exponent_prefix exponent_sign integer_literal                        " << endl;
 out << "                                                                           " << endl;
 out << "    exponent_prefix:                                                       " << endl;
 out << "      e                                                                    " << endl;
 out << "      E                                                                    " << endl;
 out << "                                                                           " << endl;
 out << "    exponent_sign:                                                         " << endl;
 out << "      +                                                                    " << endl;
 out << "      -                                                                    " << endl;
 out << "                                                                           " << endl;
 out << "    logical_literal:                                                       " << endl;
 out << "      false                                                                " << endl;
 out << "      true                                                                 " << endl;
 out << "                                                                           " << endl;
 out << "    string_literal:                                                        " << endl;
 out << "      \"textchar_sequence\"                                                " << endl;
 out << "                                                                           " << endl;
 out << "    identifier:                                                            " << endl;
 out << "      alpha_char                                                           " << endl;
 out << "      identifier alphadigit_char                                           " << endl;
 out << "                                                                           " << endl;
 out << "    keyword: one of                                                        " << endl;
 out << "      missing default void false true                                      " << endl;
 out << "      input output precision partition                                     " << endl;
 out << "      classpos classislast excluded notexcluded merged notmerged           " << endl;
 out << "      exec execall echo echoln                                             " << endl;
 out << "                                                                           " << endl;
 out << "    operator: one of                                                       " << endl;
 out << "      + - * / = > |                                                        " << endl;
 out << "                                                                           " << endl;
 out << "    punctuator: one of                                                     " << endl;
 out << "      ; , .. : ( ) { } [ ]                                                 " << endl;
 out << "                                                                           " << endl;
 out << "  >>> Lexical words:                                                       " << endl;
 out << "                                                                           " << endl;
 out << "    lexical_word:                                                          " << endl;
 out << "      procedure_tag                                                        " << endl;
 out << "      constant_name                                                        " << endl;
 out << "      variable_name                                                        " << endl;
 out << "                                                                           " << endl;
 out << "    procedure_tag:                                                         " << endl;
 out << "      procedure_name                                                       " << endl;
 out << "      procedure_symbol                                                     " << endl;
 out << "                                                                           " << endl;
 out << "    procedure_name, procedure_symbol:                                      " << endl;
 out << "      predeclared_identifier                                               " << endl;
 out << "                                                                           " << endl;
 out << "    constant_name, variable_name:                                          " << endl;
 out << "      declared_identifier                                                  " << endl;
 out << "                                                                           " << endl;
 out << "    predeclared_identifier, declared_identifier:                           " << endl;
 out << "      identifier                                                           " << endl;
 out << "                                                                           " << endl;
 out << "  >>> Syntax:                                                              " << endl;
 out << "                                                                           " << endl;
 out << "    script:                                                                " << endl;
 out << "      EOF                                                                  " << endl;
 out << "      statement script                                                     " << endl;
 out << "                                                                           " << endl;
 out << "    statement:                                                             " << endl;
 out << "      empty_statement                                                      " << endl;
 out << "      declaration_statement                                                " << endl;
 out << "      setting_statement                                                    " << endl;
 out << "      calling_statement                                                    " << endl;
 out << "                                                                           " << endl;
 out << "    empty_statement:                                                       " << endl;
 out << "      ;                                                                    " << endl;
 out << "                                                                           " << endl;
 out << "    declaration_statement:                                                 " << endl;
 out << "      constant_declaration                                                 " << endl;
 out << "      variable_declaration                                                 " << endl;
 out << "                                                                           " << endl;
 out << "    constant_declaration:                                                  " << endl;
 out << "      const SPACE constant_name=expression;                                " << endl;
 out << "                                                                           " << endl;
 out << "    variable_declaration:                                                  " << endl;
 out << "      var SPACE variable_name;                                             " << endl;
 out << "      var SPACE variable_name=expression;                                  " << endl;
 out << "                                                                           " << endl;
 out << "    setting_statement:                                                     " << endl;
 out << "      variable_setting                                                     " << endl;
 out << "      parameter_setting                                                    " << endl;
 out << "                                                                           " << endl;
 out << "    variable_setting:                                                      " << endl;
 out << "      set variable_name=expression;                                        " << endl;
 out << "                                                                           " << endl;
 out << "    parameter_setting:                                                     " << endl;
 out << "      input=expression;                                                    " << endl;
 out << "      output=expression;                                                   " << endl;
 out << "      precision=expression;                                                " << endl;
 out << "      partition=expression;                                                " << endl;
 out << "      classpos=expression;                                                 " << endl;
 out << "      classislast=expression;                                              " << endl;
 out << "      excluded=expression_list;                                            " << endl;
 out << "      notexcluded=expression_list;                                         " << endl;
 out << "      merged=expression_list;                                              " << endl;
 out << "      notmerged=expression_list;                                           " << endl;
 out << "                                                                           " << endl;
 out << "    calling_statement:                                                     " << endl;
 out << "      exec(expression);                                                    " << endl;
 out << "      execall;                                                             " << endl;
 out << "      echo(expression_list);                                               " << endl;
 out << "      echoln;                                                              " << endl;
 out << "      echoln(expression_list);                                             " << endl;
 out << "                                                                           " << endl;
 out << "    expression:                                                            " << endl;
 out << "      additive_expression                                                  " << endl;
 out << "                                                                           " << endl;
 out << "    additive_expression:                                                   " << endl;
 out << "      multiplicative_expression                                            " << endl;
 out << "      additive_expression+multiplicative_expression                        " << endl;
 out << "      additive_expression-multiplicative_expression                        " << endl;
 out << "                                                                           " << endl;
 out << "    multiplicative_expression:                                             " << endl;
 out << "      unary_expression                                                     " << endl;
 out << "      multiplicative_expression*unary_expression                           " << endl;
 out << "      multiplicative_expression/unary_expression                           " << endl;
 out << "                                                                           " << endl;
 out << "    unary_expression:                                                      " << endl;
 out << "      primary_expression                                                   " << endl;
 out << "      +unary_expression                                                    " << endl;
 out << "      -unary_expression                                                    " << endl;
 out << "                                                                           " << endl;
 out << "    primary_expression:                                                    " << endl;
 out << "      literal                                                              " << endl;
 out << "      identifier                                                           " << endl;
 out << "      procedure_invocation                                                 " << endl;
 out << "      invocation_pipeline                                                  " << endl;
 out << "      value_range                                                          " << endl;
 out << "      value_set                                                            " << endl;
 out << "      expression_phrase                                                    " << endl;
 out << "                                                                           " << endl;
 out << "    procedure_invocation:                                                  " << endl;
 out << "      procedure_tag                                                        " << endl;
 out << "      procedure_tag(expression_list)                                       " << endl;
 out << "                                                                           " << endl;
 out << "    invocation_pipeline:                                                   " << endl;
 out << "      procedure_invocation                                                 " << endl;
 out << "      serial_pipeline                                                      " << endl;
 out << "      parallel_pipeline                                                    " << endl;
 out << "                                                                           " << endl;
 out << "    serial_pipeline:                                                       " << endl;
 out << "      invocation_pipeline > invocation_pipeline > ...                      " << endl;
 out << "                                                                           " << endl;
 out << "    parallel_pipeline:                                                     " << endl;
 out << "      invocation_pipeline | invocation_pipeline | ...                      " << endl;
 out << "                                                                           " << endl;
 out << "    value_range:                                                           " << endl;
 out << "      [range_begin .. range_end]                                           " << endl;
 out << "      [range_begin .. range_end : range_step]                              " << endl;
 out << "                                                                           " << endl;
 out << "    value_set:                                                             " << endl;
 out << "      {set_element , set_element , ...}                                    " << endl;
 out << "                                                                           " << endl;
 out << "    range_begin, range_end, range_step, set_element:                       " << endl;
 out << "      expression                                                           " << endl;
 out << "                                                                           " << endl;
 out << "    expression_phrase:                                                     " << endl;
 out << "      (expression)                                                         " << endl;
 out << "                                                                           " << endl;
 out << "    expression_list:                                                       " << endl;
 out << "      expression                                                           " << endl;
 out << "      expression_list, expression                                          " << endl;
 out << "                                                                           " << endl;
 out << "  >>> Semantics:                                                           " << endl;
 out << "                                                                           " << endl;
 out << "    input=string_expr;                                                     " << endl;
 out << "      Sets up standard input file name for reading a data file,            " << endl;
 out << "      e.g.: input=\"sample.dat\";                                          " << endl;
 out << "                                                                           " << endl;
 out << "    output=string_expr;                                                    " << endl;
 out << "      Sets up standard output file name for writing a report file,         " << endl;
 out << "      e.g.: output=\"sample.rep\";                                         " << endl;
 out << "                                                                           " << endl;
 out << "    precision=real_expr;                                                   " << endl;
 out << "      Sets up I/O precision (>0 & <1) for cutting-off real numbers,        " << endl;
 out << "      e.g.: precision=0.001;                                               " << endl;
 out << "                                                                           " << endl;
 out << "    partition=invocation_expr;                                             " << endl;
 out << "      Sets up a data-domain partition method, which is done                " << endl;
 out << "      for a given invocation of predeclared procedure,                     " << endl;
 out << "      e.g.: partition=SamplePart(1,0.5,default);                           " << endl;
 out << "            partition=SamplePart(void);                                    " << endl;
 out << "            partition=SamplePart;                                          " << endl;
 out << "                                                                           " << endl;
 out << "    classpos=integer_expr;                                                 " << endl;
 out << "      Sets up a position index (>=0) of data-classes column,               " << endl;
 out << "      e.g.: classpos=1;                                                    " << endl;
 out << "                                                                           " << endl;
 out << "    classislast=logical_expr;                                              " << endl;
 out << "      Sets up a flag to indicate the data-classes column is last,          " << endl;
 out << "      e.g.: classislast=true;                                              " << endl;
 out << "                                                                           " << endl;
 out << "    excluded=simpletype_exprlist;                                          " << endl;
 out << "      Sets up labels of data-classes to exclude from the data file,        " << endl;
 out << "      which is done for a given list of simple type expressions,           " << endl;
 out << "      e.g.: excluded=1,0.5,\'A\',\"abc\",true;                             " << endl;
 out << "                                                                           " << endl;
 out << "    notexcluded=simpletype_exprlist;                                       " << endl;
 out << "      Sets up labels of data-classes which are not excluded                " << endl;
 out << "      from the data file but the all others are excluded,                  " << endl;
 out << "      note that the instruction: notexcluded=void; excludes                " << endl;
 out << "      all data-classes and makes the data file empty.                      " << endl;
 out << "                                                                           " << endl;
 out << "    merged=simpletype_exprlist;                                            " << endl;
 out << "      Sets up labels of data-classes to merge within the data file,        " << endl;
 out << "      which is done for a given list of simple type expressions,           " << endl;
 out << "      e.g.: merged=1,0.5,\'A\',\"abc\",true;                               " << endl;
 out << "                                                                           " << endl;
 out << "    notmerged=simpletype_exprlist;                                         " << endl;
 out << "      Sets up labels of data-classes which are not merged                  " << endl;
 out << "      within the data file but the all others are merged,                  " << endl;
 out << "      note that the instruction: notmerged=void; merges                    " << endl;
 out << "      all data-classes and makes their file column constant.               " << endl;
 out << "                                                                           " << endl;
 out << "    exec(pipeline_expr);                                                   " << endl;
 out << "      Calls selection algorithm executing, which is done                   " << endl;
 out << "      for a given pipeline/invocation of predeclared procedures,           " << endl;
 out << "      e.g.: exec(SampleAlg(1,missing,0.5));                                " << endl;
 out << "            exec(SampleAlg(void));                                         " << endl;
 out << "            exec(SampleAlg(void));                                         " << endl;
 out << "            exec(SampleAlg1(1)>SampleAlg2(0.5)|SampleAlg3(void));          " << endl;
 out << "                                                                           " << endl;
 out << "    execall;                                                               " << endl;
 out << "      Calls executing for all registered selection algorithms,             " << endl;
 out << "      e.g.: execall;                                                       " << endl;
 out << "                                                                           " << endl;
 out << "    echo(anytype_exprlist);                                                " << endl;
 out << "      Calls message text writing to the report file and standard output,   " << endl;
 out << "      prepared from a given list of any type expressions,                  " << endl;
 out << "      e.g.: echo(1,0.5,\'A\',\"abc\",true);                                " << endl;
 out << "                                                                           " << endl;
 out << "    echoln(anytype_exprlist);                                              " << endl;
 out << "      Calls message writing followed by the end-of-line terminator,        " << endl;
 out << "      e.g.: echoln;                                                        " << endl;
 out << "                                                                           " << endl;
}

} // end of InfoSelUI namespace

namespace { namespace Script_Processing_ { // the implementation of control script processor

namespace Logic_ { // underlying logic for the InfoSel++ library control

using namespace InfoSelUI;
using namespace InfoSelUI::Data;
using namespace InfoSelUI::Algorithms;

} // end of Logic_ namespace

/////////////////////////////
///
/// Auxiliary Declarations
///

/// commonly used logic elements:

using Logic_::Common::real;
using Logic_::Common::min2;
using Logic_::Common::mklower;
using Logic_::xUnexpFault;

/// internal constants:

const int txtline_maxlen_=1024; // maximal length of text line in source code
const int setopr_maxvals_=50; // maximal number of set operand values in object code
const int pipeopr_maxitems_=10; // maximal number of pipeline operand items in object code
const char *valless_txtrep_="$not a value$"; // text representation for value-less operand in object code
const char *novals_txtrep_="void"; // text representation for no-values operand list in object code
const char *symboprval_missing_="_"; // value of 'missing' symbol operand in object code
const char *symboprval_default_="-1"; // value of 'default' symbol operand in object code
const char *symboprval_void_=""; // value of 'void' symbol operand in object code
                      
/// record of text location in source code:

struct location_ { 
 static const location_ null; // null location outside any text
 int row; // current row of the text
 int col; // current column of the text

// create null location record:

 location_(): row(null.row), col(null.col)
 {}

// create location record with given row and column:

 location_(int r,int c): row(r), col(c)
 {}

// set up the location record at given row and column:

 void operator()(int r,int c)
 { row=r; col=c; }

// test whether the location is null:

 bool isnull() const
 { return row<=null.row || col<=null.col; }
 
// print the location to a given output stream:

 friend ostream &operator<<(ostream &out,const location_ &loc);
};

// location related definitions:

const location_ location_::null(0,0);

ostream &operator<<(ostream &out,const location_ &loc)
{ 
 out << '(';
 if (loc.isnull()) out << "out-of-text";
 else out << loc.row << ',' << loc.col; 
 out << ')';
 return out; 
}

/////////////////////////////
///
/// All Internal Exception Types
///

/// base of all script processor errors:

struct xScriptError_ : Logic_::xProcError {
 location_ loc;
 xScriptError_(const location_ &loc) throw(): loc(loc) {}
 xScriptError_(const location_ &loc,const Logic_::xError &src) throw(): loc(loc) { cause(src); }
 ~xScriptError_() throw() {}
 private: void printmsg_(ostream &out) const;
 private: virtual void print_msg_(ostream &out) const = 0;
};

void xScriptError_::printmsg_(ostream &out) const
{
 out << "Script error at " << loc << " : ";
 print_msg_(out);
}

/// the lexical errors:

struct xLexicalError_ : xScriptError_ { xLexicalError_(const location_ &loc): xScriptError_(loc) {} xLexicalError_(const location_ &loc,const Logic_::xError &couse): xScriptError_(loc,couse) {} };

struct xInvalidComment_ : xLexicalError_ { xInvalidComment_(const location_ &loc): xLexicalError_(loc) {} xInvalidComment_(const location_ &loc,const Logic_::xError &couse): xLexicalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xUntermChrLit_ : xLexicalError_ { xUntermChrLit_(const location_ &loc): xLexicalError_(loc) {} xUntermChrLit_(const location_ &loc,const Logic_::xError &couse): xLexicalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xStrLitTooLong_ : xLexicalError_ { xStrLitTooLong_(const location_ &loc): xLexicalError_(loc) {} xStrLitTooLong_(const location_ &loc,const Logic_::xError &couse): xLexicalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xUntermStrLit_ : xLexicalError_ { xUntermStrLit_(const location_ &loc): xLexicalError_(loc) {} xUntermStrLit_(const location_ &loc,const Logic_::xError &couse): xLexicalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xArtLitTooLong_ : xLexicalError_ { xArtLitTooLong_(const location_ &loc): xLexicalError_(loc) {} xArtLitTooLong_(const location_ &loc,const Logic_::xError &couse): xLexicalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xInvalidArtLit_ : xLexicalError_ { xInvalidArtLit_(const location_ &loc): xLexicalError_(loc) {} xInvalidArtLit_(const location_ &loc,const Logic_::xError &couse): xLexicalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xIdentTooLong_ : xLexicalError_ { xIdentTooLong_(const location_ &loc): xLexicalError_(loc) {} xIdentTooLong_(const location_ &loc,const Logic_::xError &couse): xLexicalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xUnknownTextChar_ : xLexicalError_ { xUnknownTextChar_(const location_ &loc): xLexicalError_(loc) {} xUnknownTextChar_(const location_ &loc,const Logic_::xError &couse): xLexicalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };

void xInvalidComment_::print_msg_(ostream &out) const
{
 out << "Invalid comment format";
}

void xUntermChrLit_::print_msg_(ostream &out) const
{
 out << "Unterminated character literal";
}

void xStrLitTooLong_::print_msg_(ostream &out) const
{
 out << "String literal too long";
}

void xUntermStrLit_::print_msg_(ostream &out) const
{
 out << "Unterminated string literal";
}

void xArtLitTooLong_::print_msg_(ostream &out) const
{
 out << "Arithmetical literal too long";
}

void xInvalidArtLit_::print_msg_(ostream &out) const
{
 out << "Invalid arithmetical literal format";
}

void xIdentTooLong_::print_msg_(ostream &out) const
{
 out << "Identifier too long";
}

void xUnknownTextChar_::print_msg_(ostream &out) const
{
 out << "Unknown text character";
}

/// the syntactical errors:

struct xSyntacticalError_ : xScriptError_ { xSyntacticalError_(const location_ &loc): xScriptError_(loc) {} xSyntacticalError_(const location_ &loc,const Logic_::xError &couse): xScriptError_(loc,couse) {} };

struct xInitSettExp_ : xSyntacticalError_ { xInitSettExp_(const location_ &loc): xSyntacticalError_(loc) {} xInitSettExp_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xCurrSettExp_ : xSyntacticalError_ { xCurrSettExp_(const location_ &loc): xSyntacticalError_(loc) {} xCurrSettExp_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xOprExprExp_ : xSyntacticalError_ { xOprExprExp_(const location_ &loc): xSyntacticalError_(loc) {} xOprExprExp_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xUnexpOprExpr_ : xSyntacticalError_ { xUnexpOprExpr_(const location_ &loc): xSyntacticalError_(loc) {} xUnexpOprExpr_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xUntermStat_ : xSyntacticalError_ { xUntermStat_(const location_ &loc): xSyntacticalError_(loc) {} xUntermStat_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xInvalidStat_ : xSyntacticalError_ { xInvalidStat_(const location_ &loc): xSyntacticalError_(loc) {} xInvalidStat_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xIdentExp_ : xSyntacticalError_ { xIdentExp_(const location_ &loc): xSyntacticalError_(loc) {} xIdentExp_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xUnknownIdent_ : xSyntacticalError_ { xUnknownIdent_(const location_ &loc): xSyntacticalError_(loc) {} xUnknownIdent_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xInvalidPipe_ : xSyntacticalError_ { xInvalidPipe_(const location_ &loc): xSyntacticalError_(loc) {} xInvalidPipe_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xRngSepExp_ : xSyntacticalError_ { xRngSepExp_(const location_ &loc): xSyntacticalError_(loc) {} xRngSepExp_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xMissRngEnd_ : xSyntacticalError_ { xMissRngEnd_(const location_ &loc): xSyntacticalError_(loc) {} xMissRngEnd_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xInvalidRng_ : xSyntacticalError_ { xInvalidRng_(const location_ &loc): xSyntacticalError_(loc) {} xInvalidRng_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xMissSetEnd_ : xSyntacticalError_ { xMissSetEnd_(const location_ &loc): xSyntacticalError_(loc) {} xMissSetEnd_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xInvalidSet_ : xSyntacticalError_ { xInvalidSet_(const location_ &loc): xSyntacticalError_(loc) {} xInvalidSet_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xExprBegExp_ : xSyntacticalError_ { xExprBegExp_(const location_ &loc): xSyntacticalError_(loc) {} xExprBegExp_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xMissExprEnd_ : xSyntacticalError_ { xMissExprEnd_(const location_ &loc): xSyntacticalError_(loc) {} xMissExprEnd_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xTooManyExprItems_ : xSyntacticalError_ { xTooManyExprItems_(const location_ &loc): xSyntacticalError_(loc) {} xTooManyExprItems_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xExprItemExp_ : xSyntacticalError_ { xExprItemExp_(const location_ &loc): xSyntacticalError_(loc) {} xExprItemExp_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xInvalidExpr_ : xSyntacticalError_ { xInvalidExpr_(const location_ &loc): xSyntacticalError_(loc) {} xInvalidExpr_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };

void xInitSettExp_::print_msg_(ostream &out) const
{
 out << "Initial setting expected";
}

void xCurrSettExp_::print_msg_(ostream &out) const
{
 out << "Current setting expected";
}

void xOprExprExp_::print_msg_(ostream &out) const
{
 out << "Operand expression expected";
}

void xUnexpOprExpr_::print_msg_(ostream &out) const
{
 out << "Unexpected operand expression";
}

void xUntermStat_::print_msg_(ostream &out) const
{
 out << "Unterminated statement";
}

void xInvalidStat_::print_msg_(ostream &out) const
{
 out << "Invalid statement";
}

void xIdentExp_::print_msg_(ostream &out) const
{
 out << "Identifier expected";
}

void xUnknownIdent_::print_msg_(ostream &out) const
{
 out << "Unknown identifier";
}

void xInvalidPipe_::print_msg_(ostream &out) const
{
 out << "Invalid invocation pipeline format";
}

void xRngSepExp_::print_msg_(ostream &out) const
{
 out << "Value range seperator expected";
}

void xMissRngEnd_::print_msg_(ostream &out) const
{
 out << "Invalid expression or missing right bracket at the end of a value range";
}

void xInvalidRng_::print_msg_(ostream &out) const
{
 out << "Invalid value range format";
}

void xMissSetEnd_::print_msg_(ostream &out) const
{
 out << "Invalid expression or missing right parenthesis at the end of a value set";
}

void xInvalidSet_::print_msg_(ostream &out) const
{
 out << "Invalid value set format";
}

void xExprBegExp_::print_msg_(ostream &out) const
{
 out << "Left parenthesis expected at the beginning of an expression phrase";
}

void xMissExprEnd_::print_msg_(ostream &out) const
{
 out << "Invalid expression or missing right parenthesis at the end of an expression phrase";
}

void xTooManyExprItems_::print_msg_(ostream &out) const
{
 out << "Too many expression list items";
}

void xExprItemExp_::print_msg_(ostream &out) const
{
 out << "Expression list item expected";
}

void xInvalidExpr_::print_msg_(ostream &out) const
{
 out << "Invalid expression";
}

/// the semantical errors:

struct xSemanticalError_ : xScriptError_ { xSemanticalError_(const location_ &loc): xScriptError_(loc) {} xSemanticalError_(const location_ &loc,const Logic_::xError &couse): xScriptError_(loc,couse) {} };

struct xIllegalStat_ : xSyntacticalError_ { xIllegalStat_(const location_ &loc): xSyntacticalError_(loc) {} xIllegalStat_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xIllegalExpr_ : xSyntacticalError_ { xIllegalExpr_(const location_ &loc): xSyntacticalError_(loc) {} xIllegalExpr_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xSymbOperReq_ : xSyntacticalError_ { xSymbOperReq_(const location_ &loc): xSyntacticalError_(loc) {} xSymbOperReq_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xSymbTypeOperReq_ : xSyntacticalError_ { xSymbTypeOperReq_(const location_ &loc): xSyntacticalError_(loc) {} xSymbTypeOperReq_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xCharOperReq_ : xSyntacticalError_ { xCharOperReq_(const location_ &loc): xSyntacticalError_(loc) {} xCharOperReq_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xCharTypeOperReq_ : xSyntacticalError_ { xCharTypeOperReq_(const location_ &loc): xSyntacticalError_(loc) {} xCharTypeOperReq_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xBoolOperReq_ : xSyntacticalError_ { xBoolOperReq_(const location_ &loc): xSyntacticalError_(loc) {} xBoolOperReq_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xBoolTypeOperReq_ : xSyntacticalError_ { xBoolTypeOperReq_(const location_ &loc): xSyntacticalError_(loc) {} xBoolTypeOperReq_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xIntOperReq_ : xSyntacticalError_ { xIntOperReq_(const location_ &loc): xSyntacticalError_(loc) {} xIntOperReq_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xIntTypeOperReq_ : xSyntacticalError_ { xIntTypeOperReq_(const location_ &loc): xSyntacticalError_(loc) {} xIntTypeOperReq_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xRealOperReq_ : xSyntacticalError_ { xRealOperReq_(const location_ &loc): xSyntacticalError_(loc) {} xRealOperReq_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xRealTypeOperReq_ : xSyntacticalError_ { xRealTypeOperReq_(const location_ &loc): xSyntacticalError_(loc) {} xRealTypeOperReq_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xStrOperReq_ : xSyntacticalError_ { xStrOperReq_(const location_ &loc): xSyntacticalError_(loc) {} xStrOperReq_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xStrTypeOperReq_ : xSyntacticalError_ { xStrTypeOperReq_(const location_ &loc): xSyntacticalError_(loc) {} xStrTypeOperReq_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xInvoOperReq_ : xSyntacticalError_ { xInvoOperReq_(const location_ &loc): xSyntacticalError_(loc) {} xInvoOperReq_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xInvoTypeOperReq_ : xSyntacticalError_ { xInvoTypeOperReq_(const location_ &loc): xSyntacticalError_(loc) {} xInvoTypeOperReq_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xPipeOperReq_ : xSyntacticalError_ { xPipeOperReq_(const location_ &loc): xSyntacticalError_(loc) {} xPipeOperReq_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xPipeTypeOperReq_ : xSyntacticalError_ { xPipeTypeOperReq_(const location_ &loc): xSyntacticalError_(loc) {} xPipeTypeOperReq_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xPartMethReq_ : xSyntacticalError_ { xPartMethReq_(const location_ &loc): xSyntacticalError_(loc) {} xPartMethReq_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xSelAlgReq_ : xSyntacticalError_ { xSelAlgReq_(const location_ &loc): xSyntacticalError_(loc) {} xSelAlgReq_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xMultiNameDecl_ : xSyntacticalError_ { xMultiNameDecl_(const location_ &loc): xSyntacticalError_(loc) {} xMultiNameDecl_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xDeclConstSett_ : xSyntacticalError_ { xDeclConstSett_(const location_ &loc): xSyntacticalError_(loc) {} xDeclConstSett_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xUndeclVarSett_ : xSyntacticalError_ { xUndeclVarSett_(const location_ &loc): xSyntacticalError_(loc) {} xUndeclVarSett_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xUninitVarUse_ : xSyntacticalError_ { xUninitVarUse_(const location_ &loc): xSyntacticalError_(loc) {} xUninitVarUse_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xIllegalPos_ : xSyntacticalError_ { xIllegalPos_(const location_ &loc): xSyntacticalError_(loc) {} xIllegalPos_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xIllegalAdd_ : xSyntacticalError_ { xIllegalAdd_(const location_ &loc): xSyntacticalError_(loc) {} xIllegalAdd_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xIllegalNeg_ : xSyntacticalError_ { xIllegalNeg_(const location_ &loc): xSyntacticalError_(loc) {} xIllegalNeg_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xIllegalSub_ : xSyntacticalError_ { xIllegalSub_(const location_ &loc): xSyntacticalError_(loc) {} xIllegalSub_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xIllegalMul_ : xSyntacticalError_ { xIllegalMul_(const location_ &loc): xSyntacticalError_(loc) {} xIllegalMul_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xIllegalDiv_ : xSyntacticalError_ { xIllegalDiv_(const location_ &loc): xSyntacticalError_(loc) {} xIllegalDiv_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xIllegalRng_ : xSyntacticalError_ { xIllegalRng_(const location_ &loc): xSyntacticalError_(loc) {} xIllegalRng_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xIllegalSet_ : xSyntacticalError_ { xIllegalSet_(const location_ &loc): xSyntacticalError_(loc) {} xIllegalSet_(const location_ &loc,const Logic_::xError &couse): xSyntacticalError_(loc,couse) {} private: void print_msg_(ostream &out) const; };

void xIllegalStat_::print_msg_(ostream &out) const
{
 out << "Illegal statement";
}

void xIllegalExpr_::print_msg_(ostream &out) const
{
 out << "Illegal expression";
}

void xSymbOperReq_::print_msg_(ostream &out) const
{
 out << "Singleton symbol operand required";
}

void xSymbTypeOperReq_::print_msg_(ostream &out) const
{
 out << "Any symbol-type operand required";
}

void xCharOperReq_::print_msg_(ostream &out) const
{
 out << "Singleton character operand required";
}

void xCharTypeOperReq_::print_msg_(ostream &out) const
{
 out << "Any character-type operand required";
}

void xBoolOperReq_::print_msg_(ostream &out) const
{
 out << "Singleton bool operand required";
}

void xBoolTypeOperReq_::print_msg_(ostream &out) const
{
 out << "Any bool-type operand required";
}

void xRealOperReq_::print_msg_(ostream &out) const
{
 out << "Singleton real operand required";
}

void xRealTypeOperReq_::print_msg_(ostream &out) const
{
 out << "Any real-type operand required";
}

void xIntOperReq_::print_msg_(ostream &out) const
{
 out << "Singleton integer operand required";
}

void xIntTypeOperReq_::print_msg_(ostream &out) const
{
 out << "Any integer-type operand required";
}

void xStrOperReq_::print_msg_(ostream &out) const
{
 out << "Singleton string operand required";
}

void xStrTypeOperReq_::print_msg_(ostream &out) const
{
 out << "Any string-type operand required";
}

void xInvoOperReq_::print_msg_(ostream &out) const
{
 out << "Singleton invocation operand required";
}

void xInvoTypeOperReq_::print_msg_(ostream &out) const
{
 out << "Any invocation-type operand required";
}

void xPipeOperReq_::print_msg_(ostream &out) const
{
 out << "Singleton pipeline operand required";
}

void xPipeTypeOperReq_::print_msg_(ostream &out) const
{
 out << "Any pipeline-type operand required";
}

void xPartMethReq_::print_msg_(ostream &out) const
{
 out << "Partition method required";
}

void xSelAlgReq_::print_msg_(ostream &out) const
{
 out << "Selection algorithm required";
}

void xMultiNameDecl_::print_msg_(ostream &out) const
{
 out << "Multiple name declaration";
}

void xDeclConstSett_::print_msg_(ostream &out) const
{
 out << "Declared constant setting";
}

void xUndeclVarSett_::print_msg_(ostream &out) const
{
 out << "Undeclared variable setting";
}

void xUninitVarUse_::print_msg_(ostream &out) const
{
 out << "Uninitialized variable use";
}

void xIllegalPos_::print_msg_(ostream &out) const
{
 out << "Illegal unary positiveness operation";
}

void xIllegalAdd_::print_msg_(ostream &out) const
{
 out << "Illegal addition";
}

void xIllegalNeg_::print_msg_(ostream &out) const
{
 out << "Illegal negation";
}

void xIllegalSub_::print_msg_(ostream &out) const
{
 out << "Illegal subtraction";
}

void xIllegalMul_::print_msg_(ostream &out) const
{
 out << "Illegal multiplication";
}

void xIllegalDiv_::print_msg_(ostream &out) const
{
 out << "Illegal division";
}

void xIllegalRng_::print_msg_(ostream &out) const
{
 out << "Illegal value range";
}

void xIllegalSet_::print_msg_(ostream &out) const
{
 out << "Illegal value set";
}

/// the executive errors:

struct xExecutiveError_ : xScriptError_ { xExecutiveError_(const location_ &loc): xScriptError_(loc) {} xExecutiveError_(const location_ &loc,const Logic_::xError &couse): xScriptError_(loc,couse) {} };

struct xZeroDiv_ : xExecutiveError_ { xZeroDiv_(const location_ &loc): xExecutiveError_(loc) {} xZeroDiv_(const location_ &loc,const Logic_::xError &couse): xExecutiveError_(loc,couse) {} private: void print_msg_(ostream &out) const; };
struct xMemoryFail_ : xExecutiveError_ { xMemoryFail_(const location_ &loc): xExecutiveError_(loc) {} xMemoryFail_(const location_ &loc,const Logic_::xError &couse): xExecutiveError_(loc,couse) {} private: void print_msg_(ostream &out) const; };

void xZeroDiv_::print_msg_(ostream &out) const
{
 out << "Division by zero";
}

void xMemoryFail_::print_msg_(ostream &out) const
{
 out << "Memory allocation failure";
}

/////////////////////////////
///
/// Library of All Standard
/// Entities with Predeclared 
/// Source Code Identifiers
///

class stdlibrary_ {
 // convert a given string to source code identifier according with this script grammar:
  static string toident_(string str);
  
 // test whether an entity is tagged:
  static bool istagged_(int group,int slot,const string &tag);
//@{
 private: //! <b> Not implemented/allowed. </b> 
  stdlibrary_();
  stdlibrary_(stdlibrary_ &);
  void operator=(stdlibrary_ &);
//@}
 public:
/// set of identified entity groups:
 
  enum {
   egNull=-1,
   egPartMeth, 
   egSelAlg,
   egCount
  };
  
  static int ngroups() 
  { return egCount; }

/// return number of slots for entities in a given group:

  static int nslots(int group); 

/// test whether a given entity is identified by a given tag:

  static bool istagged(int group,int slot,const string &tag)
  { string t=toident_(tag); return istagged_(group,slot,t); }

/// return entity placement for a given tag:

  static void place(const string &tag,int &group,int &slot);

/// find entity with a given tag and return its placement:

  static bool find(const string &tag,int &group,int &slot)
  { int g,s; place(tag,g,s); return g!=egNull ? (group=g, slot=s, true) : false; }
};

/// std. library related definitions:

string stdlibrary_::toident_(string str)
{ 
 string::iterator b=str.begin(); 
 string::iterator e=str.end(); 
 string::iterator d=b; 
 while (d!=e) {
  string::iterator c=d;
  d++;
  mklower(*c);  
  if (c==b && isdigit(*c)) str.erase(c);
  else if (*c==' ' || *c=='-') *c='_';
  else if (*c!='_' && !isalnum(*c)) str.erase(c);
 }
 return str;
}

int stdlibrary_::nslots(int group) 
{ 
 switch (group) {
  case egPartMeth: {
   int ncodes=Logic_::PartMethSet::count();
   return ncodes;
  } 
  case egSelAlg: {
   int nids=Logic_::SelAlgSet.count();
   return nids;
  } 
  default: return 0; 
 }
} 

bool stdlibrary_::istagged_(int group,int slot,const string &tag)
{
 switch (group) {
  case egPartMeth: {
   int code=slot;
   if (code<0 || code>=Logic_::PartMethSet::count()) return false;
   return tag==toident_(Logic_::PartMethSet::name(code));
  } 
  case egSelAlg: {
   int idx=slot;
   if (idx<0 || idx>=Logic_::SelAlgSet.count()) return false;
   const Logic_::Algorithm &alg=Logic_::SelAlgSet[idx];
   string t=tag;
   alg.cutvariant(t);
   return t==toident_(alg.symbol()) || t==toident_(alg.qsymbol()) || t==toident_(alg.name()) || t==toident_(alg.qname());
  } 
  default: return false; 
 }
}

void stdlibrary_::place(const string &tag,int &group,int &slot)
{ 
 group=egNull;
 slot=nslots(group)-1;
 string t=toident_(tag);
 for (int g=0; g<ngroups(); g++)
 for (int s=0; s<nslots(g); s++)
 if (istagged_(g,s,t)) {
  group=g;
  slot=s; 
  return;
 }
}

/////////////////////////////
///
/// Segment of All Main Data 
/// Processed by Object Code 
///

class datasegment_ {
  const string header_; // report file header
  string hdnote_; // report file head-note
  mutable bool is_hdr_,is_hnt_; // flags of whether header strings are printed to the report file
  string infn_,outfn_; // input/output file names
  Logic_::datfile::parsrec dpars_; // parameters for preprocessing any input data file
  mutable Logic_::datfile *datf_; // current input data file
  mutable Logic_::repfile *repf_; // current output report file
  ostream &ctllog_; // control log stream
  int execnt_; // count of algorithm executions
 
 // do initialize the segment object:
  void initialize_();
  
 // open/close current input data file: 
  void open_datfile_() const;
  void close_datfile_() const;

 // open/close current output report file: 
  void open_repfile_() const;
  void close_repfile_() const;
//@{
 private: //! <b> Not implemented/allowed. </b>
  datasegment_(datasegment_ &);
  void operator=(datasegment_ &);
//@}
 public: 
/// create a segment object for the processing with given head info and control log:

  datasegment_(const string &header,ostream &ctllog):
   header_(header), ctllog_(ctllog)
  {
   datf_=0; repf_=0;   
   initialize_();
  }

/// destroy the segment object:

  ~datasegment_()
  {
   close_datfile_();
   close_repfile_();
  }

/// reinitialize the segment object to perform a new processing:

  void reinitialize()
  {
   close_datfile_();
   close_repfile_();
   initialize_();
  }
  
/// parts of a head report section with info about the processing:

  const string &header() const
  { return header_; }
  
  void sethdnote(string n)
  { if (hdnote_!=n) { close_repfile_(); hdnote_=n; is_hnt_=!n.empty(); } }
  
  void apphdnote(string n)
  { if (!n.empty()) { close_repfile_(); hdnote_+=n; is_hnt_=true; } }
  
  const string &hdnote() const
  { return hdnote_; }
  
/// names of I/O files being the standard input/output for the processing:

  void setinfn(const string &n)
  { if (infn_!=n) { close_datfile_(); infn_=n; } }
  
  const string &infn() const
  { return infn_; }
  
  void setoutfn(const string &n)
  { if (outfn_!=n) { close_repfile_(); outfn_=n; } }
  
  const string &outfn() const
  { return outfn_; }
  
/// parameters of input data preprocessing:

  void setprec(real p)
  { if (dpars_.prec!=p) { close_datfile_(); close_repfile_(); dpars_.prec=p; } }
  
  real prec() const
  { return dpars_.prec; }
  
  void setpmth(const Logic_::partmeth_t &meth)
  { if (dpars_.pmth!=meth) { close_datfile_(); dpars_.pmth=meth; } }

  void setpmth(int code,const string &pars)
  { setpmth( Logic_::partmeth_t(code,pars) ); }

  const Logic_::partmeth_t &pmth() const
  { return dpars_.pmth; }
  
  void setcpos(int p)
  { if (dpars_.cpos!=p) { close_datfile_(); if (p>=0) dpars_.cpos=p, dpars_.lastc=false; else dpars_.lastc=true; } }
  
  int cpos() const
  { return dpars_.cpos; }
  
  void setlastc(bool l)
  { if (dpars_.lastc!=l) { close_datfile_(); dpars_.lastc=l; } }
  
  bool lastc() const
  { return dpars_.lastc; }
  
  void setexcl(const string &e)
  { if (dpars_.excl!=e) { close_datfile_(); dpars_.excl=e; } }

  const string &excl() const
  { return dpars_.excl; }

  void setnote(bool n)
  { if (dpars_.note!=n) { close_datfile_(); dpars_.note=n; } }
  
  bool note() const
  { return dpars_.note; }
  
  void setmerg(const string &m)
  { if (dpars_.merg!=m) { close_datfile_(); dpars_.merg=m; } }

  const string &merg() const
  { return dpars_.merg; }

  void setnotm(bool n)
  { if (dpars_.notm!=n) { close_datfile_(); dpars_.notm=n; } }

  bool notm() const
  { return dpars_.notm; }
  
/// current input data file:

  Logic_::datfile &datf()
  { if (!datf_) open_datfile_(); return *datf_; }

  const Logic_::datfile &datf() const
  { if (!datf_) open_datfile_(); return *datf_; }
  
/// current output report file:

  Logic_::repfile &repf()
  { if (!repf_) open_repfile_(); return *repf_; }

  const Logic_::repfile &repf() const
  { if (!repf_) open_repfile_(); return *repf_; }

/// auxiliary output streams for writing report/control messages and logs:

  ostream &reperr()
  { return repf().err(); }

  ostream &replog()
  { return repf().log(); }

  ostream &ctllog()
  { return ctllog_; }

/// auxiliary elements for executing selection algorithms:

  void exedone(bool succeeded);

  int execnt() const
  { return execnt_; }
};

/// data segment related definitions:

void datasegment_::initialize_()
{
 is_hdr_=!header_.empty();
 hdnote_="";
 is_hnt_=false;
 infn_="default.in";
 outfn_="default.out";
 dpars_();
 execnt_=0;
}

void datasegment_::open_datfile_() const //! \pre datf_==0
{
 if (!repf_) open_repfile_();
 datf_=new Logic_::datfile(infn_,dpars_);
 datf_->initiate(*repf_);
 ctllog_ << endl << "opened input data file: " << infn_ << endl;
}

void datasegment_::close_datfile_() const
{
 if (!datf_) return;
 if (!repf_) open_repfile_();
 datf_->finalize(*repf_);
 delete datf_;
 datf_=0;
}

void datasegment_::open_repfile_() const //! \pre repf_==0
{
 repf_=new Logic_::repfile(outfn_,ios::out|ios::app);
 if (is_hdr_ || is_hnt_) {
  ostream &out=repf_->section("Control Script",-1).log();
  if (is_hdr_) {
   is_hdr_=false;
   out << endl << header_ << endl;
  } 
  if (is_hnt_) {
   is_hnt_=false;
   out << endl << hdnote_ << endl;
  } 
 } 
 ctllog_ << endl << "opened output report file: " << outfn_ << endl;
}

void datasegment_::close_repfile_() const
{
 if (!repf_) return;
 close_datfile_();
 delete repf_;
 repf_=0;
}

void datasegment_::exedone(bool succeeded)
{ 
 if (succeeded) execnt_++; 
 if (!repf_ || !datf_) return;
 datf_->reinit(*repf_);
}

/////////////////////////////
///
/// Object Code Operand 
/// for Holding Values 
/// of Various Types 
///

class operand_ptr_;
class opr_rangemult_;
class opr_setmult_;

class operand_ { 
  friend class operand_ptr_;
 protected: 
  location_ loc_;
 private: 
  short lcnt_;
  void link_()
  { if (++lcnt_==1) ecreate_(); }
  void unlink_()
  { if (--lcnt_==0) destroy_(); }
 private:
  virtual void ecreate_() {}
  virtual void destroy_() { delete this; }

  virtual int compcount_() const = 0;
  virtual int valcount_() const = 0;
  virtual void selfirstval_() = 0;
  virtual bool selnextval_() = 0;
  virtual void printval_(ostream &out) const = 0;
  virtual void printvals_(ostream &out) const = 0;
  virtual opr_rangemult_ *newvalrange_() const = 0;
  virtual opr_setmult_ *newvalset_() const = 0;

  virtual operand_ *do_dup_(const location_ &loc) const { return 0; }
  virtual operand_ *do_pos_(const location_ &loc) const { return 0; }
  virtual operand_ *do_neg_(const location_ &loc) const { return 0; }
  virtual operand_ *do_add_(const operand_ &x) const { return 0; }
  virtual operand_ *do_sub_(const operand_ &x) const { return 0; }
  virtual operand_ *do_mul_(const operand_ &x) const { return 0; }
  virtual operand_ *do_div_(const operand_ &x) const { return 0; }
//@{
 private: //! <b> Not implemented/allowed. </b>
  operand_(operand_ &);
  void operator=(operand_ &);
//@}
 protected:
  operand_()
  {}
 public:
/// create/destroy operand object:
 
  explicit operand_(const location_ &loc): loc_(loc), lcnt_(0)
  {}

  virtual ~operand_()
  {
   if (lcnt_!=0) throw xMemoryFail_(loc_);
  }

/// return / set up source code location for the operand:
 
  const location_ &loc() const
  { return loc_; }

  void setloc(const location_ &loc)
  { loc_=loc; }

/// return number of components (sub-operands) for the complex operand or 1 for the simple one:
 
  int compcount() const
  { return compcount_(); }
  
/// return number of values for the multiple operand or 1 for the singleton one:
//
//! \note Single complex
//! operands can have number 
//! of values greater than one 
//! depending on the type of 
//! their components.
// 
  int valcount() const
  { return valcount_(); }
  
/// select the first/next value of the multiple operand for iterating all its values:
 
  void selfirstval()
  { selfirstval_(); } 
  
  bool selnextval()
  { return selnextval_(); }

/// print the actual value of the operand:
 
  void printval(ostream &out) const
  { printval_(out); }

/// print all values of the multiple operand in the form of source code expression:
 
  void printvals(ostream &out) const
  { printvals_(out); }

/// create a new multiple operand expressed as a value-range/set, starting from the actual value of the operand:
 
  opr_rangemult_ *newvalrange() const
  { return newvalrange_(); }

  opr_setmult_ *newvalset() const
  { return newvalset_(); }

/// perform duplication and math operations on given operands:
 
  friend operand_ *dup_(const location_ &loc,const operand_ &x) { return x.do_dup_(loc); }
  friend operand_ *pos_(const location_ &loc,const operand_ &x) { return x.do_pos_(loc); }
  friend operand_ *neg_(const location_ &loc,const operand_ &x) { return x.do_neg_(loc); }
  friend operand_ *add_(const operand_ &x,const operand_ &y) { return x.do_add_(y); }
  friend operand_ *sub_(const operand_ &x,const operand_ &y) { return x.do_sub_(y); }
  friend operand_ *mul_(const operand_ &x,const operand_ &y) { return x.do_mul_(y); }
  friend operand_ *div_(const operand_ &x,const operand_ &y) { return x.do_div_(y); }
};

/// null operand:

class opr_null_ : public virtual operand_ { 
 private:
  int compcount_() const
  { return 0; }
  int valcount_() const
  { return 0; }
  void selfirstval_()
  {}
  bool selnextval_()
  { return false; }
  void printval_(ostream &out) const
  { out << valless_txtrep_; }
  void printvals_(ostream &out) const
  { out << valless_txtrep_; }
  opr_rangemult_ *newvalrange_() const
  { return 0; }
  opr_setmult_ *newvalset_() const
  { return 0; }
 public:
  opr_null_(): operand_(location_::null)
  {}
};

/// operand pointer:

class operand_ptr_ {
  operand_ *ptr_;
  void link_()
  { ptr_->link_(); }
  void unlink_()
  { ptr_->unlink_(); }
 public:
  operand_ptr_(): ptr_(new opr_null_) 
  { link_(); }
  operand_ptr_(operand_ *p): ptr_(p ? p : new opr_null_) 
  { link_(); }
  operand_ptr_(const operand_ptr_ &x): ptr_(x.ptr_) 
  { link_(); }
  ~operand_ptr_() 
  { unlink_(); }
  operand_ptr_ &operator=(operand_ *p)
  { if (ptr_!=p) unlink_(), ptr_ = p ? p : new opr_null_, link_(); return *this; }
  operand_ptr_ &operator=(const operand_ptr_ &x)
  { if (ptr_!=x.ptr_) unlink_(), ptr_=x.ptr_, link_(); return *this; }

  bool operator!() const
  { return dynamic_cast<opr_null_ *>(ptr_)!=0; }

  operand_ *operator()()
  { return ptr_; }
  const operand_ *operator()() const
  { return ptr_; }
  operand_ &operator*()
  { return *ptr_; }
  const operand_ &operator*() const
  { return *ptr_; }
  operand_ *operator->()
  { return ptr_; }
  const operand_ *operator->() const
  { return ptr_; }
};

/// list of operands:

class operand_list_ {
  struct rec_ {
   operand_ptr_ ptr;
   rec_ *prev, *next;
   rec_(): prev(0), next(0)
   {}
  };
  int cnt_;
  rec_ *first_, *last_;
 public:
// create/copy/destroy list object:
 
  operand_list_();
  operand_list_(const operand_list_ &xx);
  ~operand_list_();
  operand_list_ &operator=(const operand_list_ &xx);

// insert a new operand item to the list:
  
  void insert(operand_ptr_ x);

// return info about number of operand items in the list:

  int count() const
  { return cnt_; }

  bool empty() const
  { return cnt_<=0; }

// return number of value-tuples combined from all values of operand items in the list:

  int valtuplecount() const;

// select the first/next value-tuple in the list for iterating all its tuples:
  
  void selfirstvaltuple();
  bool selnextvaltuple();

// print the actual value-tuple of the list with a given item separation:
  
  void printvaltuple(ostream &out,const char *sep=" ") const;

// print all values (actual or multiple) of operand items in the list, using the source code format:
  
  void printvals(ostream &out,bool multi=false) const;
};

// operand list related definitions:

operand_list_::operand_list_(): first_(0), last_(0), cnt_(0)
{}

operand_list_::operand_list_(const operand_list_ &xx): first_(0), last_(0), cnt_(0)
{
 rec_ *r=xx.first_;
 while (r) {
  insert(r->ptr);
  r=r->next;
 }
}

operand_list_::~operand_list_()
{
 while (first_) {
  rec_ *r=first_;
  first_=first_->next;
  delete r;
 }
}

operand_list_ &operand_list_::operator=(const operand_list_ &xx)
{
 if (&xx==this) return *this;
 while (first_) {
  rec_ *r=first_;
  first_=first_->next;
  delete r;
 }
 first_=0; last_=0; cnt_=0;
 rec_ *r=xx.first_;
 while (r) {
  insert(r->ptr);
  r=r->next;
 }
 return *this;
}

void operand_list_::insert(operand_ptr_ x)
{ 
 if (!x) return;
 rec_ *r=new rec_;
 r->ptr=x;
 if (!first_) first_=last_=r;
 else {
  last_->next=r;
  r->prev=last_;
  last_=r;
 }
 cnt_++;
}

int operand_list_::valtuplecount() const
{ 
 rec_ *r=first_;
 int n=1;
 while (r) {
  n*=r->ptr->valcount(); 
  r=r->next;
 }
 return n; 
}

void operand_list_::selfirstvaltuple()
{ 
 rec_ *r=first_;
 while (r) {
  r->ptr->selfirstval(); 
  r=r->next;
 }
} 

bool operand_list_::selnextvaltuple()
{ 
 rec_ *r=last_;
 while (r)
 if (r->ptr->selnextval()) break;
 else {
  r->ptr->selfirstval(); 
  r=r->prev;
 }
 return r!=0;
}

void operand_list_::printvaltuple(ostream &out,const char *sep) const
{ 
 rec_ *r=first_;
 while (r) {
  if (r!=first_) out << sep;
  r->ptr->printval(out); 
  r=r->next;
 }
}

void operand_list_::printvals(ostream &out,bool multi) const
{ 
 rec_ *r=first_;
 if (!r) out << novals_txtrep_;
 else do {
  if (r!=first_) out << ", ";
  if (multi) r->ptr->printvals(out);
  else r->ptr->printval(out);
  r=r->next;
 } while (r);
}

/// component-less operand:

class opr_simple_ : public virtual operand_ {
 private: 
  int compcount_() const
  { return 0; }
 protected:
  opr_simple_()
  {}
};

/// operand with any number of inner components:

class opr_complex_ : public virtual operand_ {
 private: 
  virtual operand_list_ getcomps_() const = 0;
 private: 
  int compcount_() const
  { return getcomps_().count(); }
 protected:
  opr_complex_()
  {}
 public:
  operand_list_ comps() const
  { return getcomps_(); }
};

/// single operand:

class opr_single_ : public virtual operand_ {
 protected:
  opr_single_()
  {}
};

/// multiple operand:

class opr_multiple_ : public virtual operand_ {
 private: 
  opr_rangemult_ *newvalrange_() const
  { return 0; }
  opr_setmult_ *newvalset_() const
  { return 0; }
 protected:
  opr_multiple_()
  {}
};

/// single-simple one-value operand:

class opr_singleton_ : public opr_single_ {
  opr_simple_ *that_() 
  { return dynamic_cast<opr_simple_ *>(this); }
  const opr_simple_ *that_() const
  { return dynamic_cast<const opr_simple_ *>(this); }
 private:
  void ecreate_() 
  { if (that_()==0) throw xUnexpFault(); }
  int valcount_() const
  { return 1; }
  void selfirstval_()
  {}
  bool selnextval_()
  { return false; }
  void printvals_(ostream &out) const
  { printval(out); }
 protected:
  opr_singleton_()
  {}
};

/// single-complex operand with a number of inner component values:

class opr_singlex_ : public opr_single_ {
  opr_complex_ *that_() 
  { return dynamic_cast<opr_complex_ *>(this); }
  const opr_complex_ *that_() const
  { return dynamic_cast<const opr_complex_ *>(this); }
 private: 
  void ecreate_() 
  { if (that_()==0) throw xUnexpFault(); }
  int valcount_() const
  { return that_()->comps().valtuplecount(); }
  void selfirstval_()
  { that_()->comps().selfirstvaltuple(); }
  bool selnextval_()
  { return that_()->comps().selnextvaltuple(); }
 protected:
  opr_singlex_()
  {}
};

/// multiple operand with a specified range of values:

class opr_rangemult_ : public opr_multiple_ {
 private:
  virtual bool setupendval_(const operand_ &x) = 0;
  virtual bool setupvalstep_(const operand_ &x) = 0;
 protected:
  opr_rangemult_()
  {}
 public:
  bool setupendval(const operand_ &x)
  { return setupendval_(x); }
  
  bool setupvalstep(const operand_ &x)
  { return setupvalstep_(x); }
};

/// multiple operand with a specified set of values:

class opr_setmult_ : public opr_multiple_ {
 private:
  virtual bool setupnextval_(const operand_ &x) = 0;
 protected:
  opr_setmult_()
  {}
 public:
  bool setupnextval(const operand_ &x)
  { return setupnextval_(x); }
};

/// operand of the symbol type:

class opr_symbtype_ : public opr_simple_ {
 public:
  typedef const char *val_t;
 private: 
  virtual val_t getval_() const = 0;
 private: 
  void printval_(ostream &out) const
  { out << getval_(); }
 protected:
  opr_symbtype_()
  {}
 public:
  val_t val() const
  { return getval_(); }
};

class opr_symb_ : public opr_symbtype_ , public opr_singleton_ {
  val_t val_;
 private: 
  val_t getval_() const
  { return val_; }
  opr_rangemult_ *newvalrange_() const
  { return 0; }
  opr_setmult_ *newvalset_() const;
  operand_ *do_dup_(const location_ &loc) const
  { return new opr_symb_(loc,val_); }
 public:
  opr_symb_(const location_ &loc,val_t a):
   operand_(loc), val_(a)
  {}
};

class opr_symb_set_ : public opr_symbtype_ , public opr_setmult_ {
  val_t *beg_;
  val_t *end_;
  val_t *val_;
 private: 
  int valcount_() const
  { return end_-beg_; }
  void selfirstval_()
  { val_=beg_; }
  bool selnextval_()
  { val_++; return val_<end_; }
  val_t getval_() const
  { return *val_; }
  void printvals_(ostream &out) const
  { 
   out << '{';
   val_t *v=beg_;
   while (v<end_) {
    if (v!=beg_) out << ", ";
    out << *v++;
   } 
   out << '}';
  }
  bool setupnextval_(const operand_ &x)
  { 
   if (end_-beg_>=setopr_maxvals_) return false;
   const opr_symbtype_ *n=dynamic_cast<const opr_symb_ *>(&x); 
   return n ? (*end_=n->val(), end_++, true) : false; 
  }
  operand_ *do_dup_(const location_ &loc) const
  { 
   val_t *v=beg_;
   opr_symb_set_ *x=new opr_symb_set_(loc,*v); 
   while (++v<end_) *x->end_=*v, x->end_++;
   return x;
  }
 public:
  opr_symb_set_(const location_ &loc,val_t a):
   operand_(loc), beg_(new val_t[setopr_maxvals_])
  {
   end_=beg_+1;
   val_=beg_;
   *val_=a;
  }

  ~opr_symb_set_()
  {
   delete[] beg_;
  }
};

opr_setmult_ *opr_symb_::newvalset_() const
{ return new opr_symb_set_(loc_,val_); }

/// operand of the character type:

class opr_chartype_ : public opr_simple_ {
 public:
  typedef char val_t;
 private: 
  virtual val_t getval_() const = 0;
 private: 
  void printval_(ostream &out) const
  { out << getval_(); }
 protected:
  opr_chartype_()
  {}
 public:
  val_t val() const
  { return getval_(); }
};

class opr_char_ : public opr_chartype_ , public opr_singleton_ {
  val_t val_;
 private: 
  val_t getval_() const
  { return val_; }
  opr_rangemult_ *newvalrange_() const;
  opr_setmult_ *newvalset_() const;
  operand_ *do_dup_(const location_ &loc) const
  { return new opr_char_(loc,val_); }
 public:
  opr_char_(const location_ &loc,val_t a):
   operand_(loc), val_(a)
  {}
};

class opr_char_range_ : public opr_chartype_ , public opr_rangemult_ {
  val_t beg_;
  val_t end_;
  val_t step_;
  val_t val_;
 private: 
  int valcount_() const
  { return (beg_-end_)/step_+1; }
  void selfirstval_()
  { val_=beg_; }
  bool selnextval_()
  { val_+=step_; return val_<=end_; }
  val_t getval_() const
  { return val_; }
  void printvals_(ostream &out) const
  { out << '[' << beg_ << ".." << end_ << ':' << step_ << ']'; }
  bool setupendval_(const operand_ &x)
  { const opr_chartype_ *e=dynamic_cast<const opr_char_ *>(&x); return e ? (end_=e->val(), true) : false; }
  bool setupvalstep_(const operand_ &x)
  { const opr_chartype_ *s=dynamic_cast<const opr_char_ *>(&x); return s ? (step_=s->val(), true) : false; }
  operand_ *do_dup_(const location_ &loc) const
  { 
   opr_char_range_ *x=new opr_char_range_(loc,beg_);
   x->end_=end_; x->step_=step_;
   return x;
  }
 public:
  opr_char_range_(const location_ &loc,val_t a):
   operand_(loc), beg_(a), end_(a), step_(1), val_(a)
  {}
};

opr_rangemult_ *opr_char_::newvalrange_() const
{ return new opr_char_range_(loc_,val_); }

class opr_char_set_ : public opr_chartype_ , public opr_setmult_ {
  val_t *beg_;
  val_t *end_;
  val_t *val_;
 private: 
  int valcount_() const
  { return end_-beg_; }
  void selfirstval_()
  { val_=beg_; }
  bool selnextval_()
  { val_++; return val_<end_; }
  val_t getval_() const
  { return *val_; }
  void printvals_(ostream &out) const
  { 
   out << '{';
   val_t *v=beg_;
   while (v<end_) {
    if (v!=beg_) out << ", ";
    out << *v++;
   } 
   out << '}';
  }
  bool setupnextval_(const operand_ &x)
  { 
   if (end_-beg_>=setopr_maxvals_) return false;
   const opr_chartype_ *n=dynamic_cast<const opr_char_ *>(&x); 
   return n ? (*end_=n->val(), end_++, true) : false; 
  }
  operand_ *do_dup_(const location_ &loc) const
  { 
   val_t *v=beg_;
   opr_char_set_ *x=new opr_char_set_(loc,*v); 
   while (++v<end_) *x->end_=*v, x->end_++;
   return x;
  }
 public:
  opr_char_set_(const location_ &loc,val_t a):
   operand_(loc), beg_(new val_t[setopr_maxvals_])
  {
   end_=beg_+1;
   val_=beg_;
   *val_=a;
  }

  ~opr_char_set_()
  {
   delete[] beg_;
  }
};

opr_setmult_ *opr_char_::newvalset_() const
{ return new opr_char_set_(loc_,val_); }

/// operand of the bool type:

class opr_booltype_ : public opr_simple_ {
 public:
  typedef bool val_t;
 private: 
  virtual val_t getval_() const = 0;
 private: 
  void printval_(ostream &out) const
  { out << boolalpha << getval_(); }
 protected:
  opr_booltype_()
  {}
 public:
  val_t val() const
  { return getval_(); }
};

class opr_bool_ : public opr_booltype_ , public opr_singleton_ {
  val_t val_;
 private: 
  val_t getval_() const
  { return val_; }
  opr_rangemult_ *newvalrange_() const
  { return 0; }
  opr_setmult_ *newvalset_() const;
  operand_ *do_dup_(const location_ &loc) const
  { return new opr_bool_(loc,val_); }
 public:
  opr_bool_(const location_ &loc,val_t a):
   operand_(loc), val_(a)
  {}
};

class opr_bool_set_ : public opr_booltype_ , public opr_setmult_ {
  val_t *beg_;
  val_t *end_;
  val_t *val_;
 private: 
  int valcount_() const
  { return end_-beg_; }
  void selfirstval_()
  { val_=beg_; }
  bool selnextval_()
  { val_++; return val_<end_; }
  val_t getval_() const
  { return *val_; }
  void printvals_(ostream &out) const
  { 
   out << '{';
   val_t *v=beg_;
   while (v<end_) {
    if (v!=beg_) out << boolalpha << ", ";
    out << *v++;
   } 
   out << '}';
  }
  bool setupnextval_(const operand_ &x)
  { 
   if (end_-beg_>=setopr_maxvals_) return false;
   const opr_booltype_ *n=dynamic_cast<const opr_bool_ *>(&x); 
   return n ? (*end_=n->val(), end_++, true) : false; 
  }
  operand_ *do_dup_(const location_ &loc) const
  { 
   val_t *v=beg_;
   opr_bool_set_ *x=new opr_bool_set_(loc,*v); 
   while (++v<end_) *x->end_=*v, x->end_++;
   return x;
  }
 public:
  opr_bool_set_(const location_ &loc,val_t a):
   operand_(loc), beg_(new val_t[setopr_maxvals_])
  {
   end_=beg_+1;
   val_=beg_;
   *val_=a;
  }

  ~opr_bool_set_()
  {
   delete[] beg_;
  }
};

opr_setmult_ *opr_bool_::newvalset_() const
{ return new opr_bool_set_(loc_,val_); }

/// operand of the real type:

class opr_realtype_ : public opr_simple_ {
 public:
  typedef real val_t;
 private: 
  virtual val_t getval_() const = 0;
 private: 
  void printval_(ostream &out) const
  { out << getval_(); }
 protected:
  opr_realtype_()
  {}
 public:
  val_t val() const
  { return getval_(); }
};

class opr_real_ : public opr_realtype_ , public opr_singleton_ {
  val_t val_;
 private: 
  val_t getval_() const
  { return val_; }
  opr_rangemult_ *newvalrange_() const;
  opr_setmult_ *newvalset_() const;
  operand_ *do_dup_(const location_ &loc) const
  { return new opr_real_(loc,val_); }
  operand_ *do_pos_(const location_ &loc) const
  { return new opr_real_(loc,+val_); }
  operand_ *do_neg_(const location_ &loc) const
  { return new opr_real_(loc,-val_); }
  operand_ *do_add_(const operand_ &x) const;
  operand_ *do_sub_(const operand_ &x) const;
  operand_ *do_mul_(const operand_ &x) const;
  operand_ *do_div_(const operand_ &x) const;
 public:
  opr_real_(const location_ &loc,val_t a):
   operand_(loc), val_(a)
  {}
};

class opr_real_range_ : public opr_realtype_ , public opr_rangemult_ {
  val_t beg_;
  val_t end_;
  val_t step_;
  val_t val_;
 private: 
  int valcount_() const
  { return (beg_-end_)/step_+1; }
  void selfirstval_()
  { val_=beg_; }
  bool selnextval_()
  { val_+=step_; return val_<=end_; }
  val_t getval_() const
  { return val_; }
  void printvals_(ostream &out) const
  { out << '[' << beg_ << ".." << end_ << ':' << step_ << ']'; }
  bool setupendval_(const operand_ &x)
  { const opr_realtype_ *e=dynamic_cast<const opr_real_ *>(&x); return e ? (end_=e->val(), true) : false; }
  bool setupvalstep_(const operand_ &x)
  { const opr_realtype_ *s=dynamic_cast<const opr_real_ *>(&x); return s ? (step_=s->val(), true) : false; }
  operand_ *do_dup_(const location_ &loc) const
  { 
   opr_real_range_ *x=new opr_real_range_(loc,beg_);
   x->end_=end_; x->step_=step_;
   return x;
  }
 public:
  opr_real_range_(const location_ &loc,val_t a):
   operand_(loc), beg_(a), end_(a), step_(1), val_(a)
  {}
};

opr_rangemult_ *opr_real_::newvalrange_() const
{ return new opr_real_range_(loc_,val_); }

class opr_real_set_ : public opr_realtype_ , public opr_setmult_ {
  val_t *beg_;
  val_t *end_;
  val_t *val_;
 private: 
  int valcount_() const
  { return end_-beg_; }
  void selfirstval_()
  { val_=beg_; }
  bool selnextval_()
  { val_++; return val_<end_; }
  val_t getval_() const
  { return *val_; }
  void printvals_(ostream &out) const
  { 
   out << '{';
   val_t *v=beg_;
   while (v<end_) {
    if (v!=beg_) out << ", ";
    out << *v++;
   } 
   out << '}';
  }
  bool setupnextval_(const operand_ &x)
  { 
   if (end_-beg_>=setopr_maxvals_) return false;
   const opr_realtype_ *n=dynamic_cast<const opr_real_ *>(&x); 
   return n ? (*end_=n->val(), end_++, true) : false; 
  }
  operand_ *do_dup_(const location_ &loc) const
  { 
   val_t *v=beg_;
   opr_real_set_ *x=new opr_real_set_(loc,*v); 
   while (++v<end_) *x->end_=*v, x->end_++;
   return x;
  }
 public:
  opr_real_set_(const location_ &loc,val_t a):
   operand_(loc), beg_(new val_t[setopr_maxvals_])
  {
   end_=beg_+1;
   val_=beg_;
   *val_=a;
  }

  ~opr_real_set_()
  {
   delete[] beg_;
  }
};

opr_setmult_ *opr_real_::newvalset_() const
{ return new opr_real_set_(loc_,val_); }

/// operand of the integer type:

class opr_inttype_ : public opr_realtype_ {
 public:
  typedef int val_t;
 private: 
  virtual val_t get_val_() const = 0;
 private: 
  opr_realtype_::val_t getval_() const
  { return get_val_(); }
  void printval_(ostream &out) const
  { out << get_val_(); }
 protected:
  opr_inttype_()
  {}
 public:
  val_t val() const
  { return get_val_(); }
};

class opr_int_ : public opr_inttype_ , public opr_singleton_ {
  val_t val_;
 private: 
  val_t get_val_() const
  { return val_; }
  opr_rangemult_ *newvalrange_() const;
  opr_setmult_ *newvalset_() const;
  operand_ *do_dup_(const location_ &loc) const
  { return new opr_int_(loc,val_); }
  operand_ *do_pos_(const location_ &loc) const
  { return new opr_int_(loc,+val_); }
  operand_ *do_neg_(const location_ &loc) const
  { return new opr_int_(loc,-val_); }
  operand_ *do_add_(const operand_ &x) const;
  operand_ *do_sub_(const operand_ &x) const;
  operand_ *do_mul_(const operand_ &x) const;
  operand_ *do_div_(const operand_ &x) const;
 public:
  opr_int_(const location_ &loc,val_t a):
   operand_(loc), val_(a)
  {}
};

class opr_int_range_ : public opr_inttype_ , public opr_rangemult_ {
  val_t beg_;
  val_t end_;
  val_t step_;
  val_t val_;
 private: 
  int valcount_() const
  { return (beg_-end_)/step_+1; }
  void selfirstval_()
  { val_=beg_; }
  bool selnextval_()
  { val_+=step_; return val_<=end_; }
  val_t get_val_() const
  { return val_; }
  void printvals_(ostream &out) const
  { out << '[' << beg_ << ".." << end_ << ':' << step_ << ']'; }
  bool setupendval_(const operand_ &x)
  { const opr_inttype_ *e=dynamic_cast<const opr_int_ *>(&x); return e ? (end_=e->val(), true) : false; }
  bool setupvalstep_(const operand_ &x)
  { const opr_inttype_ *s=dynamic_cast<const opr_int_ *>(&x); return s ? (step_=s->val(), true) : false; }
  operand_ *do_dup_(const location_ &loc) const
  { 
   opr_int_range_ *x=new opr_int_range_(loc,beg_);
   x->end_=end_; x->step_=step_;
   return x;
  }
 public:
  opr_int_range_(const location_ &loc,val_t a):
   operand_(loc), beg_(a), end_(a), step_(1), val_(a)
  {}
};

opr_rangemult_ *opr_int_::newvalrange_() const
{ return new opr_int_range_(loc_,val_); }

class opr_int_set_ : public opr_inttype_ , public opr_setmult_ {
  val_t *beg_;
  val_t *end_;
  val_t *val_;
 private: 
  int valcount_() const
  { return end_-beg_; }
  void selfirstval_()
  { val_=beg_; }
  bool selnextval_()
  { val_++; return val_<end_; }
  val_t get_val_() const
  { return *val_; }
  void printvals_(ostream &out) const
  { 
   out << '{';
   val_t *v=beg_;
   while (v<end_) {
    if (v!=beg_) out << ", ";
    out << *v++;
   } 
   out << '}';
  }
  bool setupnextval_(const operand_ &x)
  { 
   if (end_-beg_>=setopr_maxvals_) return false;
   const opr_inttype_ *n=dynamic_cast<const opr_int_ *>(&x); 
   return n ? (*end_=n->val(), end_++, true) : false; 
  }
  operand_ *do_dup_(const location_ &loc) const
  { 
   val_t *v=beg_;
   opr_int_set_ *x=new opr_int_set_(loc,*v); 
   while (++v<end_) *x->end_=*v, x->end_++;
   return x;
  }
 public:
  opr_int_set_(const location_ &loc,val_t a):
   operand_(loc), beg_(new val_t[setopr_maxvals_])
  {
   end_=beg_+1;
   val_=beg_;
   *val_=a;
  }

  ~opr_int_set_()
  {
   delete[] beg_;
  }
};

opr_setmult_ *opr_int_::newvalset_() const
{ return new opr_int_set_(loc_,val_); }

/// operand of the string type:

class opr_strtype_ : public opr_simple_ {
 public:
  typedef string val_t;
 private: 
  virtual val_t getval_() const = 0;
 private: 
  void printval_(ostream &out) const
  { out << getval_(); }
 protected:
  opr_strtype_()
  {}
 public:
  val_t val() const
  { return getval_(); }
};

class opr_str_ : public opr_strtype_ , public opr_singleton_ {
  val_t val_;
 private: 
  val_t getval_() const
  { return val_; }
  opr_rangemult_ *newvalrange_() const
  { return 0; }
  opr_setmult_ *newvalset_() const;
  operand_ *do_dup_(const location_ &loc) const
  { return new opr_str_(loc,val_); }
 public:
  opr_str_(const location_ &loc,val_t a):
   operand_(loc), val_(a)
  {}
};

class opr_str_set_ : public opr_strtype_ , public opr_setmult_ {
  val_t *beg_;
  val_t *end_;
  val_t *val_;
 private: 
  int valcount_() const
  { return end_-beg_; }
  void selfirstval_()
  { val_=beg_; }
  bool selnextval_()
  { val_++; return val_<end_; }
  val_t getval_() const
  { return *val_; }
  void printvals_(ostream &out) const
  { 
   out << '{';
   val_t *v=beg_;
   while (v<end_) {
    if (v!=beg_) out << ", ";
    out << *v++;
   } 
   out << '}';
  }
  bool setupnextval_(const operand_ &x)
  { 
   if (end_-beg_>=setopr_maxvals_) return false;
   const opr_strtype_ *n=dynamic_cast<const opr_str_ *>(&x); 
   return n ? (*end_=n->val(), end_++, true) : false; 
  }
  operand_ *do_dup_(const location_ &loc) const
  { 
   val_t *v=beg_;
   opr_str_set_ *x=new opr_str_set_(loc,*v); 
   while (++v<end_) *x->end_=*v, x->end_++;
   return x;
  }
 public:
  opr_str_set_(const location_ &loc,val_t a):
   operand_(loc), beg_(new val_t[setopr_maxvals_])
  {
   end_=beg_+1;
   val_=beg_;
   *val_=a;
  }

  ~opr_str_set_()
  {
   delete[] beg_;
  }
};

opr_setmult_ *opr_str_::newvalset_() const
{ return new opr_str_set_(loc_,val_); }

/// operand of the standard-library-procedure invocation type:

class opr_invotype_ : public opr_complex_ {
 public:
  typedef struct invorec {
   int group;
   int slot;
   string tag;
   operand_list_ pars;

   int count() const
   { return pars.count(); }

   bool of_group(int g) const
   { return group==g; }

   string parstr() const
   { 
    ostringstream bout;
    pars.printvaltuple(bout);
    return bout.str(); 
   }
   
   void print(ostream &out,bool multi=false) const
   {
    out << tag << '('; 
    pars.printvals(out,multi);
    out << ')'; 
   }
   
   friend ostream &operator<<(ostream &out,const invorec &a)
   { a.print(out); return out; }
  } val_t;
 private: 
  virtual val_t getval_() const = 0;
 private: 
  operand_list_ getcomps_() const
  { return getval_().pars; }
  void printval_(ostream &out) const
  { out << getval_(); }
 protected:
  opr_invotype_()
  {}
 public:
  val_t val() const
  { return getval_(); }
};

class opr_invo_ : public opr_invotype_ , public opr_singlex_ {
  val_t val_;
 private: 
  val_t getval_() const
  { return val_; }
  void printvals_(ostream &out) const
  { val_.print(out,true); }
  opr_rangemult_ *newvalrange_() const
  { return 0; }
  opr_setmult_ *newvalset_() const;
  operand_ *do_dup_(const location_ &loc) const
  { return new opr_invo_(loc,val_); }
 public:
  opr_invo_(const location_ &loc,val_t a):
   operand_(loc), val_(a)
  {}
};

class opr_invo_set_ : public opr_invotype_ , public opr_setmult_ {
  val_t *beg_;
  val_t *end_;
  val_t *val_;
 private: 
  int valcount_() const
  { 
   int n=0;
   for (val_t *v=beg_; v<end_; v++) n+=v->pars.valtuplecount();
   return n; 
  }
  void selfirstval_()
  { 
   val_=beg_; 
   val_->pars.selfirstvaltuple();
  }
  bool selnextval_()
  { 
   if (!val_->pars.selnextvaltuple()) {
    val_++; 
    if (val_<end_) val_->pars.selfirstvaltuple();
   }
   return val_<end_; 
  }
  val_t getval_() const
  { return *val_; }
  void printvals_(ostream &out) const
  { 
   out << '{';
   val_t *v=beg_;
   while (v<end_) {
    if (v!=beg_) out << ", ";
    (*v++).print(out,true);
   } 
   out << '}';
  }
  bool setupnextval_(const operand_ &x)
  { 
   if (end_-beg_>=setopr_maxvals_) return false;
   const opr_invotype_ *n=dynamic_cast<const opr_invo_ *>(&x); 
   return n ? (*end_=n->val(), end_++, true) : false; 
  }
  operand_ *do_dup_(const location_ &loc) const
  { 
   val_t *v=beg_;
   opr_invo_set_ *x=new opr_invo_set_(loc,*v); 
   while (++v<end_) *x->end_=*v, x->end_++;
   return x;
  }
 public:
  opr_invo_set_(const location_ &loc,val_t a):
   operand_(loc), beg_(new val_t[setopr_maxvals_])
  {
   end_=beg_+1;
   val_=beg_;
   *val_=a;
  }

  ~opr_invo_set_()
  {
   delete[] beg_;
  }
};

opr_setmult_ *opr_invo_::newvalset_() const
{ return new opr_invo_set_(loc_,val_); }

/// operand of the singlex-procedure-invocation pipeline type:

class opr_pipetype_ : public opr_complex_ {
 public:
  typedef struct piperec {
   struct item {
    opr_invo_ *invo;
    bool serialized, islast;
   } items[pipeopr_maxitems_];
   operand_list_ invos;

   int count() const
   { return invos.count(); }
   
   void print(ostream &out,bool multi=false) const
   {
    int n=count();
    for (int i=0; i<n; i++) {
     const val_t::item &itm=items[i];
     if (multi) itm.invo->printvals(out);
     else itm.invo->printval(out);
     if (!itm.islast)
     if (itm.serialized) out << " > ";
     else out << " | ";
    }
   }
   
   friend ostream &operator<<(ostream &out,const piperec &a)
   { a.print(out); return out; }
   
   bool insertfirstitem(operand_ptr_ x,bool serialized,bool islast=false)
   { return insert_(x,serialized,islast); }

   bool insertnextitem(operand_ptr_ x,bool serialized)
   { return insert_(x,serialized,false); }

   bool insertlastitem(operand_ptr_ x)
   { return insert_(x,false,true); }
   
   private: bool insert_(operand_ptr_ x,bool serialized,bool islast)
   { 
    int n=invos.count();
    if (n>=pipeopr_maxitems_) return false;
    opr_invo_ *o=dynamic_cast<opr_invo_ *>(x());
    if (!o) return false;
    item &itm=items[n];
    itm.invo=o;
    itm.serialized=serialized;
    itm.islast=islast;
    invos.insert(x);
    return true;
   }
  } val_t;
 private: 
  virtual val_t getval_() const = 0;
 private: 
  operand_list_ getcomps_() const
  { return getval_().invos; }
  void printval_(ostream &out) const
  { out << getval_(); }
 protected:
  opr_pipetype_()
  {}
 public:
  val_t val() const
  { return getval_(); }
};

class opr_pipe_ : public opr_pipetype_ , public opr_singlex_ {
  val_t val_;
 private: 
  val_t getval_() const
  { return val_; }
  void printvals_(ostream &out) const
  { val_.print(out,true); }
  opr_rangemult_ *newvalrange_() const
  { return 0; }
  opr_setmult_ *newvalset_() const;
  operand_ *do_dup_(const location_ &loc) const
  { return new opr_pipe_(loc,val_); }
 public:
  opr_pipe_(const location_ &loc,val_t a):
   operand_(loc), val_(a)
  {}
};

class opr_pipe_set_ : public opr_pipetype_ , public opr_setmult_ {
  val_t *beg_;
  val_t *end_;
  val_t *val_;
 private: 
  int valcount_() const
  { 
   int n=0;
   for (val_t *v=beg_; v<end_; v++) n+=v->invos.valtuplecount();
   return n; 
  }
  void selfirstval_()
  { 
   val_=beg_; 
   val_->invos.selfirstvaltuple();
  }
  bool selnextval_()
  { 
   if (!val_->invos.selnextvaltuple()) {
    val_++; 
    if (val_<end_) val_->invos.selfirstvaltuple();
   }
   return val_<end_; 
  }
  val_t getval_() const
  { return *val_; }
  void printvals_(ostream &out) const
  { 
   out << '{';
   val_t *v=beg_;
   while (v<end_) {
    if (v!=beg_) out << ", ";
    (*v++).print(out,true);
   } 
   out << '}';
  }
  bool setupnextval_(const operand_ &x)
  { 
   if (end_-beg_>=setopr_maxvals_) return false;
   const opr_pipetype_ *n=dynamic_cast<const opr_pipe_ *>(&x); 
   return n ? (*end_=n->val(), end_++, true) : false; 
  }
  operand_ *do_dup_(const location_ &loc) const
  { 
   val_t *v=beg_;
   opr_pipe_set_ *x=new opr_pipe_set_(loc,*v); 
   while (++v<end_) *x->end_=*v, x->end_++;
   return x;
  }
 public:
  opr_pipe_set_(const location_ &loc,val_t a):
   operand_(loc), beg_(new val_t[setopr_maxvals_])
  {
   end_=beg_+1;
   val_=beg_;
   *val_=a;
  }

  ~opr_pipe_set_()
  {
   delete[] beg_;
  }
};

opr_setmult_ *opr_pipe_::newvalset_() const
{ return new opr_pipe_set_(loc_,val_); }

/// operands related definitions:

operand_ *opr_real_::do_add_(const operand_ &x) const
{ 
 const opr_realtype_ *a=dynamic_cast<const opr_real_ *>(&x);
 if (a) return new opr_real_(loc_,val_+a->val()); 
 else {
  const opr_inttype_ *a=dynamic_cast<const opr_int_ *>(&x);
  if (a) return new opr_real_(loc_,val_+a->val());
  else return 0;
 }
}

operand_ *opr_int_::do_add_(const operand_ &x) const
{ 
 const opr_inttype_ *a=dynamic_cast<const opr_int_ *>(&x);
 if (a) return new opr_int_(loc_,val_+a->val()); 
 else {
  const opr_realtype_ *a=dynamic_cast<const opr_real_ *>(&x);
  if (a) return new opr_real_(loc_,val_+a->val());
  else return 0;
 }
}

operand_ *opr_real_::do_sub_(const operand_ &x) const
{ 
 const opr_realtype_ *a=dynamic_cast<const opr_real_ *>(&x);
 if (a) return new opr_real_(loc_,val_-a->val()); 
 else {
  const opr_inttype_ *a=dynamic_cast<const opr_int_ *>(&x);
  if (a) return new opr_real_(loc_,val_-a->val());
  else return 0;
 }
}

operand_ *opr_int_::do_sub_(const operand_ &x) const
{ 
 const opr_inttype_ *a=dynamic_cast<const opr_int_ *>(&x);
 if (a) return new opr_int_(loc_,val_-a->val()); 
 else {
  const opr_realtype_ *a=dynamic_cast<const opr_real_ *>(&x);
  if (a) return new opr_real_(loc_,val_-a->val());
  else return 0;
 }
}

operand_ *opr_real_::do_mul_(const operand_ &x) const
{ 
 const opr_realtype_ *a=dynamic_cast<const opr_real_ *>(&x);
 if (a) return new opr_real_(loc_,val_*a->val()); 
 else {
  const opr_inttype_ *a=dynamic_cast<const opr_int_ *>(&x);
  if (a) return new opr_real_(loc_,val_*a->val());
  else return 0;
 }
}

operand_ *opr_int_::do_mul_(const operand_ &x) const
{ 
 const opr_inttype_ *a=dynamic_cast<const opr_int_ *>(&x);
 if (a) return new opr_int_(loc_,val_*a->val()); 
 else {
  const opr_realtype_ *a=dynamic_cast<const opr_real_ *>(&x);
  if (a) return new opr_real_(loc_,val_*a->val());
  else return 0;
 }
}

operand_ *opr_real_::do_div_(const operand_ &x) const
{ 
 const opr_realtype_ *a=dynamic_cast<const opr_real_ *>(&x);
 if (a) {
  if (a->val()==0.0) throw xZeroDiv_(a->loc());
  return new opr_real_(loc_,val_/a->val()); 
 } else {
  const opr_inttype_ *a=dynamic_cast<const opr_int_ *>(&x);
  if (a) {
   if (a->val()==0) throw xZeroDiv_(a->loc());
   return new opr_real_(loc_,val_/a->val());
  } else return 0;
 }
}

operand_ *opr_int_::do_div_(const operand_ &x) const
{ 
 const opr_inttype_ *a=dynamic_cast<const opr_int_ *>(&x);
 if (a) {
  if (a->val()==0) throw xZeroDiv_(a->loc());
  return new opr_int_(loc_,val_/a->val()); 
 } else {
  const opr_realtype_ *a=dynamic_cast<const opr_real_ *>(&x);
  if (a) {
   if (a->val()==0.0) throw xZeroDiv_(a->loc());
   return new opr_real_(loc_,val_/a->val());
  } else return 0;
 }
}

/////////////////////////////
///
/// Object Code Instruction
///

class instruction_ptr_;
class instruction_list_;

class instruction_ {
  friend class instruction_ptr_;
  friend class instruction_list_;
 private:
  location_ loc_;
  instruction_ *prev_, *next_;
  bool disp_;
  void dispose_()
  { disp_=true; delete this; }
 private:
  virtual int oprvalcount_() const = 0;
  virtual void selfirstoprval_() = 0;
  virtual bool selnextoprval_() = 0;
  virtual void printoprval_(ostream &out) const = 0;
  virtual void printoprvals_(ostream &out) const = 0;
  virtual void execins_(datasegment_ &dseg) = 0;
//@{
 private: //! <b> Not implemented/allowed. </b>
  instruction_(instruction_ &);
  void operator=(instruction_ &);
//@}
 public:
/// create/destroy instruction object: 

  explicit instruction_(const location_ &loc): loc_(loc), prev_(0), next_(0), disp_(false)
  {}

  virtual ~instruction_()
  {
   if (!disp_) throw xMemoryFail_(loc_);
  }

/// return source code location for the instruction: 

  const location_ &loc() const
  { return loc_; }

/// manage operand values of the instruction: 

  int oprvalcount() const
  { return oprvalcount_(); }
  
  void selfirstoprval()
  { selfirstoprval_(); } 
  
  bool selnextoprval()
  { return selnextoprval_(); }

  void printoprval(ostream &out) const
  { 
   out << '(';
   printoprval_(out); 
   out << ')';
  }

  void printoprvals(ostream &out) const
  { 
   out << '(';
   printoprvals_(out); 
   out << ')';
  }

/// print source code statement for the instruction: 

  void printsrc(ostream &out) const;

/// print object code signature for the instruction: 

  void printobj(ostream &out) const;

/// execute object code of the instruction:

  void exec(datasegment_ &dseg)
  { execins_(dseg); }
};

/// instruction related definitions: 

void instruction_::printsrc(ostream &out) const
{ 
 string stm = typeid(*this).name();
 stm.erase(0,stm.find_last_of("::")+1);
 stm.erase(0,4);
 stm.erase(stm.end()-1);
 out << stm << '(';
 printoprvals_(out); 
 out << ");";
}

void instruction_::printobj(ostream &out) const
{ 
 string ins = typeid(*this).name();
 ins.erase(0,ins.find_last_of("::")+1);
 out << ins << ' ';
 printoprval_(out); 
}

/// null instruction:

class ins_null_ : public instruction_ {
 private:
  int oprvalcount_() const
  { return 0; }
  void selfirstoprval_()
  {}
  bool selnextoprval_()
  { return false; }
  void printoprval_(ostream &out) const
  { out << valless_txtrep_; }
  void printoprvals_(ostream &out) const
  { out << valless_txtrep_; }
  void execins_(datasegment_ &)
  {}
 public:
  ins_null_(): instruction_(location_::null)
  {}
};

/// instruction pointer:

class instruction_ptr_ {
  friend class instruction_list_;
 private:
  mutable instruction_ *p_;
  void zeroise_() const
  { p_=new ins_null_; }
 public:
  instruction_ptr_() : p_(new ins_null_) 
  {}
  instruction_ptr_(instruction_ *p) : p_(p ? p : new ins_null_) 
  {}
  instruction_ptr_(const instruction_ptr_ &F) : p_(F.p_) 
  { F.p_=new ins_null_; }
  ~instruction_ptr_() 
  { p_->dispose_(); }
  instruction_ptr_ &operator=(instruction_ *p)
  { if (p!=p_) p_->dispose_(), p_ = p ? p : new ins_null_; return *this; }
  instruction_ptr_ &operator=(const instruction_ptr_ &F)
  { if (F.p_!=p_) p_->dispose_(), p_=F.p_, F.p_=new ins_null_; return *this; }

  bool operator!() const
  { return dynamic_cast<ins_null_ *>(p_)!=0; }
  
  instruction_ *operator()()
  { return p_; }
  const instruction_ *operator()() const
  { return p_; }
  instruction_ &operator*()
  { return *p_; }
  const instruction_ &operator*() const
  { return *p_; }
  instruction_ *operator->()
  { return p_; }
  const instruction_ *operator->() const
  { return p_; }
};

/// list of instructions:

class instruction_list_ {
  mutable int cnt_;
  mutable instruction_ *first_;
  mutable instruction_ *last_;
 public:
// create/destroy/copy list object:
 
  instruction_list_();
  instruction_list_(const instruction_list_ &FF);
  ~instruction_list_();
  instruction_list_ &operator=(const instruction_list_ &FF);
  
// insert a new instruction to the list:  

  void insert(instruction_ptr_ F);

// return info about number of instructions in the list:

  int count() const
  { return cnt_; }

  bool empty() const
  { return cnt_<=0; }

// manage operand value-tuples for all instructions in the list:

  int oprvaltuplecount() const;
  void selfirstoprvaltuple();
  bool selnextoprvaltuple();
  void printoprvaltuple(ostream &out) const;

// print all instructions of the list in a given format:

  void printall(ostream &out,const char *ind="",bool as_src=false) const;

// execute all instructions of the list:

  void execall(datasegment_ &dseg);
};

// instruction list related definitions:

instruction_list_::instruction_list_(): first_(0), last_(0), cnt_(0)
{}

instruction_list_::instruction_list_(const instruction_list_ &FF):
 first_(FF.first_), last_(FF.last_), cnt_(FF.cnt_)
{
 FF.first_=0;
 FF.last_=0;
 FF.cnt_=0;
}

instruction_list_::~instruction_list_()
{
 while (first_) {
  instruction_ *F=first_;
  first_=first_->next_;
  F->dispose_();
 }
}

instruction_list_ &instruction_list_::operator=(const instruction_list_ &FF)
{
 if (&FF==this) return *this;
 while (first_) {
  instruction_ *F=first_;
  first_=first_->next_;
  F->dispose_();
 }
 first_=FF.first_; last_=FF.last_; cnt_=FF.cnt_;
 FF.first_=0;
 FF.last_=0;
 FF.cnt_=0;
 return *this;
}

void instruction_list_::insert(instruction_ptr_ F)
{ 
 if (!F) return;
 instruction_ *i=F();
 F.zeroise_();
 if (!first_) first_=last_=i;
 else {
  last_->next_=i;
  i->prev_=last_;
  last_=i;
 }
 cnt_++;
}

int instruction_list_::oprvaltuplecount() const
{ 
 instruction_ *F=first_;
 int n=1;
 while (F) {
  n*=F->oprvalcount(); 
  F=F->next_;
 }
 return n; 
}

void instruction_list_::selfirstoprvaltuple()
{ 
 instruction_ *F=first_;
 while (F) {
  F->selfirstoprval(); 
  F=F->next_;
 }
} 

bool instruction_list_::selnextoprvaltuple()
{ 
 instruction_ *F=last_;
 while (F)
 if (F->selnextoprval()) break;
 else {
  F->selfirstoprval(); 
  F=F->prev_;
 }
 return F!=0;
}

void instruction_list_::printoprvaltuple(ostream &out) const
{ 
 instruction_ *F=first_;
 while (F) {
  if (F!=first_) out << " , ";
  F->printoprval(out); 
  F=F->next_;
 }
}

void instruction_list_::printall(ostream &out,const char *ind,bool as_src) const
{ 
 out << endl;
 instruction_ *F=first_;
 while (F) {
  if (F!=first_) out << endl;
  out << ind;
  if (as_src) F->printsrc(out);
  else F->printobj(out);
  F=F->next_;
 }
}

void instruction_list_::execall(datasegment_ &dseg)
{ 
 instruction_ *F=first_;
 while (F) {
#ifdef SOURCE_DEBUG
  dseg.ctllog() << endl << "executing script instruction: ";
  F->printobj(dseg.ctllog());
  dseg.ctllog() << endl;
#endif
  F->exec(dseg);
  F=F->next_;
 }
}

/// operand-less instruction:

class ins_nullary_ : public instruction_ {
 private:
  int oprvalcount_() const
  { return 0; }
  void selfirstoprval_()
  {}
  bool selnextoprval_()
  { return false; }
  void printoprval_(ostream &out) const
  { out << novals_txtrep_; }
  void printoprvals_(ostream &out) const
  { out << novals_txtrep_; }
 public:
  explicit ins_nullary_(const location_ &loc): instruction_(loc)
  {}
};

/// one-operand instruction:

class ins_unary_ : public instruction_ {
  operand_ptr_ opr_;
 private:
  int oprvalcount_() const
  { return opr_->valcount(); }
  void selfirstoprval_()
  { opr_->selfirstval(); }
  bool selnextoprval_()
  { return opr_->selnextval(); }
  void printoprval_(ostream &out) const
  { opr_->printval(out); }
  void printoprvals_(ostream &out) const
  { opr_->printvals(out); }
 public:
  ins_unary_(const location_ &loc,operand_ptr_ x): instruction_(loc), opr_(x)
  {
   if (!x) throw xOprExprExp_(loc);
  }

  operand_ &opr()
  { return *opr_; }

  const operand_ &opr() const
  { return *opr_; }
};

/// two-operand instruction:
//
//! \note This class is unused at present,
//! similar definition is applied for ternary, 
//! quaternary , quinary ... instruction.
//
class ins_binary_ : public instruction_ {
  operand_ptr_ opr1_;
  operand_ptr_ opr2_;
 private:
  int oprvalcount_() const
  { return opr1_->valcount()*opr2_->valcount(); }
  void selfirstoprval_()
  { opr1_->selfirstval(); opr2_->selfirstval(); }
  bool selnextoprval_()
  { 
   if (opr2_->selnextval()) return true;
   else {
    opr2_->selfirstval();
    return opr1_->selnextval(); 
   }
  }
  void printoprval_(ostream &out) const
  {  
   opr1_->printval(out); 
   out << ", ";
   opr2_->printval(out); 
  }
  void printoprvals_(ostream &out) const
  {  
   opr1_->printvals(out); 
   out << ", ";
   opr2_->printvals(out); 
  }
 public:
  ins_binary_(const location_ &loc,operand_ptr_ x1,operand_ptr_ x2):
   instruction_(loc), opr1_(x1), opr2_(x2)
  {
   if (!x1 || !x2) throw xOprExprExp_(loc);
  }

  operand_ &opr1()
  { return *opr1_; }

  const operand_ &opr1() const
  { return *opr1_; }

  operand_ &opr2()
  { return *opr2_; }

  const operand_ &opr2() const
  { return *opr2_; }
};

/// instruction with any number of operands:

class ins_multary_ : public instruction_ {
  operand_list_ oprs_;
 private:
  int oprvalcount_() const
  { return oprs_.valtuplecount(); }
  void selfirstoprval_()
  { oprs_.selfirstvaltuple(); }
  bool selnextoprval_()
  { 
   return oprs_.selnextvaltuple(); 
  }
  void printoprval_(ostream &out) const
  {  
   oprs_.printvals(out); 
  }
  void printoprvals_(ostream &out) const
  {  
   oprs_.printvals(out,true); 
  }
 public:
  ins_multary_(const location_ &loc,const operand_list_ &xx):
   instruction_(loc), oprs_(xx)
  {}

  operand_list_ &oprs()
  { return oprs_; }

  const operand_list_ &oprs() const
  { return oprs_; }
};

/// standard input/output file name setting:

class ins_input_ : public ins_unary_ {
  opr_strtype_ *fn_;
 private:
  void execins_(datasegment_ &dseg) 
  {
   dseg.setinfn(fn_->val());
  }
 public:
  ins_input_(const location_ &loc,operand_ptr_ x): ins_unary_(loc,x)
  {
   fn_=dynamic_cast<opr_strtype_ *>(x());
   if (!fn_) throw xStrTypeOperReq_(x->loc());
  }
};

class ins_output_ : public ins_unary_ {
  opr_str_ *fn_;
 private:
  void execins_(datasegment_ &dseg) 
  {
   dseg.setoutfn(fn_->val());
  }
 public:
  ins_output_(const location_ &loc,operand_ptr_ x): ins_unary_(loc,x)
  {
   fn_=dynamic_cast<opr_str_ *>(x());
   if (!fn_) throw xStrOperReq_(x->loc());
  }
};

/// real data precision setting:

class ins_precision_ : public ins_unary_ {
  opr_realtype_ *pr_;
 private:
  void execins_(datasegment_ &dseg) 
  {
   dseg.setprec(pr_->val());
  }
 public:
  ins_precision_(const location_ &loc,operand_ptr_ x): ins_unary_(loc,x)
  {
   pr_=dynamic_cast<opr_realtype_ *>(x());
   if (!pr_) throw xRealTypeOperReq_(x->loc());
  }
};

/// data partition method setting:

class ins_partition_ : public ins_unary_ {
  opr_invotype_ *pm_;
 private:
  void execins_(datasegment_ &dseg) 
  {
   opr_invotype_::val_t a=pm_->val();
   if (!a.of_group(stdlibrary_::egPartMeth)) throw xPartMethReq_(pm_->loc());
   dseg.setpmth(a.slot,a.parstr());
  }
 public:
  ins_partition_(const location_ &loc,operand_ptr_ x): ins_unary_(loc,x)
  {
   pm_=dynamic_cast<opr_invotype_ *>(x());
   if (!pm_) throw xInvoTypeOperReq_(x->loc());
  }
};

/// data-classes column position setting:

class ins_classpos_ : public ins_unary_ {
  opr_inttype_ *cp_;
 private:
  void execins_(datasegment_ &dseg) 
  {
   dseg.setcpos(cp_->val());
  }
 public:
  ins_classpos_(const location_ &loc,operand_ptr_ x): ins_unary_(loc,x)
  {
   cp_=dynamic_cast<opr_inttype_ *>(x());
   if (!cp_) throw xIntTypeOperReq_(x->loc());
  }
};

class ins_classislast_ : public ins_unary_ {
  opr_bool_ *lc_;
 private:
  void execins_(datasegment_ &dseg) 
  {
   dseg.setlastc(lc_->val());
  }
 public:
  ins_classislast_(const location_ &loc,operand_ptr_ x): ins_unary_(loc,x)
  {
   lc_=dynamic_cast<opr_bool_ *>(x());
   if (!lc_) throw xBoolOperReq_(x->loc());
  }
};

/// data-classes manipulation:

class ins_excluded_ : public ins_multary_ {
 private:
  void execins_(datasegment_ &dseg) 
  {
   ostringstream bout;
   oprs().printvaltuple(bout);
   dseg.setexcl(bout.str());
   dseg.setnote(false);
  }
 public:
  ins_excluded_(const location_ &loc,const operand_list_ &xx): ins_multary_(loc,xx)
  {}
};  

class ins_notexcluded_ : public ins_multary_ {
 private:
  void execins_(datasegment_ &dseg) 
  {
   ostringstream bout;
   oprs().printvaltuple(bout);
   dseg.setexcl(bout.str());
   dseg.setnote(true);
  }
 public:
  ins_notexcluded_(const location_ &loc,const operand_list_ &xx): ins_multary_(loc,xx)
  {}
};  

class ins_merged_ : public ins_multary_ {
 private:
  void execins_(datasegment_ &dseg) 
  {
   ostringstream bout;
   oprs().printvaltuple(bout);
   dseg.setmerg(bout.str());
   dseg.setnotm(false);
  }
 public:
  ins_merged_(const location_ &loc,const operand_list_ &xx): ins_multary_(loc,xx)
  {}
};  

class ins_notmerged_ : public ins_multary_ {
 private:
  void execins_(datasegment_ &dseg) 
  {
   ostringstream bout;
   oprs().printvaltuple(bout);
   dseg.setmerg(bout.str());
   dseg.setnotm(true);
  }
 public:
  ins_notmerged_(const location_ &loc,const operand_list_ &xx): ins_multary_(loc,xx)
  {}
};  

/// selection algorithm executing:

class ins_exec_ : public ins_unary_ {
  opr_invotype_ *monoalg_;
  opr_pipetype_ *hybralg_;
 private:
  void exec_monoalg_(Logic_::datfile &datf,Logic_::repfile &repf,ostream &log);
  void exec_hybralg_(Logic_::datfile &datf,Logic_::repfile &repf,ostream &log);
  void execins_(datasegment_ &dseg); 
 public:
  ins_exec_(const location_ &loc,operand_ptr_ x): ins_unary_(loc,x)
  {
   hybralg_=dynamic_cast<opr_pipetype_ *>(x());
   if (!hybralg_) {
    monoalg_=dynamic_cast<opr_invotype_ *>(x());
    if (!monoalg_) throw xInvoTypeOperReq_(x->loc());
   } 
  }
};

void ins_exec_::exec_monoalg_(Logic_::datfile &datf,Logic_::repfile &repf,ostream &log)
{
 opr_invotype_::val_t a=monoalg_->val();
 if (!a.of_group(stdlibrary_::egSelAlg)) throw xSelAlgReq_(monoalg_->loc());
 Logic_::Algorithm &alg=Logic_::Algs[a.slot];
 int v=alg.retvariant(a.tag);
 alg.variant(v);
 Logic_::Algorithm::parstrstream apars(a.parstr(),Logic_::Algorithm::rdAllPars,*symboprval_missing_);
 alg.execute(apars,datf,repf,log);
}

void ins_exec_::exec_hybralg_(Logic_::datfile &datf,Logic_::repfile &repf,ostream &log)
{
 opr_pipetype_::val_t a=hybralg_->val();
 int n=a.count();
 bool serially;
 for (int i=0; i<n; i++) {
  opr_pipetype_::val_t::item &itm=a.items[i];
  if (i<=0) { if (!itm.islast) datf.mkfirstpipestep(); }
  else if (!itm.islast) datf.mknextpipestep(serially);
  else datf.mklastpipestep(serially);
  monoalg_=itm.invo;
  exec_monoalg_(datf,repf,log);
  serially=itm.serialized;
 }
}

void ins_exec_::execins_(datasegment_ &dseg) 
{
 try {
  if (hybralg_) exec_hybralg_(dseg.datf(),dseg.repf(),dseg.ctllog());
  else exec_monoalg_(dseg.datf(),dseg.repf(),dseg.ctllog());
  dseg.exedone(true);
 }
 
 catch (const Logic_::xAlgsError &exn)
 {
  dseg.reperr() << endl << exn << endl;
  dseg.exedone(false);
  throw;
 }

 catch (const Logic_::xDataError &exn)
 {
  dseg.reperr() << endl << exn << endl;
  dseg.exedone(false);
  throw;
 }
  
 catch (const Logic_::xProcError &exn)
 {
  dseg.reperr() << endl << exn << endl;
  dseg.exedone(false);
  throw;
 }

 catch (const Logic_::xCommError &exn)
 {
  dseg.reperr() << endl << exn << endl;
  dseg.exedone(false);
  throw;
 }

 catch (const exception &e)
 {
  Logic_::xUnexpctExn exn(e);
  dseg.reperr() << endl << exn << endl;
  dseg.exedone(false);
  throw;
 }

 catch (...)
 {
  Logic_::xUnknownExn exn;
  dseg.reperr() << endl << exn << endl;
  dseg.exedone(false);
  throw;
 }
}

class ins_execall_ : public ins_nullary_ {
 private:
  void execins_(datasegment_ &dseg); 
 public:
  explicit ins_execall_(const location_ &loc): ins_nullary_(loc)
  {}
};

void ins_execall_::execins_(datasegment_ &dseg) 
{
 try {
  for (int i=0; i<Logic_::Algs.count(); i++) {
   Logic_::Algs[i].execute(Logic_::Algorithm::pars(),dseg.datf(),dseg.repf(),dseg.ctllog());
   dseg.exedone(true);
  }
 }
 
 catch (const Logic_::xAlgsError &exn)
 {
  dseg.reperr() << endl << exn << endl;
  dseg.exedone(false);
  throw;
 }

 catch (const Logic_::xDataError &exn)
 {
  dseg.reperr() << endl << exn << endl;
  dseg.exedone(false);
  throw;
 }

 catch (const Logic_::xProcError &exn)
 {
  dseg.reperr() << endl << exn << endl;
  dseg.exedone(false);
  throw;
 }

 catch (const Logic_::xCommError &exn)
 {
  dseg.reperr() << endl << exn << endl;
  dseg.exedone(false);
  throw;
 }

 catch (const exception &e)
 {
  Logic_::xUnexpctExn exn(e);
  dseg.reperr() << endl << exn << endl;
  dseg.exedone(false);
  throw;
 }

 catch (...)
 {
  Logic_::xUnknownExn exn;
  dseg.reperr() << endl << exn << endl;
  dseg.exedone(false);
  throw;
 }
}

/// message printing:

class ins_echo_ : public ins_multary_ {
 private:
  void execins_(datasegment_ &dseg) 
  {
   oprs().printvaltuple(dseg.replog(),"");
   oprs().printvaltuple(dseg.ctllog(),"");
  }
 public:
  ins_echo_(const location_ &loc,const operand_list_ &xx): ins_multary_(loc,xx)
  {}
};

class ins_echoln_ : public ins_multary_ {
 private:
  void execins_(datasegment_ &dseg) 
  {
   oprs().printvaltuple(dseg.replog(),""); 
   dseg.replog() << endl;
   oprs().printvaltuple(dseg.ctllog(),""); 
   dseg.ctllog() << endl;
  }
 public:
  ins_echoln_(const location_ &loc,const operand_list_ &xx): ins_multary_(loc,xx)
  {}
};

/////////////////////////////
///
/// Segment of All Compiled
/// Object Code Instructions
///

class codesegment_ {
  instruction_list_ instrs_;
//@{
 private: //! <b> Not implemented/allowed. </b>
  codesegment_(codesegment_ &);
  void operator=(codesegment_ &);
//@}
 public:
/// create a segment object:
  
  codesegment_()
  {}

/// assemble object code instructions:
  
  void assemble(instruction_ptr_ F)
  {
   instrs_.insert(F);
  }

/// print disassembled object code instructions:
  
  void disassemble(ostream &out,const char *ind="") const
  {
   instrs_.printall(out,ind);
  }

/// execute object code instructions:
  
  void execute(datasegment_ &dseg)
  {
   instrs_.execall(dseg);
  }
  
/// return number of execution repetitions according with the value-tuple count in the code:
  
  int nlaps() const
  { 
   return instrs_.oprvaltuplecount();
  }
  
/// continue/reinitiate execution by selecting the next/first value-tuple in the code:
  
  bool continuare()
  {
   return instrs_.selnextoprvaltuple();
  }
  
  void reinitialize()
  { 
   instrs_.selfirstoprvaltuple();
  } 
};

/////////////////////////////
///
/// Root of Run-Time Structure 
/// for Executing Object Code 
///

class program_ {
  int iniexc_; // initial count of algorithm executions for the current code execution lap
  
 // make prolog/epilog for the program code at its current execution lap using program data:
  void code_prolog_(datasegment_ &data);
  void code_epilog_(datasegment_ &data);
  
 public:
/// program data and code segments:
 
  datasegment_ data;
  codesegment_ code;

/// create a program object giving its data header and logs output:

  program_(const string &header,ostream &ctllog): data(header,ctllog)
  {}

/// perform all code execution laps for the program:
  
  void Run();

/// reset the program state to make next run (unused at present):
  
  void Reset()
  {
   data.reinitialize();
   code.reinitialize();
  }
};

/// program related definitions:
  
void program_::Run()
{
 try {
  data.ctllog() << endl << "running the script code...." << endl;

  do {
   code_prolog_(data);
   code.execute(data);
   code_epilog_(data);
  } while (code.continuare());

  data.ctllog() << endl << "executed successfully " << data.execnt() << " algorithm(s)" << endl;
 }
          
 catch (const Logic_::xError &)
 {
  code_epilog_(data);
  data.ctllog() << endl << "executed the script code: failure" << endl;
  throw;
 }

 catch (const exception &)
 {
  code_epilog_(data);
  data.ctllog() << endl << "executed the script code: unexpected fault" << endl;
  throw;
 }

 catch (...)
 {
  code_epilog_(data);
  data.ctllog() << endl << "executed the script code: unknown fault" << endl;
  throw;
 }
}

void program_::code_prolog_(datasegment_ &data)
{
 bool ishnt=true;
#ifndef SOURCE_DEBUG
 ishnt = code.nlaps()>1;
#endif
 if (ishnt) {
  ostringstream bout;
  bout << "script instructions to execute:" << endl;
  code.disassemble(bout,"\t");
  string hnt=bout.str();
  data.sethdnote(hnt);
 } 
 iniexc_=data.execnt();
}

void program_::code_epilog_(datasegment_ &data)
{
 int cnt=data.execnt()-iniexc_;
 ostream &out=data.replog();
 const string &name=data.datf().name();
 
 if (cnt>0) out << endl << "Executed successfully " << cnt << " algorithm(s) ";
 else out << endl << "Warning: No algorithms have been executed ";

 out << "for the input data file: " << name;

 if (cnt>0) out << " ." << endl;
 else out << " !" << endl;
}

/////////////////////////////
///
/// Script Language Token
///

enum token_type_ { // type of token symbols
 tok_null_, // null token

 tok_liter_, // literal
 tok_ident_, // identifier
   
 tok_const_,       // constant declaration keyword
 tok_var_,         // variable declaration keyword
 tok_set_,         // variable setting keyword
 tok_input_,       // standard input file name setting keyword
 tok_output_,      // standard output file name setting keyword
 tok_precision_,   // real data precision setting keyword
 tok_partition_,   // data partition method setting keyword
 tok_classpos_,    // data-classes column position setting keyword
 tok_classislast_, // data-classes column-is-last flag setting keyword
 tok_excluded_,    // data-classes exclusion setting keyword
 tok_notexcluded_, // data-classes un-exclusion setting keyword
 tok_merged_,      // data-classes mergence setting keyword
 tok_notmerged_,   // data-classes un-mergence setting keyword
 tok_exec_,        // given selection algorithm execution calling keyword
 tok_execall_,     // all selection algorithm execution calling keyword
 tok_echo_,        // message print calling keyword
 tok_echoln_,      // message & e-of-l print calling keyword
   
 tok_stt_, // setting operator
 tok_pos_, // positiveness operator
 tok_add_ = tok_pos_, // addition operator
 tok_neg_, // negativeness operator
 tok_sub_ = tok_neg_, // substraction operator
 tok_mul_, // multiplication operator
 tok_div_, // division operator
 tok_srl_, // serial pipeline operator
 tok_prl_, // parallel pipeline operator
   
 tok_rnbeg_, // range beginning punctuator
 tok_rnend_, // range ending punctuator
 tok_rnsep_, // range element separating punctuator
 tok_rnstp_, // range step specification punctuator
 tok_exbeg_, // expression list/phrase beginning punctuator
 tok_exend_, // expression list/phrase ending punctuator
 tok_exsep_, // expression separating punctuator
 tok_stsep_ = tok_exsep_, // set element separating punctuator
 tok_stbeg_, // set beginning punctuator
 tok_stend_, // set ending punctuator
 tok_smter_, // statement termination punctuator
 tok_scter_  // script termination punctuator
};

class tokenizer_;

class token_ {
  friend class tokenizer_;
 private: 
  token_type_ type_;
  location_ loc_;
  operand_ptr_ litval_;
  string idstr_;

 // create a token object:
  token_();
  void init_();
  
 public:
/// destroy the token object:
 
  ~token_();

/// copy the token object:
 
  token_(const token_ &tok);
  token_ &operator=(const token_ &tok);

/// return type of the token:

  operator token_type_() const
  { return type_; }
    
/// return source code location of the token:

  const location_ &loc() const
  { return loc_; }
    
/// return value of the literal token:

  operand_ptr_ litval() const
  { return litval_; }
    
/// return string of the identifier token:

  const string &idstr() const
  { return idstr_; }
};

/// token related definitions:

token_::token_(): type_(tok_null_)
{}

void token_::init_()
{
 type_=tok_null_;
 loc_=location_::null;
 litval_=0;
 idstr_="";
}

token_::~token_()
{}

token_::token_(const token_ &tok):
 type_(tok.type_),
 loc_(tok.loc_),
 litval_(tok.litval_),
 idstr_(tok.idstr_)
{}

token_ &token_::operator=(const token_ &tok)
{
 type_=tok.type_;
 loc_=tok.loc_;
 litval_=tok.litval_;
 idstr_=tok.idstr_;
 return *this;
}

/////////////////////////////
///
/// Lexical Analyzer for Translating 
/// Script Text into Token Sequence
/// (text-to-token translator)
///

class tokenizer_ {
  istream *txtin_;
  streampos tinpos_;
  token_ curtok_;
  location_ nextloc_;
  char auxbuf_[txtline_maxlen_];

  void next_step_();
 private: //! <b> Not implemented/allowed. </b> 
  tokenizer_();
 public:
/// create/copy/destroy tokenizer object:

  explicit tokenizer_(istream &txtin);
  tokenizer_(const tokenizer_ &tkr);
  ~tokenizer_();
  tokenizer_ &operator=(const tokenizer_ &tkr);

/// analyze next token:

  void operator++() 
  { next_step_(); }
  
/// return the current token:

  const token_ *operator->() const 
  { return &curtok_; }
  
  const token_ &operator*() const 
  { return curtok_; }
};

/// tokenizer related definitions:

tokenizer_::tokenizer_(istream &txtin):
 txtin_(&txtin),
 nextloc_(location_::null)
{
 tinpos_=txtin.tellg();
 nextloc_.row++;
 nextloc_.col++;
}

tokenizer_::tokenizer_(const tokenizer_ &tkr):
 txtin_(tkr.txtin_), 
 tinpos_(tkr.tinpos_), 
 curtok_(tkr.curtok_),
 nextloc_(tkr.nextloc_)
{}

tokenizer_::~tokenizer_()
{}

tokenizer_ &tokenizer_::operator=(const tokenizer_ &tkr)
{
 txtin_=tkr.txtin_;
 tinpos_=tkr.tinpos_;
 curtok_=tkr.curtok_;
 nextloc_=tkr.nextloc_;
 return *this;
}

void tokenizer_::next_step_()
{
 istream &tin=*txtin_;
 tin.seekg(tinpos_);
 location_ &loc=nextloc_;
 int &row=loc.row;
 int &col=loc.col;
     
 char ch='\0';
 while (!tin.eof()) {
  ch=tin.get(); 
  while (isspace(ch)) {
   if (ch=='\n') {
    row++;
    col=1;
   } else col++;
   ch=tin.get();
  }
  if (ch=='\\') {
   col++;
   ch=tin.get();
   if (ch!='#') throw xInvalidComment_(loc);
   col++;
   char pch=ch;
   ch=tin.get();
   while (ch!='\\' && ch!=EOF && ch!='\0') {
    if (ch=='\n') {
     row++;
     col=1;
    } else col++;
    pch=ch;
    ch=tin.get();
   } 
   if (ch=='\\') {
    if (pch!='#') throw xInvalidComment_(loc);
    col++;
   } else break;
  } else
  if (ch=='#') {
   while (ch!='\n' && ch!=EOF && ch!='\0') {
    col++;
    ch=tin.get();
   } 
   if (ch=='\n') {
    row++;
    col=1;
   } else break;
  } else break;
 }

 token_ &tok=curtok_;
 tok.init_();
 
 if (tin.eof() || ch==EOF || ch=='\0') {
  tok.type_=tok_scter_;
  tok.loc_=loc;
  col++;
 } else
 if (isalpha(ch)) {
  char *buf=auxbuf_;
  int n=sizeof auxbuf_;
  int i=0;
  while (i<n && (isalpha(ch) || isdigit(ch) || ch=='_')) {
   buf[i++]=ch;
   ch=tin.get();
  } 
  tin.putback(ch);
  if (i==n) throw xIdentTooLong_(loc);
  buf[i]='\0';
  string bstr=buf;
  tok.loc_=loc;
  col+=i;
  if (bstr=="const") tok.type_=tok_const_;
  else
  if (bstr=="var") tok.type_=tok_var_;
  else
  if (bstr=="set") tok.type_=tok_set_;
  else
  if (bstr=="input") tok.type_=tok_input_;
  else
  if (bstr=="output") tok.type_=tok_output_;
  else
  if (bstr=="precision") tok.type_=tok_precision_;
  else
  if (bstr=="partition") tok.type_=tok_partition_;
  else
  if (bstr=="classpos") tok.type_=tok_classpos_;
  else
  if (bstr=="classislast") tok.type_=tok_classislast_;
  else
  if (bstr=="excluded") tok.type_=tok_excluded_;
  else
  if (bstr=="notexcluded") tok.type_=tok_notexcluded_;
  else
  if (bstr=="merged") tok.type_=tok_merged_;
  else
  if (bstr=="notmerged") tok.type_=tok_notmerged_;
  else
  if (bstr=="exec") tok.type_=tok_exec_;
  else
  if (bstr=="execall") tok.type_=tok_execall_;
  else
  if (bstr=="echo") tok.type_=tok_echo_;
  else
  if (bstr=="echoln") tok.type_=tok_echoln_;
  else
  if (bstr=="missing") {
   tok.type_=tok_liter_;
   tok.litval_=new opr_symb_(tok.loc_,symboprval_missing_);
  } else
  if (bstr=="default") {
   tok.type_=tok_liter_;
   tok.litval_=new opr_symb_(tok.loc_,symboprval_default_);
  } else
  if (bstr=="void") {
   tok.type_=tok_liter_;
   tok.litval_=new opr_symb_(tok.loc_,symboprval_void_);
  } else
  if (bstr=="false") {
   tok.type_=tok_liter_;
   tok.litval_=new opr_bool_(tok.loc_,false);
  } else
  if (bstr=="true") {
   tok.type_=tok_liter_;
   tok.litval_=new opr_bool_(tok.loc_,true);
  } else {
   tok.type_=tok_ident_;
   tok.idstr_=bstr;
  }
 } else
 if (isdigit(ch)) {
  char *buf=auxbuf_;
  int n=sizeof auxbuf_;
  bool was_dot=false;
  bool was_eE=false;
  bool isint=true;
  int i=0;
  while (i<n && (isdigit(ch) || ch=='.' || ch=='e' || ch=='E' || ch=='+' || ch=='-')) {
   if (!isdigit(ch)) { // i>0
    char pch=buf[i-1];
    char nch=tin.peek();
    if (ch=='.' && nch=='.') break;
    else 
    if ((ch=='+' || ch=='-') && !(pch=='e' || pch=='E')) break;
    if (ch=='.') {
     if (was_dot) break;
     was_dot=true;
    } else
    if (ch=='e' || ch=='E') {
     if (was_eE) break;
     was_eE=true;
    }
    isint=false;
   } 
   buf[i++]=ch;
   ch=tin.get();
  } 
  tin.putback(ch);
  if (i==n) throw xArtLitTooLong_(loc);
  buf[i]='\0';
  tok.loc_=loc;
  col+=i;
  tok.type_=tok_liter_;
  istringstream bin(buf);
  if (isint) {
   int a=0;
   bin >> a;
   tok.litval_=new opr_int_(tok.loc_,a);
  } else {
   real a=0.0;
   bin >> a;
   tok.litval_=new opr_real_(tok.loc_,a);
  }
  if (!bin) throw xInvalidArtLit_(loc);
 } else
 if (ch=='\"') {
  char *buf=auxbuf_;
  int n=sizeof auxbuf_;
  int i=0;
  char pch=0;
  while (i<n && pch!='\"' && ch!='\n') {
   buf[i]=ch;
   if (i>0) pch=ch;
   i++;
   ch=tin.get();
  } 
  tin.putback(ch);
  if (i==n) throw xStrLitTooLong_(loc);
  else if (pch!='\"') throw xUntermStrLit_(loc);
  buf[i]='\0';
  tok.loc_=loc;
  col+=i;
  tok.type_=tok_liter_;
  tok.litval_=new opr_str_(tok.loc_,string(&buf[1],i-2));
 } else
 if (ch=='\'') {
  char *buf=auxbuf_;
  int n=min2(sizeof auxbuf_,(size_t)3);
  int i=0;
  char pch=0;
  while (i<n && pch!='\'' && ch!='\n') {
   buf[i]=ch;
   if (i>0) pch=ch;
   i++;
   ch=tin.get();
   if (i==n-1 && ch!='\'') break;
  } 
  tin.putback(ch);
  if (pch!='\'') throw xUntermChrLit_(loc);
  buf[i]='\0';
  tok.loc_=loc;
  col+=i;
  tok.type_=tok_liter_;
  tok.litval_=new opr_char_(tok.loc_,buf[1]);
 } else
 if (ch=='=') {
  tok.type_=tok_stt_;
  tok.loc_=loc;
  col++;
 } else
 if (ch=='+') {
  tok.type_=tok_pos_;
  tok.loc_=loc;
  col++;
 } else
 if (ch=='-') {
  tok.type_=tok_neg_;
  tok.loc_=loc;
  col++;
 } else
 if (ch=='*') {
  tok.type_=tok_mul_;
  tok.loc_=loc;
  col++;
 } else
 if (ch=='/') {
  tok.type_=tok_div_;
  tok.loc_=loc;
  col++;
 } else
 if (ch=='>') {
  tok.type_=tok_srl_;
  tok.loc_=loc;
  col++;
 } else
 if (ch=='|') {
  tok.type_=tok_prl_;
  tok.loc_=loc;
  col++;
 } else
 if (ch=='[') {
  tok.type_=tok_rnbeg_;
  tok.loc_=loc;
  col++;
 } else
 if (ch==']') {
  tok.type_=tok_rnend_;
  tok.loc_=loc;
  col++;
 } else
 if (ch=='.') {
  ch=tin.get();
  if (ch=='.') {
   tok.type_=tok_rnsep_;
   tok.loc_=loc;
   col+=2;
  } else {
   tin.putback(ch);
   throw xUnknownTextChar_(loc);
  }
 } else
 if (ch==':') {
  tok.type_=tok_rnstp_;
  tok.loc_=loc;
  col++;
 } else
 if (ch=='(') {
  tok.type_=tok_exbeg_;
  tok.loc_=loc;
  col++;
 } else
 if (ch==')') {
  tok.type_=tok_exend_;
  tok.loc_=loc;
  col++;
 } else
 if (ch==',') {
  tok.type_=tok_exsep_;
  tok.loc_=loc;
  col++;
 } else
 if (ch=='{') {
  tok.type_=tok_stbeg_;
  tok.loc_=loc;
  col++;
 } else
 if (ch=='}') {
  tok.type_=tok_stend_;
  tok.loc_=loc;
  col++;
 } else
 if (ch==';') {
  tok.type_=tok_smter_;
  tok.loc_=loc;
  col++;
 } else {
  tin.putback(ch);
  throw xUnknownTextChar_(loc);
 } 

 tinpos_=tin.tellg();
}

/////////////////////////////
///
/// Table of User Identifiers 
/// & Its Expressions Declared 
/// Explicitly in Source Code
///

class idtable_ {
  struct idrec_ {
   string iden;
   location_ loc;
   operand_ptr_ expr;
   bool ismutable;
   idrec_ *next;
  } *first_;
  
  bool declare_(const string &name,location_ &loc,operand_ptr_ x,bool mut);
//@{
 private: //! <b> Not implemented/allowed. </b>
  idtable_(idtable_ &);
  void operator=(idtable_ &);
//@}
 public:
/// create/destroy table object:
 
  idtable_();
  ~idtable_();

/// declare an identifier with constant/variable value:
  
  bool decl_const(const string &name,location_ &loc,operand_ptr_ x)
  { return declare_(name,loc,x,false); }

  bool decl_var(const string &name,location_ &loc,operand_ptr_ x)
  { return declare_(name,loc,x,true); }

  bool decl_var(const string &name,location_ &loc)
  { return declare_(name,loc,new opr_null_,true); }

/// read/write value for a given identifier:

  bool read(const string &name,location_ &loc,operand_ptr_ &x);
  bool write(const string &name,location_ &loc,operand_ptr_ x);
};

/// id. table related definitions:

idtable_::idtable_(): first_(0)
{}

idtable_::~idtable_()
{
 while (first_) {
  idrec_ *r=first_;
  first_=first_->next;
  delete r;
 }
}

bool idtable_::declare_(const string &name,location_ &loc,operand_ptr_ x,bool mut)
{
 idrec_ *r=first_, *last=0;
 while (r) {
  if (r->iden==name) {
   loc=r->loc;
   return false;
  } 
  last=r;
  r=r->next;
 }
 r=new idrec_;
 if (!last) first_=r;
 else last->next=r;
 r->iden=name;
 r->loc=loc;
 r->expr=x;
 r->ismutable=mut;
 r->next=0;
 return true;
}

bool idtable_::read(const string &name,location_ &loc,operand_ptr_ &x)
{
 idrec_ *r=first_;
 while (r) {
  if (r->iden==name) return loc=r->loc, x=r->expr, true;
  r=r->next;
 }
 return false;
}

bool idtable_::write(const string &name,location_ &loc,operand_ptr_ x)
{
 idrec_ *r=first_;
 while (r) {
  if (r->iden==name) {
   loc=r->loc;
   if (!r->ismutable) return false;
   else return r->expr=x, true;
  } 
  r=r->next;
 }
 return false;
}

/////////////////////////////
///
/// Syntax Analyzer for Parsing Script Source 
/// Code Text & Compiling Script Object Code
/// (token-to-instruction downward-analysis 
///  recursive translator)
///

class parser_ {
  idtable_ tab_;
  tokenizer_ tok_;

 // do parse script and all its statements:
  void parse_script_(program_ &obj);
  instruction_ptr_ parse_statement_();

 // do parse all script expressions:
  operand_list_ parse_expression_list_(int nexprs=-1);
  operand_ptr_ parse_expression_();
  operand_ptr_ parse_additive_expression_();
  operand_ptr_ parse_multiplicative_expression_();
  operand_ptr_ parse_unary_expression_();
  operand_ptr_ parse_primary_expression_();

 public:
/// create/destroy parser object for a given input stream with script source code:
 
  parser_(istream &src);
  ~parser_();
  
/// perform parsing the script and compile its object code

  void Compile(program_ &obj);
};

/// parser related definitions:

parser_::parser_(istream &src):
 tok_(src)
{}

parser_::~parser_()
{}

void parser_::Compile(program_ &obj)
{
 try {
  obj.data.ctllog() << endl << "parsing the script text...." << endl;
  ++tok_;
  parse_script_(obj);
  obj.data.ctllog() << endl << "compiled the script code: success" << endl;
 }
          
 catch (const xLexicalError_ &)
 {
  obj.data.ctllog() << endl << "compiled the script code: lexical fault" << endl;
  throw;
 }

 catch (const xSyntacticalError_ &)
 {
  obj.data.ctllog() << endl << "compiled the script code: syntactical fault" << endl;
  throw;
 }

 catch (const xSemanticalError_ &)
 {
  obj.data.ctllog() << endl << "compiled the script code: semantical fault" << endl;
  throw;
 }

 catch (const xExecutiveError_ &)
 {
  obj.data.ctllog() << endl << "compiled the script code: executive fault" << endl;
  throw;
 }

 catch (const exception &)
 {
  obj.data.ctllog() << endl << "compiled the script code: unexpected fault" << endl;
  throw;
 }

 catch (...)
 {
  obj.data.ctllog() << endl << "compiled the script code: unknown fault" << endl;
  throw;
 }
}

void parser_::parse_script_(program_ &obj)
{
 while (*tok_!=tok_scter_) {
  instruction_ptr_ F=parse_statement_();
#ifdef SOURCE_DEBUG
  obj.data.ctllog() << endl << "parsed script statement: ";
  F->printsrc(obj.data.ctllog());
  obj.data.ctllog() << endl;
#endif
  obj.code.assemble(F);
 }
}

instruction_ptr_ parser_::parse_statement_()
{
 instruction_ptr_ F;
 switch (*tok_) {
  case tok_smter_:
  {
   ++tok_;
   break;
  }
  case tok_const_:
  {
   ++tok_;
   if (*tok_!=tok_ident_) throw xIdentExp_(tok_->loc());
   location_ iloc=tok_->loc();
   string id=tok_->idstr();
   ++tok_;
   if (*tok_!=tok_stt_) throw xInitSettExp_(tok_->loc());
   ++tok_;
   operand_ptr_ x=parse_expression_();
   if (*tok_!=tok_smter_) throw xUntermStat_(tok_->loc());
   ++tok_;
   location_ dloc=iloc;
   if (!tab_.decl_const(id,dloc,x)) throw xIllegalStat_(iloc,xMultiNameDecl_(dloc)); 
   break;
  }
  case tok_var_:
  {
   ++tok_;
   if (*tok_!=tok_ident_) throw xIdentExp_(tok_->loc());
   location_ iloc=tok_->loc();
   string id=tok_->idstr();
   ++tok_;
   if (*tok_==tok_smter_) {
    location_ dloc=iloc;
    if (!tab_.decl_var(id,dloc)) throw xIllegalStat_(iloc,xMultiNameDecl_(dloc)); 
   } else {
    if (*tok_!=tok_stt_) throw xInitSettExp_(tok_->loc());
    ++tok_;
    operand_ptr_ x=parse_expression_();
    if (*tok_!=tok_smter_) throw xUntermStat_(tok_->loc());
    ++tok_;
    location_ dloc=iloc;
    if (!tab_.decl_var(id,dloc,x)) throw xIllegalStat_(iloc,xMultiNameDecl_(dloc)); 
   } 
   break;
  }
  case tok_set_:
  {
   ++tok_;
   if (*tok_!=tok_ident_) throw xIdentExp_(tok_->loc());
   location_ iloc=tok_->loc();
   string id=tok_->idstr();
   ++tok_;
   if (*tok_!=tok_stt_) throw xCurrSettExp_(tok_->loc());
   ++tok_;
   operand_ptr_ x=parse_expression_();
   if (*tok_!=tok_smter_) throw xUntermStat_(tok_->loc());
   ++tok_;
   location_ dloc;
   if (!tab_.write(id,dloc,x)) 
   if (dloc.isnull()) throw xUndeclVarSett_(iloc);
   else throw xIllegalStat_(iloc,xDeclConstSett_(dloc)); 
   break;
  } 
  case tok_input_:
  case tok_output_:
  case tok_precision_:
  case tok_partition_:
  case tok_classpos_:
  case tok_classislast_:
  {
   token_type_ ktok=*tok_;
   location_ kloc=tok_->loc();
   ++tok_;
   if (*tok_!=tok_stt_) throw xCurrSettExp_(tok_->loc());
   ++tok_;
   operand_ptr_ x=parse_expression_();
   if (*tok_!=tok_smter_) throw xUntermStat_(tok_->loc());
   ++tok_;
   switch (ktok) {
    case tok_input_: F=new ins_input_(kloc,x); break;
    case tok_output_: F=new ins_output_(kloc,x); break;
    case tok_precision_: F=new ins_precision_(kloc,x); break;
    case tok_partition_: F=new ins_partition_(kloc,x); break;
    case tok_classpos_: F=new ins_classpos_(kloc,x); break;
    case tok_classislast_: F=new ins_classislast_(kloc,x); break;
   }
   break;
  }
  case tok_excluded_:
  case tok_notexcluded_:
  case tok_merged_:
  case tok_notmerged_:
  {
   token_type_ ktok=*tok_;
   location_ kloc=tok_->loc();
   ++tok_;
   if (*tok_!=tok_stt_) throw xCurrSettExp_(tok_->loc());
   ++tok_;
   operand_list_ xx=parse_expression_list_();
   if (*tok_!=tok_smter_) throw xUntermStat_(tok_->loc());
   ++tok_;
   switch (ktok) {
    case tok_excluded_: F=new ins_excluded_(kloc,xx); break;
    case tok_notexcluded_: F=new ins_notexcluded_(kloc,xx); break;
    case tok_merged_: F=new ins_merged_(kloc,xx); break;
    case tok_notmerged_: F=new ins_notmerged_(kloc,xx); break;
   }
   break;
  }
  case tok_execall_:
  {
   token_type_ ktok=*tok_;
   location_ kloc=tok_->loc();
   ++tok_;
   if (*tok_!=tok_smter_) {
    operand_ptr_ x=parse_expression_();
    if (!x); else throw xUnexpOprExpr_(tok_->loc()); 
    if (*tok_!=tok_smter_) throw xUntermStat_(tok_->loc());
   } 
   ++tok_;
   switch (ktok) {
    case tok_execall_: F=new ins_execall_(kloc); break;
   }
   break;
  }
  case tok_exec_:
  {
   token_type_ ktok=*tok_;
   location_ kloc=tok_->loc();
   ++tok_;
   if (*tok_!=tok_exbeg_) throw xExprBegExp_(tok_->loc());
   ++tok_;
   operand_ptr_ x=parse_expression_();
   if (*tok_!=tok_exend_) throw xMissExprEnd_(tok_->loc());
   ++tok_;
   if (*tok_!=tok_smter_) throw xUntermStat_(tok_->loc());
   ++tok_;
   switch (ktok) {
    case tok_exec_: F=new ins_exec_(kloc,x); break;
   }
   break;
  }
  case tok_echo_:
  case tok_echoln_:
  {
   token_type_ ktok=*tok_;
   location_ kloc=tok_->loc();
   ++tok_;
   operand_list_ xx;
   if (ktok==tok_echo_ && *tok_!=tok_exbeg_) throw xExprBegExp_(tok_->loc());
   if (*tok_==tok_exbeg_) {
    ++tok_;
    xx=parse_expression_list_();
    if (*tok_!=tok_exend_) throw xMissExprEnd_(tok_->loc());
    ++tok_;
   } 
   if (*tok_!=tok_smter_) throw xUntermStat_(tok_->loc());
   ++tok_;
   switch (ktok) {
    case tok_echo_: F=new ins_echo_(kloc,xx); break;
    case tok_echoln_: F=new ins_echoln_(kloc,xx); break;
   }
   break;
  }
  default: throw xInvalidStat_(tok_->loc());
 }
 return F;
}

operand_list_ parser_::parse_expression_list_(int nexprs) //! \pre nexprs==-1 || nexprs>=0
{
 operand_list_ xx;
 if (nexprs==0 && *tok_==tok_exend_) return xx;
 int n=0;
 while (true) {
  operand_ptr_ x=parse_expression_();
  xx.insert(x);
  n++;
  if (*tok_==tok_exsep_) {
   if (nexprs>0 && n>=nexprs) throw xTooManyExprItems_(tok_->loc());
   ++tok_;
  } else {
   if (nexprs>0 && n<nexprs) throw xExprItemExp_(tok_->loc());
   break;
  } 
 }
 return xx;
}

operand_ptr_ parser_::parse_expression_()
{
 return parse_additive_expression_();
}

operand_ptr_ parser_::parse_additive_expression_()
{
 operand_ptr_ x=parse_multiplicative_expression_();
 for (;;) switch (*tok_) {
  case tok_pos_:
  {
   location_ iloc=tok_->loc();
   ++tok_;
   operand_ptr_ y=parse_multiplicative_expression_();
   x=add_(*x,*y);
   if (!x) throw xIllegalAdd_(iloc);
   break;
  }
  case tok_neg_:
  {
   location_ iloc=tok_->loc();
   ++tok_;
   operand_ptr_ y=parse_multiplicative_expression_();
   x=sub_(*x,*y);
   if (!x) throw xIllegalSub_(iloc);
   break;
  }
  default: return x;
 }
}

operand_ptr_ parser_::parse_multiplicative_expression_()
{
 operand_ptr_ x=parse_unary_expression_();
 for (;;) switch (*tok_) {
  case tok_mul_:
  {
   location_ iloc=tok_->loc();
   ++tok_;
   operand_ptr_ y=parse_unary_expression_();
   x=mul_(*x,*y);
   if (!x) throw xIllegalMul_(iloc);
   break;
  }
  case tok_div_:
  {
   location_ iloc=tok_->loc();
   ++tok_;
   operand_ptr_ y=parse_unary_expression_();
   x=div_(*x,*y);
   if (!x) throw xIllegalDiv_(iloc);
   break;
  }
  default: return x;
 }
}

operand_ptr_ parser_::parse_unary_expression_()
{
 switch (*tok_) {
  case tok_pos_:
  {
   location_ oloc=tok_->loc();
   ++tok_;
   operand_ptr_ x=parse_unary_expression_();
   x=pos_(oloc,*x);
   if (!x) throw xIllegalPos_(oloc);
   return x;
  }
  case tok_neg_:
  {
   location_ oloc=tok_->loc();
   ++tok_;
   operand_ptr_ x=parse_unary_expression_();
   x=neg_(oloc,*x);
   if (!x) throw xIllegalNeg_(oloc);
   return x;
  }
  default: return parse_primary_expression_();
 }
}

operand_ptr_ parser_::parse_primary_expression_()
{
 switch (*tok_) {
  case tok_liter_:
  {
   operand_ptr_ x=tok_->litval();
   ++tok_;
   return x;
  }
  case tok_ident_:
  {
   location_ iloc=tok_->loc();
   string id=tok_->idstr();
   bool idfound;
   location_ dloc;
   operand_ptr_ iexp;
   int igrp=stdlibrary_::egNull,islt;
   if (tab_.read(id,dloc,iexp)) {
    ++tok_;
    if (!iexp) throw xIllegalExpr_(iloc,xUninitVarUse_(dloc)); 
    operand_ptr_ x=dup_(iloc,*iexp);
    if (!x) throw xIllegalExpr_(iloc);
    return x;
   } else
   if ((idfound=stdlibrary_::find(id,igrp,islt)) && igrp==stdlibrary_::egPartMeth) {
    ++tok_;
    operand_list_ xx;
    if (*tok_==tok_exbeg_) {
     ++tok_;
     xx=parse_expression_list_();
     if (*tok_!=tok_exend_) throw xMissExprEnd_(tok_->loc());
     ++tok_;
    }
    opr_invotype_::val_t a;
    a.group=igrp;
    a.slot=islt;
    a.tag=id;
    a.pars=xx;
    operand_ptr_ x=new opr_invo_(iloc,a);
    return x;
   } else 
   if (idfound && igrp==stdlibrary_::egSelAlg) {
    ++tok_;
    opr_pipetype_::val_t apipe;
    for (;;) {
     operand_list_ xx;
     if (*tok_==tok_exbeg_) {
      ++tok_;
      xx=parse_expression_list_();
      if (*tok_!=tok_exend_) throw xMissExprEnd_(tok_->loc());
      ++tok_;
     }
     opr_invotype_::val_t a;
     a.group=igrp;
     a.slot=islt;
     a.tag=id;
     a.pars=xx;
     operand_ptr_ xinvo=new opr_invo_(iloc,a);
     if (*tok_==tok_srl_) {
      ++tok_;
      if (apipe.count()<=0) apipe.insertfirstitem(xinvo,true);
      else apipe.insertnextitem(xinvo,true);
     } else if (*tok_==tok_prl_) {
      ++tok_;
      if (apipe.count()<=0) apipe.insertfirstitem(xinvo,false);
      else apipe.insertnextitem(xinvo,false);
     } else {
      operand_ptr_ x;
      if (apipe.count()<=0) x=xinvo; 
      else {
       apipe.insertlastitem(xinvo);
       x=new opr_pipe_(iloc,apipe);
      } 
      return x;
     }
     if (*tok_!=tok_ident_) throw xInvalidPipe_(tok_->loc());
     iloc=tok_->loc();
     id=tok_->idstr();
     if (!stdlibrary_::find(id,igrp,islt) || igrp!=stdlibrary_::egSelAlg) throw xInvalidPipe_(iloc);
     ++tok_;
    }
   } else {
    throw xUnknownIdent_(iloc);
    return 0;
   }
  }
  case tok_rnbeg_:
  {
   location_ rloc=tok_->loc();
   ++tok_;
   operand_ptr_ xb=parse_expression_();
   opr_rangemult_ *r=xb->newvalrange();
   operand_ptr_ x=r;
   if (!r) throw xIllegalRng_(rloc);
   if (*tok_!=tok_rnsep_) throw xRngSepExp_(tok_->loc());
   ++tok_;
   operand_ptr_ xe=parse_expression_();
   if (!r->setupendval(*xe)) throw xInvalidRng_(xe->loc());
   if (*tok_==tok_rnstp_) {
    ++tok_;
    operand_ptr_ xs=parse_expression_();
    if (!r->setupvalstep(*xs)) throw xInvalidRng_(xs->loc());
   }
   if (*tok_!=tok_rnend_) throw xMissRngEnd_(tok_->loc());
   ++tok_;
   x->setloc(rloc);
   return x;
  }
  case tok_stbeg_:
  {
   location_ sloc=tok_->loc();
   ++tok_;
   operand_ptr_ xn=parse_expression_();
   operand_ptr_ x=xn;
   if (*tok_==tok_stsep_) {
    opr_setmult_ *s=xn->newvalset();
    x=s;
    if (!s) throw xIllegalSet_(sloc);
    do {
     ++tok_;
     xn=parse_expression_();
     if (!s->setupnextval(*xn)) throw xInvalidSet_(xn->loc());
    } while (*tok_==tok_stsep_);
   }
   if (*tok_!=tok_stend_) throw xMissSetEnd_(tok_->loc());
   ++tok_;
   x->setloc(sloc);
   return x;
  }
  case tok_exbeg_:
  {
   location_ eloc=tok_->loc();
   ++tok_;
   operand_ptr_ x;
   if (*tok_!=tok_exend_) {
    x=parse_expression_();
    if (*tok_!=tok_exend_) throw xMissExprEnd_(tok_->loc());
   } 
   ++tok_;
   x->setloc(eloc);
   return x;
  }
  default: 
  {
   throw xInvalidExpr_(tok_->loc());
   return 0;
  } 
 }
}

}} // end of Script_Processing_ namespace

namespace InfoSelUI {

/////////////////////////////
////
//// Master Driver Routine
//// for Processing Script 
//// Text of Control File 
////

bool ctlfile::perform_script_processing_(istream &script,const string &header,ostream &ctllog) throw()
{
 using namespace Data;
 using namespace Algorithms;
 using namespace Script_Processing_;
 
 ctllog << endl << "processing the control script...." << endl;
 
 try {
  parser_ parser(script);
  program_ program(header,ctllog);
  
  parser.Compile(program);
  program.Run();
 }
 
 catch (const xError &exn)
 {
  ctllog << endl << exn << endl;
  return false;
 }

 catch (const exception &e)
 {
  Logic_::xUnexpctExn exn(e);
  ctllog << endl << exn << endl;
  return false;
 }

 catch (...)
 {
  xUnknownExn exn;
  ctllog << endl << exn << endl;
  return false;
 }

 return true;
}

} // end of InfoSelUI namespace
