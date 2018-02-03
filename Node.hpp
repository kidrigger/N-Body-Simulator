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
        
        Node* parent;

        // Create new Node at the center that contains the subsystem in the Quad
        // with sidelength side
        // the in the tree is calculated automatically on create sub nodes.
        Node(Node* parent, const Vector3d& center, double side,int id);

        // Adds a new body to the sub-system
        // returns the reference to the cgBody of the system
        void Add(const Body& data);

        // Checks if the particular body belongs to the said subsystem
        // returns true if it does
        bool Contains(const Body& body ) {
            return containQuad.Contains(body.position);
        }

        // Returns the quadrilateral
        Quad GetQuad() const {
            return containQuad;
        }

        // Returns the body containing the center of mass of the subsystem
        Body GetCG() const {
            return bodyCG;
        }

        // Returns the ID of the current node.
        int GetID() const {
            return id;
        }
        
        // Clears the node (Clear all sub-nodes and then itself.)
        void Clear() {
            for(auto it = nodeArray.begin(); it != nodeArray.end(); ++it) {
                it->Clear();
            }
            nodeArray.clear();
        }

        // Printing tool to convert the state to string and return
        std::string StateToString() const;
        
        // Printing tool to convert the quad to string and return
        std::string QuadToString() const;

        // Calculates the total force on the particle by the subsystem
        Vector3d TotalAcceleration(const Body& particle, double tolerance) const ;
    private:
        // The body containing total mass of subsystem and center of mass
        Body bodyCG; 
        // The containing quad of the node
        Quad containQuad;
        int id;
        // Removes flinging error in the integration
        float spat_tol;
        // Creates and initialized the 4 subnodes in the nodeArray
        void CreateSubNodes();
    };
}



#endif /* Node_hpp */
