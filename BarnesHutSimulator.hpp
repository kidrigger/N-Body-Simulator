//
//  BarnesHutSimulator.hpp
//  nbody_bh
//
//  Created by Anish Bhobe on 7/1/17.
//  Copyright © 2017 Anish Bhobe. All rights reserved.
//

#ifndef BarnesHutSimulator_hpp
#define BarnesHutSimulator_hpp

#include <stdio.h>
#include <vector>
#include "Body.hpp"
#include "octree.hpp"
#include "Graphics.hpp"

namespace Celestial {
    class BarnesHutSimulator {
    public:
        BarnesHutSimulator();
        void Create(const std::vector<Body>& bodies);
        void Run(double T, double dt);
        void Destroy();
        void Test(int n);
    private:
        double span;
        void centralize(const Body& center);
        std::vector<Body> bodies;
        Graphics graphics;
    };
}

#endif /* BarnesHutSimulator_hpp */
