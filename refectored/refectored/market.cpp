//
//  market.cpp
//  refectored
//
//  Created by Hui Zhou on 4/8/19.
//  Copyright © 2019 Hui Zhou. All rights reserved.
//

#include "market.hpp"

void Market::createCustomer(const double &serviceTimeMax) {
    cashier _cashier = getShortestWait();
    double randNum = fmod(rand(), serviceTimeMax);
    customer _customer = customer(randNum);
    _cashier.addToLine(_customer);
    allCustomers.push_back(_customer);
}

Facility::cashier Market::getShortestWait() {
    cashier shortest = cashiers.at(0);
    double wait = cashiers.at(0).getCurrentWait();
    for(cashier c: cashiers) {
        if(c.getCurrentWait() < wait) {
            shortest = c;
            wait = c.getCurrentWait();
        }
    }
    return shortest;
}

void Market::work() { /* runs once a second */
    for(cashier c: cashiers) {
        c.work();
    }
}

void runMarket(double customerArrivalInterval, double &serviceTimeMax) {
    Market m = Market();
    int time = 0.0;
    double customerArrivalIntervalBase = customerArrivalInterval;
    while(time != 43200) { // number of seconds in 12 hours
        if(customerArrivalInterval >= 1.0) { // when a "whole" customer arrives, add them
            m.createCustomer(serviceTimeMax);
            --customerArrivalInterval;
        }
        m.work();
        customerArrivalInterval += customerArrivalIntervalBase;
        ++time;
    }
    // calculate after run
    double averageTime = 0.0;
    std::vector<double> totalTimes;
    for(Facility::customer c: m.allCustomers) {
        averageTime += c.totalServiceTime;
        totalTimes.push_back(c.totalServiceTime);
    }
    averageTime = averageTime / m.allCustomers.size();
    //    std::cout << "Total Market Customers: " << m.allCustomers.size() << std::endl;
    std::cout << "Average Time: " << averageTime << std::endl;
    std::sort(totalTimes.begin(), totalTimes.end(), std::less<double>());
    double onePercent = m.allCustomers.size() / 100.0;
    int tenth = onePercent * 10.0;
    int fifty = onePercent * 50.0;
    int ninty = onePercent * 90.0;
    std::cout << " Market service times in minutes: 10th %ile " << totalTimes.at(tenth) << ", 50th %ile " << totalTimes.at(fifty) << ", 90th %ile " << totalTimes.at(ninty) << std::endl;
}
