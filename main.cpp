//
//  main.cpp
//  nbody_bh
//
//  Created by Anish Bhobe on 6/16/17.
//  Copyright © 2017 Anish Bhobe. All rights reserved.
//

#include <iostream>
#include <vector>
#include "BarnesHutSimulator.hpp"

int main(int argc, char * argv[]) {
    
    std::vector<double> bs{1, 4, 4, 0, -4, 4, 0};
    std::vector<double> br{1, -4, -4, 0, 4, -4, 0};
    std::vector<double> bc{1, 2, 2, 0, 2, 2, 0};
    std::vector<double> be{1, 2, 3, 0, 2, 2, 0};
    std::vector<double> ba{1, 1.9,3,0,0,0,0};
    std::vector<Celestial::Body> bodies(5);
    
    bodies[0].Create(bs);
    bodies[1].Create(br);
    bodies[2].Create(bc);
    bodies[3].Create(be);
    bodies[4].Create(ba);
    
    Celestial::BarnesHutSimulator sim;
    sim.Create(bodies);
    sim.Draw();
    sim.Print();
    
    std::cin.ignore();
    
    return 0;
}
