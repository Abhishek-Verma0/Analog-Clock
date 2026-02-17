#include<SDL3/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<time.h>
#include<math.h>

#define WIDTH 600
#define HEIGHT 600

#define CENTER_X 300
#define CENTER_Y 300
#define RADIUS 250



//  function for drawing circle using midPoint algorithm   circle which will represent frame of clock

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   
   {
      //  Each of the following renders an octant of the circle
      SDL_RenderPoint(renderer, centreX + x, centreY + y);
      SDL_RenderPoint(renderer, centreX + x, centreY - y);
      SDL_RenderPoint(renderer, centreX - x, centreY - y);
      SDL_RenderPoint(renderer, centreX - x, centreY + y);
      SDL_RenderPoint(renderer, centreX + y, centreY - x);
      SDL_RenderPoint(renderer, centreX + y, centreY + x);
      SDL_RenderPoint(renderer, centreX - y, centreY - x);
      SDL_RenderPoint(renderer, centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}



//  using gpu geometry to render thick  lines
void DrawThickLine(SDL_Renderer* renderer,  float sx, float sy,float ex, float ey,float thickness,SDL_Color color)
{
    // direction vector
    float dx = ex - sx;
    float dy = ey - sy;

    // normalize
    float len = sqrtf(dx*dx + dy*dy);
    if (len == 0) return;
    dx /= len;
    dy /= len;

    // perpendicular vector scaled by half thickness
    float px = -dy * (thickness / 2.0f);
    float py =  dx * (thickness / 2.0f);

    // 4 corners of the rectangle
    SDL_Vertex vertices[4];

    // SDL3 uses SDL_FColor
    SDL_FColor fcolor = {
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f,
        color.a / 255.0f
    };

    vertices[0].position = (SDL_FPoint){ sx + px, sy + py }; // A
    vertices[1].position = (SDL_FPoint){ ex + px, ey + py }; // B
    vertices[2].position = (SDL_FPoint){ sx - px, sy - py }; // C
    vertices[3].position = (SDL_FPoint){ ex - px, ey - py }; // D

    for (int i = 0; i < 4; i++)
        vertices[i].color = fcolor;

    // 2 triangles making the quad
    // triangle 1: A B C
    // triangle 2: B C D
    int indices[6] = { 0, 1, 2, 1, 2, 3 };

    SDL_RenderGeometry(renderer, NULL, vertices, 4, indices, 6);
}

//  function to render time on the clock 
void drawHours(SDL_Renderer *renderer,uint32_t center_x,uint32_t center_y, uint32_t radius){
   
  
   //  here calculated angle is converted to radian as sdl expects radian not degree
   for (int h = 1; h <=12;h++){
      float angle = h * 30.0f * (SDL_PI_F / 180.0f);

      //  starting point of line for the time marking
      float stLine_x = center_x + radius * SDL_sinf(angle);
      float stLine_y = center_y - radius * SDL_cosf(angle);
      //  finding the ending point of line  where upto draw
      float edLine_x = center_x + (radius* 0.90f) * SDL_sinf(angle);
      float edLine_y = center_y - (radius*0.90f) * SDL_cosf(angle);
      SDL_Color black = {0, 0, 0};
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
      DrawThickLine(renderer, stLine_x, stLine_y, edLine_x, edLine_y, 3.0f,black);

 
   }
}



//  we may use gpu geometry for more smooth rendereing 

void drawCircleGPU(SDL_Renderer *renderer ,uint32_t center_x, uint32_t center_y,uint32_t radius){
    SDL_CreateGPUDevice(0, 0, "Device");
}


int main(){

    SDL_Window *window;
    SDL_Init(SDL_INIT_VIDEO);
    
    window = SDL_CreateWindow("Clock", WIDTH, HEIGHT, 0);
    bool done = true;
    SDL_Renderer *renderer = NULL;
    renderer = SDL_CreateRenderer(window, NULL);

    while(done){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_EVENT_QUIT){
            done = false;
            }
            SDL_SetRenderDrawColor(renderer, 204, 204, 255, 0);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            DrawCircle(renderer,300,300,250);
            DrawCircle(renderer,300,300,249);
            DrawCircle(renderer,300,300,248);
            //  function to draw hour line
            drawHours(renderer, CENTER_X, CENTER_Y, RADIUS);

            SDL_RenderPresent(renderer);


    }
    
}
SDL_DestroyWindow(window);
SDL_Quit();
return 0;
}