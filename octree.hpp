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

#define PARALLEL
#define DYN_SPAN

using Eigen::Vector3d;
using std::vector;
namespace Celestial {
    class Octree {
    public:
        double size;
        Octree();
        Octree(const Octree& other) = default;
        Octree(Octree&& other) = default;
        Octree& operator= (const Octree& other) = default;
        Octree& operator= (Octree&& other) = default;
        double Build(const vector<Body>& bodies, double span = 0);
        void Draw(Graphics& graphics);
        void Print();
        void CalculateAcceleration(double theta = 0.5);
        std::vector<Body> Update(double dt);
        Body GetSystemCG () { return root.GetCG(); }
        void Recalculate (cost Node& head);
        static Octree MakeAcceleratedOctree(const vector<Body>& bodies, double span = 0) {
            Octree oct;
            oct.Build(bodies,span);
            oct.CalculateAcceleration();
            return oct;
        }
    private:
        vector<Body> bodies;
        void DrawDFS(Graphics& graphics, const Node& head);
        void PrintDFS(const Node& head, int level = 0);
        void Clear() {root.Clear();}
        Node root;
    };

    double Span(const vector<Celestial::Body>& bodies);
}

#endif /* octree_hpp */
