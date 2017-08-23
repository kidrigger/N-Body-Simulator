//
//  main.cpp
//  nbody_bh
//
//  Created by Anish Bhobe on 6/16/17.
//  Copyright © 2017 Anish Bhobe. All rights reserved.
//

#include <iostream>
#include <vector>
#include "Input.hpp"
#include "BarnesHutSimulator.hpp"
#include <future>



int main(int argc, char * argv[]) {
    
    Celestial::NBodySimInput input;
    auto bodies = input.GetBodyInput();
    
    Celestial::BarnesHutSimulator sim;
    sim.Create(bodies);
    sim.Run(1000,0.01);
    std::cout << "Completed" << std::endl;
    
    return 0;
}
