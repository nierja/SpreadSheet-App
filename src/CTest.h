#ifndef CTEST_H
#define CTEST_H

#include "includes.h"

/// @brief Helper function to run a test and print result
/// @param testName string containing the name of the test case
/// @param testFunc function to be executed upon test, usually a lambda
void                                    runTest                                 ( const std::string                   & testName, 
                                                                                  const std::function<void()>         & testFunc );

/// @brief corretly compare CValues (std::variant<T>())
/// @param r reference to first cell's CValue
/// @param s reference to second cell's CValue
/// @return equality (boolean)
bool                                    valueMatch                              ( const CValue                        & r,
                                                                                  const CValue                        & s );
                                                                                  
/// @brief Utility function for printing CCell contents
/// @param value 
void                                    printCValue                             ( const CValue                        & value ) ;

#endif // CTEST_H