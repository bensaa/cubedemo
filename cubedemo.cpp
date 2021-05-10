// g++ -O2 `pkg-config gtkmm-3.0 --cflags --libs gio-unix-2.0` cubedemo.cpp -o cubedemo && ./cubedemo
#include "elements.h"

#define CUBECOUNT 100
float HscreenW,HscreenH;

Glib::RefPtr<Gtk::Application>* Happ = NULL;
float camerax = 0,cameray = 100,cameraz = 0;

cube cubelist[CUBECOUNT];
cube* order[CUBECOUNT];
int iterations = 0;
int fps = 0;
bool gamerunning = true;

bool comparecubes(cube* cp1, cube* cp2) {
    return (cp1->locz > cp2->locz);
}
static void initplaces(){
    for(int i = 0; i < CUBECOUNT;i++){
        for(int i2 = 0; i2 < 8;i2++){
            point* P = &cubelist[i].points[i2];
            P->dx = cubelist[i].locx + P->sx;
            P->dy = cubelist[i].locy + P->sy;
            P->dz = cubelist[i].locz + P->sz;
        }
    }
}
static void calcplace(){

    for(int i = CUBECOUNT-1; i >= 0;i--){
        int i2 = 0;
        for(; i2 < 8;i2++){
            point* P = &order[i]->points[i2];
            P->fz = P->dz - cameraz;

            if(P->fz < -400){
                order[i]->locz = order[i]->locz + 4000;
                order[i]->locx = rand()%1000 -500;
                order[i]->locy = rand()%1000 -500 ;
                for(int i3 = 0; i3 < 8;i3++){
                    point* P2 = &order[i]->points[i3];
                    P2->dx = order[i]->locx + P2->sx;
                    P2->dy = order[i]->locy + P2->sy;
                    P2->dz = order[i]->locz + P2->sz;
		        }

				cube* tmp_cube = order[i];

				for(int i3 = CUBECOUNT - 1; i3 > 0;i3--){
					order[i3] = order[i3-1];
				}
				order[0] = tmp_cube;
                i = CUBECOUNT;

				break;
            }
			else{
				P->fx = P->dx - camerax;
				P->fy = P->dy - cameray;
				P->fx = HscreenW + P->fx * 600 /((P->fz + 600));
				P->fy = HscreenH - P->fy * 600 /((P->fz + 600));
			}
        }
    }
}

class Window3d : public Gtk::DrawingArea{
public:
    Window3d(){
        Glib::signal_timeout().connect( sigc::mem_fun(*this, &Window3d::on_timeout), 1000 );
    };
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override{
        cr->set_line_width(1.0);
        for(int j = 0; j < CUBECOUNT;j++){
			for(int i = 0; i < 12;i++){
				kolmio* KP = &(order[j]->kolmiot[i]);
				float P1x = KP->points[2]->fx - KP->points[0]->fx;
				float P1y = KP->points[2]->fy - KP->points[0]->fy;
				float P2x = KP->points[1]->fx - KP->points[0]->fx;
				float P2y = KP->points[1]->fy - KP->points[0]->fy;
				float value = P1x*P2y - P1y*P2x;
				if(value > 0){
					cr->set_source_rgb(255, 0, 0);
					cr->move_to(KP->points[0]->fx, KP->points[0]->fy);
					cr->line_to(KP->points[1]->fx, KP->points[1]->fy);
					cr->line_to(KP->points[2]->fx, KP->points[2]->fy);
					cr->close_path();
					cr->fill_preserve();
					cr->set_source_rgb(0.0, 0.0, 0.0);
					cr->stroke();
				}
            }
        }

        std::string s = ("FPS: " + std::to_string(fps));
        auto layout = create_pango_layout(s);
        cr->move_to(25, 25);
        layout->show_in_cairo_context(cr);

        return true;
    };
    bool on_timeout(){
        fps = iterations;
        iterations = 0;
        return true;
    };
};
Window3d* Hwin = NULL;

static void onResize(){
    HscreenW = Hwin->get_width()/2;
    HscreenH = Hwin->get_height()/2;
}
static void gameloop(){
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
	while(gamerunning){
        cameraz = cameraz + 1;
	    calcplace();
        Hwin->queue_draw();
        iterations++;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

static bool onKeyPress(GdkEventKey* event){
    switch(event->hardware_keycode){
	case 24:{
		gamerunning = false;
		((Gtk::Application*)Happ)->quit();
		break;
	}
	case 9:{
		gamerunning = false;
		((Gtk::Application*)Happ)->quit();
		break;                                                                                   }
	}
    return true;
}

int main(int argc, char** argv){
    for(int i = 0;i < CUBECOUNT;i++){
        createcube(&cubelist[i],100.0f);
        order[i] = &cubelist[i];
        cubelist[i].locx = rand() % 1200 -600;
        cubelist[i].locy = rand() % 1200 -600;
        cubelist[i].locz = rand() % 3800 +200;
    }
    initplaces();
   	std::sort(&order[0],&order[CUBECOUNT],comparecubes);
	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
    Happ = &app;
    Gtk::Window win;
    win.set_title("Window3d");
    Window3d drawarea_3D;
    win.fullscreen();
    Hwin = &drawarea_3D;
    win.signal_key_press_event().connect(sigc::ptr_fun(onKeyPress),false );
    win.signal_check_resize().connect(sigc::ptr_fun(onResize),false );
    win.add(drawarea_3D);
    drawarea_3D.show();
    std::thread t(gameloop);
    return app->run(win);
}
