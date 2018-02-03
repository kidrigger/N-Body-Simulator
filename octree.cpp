//
//  octree.cpp
//  nbody_bh
//
//  Created by Anish Bhobe on 6/16/17.
//  Copyright © 2017 Anish Bhobe. All rights reserved.
//

#include "octree.hpp"
#include <future>
#include <algorithm>
#include <iostream>

Celestial::Octree::Octree():root(nullptr,Vector3d(0,0,0),0,0) {}

#ifdef PARALLEL

//
// Find maximum between the two points
double maxim(int initial, int final, const vector<Celestial::Body> bodies){
    double sqrmax = 0;
    // std::cout << initial << " started\n";
    for(int i = initial; i != final; ++i){
        double sqrnrm = bodies[i].position.squaredNorm();
        sqrmax = (sqrnrm>sqrmax)?sqrnrm:sqrmax;
    }
    // std::cout << final << " completed\n";
    return sqrmax;
}

// Divide the job into three and launch async
double Celestial::Span(const vector<Celestial::Body>& bodies){
    auto len = bodies.size();
    auto first_border = len>>2;
    auto half_border = len>>1;
    auto three_quarts = (len>>1) + (len>>2);
    auto h1 = std::async(std::launch::async,maxim,0,first_border,bodies);
    auto h2 = std::async(std::launch::async,maxim,first_border,half_border,bodies);
    auto h3 = std::async(std::launch::async,maxim,half_border,three_quarts,bodies);
    auto h4 = std::async(std::launch::async,maxim,three_quarts,len,bodies);
    double sqrmax = 0;
    sqrmax = h1.get();
    double temp = h2.get();
    sqrmax = (temp>sqrmax)?temp:sqrmax;
    temp = h3.get();
    sqrmax = (temp>sqrmax)?temp:sqrmax;
    temp = h4.get();
    sqrmax = (temp>sqrmax)?temp:sqrmax;
    return sqrt(sqrmax);
}

#else // if PARALLEL not defined


double Celestial::Span(const vector<Celestial::Body>& bodies){
    double sqrmax = 0;
    for(auto it = bodies.begin(); it != bodies.end(); ++it){
        sqrmax = std::max(sqrmax,it->position.squaredNorm());
    }
    return sqrt(sqrmax);
}

#endif // end if statement

void Celestial::Octree::Print(){
    PrintDFS(root);
}

void Celestial::Octree::PrintDFS(const Celestial::Node &head, int level){
    for(int i = 0 ; i != level; ++i){
        std::cout << "   ";
    }
    std::cout << "[" << head.GetID() << "]<"<< head.StateToString() << "> "<< head.GetCG().ToString() << "\n";
    if(head.nodeState != NodeState::Branch)
    {
        return;
    }
    for(int i = 0; i != 4; ++i){
        PrintDFS(head.nodeArray[i],level+1);
    }
}

double Celestial::Octree::Build(const vector<Body>& bodies, double span){
    this->bodies = bodies;
#ifndef DYN_SPAN
    if(span == 0)
#endif
        size = 2*Span(bodies);
#ifndef DYN_SPAN
    else
        size = 2*span;
#endif
    root = Node(nullptr,Eigen::Vector3d(0,0,0),size,0);
    for(auto it = bodies.begin(); it != bodies.end(); ++it){
        root.Add(*it);
        // <<<<<<<< DEBUG ONLY >>>>>>>> //
        /*
        this->Print();
        std::cin.ignore();
        std::cout << std::endl;
        //*/
    }
    return size/2;
}

void Celestial::Octree::Draw(Celestial::Graphics &graphics){
    graphics.Clear();
    DrawDFS(graphics, root);
    graphics.Draw();
}

void Celestial::Octree::DrawDFS(Celestial::Graphics &graphics, const Celestial::Node &head){
    graphics.DrawQuad(head.GetQuad());
    if(head.nodeState == NodeState::Leaf)
    {
        graphics.DrawPoint(head.GetCG());
        return;
    }
    if(head.nodeState == NodeState::Empty)
    {
        return;
    }
    for(int i = 0; i != 4; ++i){
        DrawDFS(graphics, head.nodeArray[i]);
    }
}

void Celestial::Octree::CalculateAcceleration(double theta){
    auto n = bodies.size();
    for (int i = 0; i != n; ++i) {
        bodies[i].acceleration = root.TotalAcceleration(bodies[i], theta);
    }
}

std::vector<Celestial::Body> Celestial::Octree::Update(double dt){
    int debug_i = 0;
    for(auto it = bodies.begin(); it != bodies.end(); ++it, debug_i++) {
        it->velocity += 0.5 * it->acceleration * dt;
        it->position += it->velocity * dt;
        it->velocity += 0.5 * it->acceleration * dt;
        it->ResetAcceleration();
    }
    return bodies;
}
