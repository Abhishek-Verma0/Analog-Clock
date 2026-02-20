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

//  colors  may change on apppearance
SDL_Color BG_COLOR       = {26,  29,  46,  255};  // dark blue-gray background
SDL_Color RING_DARK      = {15,  20,  25,  255};  // outer ring dark
SDL_Color RING_GOLD      = {212, 175, 55,  255};  // gold accent
SDL_Color HOUR_MAJOR     = {212, 175, 55,  255};  // 12,3,6,9 gold
SDL_Color HOUR_MINOR     = {138, 138, 158, 255};  // other hours gray
SDL_Color MINUTE_TICK    = {74,  74,  94,  255};  // minute ticks subtle
SDL_Color HAND_HOUR      = {212, 175, 55,  255};  // hour hand gold
SDL_Color HAND_MINUTE    = {232, 232, 240, 255};  // minute hand silver
SDL_Color HAND_SECOND    = {220, 20,  60,  255};  // second hand crimson




//  function for drawing circle using midPoint algorithm   circle which will represent frame of clock

//  new alog uses gpu to draw circle
void DrawCircleOutline(SDL_Renderer* renderer, float cx, float cy,float radius, float thickness,SDL_Color color, int segments)
{
    int vertCount = (segments + 1) * 2;
    int idxCount  = segments * 6;

    SDL_Vertex* verts   = malloc(vertCount * sizeof(SDL_Vertex));
    int*        indices = malloc(idxCount  * sizeof(int));
    if (!verts || !indices) { free(verts); free(indices); return; }

    SDL_FColor fc = {
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f,
        color.a / 255.0f
    };

    float innerR = radius - thickness / 2.0f;
    float outerR = radius + thickness / 2.0f;

    // create vertices
    for (int i = 0; i <= segments; i++)
    {
        float angle = (2.0f * SDL_PI_F * i) / segments;
        float nx = SDL_cosf(angle);
        float ny = SDL_sinf(angle);

        // outer vertex
        verts[i*2+0].position = (SDL_FPoint){ cx + nx * outerR, cy + ny * outerR };
        verts[i*2+0].color    = fc;

        // inner vertex
        verts[i*2+1].position = (SDL_FPoint){ cx + nx * innerR, cy + ny * innerR };
        verts[i*2+1].color    = fc;
    }

    // create indices (2 triangles per segment)
    for (int i = 0; i < segments; i++)
    {
        int b = i * 2;
        indices[i*6+0] = b+0;
        indices[i*6+1] = b+1;
        indices[i*6+2] = b+2;
        indices[i*6+3] = b+1;
        indices[i*6+4] = b+2;
        indices[i*6+5] = b+3;
    }

    SDL_RenderGeometry(renderer, NULL, verts, vertCount, indices, idxCount);
    free(verts);
    free(indices);
}



//  using gpu geometry to render thick  lines
void DrawThickLine(SDL_Renderer* renderer,  float sx, float sy,float ex, float ey,float thickness,SDL_Color color)
{
    // dirxn vector
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
      
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
      if(h==12 || h==3 || h==6 || h==9){
         
         DrawThickLine(renderer, stLine_x, stLine_y, edLine_x, edLine_y, 5.0f,HOUR_MAJOR);
      }
      DrawThickLine(renderer, stLine_x, stLine_y, edLine_x, edLine_y, 3.0f,HOUR_MINOR);

 
   }
}


//  function to draw second or minutes ticks 

void secondTick(SDL_Renderer *renderer ,uint32_t center_x, uint32_t center_y,uint32_t radius){
 for (int h = 1; h <=60;h++){
      float angle = h * 6.0f * (SDL_PI_F / 180.0f);
   if (h % 5 == 0) continue;
      //  starting point of line for the time marking
      float stLine_x = center_x +  radius* 0.95f * SDL_sinf(angle);
      float stLine_y = center_y - radius* 0.95f * SDL_cosf(angle);
      //  finding the ending point of line  where upto draw
      float edLine_x = center_x + (radius* 0.90f) * SDL_sinf(angle);
      float edLine_y = center_y - (radius*0.90f) * SDL_cosf(angle);
      
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

      DrawThickLine(renderer, stLine_x, stLine_y, edLine_x, edLine_y, 2.0f,MINUTE_TICK);

 
   
}
}


