#include<SDL3/SDL.h>
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<time.h>


#define WIDTH 600
#define HEIGHT 600





//  function for drawing circle using midPoint algorithm





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
            // DrawCircle(renderer,300,300,249);
            // DrawCircle(renderer,300,300,298);
            SDL_SetRenderDrawColor(renderer, 0, 0, 255,0);
            SDL_RenderPoint(renderer, 300, 300);
            SDL_RenderPresent(renderer);
    }
    
}
SDL_DestroyWindow(window);
SDL_Quit();
return 0;
}