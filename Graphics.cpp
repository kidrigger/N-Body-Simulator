//
//  Graphics.cpp
//  nbody_bh
//
//  Created by Anish Bhobe on 6/29/17.
//  Copyright © 2017 Anish Bhobe. All rights reserved.
//

#include "Graphics.hpp"
#include <iostream>
#include <allegro5/allegro_primitives.h>
    
Celestial::Graphics::Graphics(float span, int screenSide):screenSide(screenSide),scale(screenSide/span),offset(screenSide/2) {
    if(!al_init()){
        std::cerr << "ERROR: Allegro could not be initialized." << std::endl;
        exit(1);
    }
    else {
        std::cerr << "Allegro initialized." << std::endl;
    }

    if((display = al_create_display(screenSide, screenSide)) == nullptr){
        std::cerr << "ERROR: Display could not be created" << std::endl;
        exit(1);
    }
    else {
        std::cerr << "Display (" << screenSide << "x" << screenSide <<")created." << std::endl;
    }

    White = al_map_rgb(255, 255, 255);
    Red = al_map_rgb(255, 0, 0);

}

void Celestial::Graphics::DrawPoint(double x, double y) {
    x *= scale;
    y *= scale;
    x += offset;
    y += offset;
    if(x > 0 && y > 0)
        al_draw_pixel(x, y, White);
}

void Celestial::Graphics::DrawPoint(const Body& bd){
    DrawPoint(bd.position[0] , bd.position[1]);
}

void Celestial::Graphics::DrawLine(double x1, double y1, double x2, double y2) {
    x1+=offset;
    x2+=offset;
    y1+=offset;
    y2+=offset;
    if(x1>0 && x2 >0 && y1>0 && y2>0)
        al_draw_line(x1, y1, x2, y2, White, 1);
}

void Celestial::Graphics::DrawQuad(double x, double y, double side){
    x*=scale;
    y*=scale;
    side*=scale;
    x+= offset;
    y+= offset;
    if(x+y > side)
        al_draw_rectangle(x - side/2, y - side/2, x + side/2, y + side/2, al_map_rgb(255,255,255), 1);
}

void Celestial::Graphics::DrawQuad(const Quad& toDrawQuad){
    DrawQuad(toDrawQuad.center[0], toDrawQuad.center[1], toDrawQuad.side);
}

void Celestial::Graphics::Draw() {
    al_flip_display();
}

Celestial::Graphics::~Graphics(){
        al_destroy_display(display);
}
