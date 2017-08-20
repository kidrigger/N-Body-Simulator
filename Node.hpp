//
//  Node.hpp
//  nbody_bh
//
//  Created by Anish Bhobe on 6/29/17.
//  Copyright © 2017 Anish Bhobe. All rights reserved.
//

#ifndef Node_hpp
#define Node_hpp

#include <vector>
#include <iostream>
#include "quad.hpp"
#include "Body.hpp"

namespace Celestial {

    enum class NodeState:uint8_t {
        Empty = 0,
        Leaf = 1,
        Branch = 2
    };

    class Node {
    public:
        // The state of this node, Empty, Leaf or Branch
        NodeState nodeState;

        // The vector containing all the child nodes of the tree;
        std::vector<Node> nodeArray;

        // Create new Node at the center that contains the subsystem in the Quad
        // with sidelength side
        // the in the tree is calculated automatically on create sub nodes.
        Node(const Vector3d& center, double side,int id);

        // Adds a new body to the sub-system
        // returns the reference to the cgBody of the system
        void Add(const Body& data);

        // Checks if the particular body belongs to the said subsystem
        // returns true if it does
        bool Contains(const Body& body ) {
            return containQuad.Contains(body.position);
        }

        Quad GetQuad() const {
            return containQuad;
        }

        Body GetCG() const {
            return bodyCG;
        }

        int GetID() const {
            return id;
        }
        
        void Clear() {
            for(auto it = nodeArray.begin(); it != nodeArray.end(); ++it) {
                it->Clear();
            }
            nodeArray.clear();
        }

        std::string StateToString() const;
        std::string QuadToString() const;

        Vector3d TotalForce(const Body& particle, double tolerance) const ;
    private:
        // The body containing total mass of subsystem and center of mass
        Body bodyCG; 
        // The containing quad of the node
        Quad containQuad;
        int id;
        float spat_tol;

        void CreateSubNodes();
    };
}



#endif /* Node_hpp */
