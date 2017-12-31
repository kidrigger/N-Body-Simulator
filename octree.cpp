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

//#define PARALLEL

Celestial::Octree::Octree():root(Vector3d(0,0,0),0,0) {}

#ifdef PARALLEL

// Find maximum between the two points
double maxim(int initial, int final, const vector<Celestial::Body>& bodies){
    double sqrmax = 0;
    std::cout << initial << " started\n";
    for(int i = initial; i != final; ++i){
        double sqrnrm = bodies[i].position.squaredNorm();
        sqrmax = (sqrnrm>sqrmax)?sqrnrm:sqrmax;
    }
    std::cout << initial << " completed\n";
    return sqrmax;
}

// Divide the job into three and launch async
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

//Finds the squared maximum from the bodies and returns the square root
double Celestial::Span(const vector<Celestial::Body>& bodies){
    double sqrmax = 0;
    for(auto it = bodies.begin(); it != bodies.end(); ++it){
        sqrmax = std::max(sqrmax,it->position.squaredNorm());
    }
    return sqrt(sqrmax);
}

#endif // end if statement

//Calls PrintDFS
void Celestial::Octree::Print(){
    PrintDFS(root);
}

//Prints in the console the ID and State and other parameters
//of the Node
void Celestial::Octree::PrintDFS(const Celestial::Node &head, int level){
    for(int i = 0 ; i != level; ++i){
        std::cout << "   ";
    }
    std::cout << "[" << head.GetID() << "]<"<< head.StateToString() << "> "<< head.GetCG().ToString() << "\n";
    if(head.nodeState != NodeState::Branch)
    {
        return;
    }
    for(int i = 0; i != 4; ++i){			//The "4" here changes the octree into a quadtree
        PrintDFS(head.nodeArray[i],level+1);
    }
}

//Builds the quadtree in the quad with the parameters provided
//starts with the first body till the last body with "auto" to handle the
//data type issues
void Celestial::Octree::Build(const vector<Body>& bodies){
    size = 2*Span(bodies)+1;
    root = Node(Eigen::Vector3d(0,0,0),size,0);
    for(auto it = bodies.begin(); it != bodies.end(); ++it){
        root.Add(*it);
        // <<<<<<<< DEBUG ONLY >>>>>>>> //
        this->Print();
        std::cin.ignore();
        std::cout << std::endl;
    }
}

//Draws the bodies after clearing the screen and calls the DrawDFS
void Celestial::Octree::Draw(Celestial::Graphics &graphics){
    graphics.Clear();
    DrawDFS(graphics, root);
    graphics.Draw();
}

//Draws each leaf in the quad and sub quads then moves to the branch of that quad to draw its content
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
    for(int i = 0; i != 4; ++i){		//the "4" here makes the octree a quadtree
        DrawDFS(graphics, head.nodeArray[i]);
    }
}

//Calculates the acceleration using the total force experienced by the body
void Celestial::Octree::CalculateAcceleration(double theta){
    auto n = bodies.size();
    for (int i = 0; i != n; ++i) {
        bodies[i].acceleration = root.TotalForce(bodies[i], theta);
    }
}
