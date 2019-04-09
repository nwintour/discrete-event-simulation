//
//  main.cpp
//  DiscreteEventSimulation
//
//  Created by Natalie Wintour on 4/2/19.
//  Copyright © 2019 Natalie Wintour. All rights reserved.
//

#include <iostream>
#include <vector>
#include <chrono>
#include <math.h>
#include "facility.hpp"
#include "bank.hpp"
#include "market.hpp"

int main(int argc, const char * argv[]) {
    if(argc < 4) {
        std::cout << "Invalid input." << std::endl;
        exit(1);
    }
    double CAI = atof(argv[1]);
    double STM = atof(argv[2]);
    double SEED = atof(argv[3]);
    double customerArrivalInterval = CAI / 60.0; // number of customers per minute
    double serviceTimeMax = STM;
    if(customerArrivalInterval == 0 || serviceTimeMax == 0) {
        std::cout << "Invalid input. Input must be three values, i.e. '0.00 0.00 0.00'";
        exit(1);
    }
    srand(SEED);
    runMarket(customerArrivalInterval, serviceTimeMax);
    runBank(customerArrivalInterval, serviceTimeMax);
    return 0;
    /* Your program should produce output something like this:
     
     Bank service times in minutes: 10th %ile 3.52, 50th %ile 12.3, 90th %ile 41.3
     Supermarket service times in minutes: 10th %ile 5.22, 50th %ile 15.3, 90th %ile 61.3
     
     The Nth %ile (percentile) is the total service time of the customer who waited longer than N percent of customers and shorter than (100-N) percent of customers. */
}

