#include <iostream>
#include "SDL2/SDL.h"
#include <unistd.h>

const int WIDTH = 800, HEIGHT = 600;

struct dataa{
    SDL_sem * lock = nullptr;
    signed int varum = 500;
};

int fibonacci(int n){
    return (int)(SDL_pow((1+SDL_sqrt(5))/2,n)/SDL_sqrt(5));
};

int func(void * data){
    dataa * paul = (dataa *)data;
    std::cout << "linga guli guli gua sha linga gu linga gu \n";
    
    SDL_SemWait((*paul).lock);
    while(paul->varum > 0){
        std::cout << "preggy where u been at\n";
        paul->varum -= 1;
    }
    SDL_SemPost((*paul).lock);

    /*
    while(*paul != 0){
        std::cout << fibonacci(*paul) << '\n';
        *paul -= 1;
    }*/
    
    return 0;
}


int main( int argc, char *argv[] )
{

    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_Window *window = SDL_CreateWindow( "Hello SDL WORLD", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI );
    bool runinng = true;

    if ( NULL == window )
    {
        std::cout << "Could not create window: " << SDL_GetError( ) << std::endl;
        return 1;
    }

    SDL_Event windowEvent;

    dataa dataBuffer;
    dataBuffer.lock = SDL_CreateSemaphore(1);
    dataBuffer.varum = 555;

    SDL_Thread * threadito = SDL_CreateThread(func,"linga guli guli gua sha", (void*)(&dataBuffer)); 

    while ( runinng )
    {
        while( SDL_PollEvent( &windowEvent ) )
        {
            if ( SDL_QUIT == windowEvent.type )
            { runinng = false; }
        }

        SDL_SemWait(dataBuffer.lock);
        std::cout << "bop on " << dataBuffer.varum << '\n';
        /*
        if (dataBuffer.varum == 0){
            runinng = false;
        }
        */
        SDL_SemPost(dataBuffer.lock);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}