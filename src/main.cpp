#include "includes.h"
#include "CPosition.h"
#include "CCell.h"
#include "CExpression.h"
#include "CTest.h"
#include "CSpreadsheet.h"

/**
 * @brief SpreadSheetApp
 *        Jakub Nierostek, 2024
 */

int main ()
{
    /**
     * @brief Given and custom implementation of requirment tests
     * 
     */

    CSpreadsheet x0, x1;
    std::ostringstream oss;
    std::istringstream iss;
    std::string data;

    /* Test CPosition */
    runTest("Test 1: Class CPosition", []() {
        CPosition p0 = CPosition("A1");
        assert ( p0.getRow() == 1 && p0.getCol() == 1 && p0.getRowAbs() == false && p0.getColAbs() == false );
        CPosition p1 = CPosition("BA232");
        assert ( p1.getRow() == 232 && p1.getCol() == 53 && p1.getRowAbs() == false && p1.getColAbs() == false );
        CPosition p2 = CPosition("c3");
        assert ( p2.getRow() == 3 && p2.getCol() == 3 && p2.getRowAbs() == false && p2.getColAbs() == false );
        CPosition p3 = CPosition("$A$1");
        assert ( p3.getRow() == 1 && p3.getCol() == 1 && p3.getRowAbs() == true && p3.getColAbs() == true );
        CPosition p4 = CPosition("A$1");
        assert ( p4.getRow() == 1 && p4.getCol() == 1 && p4.getRowAbs() == true && p4.getColAbs() == false );
        CPosition p5 = CPosition("$A1");
        assert ( p5.getRow() == 1 && p5.getCol() == 1 && p5.getRowAbs() == false && p5.getColAbs() == true );
    });

    CExpressionBuilder builder = CExpressionBuilder( x0 );
    runTest("Test 2: Class CExpression", [&]() {
        CExpressionBuilder builder = CExpressionBuilder( x0 );
        parseExpression( "=1+2*3-A5^$F$8", builder );
        parseExpression( "23", builder );
    });
    

    runTest("Test 3: Static Linking of a Parser", [&]() {
    try {
        parseExpression( "=asd457aw7f545as45+hello_world", builder );
    } catch ( std::invalid_argument & ) {
        // std::cout << "Cought invalid argument exception" << std::endl;
    }
    //parseExpression( "=1+2*3-A5AA5g", builder );
    });

    runTest("Test 4: Method CCell::setCell - various datatypes", [&]() {
        assert ( x0 . setCell ( CPosition ( "A1" ), "10" ) );
        assert ( x0 . setCell ( CPosition ( "A2" ), "20.5" ) );
        assert ( x0 . setCell ( CPosition ( "A3" ), "3e1" ) );
        assert ( x0 . setCell ( CPosition ( "A4" ), "=40" ) );
        assert ( x0 . setCell ( CPosition ( "A5" ), "=5e+1" ) );
        assert ( x0 . setCell ( CPosition ( "A6" ), "raw text with any characters, including a quote \" or a newline\n" ) );
        assert ( x0 . setCell ( CPosition ( "A7" ), "=\"quoted string, quotes must be doubled: \"\". Moreover, backslashes are needed for C++.\"" ) );
    });

    runTest("Test 5: Method CCell::getValue", [&]() {
        assert ( valueMatch ( x0 . getValue ( CPosition ( "A1" ) ), CValue ( 10.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "A2" ) ), CValue ( 20.5 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "A3" ) ), CValue ( 30.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "A4" ) ), CValue ( 40.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "A5" ) ), CValue ( 50.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "A6" ) ), CValue ( "raw text with any characters, including a quote \" or a newline\n" ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "A7" ) ), CValue ( "quoted string, quotes must be doubled: \". Moreover, backslashes are needed for C++." ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "A8" ) ), CValue() ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "AAAA9999" ) ), CValue() ) );
    });

    runTest("Test 6: Set and evaluate expression as cell content", [&]() {
        assert ( x0 . setCell ( CPosition ( "B1" ), "=A1+A2*A3" ) );
        assert ( x0 . setCell ( CPosition ( "B2" ), "= -A1 ^ 2 - A2 / 2   " ) );
        assert ( x0 . setCell ( CPosition ( "B3" ), "= 2 ^ $A$1" ) );
        assert ( x0 . setCell ( CPosition ( "B4" ), "=($A1+A$2)^2" ) );
        assert ( x0 . setCell ( CPosition ( "B5" ), "=B1+B2+B3+B4" ) );
        assert ( x0 . setCell ( CPosition ( "B6" ), "=B1+B2+B3+B4+B5" ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "B1" ) ), CValue ( 625.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "B2" ) ), CValue ( -110.25 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "B3" ) ), CValue ( 1024.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "B4" ) ), CValue ( 930.25 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "B5" ) ), CValue ( 2469.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "B6" ) ), CValue ( 4938.0 ) ) );
        assert ( x0 . setCell ( CPosition ( "A1" ), "12" ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "B1" ) ), CValue ( 627.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "B2" ) ), CValue ( -154.25 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "B3" ) ), CValue ( 4096.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "B4" ) ), CValue ( 1056.25 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "B5" ) ), CValue ( 5625.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "B6" ) ), CValue ( 11250.0 ) ) );
        x1 = x0;
        assert ( x0 . setCell ( CPosition ( "A2" ), "100" ) );
        assert ( x1 . setCell ( CPosition ( "A2" ), "=A3+A5+A4" ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "B1" ) ), CValue ( 3012.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "B2" ) ), CValue ( -194.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "B3" ) ), CValue ( 4096.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "B4" ) ), CValue ( 12544.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "B5" ) ), CValue ( 19458.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "B6" ) ), CValue ( 38916.0 ) ) );
        assert ( valueMatch ( x1 . getValue ( CPosition ( "B1" ) ), CValue ( 3612.0 ) ) );
        assert ( valueMatch ( x1 . getValue ( CPosition ( "B2" ) ), CValue ( -204.0 ) ) );
        assert ( valueMatch ( x1 . getValue ( CPosition ( "B3" ) ), CValue ( 4096.0 ) ) );
        assert ( valueMatch ( x1 . getValue ( CPosition ( "B4" ) ), CValue ( 17424.0 ) ) );
        assert ( valueMatch ( x1 . getValue ( CPosition ( "B5" ) ), CValue ( 24928.0 ) ) );
        assert ( valueMatch ( x1 . getValue ( CPosition ( "B6" ) ), CValue ( 49856.0 ) ) );
    });

    runTest("Test 7: Save and load spreadsheet - basic save and load", [&]() {
        oss . clear ();
        oss . str ( "" );
        assert ( x0 . save ( oss ) );
        data = oss . str ();
        // std::cout << data;
        iss . clear ();
        iss . str ( data );
        assert ( x1 . load ( iss ) );
        assert ( valueMatch ( x1 . getValue ( CPosition ( "B1" ) ), CValue ( 3012.0 ) ) );
        assert ( valueMatch ( x1 . getValue ( CPosition ( "B2" ) ), CValue ( -194.0 ) ) );
        assert ( valueMatch ( x1 . getValue ( CPosition ( "B3" ) ), CValue ( 4096.0 ) ) );
        assert ( valueMatch ( x1 . getValue ( CPosition ( "B4" ) ), CValue ( 12544.0 ) ) );
        assert ( valueMatch ( x1 . getValue ( CPosition ( "B5" ) ), CValue ( 19458.0 ) ) );
        assert ( valueMatch ( x1 . getValue ( CPosition ( "B6" ) ), CValue ( 38916.0 ) ) );
        assert ( x0 . setCell ( CPosition ( "A3" ), "4e1" ) );
        assert ( valueMatch ( x1 . getValue ( CPosition ( "B1" ) ), CValue ( 3012.0 ) ) );
        assert ( valueMatch ( x1 . getValue ( CPosition ( "B2" ) ), CValue ( -194.0 ) ) );
        assert ( valueMatch ( x1 . getValue ( CPosition ( "B3" ) ), CValue ( 4096.0 ) ) );
        assert ( valueMatch ( x1 . getValue ( CPosition ( "B4" ) ), CValue ( 12544.0 ) ) );
        assert ( valueMatch ( x1 . getValue ( CPosition ( "B5" ) ), CValue ( 19458.0 ) ) );
        assert ( valueMatch ( x1 . getValue ( CPosition ( "B6" ) ), CValue ( 38916.0 ) ) );
    });

    runTest("Test 8: Save and load spreadsheet - handle data corruption", [&]() {
        oss . clear ();
        oss . str ( "" );
        assert ( x0 . save ( oss ) );
        data = oss . str ();
        for ( size_t i = 0; i < std::min<size_t> ( data . length (), 10 ); i ++ )
            data[i] ^=0x5a;
        iss . clear ();
        iss . str ( data );
        assert ( ! x1 . load ( iss ) );
    });

    ///*
    runTest("Test 9: Save and load spreadsheet - handle cell references", [&]() {
    assert ( x0 . setCell ( CPosition ( "D0" ), "10" ) );
    assert ( x0 . setCell ( CPosition ( "D1" ), "20" ) );
    assert ( x0 . setCell ( CPosition ( "D2" ), "30" ) );
    assert ( x0 . setCell ( CPosition ( "D3" ), "40" ) );
    assert ( x0 . setCell ( CPosition ( "D4" ), "50" ) );
    assert ( x0 . setCell ( CPosition ( "E0" ), "60" ) );
    assert ( x0 . setCell ( CPosition ( "E1" ), "70" ) );
    assert ( x0 . setCell ( CPosition ( "E2" ), "80" ) );
    assert ( x0 . setCell ( CPosition ( "E3" ), "90" ) );
    assert ( x0 . setCell ( CPosition ( "E4" ), "100" ) );
    assert ( x0 . setCell ( CPosition ( "F10" ), "=D0+5" ) );
    assert ( x0 . setCell ( CPosition ( "F11" ), "=$D0+5" ) );
    assert ( x0 . setCell ( CPosition ( "F12" ), "=D$0+5" ) );
    assert ( x0 . setCell ( CPosition ( "F13" ), "=$D$0+5" ) );
    oss . clear ();
    oss . str ( "" );
    assert ( x0 . save ( oss ) );
    data = oss . str ();
    // std::cout << data;
    iss . clear ();
    iss . str ( data );
    assert ( x0 . load ( iss ) );
    });

    runTest("Test 10: Copy cells - basic copying (incl. references)", [&]() {
    x0 . copyRect ( CPosition ( "G11" ), CPosition ( "F10" ), 1, 4 );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "F10" ) ), CValue ( 15.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "F11" ) ), CValue ( 15.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "F12" ) ), CValue ( 15.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "F13" ) ), CValue ( 15.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "F14" ) ), CValue() ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "G10" ) ), CValue() ) );
        /* --- debugging --- */
        // std::cout << x0 . getCell( CPosition ( "G11" ) ) << std::endl;
        // CValue v7 = x0 . getValue ( CPosition ( "G11" ) );
        // printCValue ( v7 ); 
        /* --- debugging --- */
        assert ( valueMatch ( x0 . getValue ( CPosition ( "G11" ) ), CValue ( 75.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "G12" ) ), CValue ( 25.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "G13" ) ), CValue ( 65.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "G14" ) ), CValue ( 15.0 ) ) );
    });

    runTest("Test 11: Copy cells - advanced copying (recursive references)", [&]() {
        x0 . copyRect ( CPosition ( "G11" ), CPosition ( "F10" ), 2, 4 );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "F10" ) ), CValue ( 15.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "F11" ) ), CValue ( 15.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "F12" ) ), CValue ( 15.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "F13" ) ), CValue ( 15.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "F14" ) ), CValue() ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "G10" ) ), CValue() ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "G11" ) ), CValue ( 75.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "G12" ) ), CValue ( 25.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "G13" ) ), CValue ( 65.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "G14" ) ), CValue ( 15.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "H10" ) ), CValue() ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "H11" ) ), CValue() ) );
    });

    /* Test copying and saving */
    runTest("Test 12: Save and load spreadsheet - recursive references", [&]() {
        /* --- debugging --- */
        // std::cout << x0 . getCell( CPosition ( "H12" ) ) << std::endl;
        // CValue v4 = x0 . getValue ( CPosition ( "H12" ) );
        // printCValue ( v4 ); 
        /* --- debugging --- */
        oss . clear ();
        oss . str ( "" );
        assert ( x0 . save ( oss ) );
        data = oss . str ();
        // std::cout << data;
        iss . clear ();
        iss . str ( data );
        assert ( x0 . load ( iss ) );
        /* --- debugging --- */
        // std::cout << x0 . getCell( CPosition ( "H12" ) ) << std::endl;
        // CValue v4 = x0 . getValue ( CPosition ( "H12" ) );
        // printCValue ( v4 ); 
        /* --- debugging --- */
        assert ( valueMatch ( x0 . getValue ( CPosition ( "H12" ) ), CValue() ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "H13" ) ), CValue ( 35.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "H14" ) ), CValue() ) );
        assert ( x0 . setCell ( CPosition ( "F0" ), "-27" ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "H14" ) ), CValue ( -22.0 ) ) );
        x0 . copyRect ( CPosition ( "H12" ), CPosition ( "H13" ), 1, 2 );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "H12" ) ), CValue ( 25.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "H13" ) ), CValue ( -22.0 ) ) );
        assert ( valueMatch ( x0 . getValue ( CPosition ( "H14" ) ), CValue ( -22.0 ) ) );
    });
    //*/



    /* Test comparison operators */

    CSpreadsheet x2;
    std::ostringstream oss2;
    std::istringstream iss2;

    runTest("Test 13: Operators - comparative and arithmetic operators", [&]() {
        assert(x2.setCell(CPosition("A1"), "=100"));
        assert(x2.setCell(CPosition("A2"), "=200"));
        assert(x2.setCell(CPosition("A3"), "=300"));
        assert(x2.setCell(CPosition("A4"), "=0"));
        assert(x2.setCell(CPosition("A5"), "=150"));
        assert(x2.setCell(CPosition("B1"), "=A1 + A2"));
        assert(valueMatch(x2.getValue(CPosition("B1")), CValue(300.0)));
        assert(x2.setCell(CPosition("B2"), "=A3 - A1"));
        assert(valueMatch(x2.getValue(CPosition("B2")), CValue(200.0)));
        assert(x2.setCell(CPosition("B3"), "=A1 * A2"));
        assert(valueMatch(x2.getValue(CPosition("B3")), CValue(20000.0)));
        assert(x2.setCell(CPosition("B4"), "=A3 / A5"));
        assert(valueMatch(x2.getValue(CPosition("B4")), CValue(2.0)));
        assert(x2.setCell(CPosition("B5"), "=A5 ^ 2"));
        assert(valueMatch(x2.getValue(CPosition("B5")), CValue(22500.0)));
        assert(x2.setCell(CPosition("B6"), "=-A5"));
        assert(valueMatch(x2.getValue(CPosition("B6")), CValue(-150.0)));
        assert(x2.setCell(CPosition("C1"), "=A1 = 100"));
        assert(valueMatch(x2.getValue(CPosition("C1")), CValue(1.0)));
        assert(x2.setCell(CPosition("C2"), "=A1 <> A2"));
        assert(valueMatch(x2.getValue(CPosition("C2")), CValue(1.0)));
        assert(x2.setCell(CPosition("C3"), "=A1 < A2"));
        assert(valueMatch(x2.getValue(CPosition("C3")), CValue(1.0)));
        assert(x2.setCell(CPosition("C4"), "=A1 <= 100"));
        assert(valueMatch(x2.getValue(CPosition("C4")), CValue(1.0)));
        assert(x2.setCell(CPosition("C5"), "=A3 > A2"));
        assert(valueMatch(x2.getValue(CPosition("C5")), CValue(1.0)));
        assert(x2.setCell(CPosition("C6"), "=A3 >= 300"));
        assert(valueMatch(x2.getValue(CPosition("C6")), CValue(1.0)));
        assert(x2.setCell(CPosition("C7"), "=10 + 20"));
        assert(valueMatch(x2.getValue(CPosition("C7")), CValue(30.0)));
    });



    /* Test cyclic dependencies */

    CSpreadsheet x3;
    std::ostringstream oss3;
    std::istringstream iss3;

    runTest("Test 14: Cyclic dependencies", [&]() {
        x3.setCell(CPosition("A1"), "=B1");
        x3.setCell(CPosition("B1"), "=A1");
        try {
            auto val = x3.getValue(CPosition("A1"));
        } catch ( const std::runtime_error & e ) {
            std::cout << "Detected cycle: " << e.what() << std::endl;
        }
        x3.setCell(CPosition("A3"), "=A3");
        try {
            auto val = x3.getValue(CPosition("A3"));
        } catch ( const std::runtime_error & e ) {
            std::cout << "Detected cycle: " << e.what() << std::endl;
        }
    });

    return EXIT_SUCCESS;
}