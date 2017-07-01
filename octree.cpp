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

#define PARALLEL

Celestial::Octree::Octree():root(Vector3d(0,0,0),0,0) {}

#ifdef PARALLEL

double maxim(int initial, int final, const vector<Celestial::Body>& bodies){
    double sqrmax = 0;
    for(int i = initial; i != final; ++i){
        double sqrnrm = bodies[i].position.squaredNorm();
        sqrmax = (sqrnrm>sqrmax)?sqrnrm:sqrmax;
    }
    return sqrmax;
}

double Celestial::Span(const vector<Celestial::Body>& bodies){
    auto len = bodies.size();
    auto h1 = std::async(std::launch::async,maxim,0,(int)(len/3),bodies);
    auto h2 = std::async(std::launch::async,maxim,(int)(len/3),(int)(len*2/3),bodies);
    auto h3 = std::async(std::launch::async,maxim,(int)(len*2/3),len,bodies);
    double sqrmax = 0;
    sqrmax = h1.get();
    double temp = h2.get();
    sqrmax = (temp>sqrmax)?temp:sqrmax;
    temp = h3.get();
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

void Celestial::Octree::Build(const vector<Body>& bodies){
    size = 2*Span(bodies)+1;
    root = Node(Eigen::Vector3d(0,0,0),size,0);
    for(auto it = bodies.begin(); it != bodies.end(); ++it){
        root.Add(*it);
    }
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


