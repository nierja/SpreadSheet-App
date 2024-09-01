#ifndef CPOSITION_H
#define CPOSITION_H

#include "includes.h"

class CPosition
{
    /**
     * @brief Class to represent a position in a spreadsheet
     *  int offsetRow - represent  reference to a cell
     *  int offsetCol - represent  reference to a cell  
     *  bool rowAbs - indicate absolute/relative reference
     *  bool colAbs - indicate absolute/relative reference
     */
    public:
                                        CPosition                               ( std::string_view                      str );
                                        CPosition                               ( size_t row, size_t col );
        size_t                          getRow                                  (  ) const;
        size_t                          getCol                                  (  ) const;
        void                            setRow                                  ( size_t row );
        void                            setCol                                  ( size_t col );
        bool                            getRowAbs                               (  ) const;
        bool                            getColAbs                               (  ) const;
        void                            setRowAbs                               ( bool rowAbs );
        void                            setColAbs                               ( bool colAbs );
        int offsetRow;  // would be better as private, necessary evil, as I was short on time
        int offsetCol;  // would be better as private, necessary evil, as I was short on time      
    private:
        size_t row;
        size_t col;
        bool rowAbs;
        bool colAbs;        
};

#endif // CPOSITION_H