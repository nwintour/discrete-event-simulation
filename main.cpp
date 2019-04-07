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

struct bank {
    double currentWait;
    std::vector<teller> tellers; // vector always size 1 or 0 depending on whether the teller has a customer
    std::vector<customer> line;
    std::vector<customer> allCustomers;
    
    bank() {
        currentWait = 0.0;
    }
    double getCurrentWait() {
        if(line.size() == 0) {
            return 0.0;
        }
        for(customer c: line) {
            currentWait += c.serviceTime;
        }
        return currentWait;
    }
    int availableTeller() {
        for(int tellerIndex = 0; tellerIndex < tellers.size(); ++tellerIndex) {
            if(tellers.at(tellerIndex).currentCustomer.serviceTime < 0.0) {
                return tellerIndex;
            }
        }
        return -1;
    }
    void createCustomer(const double &serviceTimeMax) {
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
    void addToLine(customer c) {
        c.totalServiceTime = getCurrentWait(); // calculate how long they are going to wait
        line.push_back(c); // add them to the line
    }
    void removeFromLine() {
        line.erase(line.begin(), line.begin() + 1); // erase first element in vector (bad priority queue)
    }
    void work() { /* runs once a second */
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
};

struct market {
    std::vector<cashier> cashiers;
    std::vector<customer> allCustomers;
    
    market() {
        for(int i = 0; i < 6; ++i) {
            cashier c = cashier();
            cashiers.push_back(c);
        }
    }
    void createCustomer(const double &serviceTimeMax) {
        cashier _cashier = getShortestWait();
        double randNum = fmod(rand(), serviceTimeMax);
        customer _customer = customer(randNum);
        _cashier.addToLine(_customer);
        allCustomers.push_back(_customer);
    }
    cashier getShortestWait() {
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
    void work() { /* runs once a second */
        for(cashier c: cashiers) {
            c.work();
        }
    }
};

void runMarket(double customerArrivalInterval, double &serviceTimeMax) {
    market m = market();
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
    for(customer c: m.allCustomers) {
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

void runBank(double customerArrivalInterval, double &serviceTimeMax) {
    bank b = bank();
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
    for(customer c: b.allCustomers) {
        averageTime += c.totalServiceTime;
        totalTimes.push_back(c.totalServiceTime);
    }
    averageTime = averageTime / b.allCustomers.size();
//    std::cout << "Total Bank Customers: " << b.allCustomers.size() << std::endl;
    std::cout << "Average Time: " << averageTime << std::endl;
    std::sort(totalTimes.begin(), totalTimes.end(), std::less<double>());
    double onePercent = b.allCustomers.size() / 100.0;
    int tenth = onePercent * 10.0;
    int fifty = onePercent * 50.0;
    int ninty = onePercent * 90.0;
    std::cout << " Bank service times in minutes: 10th %ile " << totalTimes.at(tenth) << ", 50th %ile " << totalTimes.at(fifty) << ", 90th %ile " << totalTimes.at(ninty) << std::endl;
}


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
