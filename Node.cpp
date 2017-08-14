//
//  Node.cpp
//  nbody_bh
//
//  Created by Anish Bhobe on 6/29/17.
//  Copyright © 2017 Anish Bhobe. All rights reserved.
//

#include "Node.hpp"
#include "Constants.hpp"

namespace Celestial {

    Node::Node(const Vector3d& center, double side, int id):id(id),nodeState(NodeState::Empty),bodyCG(),containQuad(center,side) {}

    void Node::Add(const Body &data) {
        if(nodeState == NodeState::Empty) {
            bodyCG = data;
            nodeState = NodeState::Leaf;
        }
        else if(nodeState == NodeState::Leaf) {
            CreateSubNodes();
            auto tempBody = bodyCG;
            bodyCG = Body();
            for(auto it = nodeArray.begin(); it != nodeArray.end(); ++it) {
                if(it->Contains(data)) {
                    it->Add(data);
                    bodyCG = Body::CalculateCG(bodyCG,data);
                }
                if(it->Contains(tempBody)) {
                    it->Add(tempBody);
                    bodyCG = Body::CalculateCG(bodyCG,tempBody);
                }
            }
            nodeState = NodeState::Branch;
        }
        else if(nodeState == NodeState::Branch) {
            for(auto it = nodeArray.begin(); it != nodeArray.end(); ++it) {
                if(it->Contains(data)) {
                    it->Add(data);
                    bodyCG = Body::CalculateCG(bodyCG,data);
                }
            }
        }
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
        if(nodeState == NodeState::Empty) {
            strem << "Empty";
            return strem.str();
        }
        if(nodeState == NodeState::Leaf) {
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

    Vector3d Node::TotalForce (const Body& particle, double tolerance) const {
        Vector3d force(0, 0, 0);
        if (nodeState == NodeState::Empty) {
            return force;
        }
        Vector3d dir = bodyCG.position - particle.position;
        if (nodeState == NodeState::Leaf) {
            double mag = (Constants::Gravitation * bodyCG.mass * particle.mass) / (dir.squaredNorm() * dir.norm());
            force = (mag * dir);
        }
        else { //if(nodeState == NodeState::branch){
            if ((containQuad.side*containQuad.side / dir.squaredNorm()) < tolerance*tolerance) {
                double mag = (Constants::Gravitation * bodyCG.mass * particle.mass) / (dir.squaredNorm() * dir.norm());
                force = (mag * dir);
            }
            else {
                for (const Node &bnode : nodeArray) {
                    force = force + bnode.TotalForce(particle, tolerance);
                }
            }
        }
        return force;
    }

}
