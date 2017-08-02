//
//  BarnesHutSimulator.cpp
//  nbody_bh
//
//  Created by Anish Bhobe on 7/1/17.
//  Copyright © 2017 Anish Bhobe. All rights reserved.
//

#include "BarnesHutSimulator.hpp"

Celestial::BarnesHutSimulator::BarnesHutSimulator(){}

void Celestial::BarnesHutSimulator::Create(const std::vector<Body> &bodies){
    this->bodies = bodies;
    octree.Build(bodies);
    graphics.setSpan(octree.size);
}

void Celestial::BarnesHutSimulator::Run(double T, double dt){
    for(double time = 0; time < T; time += dt){
        octree.Build(bodies);
        octree.CalculateAcceleration();
        octree.Update(dt);
    }
}

void Celestial::BarnesHutSimulator::Draw(){
    octree.Draw(graphics);
}

void Celestial::BarnesHutSimulator::Print(){
    octree.Print();
}