//  function to get current time from pc 
void getCurrentTime(int* hours, int* minutes, int* seconds)
{
    time_t rawtime;
    time(&rawtime);

    struct tm* timeinfo;
    timeinfo = localtime(&rawtime);

    *hours   = timeinfo->tm_hour;
    *minutes = timeinfo->tm_min;
    *seconds = timeinfo->tm_sec;
}



//  function to draw second hand other function for hour and minute will be similar just diffrence of movement 

void drawSecondHand(SDL_Renderer* renderer,float cx, float cy, float radius,int seconds)
{
    

    // each second = 6° (360° / 60 = 6°)
    float angle = seconds * 6.0f * (SDL_PI_F / 180.0f);

    float sx = cx;
    float sy = cy;
    
    
    float ex = cx + radius * 0.85f * SDL_sinf(angle);
    float ey = cy - radius * 0.85f * SDL_cosf(angle);

    DrawThickLine(renderer, sx, sy, ex, ey, 1.0f, HAND_SECOND);
}


//  for minute hand 
 void drawMinuteHand(SDL_Renderer* renderer,uint32_t cx ,uint32_t cy, float radius,int minutes){
    

    // each second = 6° (360° / 60 = 6°)
    float angle = minutes * 6.0f * (SDL_PI_F / 180.0f);

    float sx = cx;
    float sy = cy;
    
    
    float ex = cx + radius * 0.75f * SDL_sinf(angle);
    float ey = cy - radius * 0.75f * SDL_cosf(angle);

    DrawThickLine(renderer, sx, sy, ex, ey, 2.0f, HAND_MINUTE);
 }


//   for hour hand 

 void drawHourHand(SDL_Renderer* renderer,uint32_t cx ,uint32_t cy, float radius,int hour,int minutes){
    

    int hours12 = hour % 12;
    

    float angle = hours12 * 30.0f +minutes*0.5f;
    angle *= (SDL_PI_F / 180.0f);

    float sx = cx;
    float sy = cy;
    
    
    float ex = cx + radius * 0.5f * SDL_sinf(angle);
    float ey = cy - radius * 0.5f * SDL_cosf(angle);

    DrawThickLine(renderer, sx, sy, ex, ey, 4.0f, HAND_HOUR);
 }


//  drawing circle at center of clock

void drawCenterDot(SDL_Renderer* renderer, float cx, float cy)
{
    // draw small gold circle at center
    for (int r = 0; r < 8; r++)
    {
        for (int a = 0; a < 360; a += 10)
        {
            float rad = a * (SDL_PI_F / 180.0f);
            int x = (int)(cx + r * SDL_cosf(rad));
            int y = (int)(cy + r * SDL_sinf(rad));
            SDL_SetRenderDrawColor(renderer, RING_GOLD.r, RING_GOLD.g, RING_GOLD.b, 255);
            SDL_RenderPoint(renderer, x, y);
        }
    }
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
         }
            //  getting time 
            int hours, minutes, seconds;
            getCurrentTime(&hours, &minutes, &seconds);

            SDL_SetRenderDrawColor(renderer, BG_COLOR.r,BG_COLOR.g,BG_COLOR.b,255);
            SDL_RenderClear(renderer);
            DrawCircleOutline(renderer, CENTER_X, CENTER_Y, RADIUS, 6.0f, RING_GOLD, 128);
         DrawCircleOutline(renderer, CENTER_X, CENTER_Y, RADIUS - 8, 3.0f, RING_DARK, 128);

            //  function to draw hour line
            drawHours(renderer, CENTER_X, CENTER_Y, RADIUS);
            secondTick(renderer, CENTER_X, CENTER_Y, RADIUS);

            // drawing hands of clock
            drawSecondHand(renderer, CENTER_X, CENTER_Y, RADIUS, seconds);
            drawMinuteHand(renderer, CENTER_X, CENTER_Y, RADIUS, minutes);
            drawHourHand(renderer, CENTER_X, CENTER_Y, RADIUS, hours, minutes);


            //  drawing center
            drawCenterDot(renderer, CENTER_X, CENTER_Y);
            SDL_RenderPresent(renderer);


    
}
SDL_DestroyWindow(window);
SDL_Quit();
return 0;
}