//
//  bank.cpp
//  refectored
//
//  Created by Hui Zhou on 4/8/19.
//  Copyright © 2019 Hui Zhou. All rights reserved.
//

#include "bank.hpp"

double Bank::getCurrentWait() {
    if(line.size() == 0) {
        return 0.0;
    }
    for(customer c: line) {
        currentWait += c.serviceTime;
    }
    return currentWait;
}

int Bank::availableTeller() {
    for(int tellerIndex = 0; tellerIndex < tellers.size(); ++tellerIndex) {
        if(tellers.at(tellerIndex).currentCustomer.serviceTime < 0.0) {
            return tellerIndex;
        }
    }
    return -1;
}

void Bank::createCustomer(const double &serviceTimeMax) {
    double randNum = fmod(rand(), serviceTimeMax);
    customer c = customer(randNum);
    if(availableTeller() > 0) { // if there is an idle teller
        tellers.at(availableTeller()).currentCustomer = c;
        tellers.at(availableTeller()).available = false;
    } else { // else get in line
        addToLine(c);
    }
    allCustomers.push_back(c); // add to daily total
}

void Bank::addToLine(customer c) {
    c.totalServiceTime = getCurrentWait(); // calculate how long they are going to wait
    line.push_back(c); // add them to the line
}

void Bank::removeFromLine(){
    line.erase(line.begin(), line.begin() + 1); // erase first element in vector (bad priority queue)
}

void Bank::work() { /* runs once a second */
    for(int tellerIndex = 0; tellerIndex < tellers.size(); ++tellerIndex) {
        if(tellers.at(tellerIndex).currentCustomer.serviceTimeRemaining > 0.0) {
            tellers.at(tellerIndex).currentCustomer.serviceTimeRemaining = tellers.at(tellerIndex).currentCustomer.serviceTimeRemaining - 0.01;
        } else {
            tellers.at(tellerIndex).finished(); // mark teller as available and give them a customer
            if(getCurrentWait() > 0.0) { // check if there is a line and assign new customer if true
                tellers.at(tellerIndex).currentCustomer = line.at(0);
                removeFromLine();
            }
        }
    }
}

void runBank(double customerArrivalInterval, double &serviceTimeMax) {
    Bank b = Bank();
    int time = 0.0;
    double customerArrivalIntervalBase = customerArrivalInterval;
    while(time != 43200) { // number of seconds in 12 hours
        if(customerArrivalInterval >= 1.0) { // when a "whole" customer arrives, add them
            b.createCustomer(serviceTimeMax);
            --customerArrivalInterval;
        }
        b.work();
        customerArrivalInterval += customerArrivalIntervalBase;
        ++time;
    }
    // calculate after run
    double averageTime = 0.0;
    std::vector<double> totalTimes;
    for(Facility::customer c: b.allCustomers) {
        averageTime += c.totalServiceTime;
        totalTimes.push_back(c.totalServiceTime);
    }
    averageTime = averageTime / b.allCustomers.size();
    
    std::cout << "Average Time: " << averageTime << std::endl;
    std::sort(totalTimes.begin(), totalTimes.end(), std::less<double>());
    double onePercent = b.allCustomers.size() / 100.0;
    int tenth = onePercent * 10.0;
    int fifty = onePercent * 50.0;
    int ninty = onePercent * 90.0;
    std::cout << " Bank service times in minutes: 10th %ile " << totalTimes.at(tenth) << ", 50th %ile " << totalTimes.at(fifty) << ", 90th %ile " << totalTimes.at(ninty) << std::endl;
}

