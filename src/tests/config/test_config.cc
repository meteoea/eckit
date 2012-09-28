/*
 * (C) Copyright 1996-2012 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eclib/Log.h"
#include "eclib/Resource.h"
#include "eclib/ResourceMgr.h"
#include "eclib/Types.h"
#include "eclib/Tool.h"

#include "eclib/config/Script.h"
#include "eclib/config/Compiler.h"

class TestConfig : public Tool {
public:

    TestConfig(int argc,char **argv): Tool(argc,argv) {}

    ~TestConfig() {}

    virtual void run();

    void test_parse();
    void test_resource();
};

//-----------------------------------------------------------------------------

void TestConfig::test_parse()
{
    ostringstream code;
     
    code <<        
            " a = 1; "                          // assign digit
            " b = lolo; "                       // assign string
            " "
//            " [ if class in [ od , rd ] && stream = oper ]"   // condition with multiple statement
            " [ if class = od || rd && stream = oper ]"   // condition with multiple statement
            " {  "
            "   fdbRoot = \'/tmp/fdb\';  "       // paths
            " "
            "   c1 = \"4 x 4\"; "               // double quote
            "   [ if date = today ] "              // nested branch
            "   {"
            "       d = 4; "
            "   }"                              // close after semi-colon
            "   c2 = '5 x 5'; "                 // single quote
            "   e = '6'; "
            "   e = '66'; "                       // override
            "   f : 'fofo'; "                     // assign with :
            " }"
            " "
            " [ if xxx = yyy ] { f = ignored; }"   // branch not visited
            " "
            " [ if xxx = yyy ] {} || { s = ss } "  // else branch visited
            " "
            " [ if class = od ] {} "               // empty branch
            " "
            " [ if class = od || rd ] { t = 22 }"  // double or in branch
            " "
            " [ if ] { h = here ; }"               // always true
            " "
            " g = go; "                         // isolated sataement
            " "
            " { } "                             // empty block
            " "
            " { j = jojo  } "                   // assigment without ';' @ end
            " { k = koko; } "                   // stand alone block
            " "
            " [ function foo ] { m = momo }"   // function definition
            " "
            " [ call foo ]"                    // function call
            ; 
     
    istringstream in(code.str());
    
    config::Compiler c(in);
    
    config::Script s(c);
    
//    s.print( std::cout );
    
    StringDict din;
    
    din["class"]    = "od";
    din["stream"]   = "oper";
    din["date"]     = "today";

    StringDict dout;
    
    s.execute(din,dout);
    
//    for( StringDict::const_iterator i = dout.begin(); i != dout.end(); ++i )
//        std::cout << i->first << " : " << i->second << std::endl;    
    
    ASSERT( dout["a"] == "1" );
    ASSERT( dout["b"] == "lolo" );
    ASSERT( dout["fdbRoot"] == "/tmp/fdb" );
    ASSERT( dout["c1"] == "4 x 4" );
    ASSERT( dout["d"] == "4" );
    ASSERT( dout["c2"] == "5 x 5" );
    ASSERT( dout["e"] == "66" );
    ASSERT( dout["s"] == "ss" );
    ASSERT( dout["t"] == "22" );
    ASSERT( dout["h"] == "here" );
    ASSERT( dout["g"] == "go" );
    ASSERT( dout["k"] == "koko" );
    ASSERT( dout["j"] == "jojo" );
    ASSERT( dout["m"] == "momo" );
    
    
}

//-----------------------------------------------------------------------------

void TestConfig::test_resource()
{
    ostringstream code;
     
    code << " b = foo " << std::endl                  
         << " [ if class = od ] b = bar " << std::endl;
    
    istringstream in(code.str());
    
    ResourceMgr::appendConfig(in);

    StringDict args;
    
    args["class"] = "od";
    
    string b = Resource<string>("b","none",args);
    
//    std::cout << "b [" << b << "]" << std::endl;
    
    ASSERT( b == "bar");
}

//-----------------------------------------------------------------------------
            
void TestConfig::run()
{
    test_parse();
    test_resource();
}

//-----------------------------------------------------------------------------

int main(int argc,char **argv)
{
    TestConfig mytest(argc,argv);
    mytest.start();
    return 0;
}

