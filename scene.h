#pragma once
#include "SDL2/SDL.h"
#include <vector>

struct scene{
    public:
        int (*mainLoop)(void *) = nullptr;
        int (*loadFunc)(void *) = nullptr;
        int (*unload)(void *) = nullptr;
        void* data = nullptr;
};

struct threadData{
    void *data = nullptr;
    SDL_mutex * lock = nullptr;
    int (*func)(void*) = nullptr;
};

//threadata is a pointer to a threadData Object
int threadFunc(void * threadata){
    SDL_LockMutex((*(threadData*)threadata).lock);
    int result = (*(threadData*)threadata).func((*(threadData*)threadata).data);
    SDL_UnlockMutex((*(threadData*)threadata).lock);
    return result;
};

class scenesHandler{
    private:
        std::vector<scene> scenes;
        scene transitionScene;

        signed int runningScene = 0;
        bool IsASceneRunningOrTheLoadingScene = true;
        SDL_Thread * LoaderThread = nullptr;
        
        threadData threaddata;
        
    public:
        int changeScene(int sceneIndex){
            //we first switch to loding scene
            scenes[runningScene].unload(scenes[runningScene].data);
            transitionScene.loadFunc(transitionScene.data);
            IsASceneRunningOrTheLoadingScene = false;
            //then, prepare the data that will be sent to the thread
            threaddata.func = scenes[sceneIndex].loadFunc;
            threaddata.data = scenes[sceneIndex].data;
            //then launch the thread
            LoaderThread = SDL_CreateThread( threadFunc,"SceneLoader", (void*)&threaddata);
            return 0;
        };

        int Update(){
            if(IsASceneRunningOrTheLoadingScene){
                scenes[runningScene].mainLoop(scenes[runningScene].data);
            }else if(SDL_TryLockMutex(threaddata.lock)){
                IsASceneRunningOrTheLoadingScene = true;
                transitionScene.unload(transitionScene.data);
                SDL_UnlockMutex(threaddata.lock);
            }else{
                transitionScene.mainLoop(transitionScene.data);
            }
            return 0;
        }

        scenesHandler(void * TransSceneData, int (*mainLoop)(void*), int (*Load)(void*), int (*unload)(void*)){
            threaddata.lock = SDL_CreateMutex();
            transitionScene.data = TransSceneData;
            transitionScene.mainLoop = mainLoop;
            transitionScene.loadFunc = Load;
            transitionScene.unload = unload;
        }

        int addAscene(){

        }
};

/*


## README ##

so First, we have the scenehandler and it need a first trasnitionScene
and then we had scenes to the scenes vector, just remember the index,
I also question myself if it is necessary to make it a dynamically sized container,
a fixed sized dynamically allocated one would be fine, like new Scene[numberOfExpectedScenes]


*/