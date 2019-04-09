//
//  bank.hpp
//  refectored
//
//  Created by Hui Zhou on 4/8/19.
//  Copyright © 2019 Hui Zhou. All rights reserved.
//

#ifndef bank_hpp
#define bank_hpp

#include <stdio.h>
#include "facility.hpp"
#include <math.h>

class Bank : Facility{
public:
    
    double currentWait;
    std::vector<teller> tellers; // vector always size 1 or 0 depending on whether the teller has a customer
    std::vector<customer> line;
    std::vector<customer> allCustomers;
    
    Bank() {
        currentWait = 0.0;
    }
    
    double getCurrentWait();
    
    int availableTeller();
    
    void createCustomer(const double &serviceTimeMax);
    
    void addToLine(customer c);
    
    void removeFromLine();
    
    void work();
};

void runBank(double customerArrivalInterval, double &serviceTimeMax);

#endif /* bank_hpp */
