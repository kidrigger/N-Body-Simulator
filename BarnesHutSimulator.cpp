//
//  BarnesHutSimulator.cpp
//  nbody_bh
//
//  Created by Anish Bhobe on 7/1/17.
//  Copyright © 2017 Anish Bhobe. All rights reserved.
//

#include "BarnesHutSimulator.hpp"
#include <future>

Celestial::BarnesHutSimulator::BarnesHutSimulator():graphics(800,800){}

void Celestial::BarnesHutSimulator::Create(const std::vector<Body> &bodies){
    this->bodies = bodies;
    {
        Octree octree;
        span = octree.Build(bodies);
        centralize(octree.GetSystemCG());
        span = octree.Build(bodies);
        // octree.Print();
        graphics.setSpan(octree.size);
    }
}

void Celestial::BarnesHutSimulator::Run(double T, double dt){
    Octree octree;
    octree.Build(bodies,span);
    octree.CalculateAcceleration(0.5);
    std::cout << std::endl;
    for(double time = 0; time < T; time += dt){
        // printf("%f\n",time);
        // octree.Print();
        this->bodies = octree.Update(dt);
        auto future_oct = std::async(std::launch::async,Octree::MakeAcceleratedOctree,bodies,span);
        octree.Draw(graphics);
        octree = future_oct.get();
        // graphics.setSpan(octree.size);
        printf("%f",time);
        fflush(stdout);
        printf("\r");
    }
}

void Celestial::BarnesHutSimulator::Test(int n) {
    for(auto i = 0; i != n; ++i)
    {
        Octree octree;
        octree.Build(bodies);
        printf("%i\n",i);
    }
    Octree octree;
    octree.Build(bodies);
    octree.Draw(graphics);
}

void Celestial::BarnesHutSimulator::centralize(const Celestial::Body& center) {
    for(auto it = bodies.begin(); it != bodies.end(); ++it) {
        it->position = it->position - center.position;
        it->velocity = it->velocity - center.velocity;
    }
}