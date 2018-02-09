//
//  Input.hpp
//  nbody_bh
//
//  Created by Anish Bhobe on 8/18/17.
//  Copyright © 2017 Anish Bhobe. All rights reserved.
//

#ifndef Input_hpp
#define Input_hpp

#include <iostream>
#include <vector>
#include <array>
#include "Body.hpp"

namespace Celestial {
    class NBodySimInput {
    public:
        std::vector<Body> GetBodyInput() {
            int n;
            std::cin >> n;
            std::vector<Body> bodies(n);
            for(int i = 0; i != n; ++i) {
                bodies[i].Create(getArrayInput());
            }
            return bodies;
        }
    private:
        std::array<double,7> getArrayInput() {
            double m,x,y,z,vx,vy,vz;
            std::cin >> m >> x >> y >> z >> vx >> vy >> vz;
            return std::array<double,7>{m,x,y,z,vx,vy,vz};
        }
    };
}

#endif /* Input_hpp */
