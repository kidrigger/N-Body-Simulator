//
//  Graphics.hpp
//  nbody_bh
//
//  Created by Anish Bhobe on 6/29/17.
//  Copyright © 2017 Anish Bhobe. All rights reserved.
//

#ifndef Graphics_hpp
#define Graphics_hpp

#include <stdio.h>
#include <queue>
#include <map>
#include <allegro5/allegro.h>
#include "quad.hpp"
#include "Body.hpp"

using std::string;
using std::queue;
using std::map;
using std::pair;

namespace Celestial {
    
    class Graphics {
    public:
        Graphics(float span = 640, int screenSide = 640);
        ~Graphics();
        
        void setSpan(float span) { scale = screenSide/span; }
        void DrawPoint(double x, double y);
        void DrawPoint(const Body& bd);
        void DrawQuad(double x, double y, double side);
        void DrawQuad(const Quad& toDrawQuad);
        void DrawLine(double x1, double y1, double x2, double y2);
        void Draw();
        void Clear() { al_clear_to_color(al_map_rgb(0, 0, 0)); }
        void Wait(float seconds) { al_rest(seconds); }
    private:
        int screenSide;
        float scale;
        double offset;
        ALLEGRO_COLOR White;
        ALLEGRO_COLOR Red;
        ALLEGRO_DISPLAY *display = nullptr;
    };
}

#endif /* Graphics_hpp */
