//
//  quad.hpp
//  nbody_bh
//
//  Created by Anish Bhobe on 6/29/17.
//  Copyright © 2017 Anish Bhobe. All rights reserved.
//

#ifndef quad_h
#define quad_h

#include "Eigen/Core"

using Eigen::Vector3d;
namespace Celestial {
    class Quad {
    public:
        // Center of the quad
        Vector3d center;
        
        // Half of the side of the quad
        double halfside;
        
        // Side of the quad
        double side;
        
        // Constructs a quad as per info
        Quad(const Vector3d& center, double side):center(center),halfside(side/2),side(side) {}
        
        // Checks if the quad contains the particular point
        // returns true if it does
        bool Contains(Vector3d point) {
            return (point[0] >= center[0] + halfside ||
                    point[0] <  center[0] - halfside ||
                    point[1] >= center[1] + halfside ||
                    point[1] <  center[1] - halfside )?false:true;
        }
    };
}

#endif /* quad_h */
