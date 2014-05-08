#pragma once

#include "ofEvents.h"

class LoadingEvents
{
public:

	static LoadingEvents* Instance()
    {
        static LoadingEvents inst ; 
        return &inst ; 
    }

	ofEvent<string> FILE_LOADED ; 
};