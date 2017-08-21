//
//  Body.hpp
//  nbody_bh
//
//  Created by Anish Bhobe on 6/29/17.
//  Copyright © 2017 Anish Bhobe. All rights reserved.
//

#ifndef Body_h
#define Body_h

#include <vector>
#include "Eigen/Core"
#include <array>
using Eigen::Vector3d;

namespace Celestial {
    class Body {
    public:
        
        // Mass of the body
        double mass;
        
        // Position of the body
        Vector3d position;
        
        // Velocity of the body
        Vector3d velocity;
        
        // Acceleration of the body
        // To be reset every frame.
        Vector3d acceleration;
        
        // Constructs empty body
        Body():mass(0),position(0,0,0),velocity(0,0,0),acceleration(0,0,0) {}
        
        // Copy the body
        void operator= (const Body& body) {
            mass = body.mass;
            position = body.position;
            velocity = body.velocity;
            acceleration = body.acceleration;
        }
        
        // Resets the acceleration of the body for the next frame
        void ResetAcceleration() { acceleration << 0,0,0; }
        
        // Creates the body out of a vector or array of the form:
        // m rx ry rz vx vy vz
        void Create(const std::vector<double> &vec) {
            mass = vec[0];
            position << vec[1], vec[2], vec[3];
            velocity << vec[4], vec[5], vec[6];
        }
        
        void Create(std::vector<double> &&vec) {
            mass = vec[0];
            position << vec[1], vec[2], vec[3];
            velocity << vec[4], vec[5], vec[6];
        }
        
        void Create(const std::array<double,7> &vec) {
            mass = vec[0];
            position << vec[1], vec[2], vec[3];
            velocity << vec[4], vec[5], vec[6];
        }
        
        void Create(std::array<double,7> &&vec) {
            mass = vec[0];
            position << vec[1], vec[2], vec[3];
            velocity << vec[4], vec[5], vec[6];
        }
        
        // Prints the object as a string
        std::string ToString() const {
            std::stringstream iostr;
            iostr << mass << " " << position[0] << " " << position[1] << " " << velocity[0] << " " << velocity[1];
            return iostr.str();
        }
        
        // Calculates the center of mass and total mass of the two bodies
        static Body CalculateCG(const Body& A, const Body& B) {
            Body temp;
            temp.mass = A.mass + B.mass;
            temp.position = (A.mass * A.position + B.mass * B.position)/temp.mass;
            temp.velocity = (A.mass * A.velocity + B.mass * B.velocity)/temp.mass;
            return temp;
        }
    };
}

#endif /* Body_h */
