//
//  Node.cpp
//  nbody_bh
//
//  Created by Anish Bhobe on 6/29/17.
//  Copyright © 2017 Anish Bhobe. All rights reserved.
//

#include "Node.hpp"

namespace Celestial{

    Node::Node(const Vector3d& center, double side, int id):id(id),nodeState(NodeState::Empty),bodyCG(),containQuad(center,side) {}

    Body& Node::Add(const Body &data) {
        if(nodeState == NodeState::Empty) {
            bodyCG = data;
            nodeState = NodeState::Leaf;
        }
        else if(nodeState == NodeState::Leaf){
            CreateSubNodes();
            auto tempBody = bodyCG;
            bodyCG = Body();
            for(auto it = nodeArray.begin(); it != nodeArray.end(); ++it)
            {
                if(it->Contains(data))
                {
                    bodyCG = Body::CalculateCG(bodyCG,it->Add(data));
                }
                if(it->Contains(tempBody))
                {
                    bodyCG = Body::CalculateCG(bodyCG,it->Add(tempBody));
                }
            }
            nodeState = NodeState::Branch;
        }
        else if(nodeState == NodeState::Branch){
            for(auto it = nodeArray.begin(); it != nodeArray.end(); ++it)
            {
                if(it->Contains(data))
                {
                    bodyCG = Body::CalculateCG(bodyCG,it->Add(data));
                }
            }
        }
        return bodyCG;
    }
    
    void Node::CreateSubNodes() {
        int idc = 1;
        for(int i = -1; i <= 1; i+=2) {
            for(int j = -1; j <= 1; j+=2) {
                Eigen::Vector3d tempVec(containQuad.center.x() - i*containQuad.halfside/2, containQuad.center.y() - j*containQuad.halfside/2, containQuad.center.z());
                nodeArray.push_back(Node(tempVec,containQuad.halfside,4*id + idc));
                idc++;
            }
        }
    }
    
    std::string Node::StateToString() const {
        std::stringstream strem;
        if(nodeState == NodeState::Empty){
            strem << "Empty";
            return strem.str();
        }
        if(nodeState == NodeState::Leaf){
            strem << "Leaf";
            return strem.str();
        }
        strem << "Branch";
        return strem.str();
    }
    
    std::string Node::QuadToString() const {
        std::stringstream strem;
        strem << "(" << containQuad.center[0] << "," << containQuad.center[1] << "):" << containQuad.side;
        return strem.str();
    }

}