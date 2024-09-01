#include "includes.h"

// Helper function to run a test and print result
void runTest(const std::string& testName, const std::function<void()>& testFunc) {
    try {
        testFunc();
        std::cout << "PASS - " << testName << std::endl;
    } catch (const std::exception& e) {
        std::string errorMsg = e.what();
        // Replace newline characters in the error message with spaces
        std::replace(errorMsg.begin(), errorMsg.end(), '\n', ' ');
        std::cout << "FAIL - " << testName << " - " << errorMsg << std::endl;
    } catch (...) {
        std::cout << "FAIL - " << testName << " - Unknown error" << std::endl;
    }
}

bool                                   valueMatch                              ( const CValue                        & r,
                                                                                 const CValue                        & s )
/**
 * @brief Return a boolean indicating whether CValuae r,s match.
 * 
 */
{
  if ( r . index () != s . index () )
    return false;
  if ( r . index () == 0 )
    return true;
  if ( r . index () == 2 )
    return std::get<std::string> ( r ) == std::get<std::string> ( s );
  if ( std::isnan ( std::get<double> ( r ) ) && std::isnan ( std::get<double> ( s ) ) )
    return true;
  if ( std::isinf ( std::get<double> ( r ) ) && std::isinf ( std::get<double> ( s ) ) )
    return ( std::get<double> ( r ) < 0 && std::get<double> ( s ) < 0 )
           || ( std::get<double> ( r ) > 0 && std::get<double> ( s ) > 0 );
  return fabs ( std::get<double> ( r ) - std::get<double> ( s ) ) <= 1e8 * DBL_EPSILON * fabs ( std::get<double> ( r ) );
}

void printCValue( const CValue & value ) 
{
    /**
     * @brief Utility function to print cells value
     *        For debugging purposes only
     */
    if (std::holds_alternative<double>(value))
        std::cout << "CValue<double> : " << std::get<double>(value) << std::endl;
    if (std::holds_alternative<std::string>(value))
        std::cout << "CValue<std::string> : " << std::get<std::string>(value) << std::endl;
    if (std::holds_alternative<std::monostate>(value))
        std::cout << "CValue<std::monostate> : uninitialized" << std::endl;
}

