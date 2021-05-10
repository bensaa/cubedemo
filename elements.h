
#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <gtkmm-3.0/gtkmm/widget.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <glibmm/main.h>
#include <cairomm/context.h>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <vector>
#include <thread>

struct point{
    float sx,sy,sz;
    float dx,dy,dz;
    float fx,fy,fz;
};
struct kolmio{
    point* points[3];
};
struct cube{
    kolmio kolmiot[12];
    point points[8];
    float locx,locy,locz;
};

static void createcube(cube* E, float radius){
    E->points[0] = {.sx = radius,.sy = radius,.sz = -radius};
    E->points[1] = {.sx =-radius,.sy = radius,.sz = -radius};
    E->points[2] = {.sx =-radius,.sy =-radius,.sz = -radius};
    E->points[3] = {.sx = radius,.sy =-radius,.sz = -radius};
    E->points[4] = {.sx = radius,.sy = radius,.sz =  radius};
    E->points[5] = {.sx =-radius,.sy = radius,.sz =  radius};
    E->points[6] = {.sx =-radius,.sy =-radius,.sz =  radius};
    E->points[7] = {.sx = radius,.sy =-radius,.sz =  radius};
    E->kolmiot[0] = {.points{&E->points[0],&E->points[1],&E->points[2]}};//front1
    E->kolmiot[1] = {.points{&E->points[0],&E->points[2],&E->points[3]}};//front2
    E->kolmiot[2] = {.points{&E->points[1],&E->points[5],&E->points[6]}};//left1
    E->kolmiot[3] = {.points{&E->points[1],&E->points[6],&E->points[2]}};//left2
    E->kolmiot[4] = {.points{&E->points[0],&E->points[4],&E->points[5]}};//roof1
    E->kolmiot[5] = {.points{&E->points[0],&E->points[5],&E->points[1]}};//roof2
    E->kolmiot[6] = {.points{&E->points[0],&E->points[7],&E->points[4]}};//right1
    E->kolmiot[7] = {.points{&E->points[0],&E->points[3],&E->points[7]}};//right2
    E->kolmiot[8] = {.points{&E->points[4],&E->points[6],&E->points[5]}};//back1
    E->kolmiot[9] = {.points{&E->points[4],&E->points[7],&E->points[6]}};//back2
    E->kolmiot[10] = {.points{&E->points[3],&E->points[6],&E->points[7]}};//floor1
    E->kolmiot[11] = {.points{&E->points[3],&E->points[2],&E->points[6]}};//floor2
};

#endif

