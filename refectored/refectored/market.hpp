//
//  market.hpp
//  refectored
//
//  Created by Hui Zhou on 4/8/19.
//  Copyright © 2019 Hui Zhou. All rights reserved.
//

#ifndef market_hpp
#define market_hpp

#include <stdio.h>
#include "facility.hpp"
#include <math.h>

class Market : Facility{
public:
    
    std::vector<cashier> cashiers;
    std::vector<customer> allCustomers;
    
    Market() {
        for(int i = 0; i < 6; ++i) {
            cashier c = cashier();
            cashiers.push_back(c);
        }
    }
    
    void createCustomer(const double &serviceTimeMax);
    
    cashier getShortestWait();
    
    void work();
    
};

void runMarket(double customerArrivalInterval, double &serviceTimeMax);

#endif /* market_hpp */
