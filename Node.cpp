//
//  Node.cpp
//  nbody_bh
//
//  Created by Anish Bhobe on 6/29/17.
//  Copyright © 2017 Anish Bhobe. All rights reserved.
//

#include "Node.hpp"
#include "Constants.hpp"
#include <future>

namespace Celestial {

    Node::Node(Node* parent, const Vector3d& center, double side, int id):id(id),nodeState(NodeState::Empty),bodyCG(),containQuad(center,side),spat_tol(0.4),parent(parent) {}

    void Node::Add(const Body &data) {
        //std::cout << id << std::endl;
        if(nodeState == NodeState::Empty) {
            bodyCG = data;
            nodeState = NodeState::Leaf;
        }
        else if(nodeState == NodeState::Leaf) {
            CreateSubNodes();
            auto tempBody = bodyCG;
            bodyCG = Body();
            nodeState = NodeState::Branch;
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
        // std::cout << static_cast<int>(nodeState) << std::endl;
        int idc = 1;
        for(int i = -1; i <= 1; i+=2) {
            for(int j = -1; j <= 1; j+=2) {
                Eigen::Vector3d tempVec(containQuad.center.x() - i*containQuad.halfside/2, containQuad.center.y() - j*containQuad.halfside/2, containQuad.center.z());
                nodeArray.push_back(std::move(Node(this,tempVec,containQuad.halfside,4*id + idc)));
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

    Vector3d Node::TotalAcceleration (const Body& particle, double tolerance) const {
        Vector3d force(0, 0, 0);
        if (nodeState == NodeState::Empty) {
            return force;
        }
        if ((particle.position - bodyCG.position).squaredNorm() == 0) {
            return force;
        }
        Vector3d dir = bodyCG.position - particle.position;
        if (nodeState == NodeState::Leaf) {
            double mag = (Constants::Gravitation * bodyCG.mass) / (dir.squaredNorm() * dir.norm() + spat_tol);
            force = (mag * dir);
        }
        else { //if(nodeState == NodeState::branch){
            /*
            if(id <= 4) {
                std::vector<std::future<Vector3d>> future_doubles;
                future_doubles.reserve(4);
                for (auto& x : nodeArray) {
                    // Might block, but also might not.
                    future_doubles.push_back(std::async(std::launch::async,&Node::TotalAcceleration, &x, particle, tolerance));
                }
                // Now block on all of them one at a time.
                for (auto& f_d : future_doubles) {
                    force += f_d.get();
                }
            }*/
            //else {
                if ((containQuad.side*containQuad.side / dir.squaredNorm()) < tolerance*tolerance) {
                    double mag = (Constants::Gravitation * bodyCG.mass) / (dir.squaredNorm() * dir.norm() + spat_tol);
                    force = (mag * dir);
                }
                else {
                    for (auto it = nodeArray.begin(); it != nodeArray.end(); ++it) {
                        force = force + it->TotalAcceleration(particle, tolerance);
                    }
                }
            //}
        }
        return force;
    }

}
