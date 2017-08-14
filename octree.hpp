//
//  octree.hpp
//  nbody_bh
//
//  Created by Anish Bhobe on 6/16/17.
//  Copyright © 2017 Anish Bhobe. All rights reserved.
//

#ifndef octree_hpp
#define octree_hpp

#include "Eigen/Core"
#include <vector>
#include "Node.hpp"
#include "Graphics.hpp"

using Eigen::Vector3d;
using std::vector;
namespace Celestial {
    class Octree {
    public:
        double size;
        Octree();
        void Build(const vector<Body>& bodies);
        void Draw(Graphics& graphics);
        void Print();
        void CalculateAcceleration(double theta = 0.5);
        //TODO: Implement
        void Update(double dt){}
    private:
        vector<Body> bodies;
        void DrawDFS(Graphics& graphics, const Node& head);
        void PrintDFS(const Node& head, int level = 0);
        Node root;
    };
    
    double Span(const vector<Celestial::Body>& bodies);
}

#endif /* octree_hpp */
