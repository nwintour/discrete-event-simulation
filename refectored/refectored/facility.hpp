//
//  facility.hpp
//  refectored
//
//  Created by Hui Zhou on 4/8/19.
//  Copyright © 2019 Hui Zhou. All rights reserved.
//

#ifndef facility_hpp
#define facility_hpp

#include <stdio.h>
#include <iostream>
#include <vector>

class Facility{
public:
    struct customer {
        double serviceTime; // random amount of time it will take this customer in seconds
        double serviceTimeRemaining; // amount of time remaining while customer is being attended
        double totalServiceTime; // total time it took for this customer in seconds
        
        customer(const double serviceTime) { // initialize all to service time
            this->serviceTime = serviceTime;
            this->serviceTimeRemaining = serviceTime;
            this->totalServiceTime = serviceTime;
        }
    };
    
    struct cashier {
        double currentWait; // current wait of all custumers in their line
        std::vector<customer> line; //customer at index 0 is currently being serviced
        
        cashier() {
            currentWait = 0.0;
        }
        double getCurrentWait() {
            if(line.size() == 0) {
                return 0.0;
            }
            for(customer c: line) {
                currentWait += c.serviceTime;
            }
            currentWait += line.at(0).serviceTimeRemaining;
            return currentWait;
        }
        void addToLine(customer c) {
            c.totalServiceTime = getCurrentWait();
            line.push_back(c);
        }
        void removeFromLine() {
            line.erase(line.begin(), line.begin() + 1); // erase first element
        }
        void work() {
            if(line.size() > 0) {
                if(line.at(0).serviceTimeRemaining > 0.0) {
                    line.at(0).serviceTimeRemaining = line.at(0).serviceTimeRemaining - 0.01;
                } else {
                    removeFromLine();
                }
            }
        }
    };
    
    struct teller {
        customer currentCustomer = customer(-1); // add a dummy customer
        bool available;
        teller() {
            available = true;
        }
        void finished() {
            currentCustomer = customer(-1); // add a dummy customer
            available = true;
        }
    };
};

#endif /* facility_hpp */
