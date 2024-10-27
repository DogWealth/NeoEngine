//
// Created by NeoLiu on 2024/10/23.
//

#include <core/logger.h>
#include <platform/platform.h>
using namespace NeoEngine;
int main() {
    NEO_FATAL("Error", 1);
    Platform* window_platform = Platform::Create();
    if(window_platform->PlatformStartup("Neo Engine TestBed", 100, 100, 1280, 720)) {
        while(true) {
            window_platform->PlatformPumpMessage();
        }
        window_platform->PlatformShutdown();
    }
}