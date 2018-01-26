//
//  BarnesHutSimulator.cpp
//  nbody_bh
//
//  Created by Anish Bhobe on 7/1/17.
//  Copyright © 2017 Anish Bhobe. All rights reserved.
//

#include "BarnesHutSimulator.hpp"

Celestial::BarnesHutSimulator::BarnesHutSimulator(){}

//Builds the bodies using the function in octree
//Sets Span using the size of the octree
void Celestial::BarnesHutSimulator::Create(const std::vector<Body> &bodies){
    this->bodies = bodies;
    octree.Build(bodies);
    graphics.setSpan(octree.size);
}

//Builds the bodies using the function in octree
//calculates the acceleration of the body with the theta = 0.5
//theta is the compromise in accuracy for performance
void Celestial::BarnesHutSimulator::Run(double T, double dt){
    for(double time = 0; time < T; time += dt){
        octree.Build(bodies);
        octree.CalculateAcceleration(0.5);
        octree.Update(dt);
    }
}

//Calls Draw in octree
void Celestial::BarnesHutSimulator::Draw(){
    octree.Draw(graphics);
}

//Calls Print in octree 
void Celestial::BarnesHutSimulator::Print(){
    octree.Print();
}
