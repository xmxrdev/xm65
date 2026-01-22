#include <stdio.h>
#include <math.h>

#include "xm65/cli.h"
#include "xm65/utils.h"
#include "xm65/vm.h"

#include <SDL3/SDL.h>

int main(int argc, char *argv[]) {
    XM65_Cli cli;
    XM65_ParseArguments(argc, argv, &cli);

    if (cli.target == NULL) XM65_ThrowError(XM65_ERROR_NO_TARGETS);

    XM65_PrintInfo(XM65_INFO_TARGET, cli.target);
    XM65_PrintInfo(XM65_INFO_FLAGS, (size_t) cli.flags);

    if (cli.flags & XM65_CLI_EMULATE_TARGET) {
        if (!XM65_CanReadFile(cli.target)) XM65_ThrowError(XM65_ERROR_FILE_OPEN, cli.target);

        XM65_PrintInfo(XM65_INFO_POWERING);
        XM65_VM vm;
        XM65_Power_VM(&vm, (bool)(cli.flags & XM65_CLI_MEMORY_ZERO));

        XM65_PrintInfo(XM65_INFO_LOADING, cli.target);
        XM65_ProgramVM(&vm, cli.target);

        XM65_PrintInfo(XM65_INFO_RESETTING);
        XM65_ResetVM(&vm);

        XM65_PrintInfo(XM65_INFO_EMULATION);

        vm.status = XM65_VM_STATUS_INTERRUPTED;
        vm.status = XM65_VM_STATUS_IDLE;

        XM65_PrintInfo(XM65_INFO_SDL_INIT);

        SDL_Window *window = NULL;
        SDL_Renderer *renderer = NULL;
        SDL_Event event;
        bool running = true;

        SDL_SetAppMetadata(
            "XM65 Emulator & Debugger",
            "1.0",
            "com.xmxr.xm65"
        );

        if (!SDL_Init(SDL_INIT_VIDEO)) XM65_ThrowError(XM65_ERROR_SDL_INIT, SDL_GetError());

        int screen_width = 640;
        int screen_height = 640;

        if (!SDL_CreateWindowAndRenderer(
            "Emulation",
            screen_width, screen_height,
            SDL_WINDOW_RESIZABLE,
            &window,
            &renderer))
        {
            SDL_Quit();
            XM65_ThrowError(XM65_ERROR_SDL_CREATE, SDL_GetError());
        }

        SDL_Texture *screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB332, SDL_TEXTUREACCESS_STREAMING, 32, 32);
        uint8_t *memory_ppu = &vm.ram.data[0xFB00];

        while (running) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT) running = false;
            }


            if (vm.status != XM65_VM_STATUS_INTERRUPTED) XM65_RunVM(&vm);

            SDL_GetWindowSize(window, &screen_width, &screen_height);

            SDL_SetRenderDrawColor(renderer, 0x10, 0x10, 0x10, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);

            uint8_t *pixels;
            int pitch;

            SDL_LockTexture(screen, NULL, (void**) &pixels, &pitch);

            for (int y = 0; y < 32; y++) {
                memcpy(pixels + y * pitch, memory_ppu + y * 32, 32);
            }

            SDL_UnlockTexture(screen);

            SDL_FRect dest;

            {
                const float screen_w = 32;
                const float screen_h = 32;
                float scale = fminf((float) screen_width / screen_w, (float) screen_height / screen_h);
                dest.w = (float)(screen_w * scale);
                dest.h = (float)(screen_h * scale);
                dest.x = ((float) screen_width - dest.w) / 2;
                dest.y = ((float) screen_height - dest.h) / 2;
            }

            SDL_SetTextureScaleMode(screen, SDL_SCALEMODE_NEAREST);
            SDL_RenderTexture(renderer, screen, NULL, &dest);

            SDL_RenderPresent(renderer);
        }

        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyTexture(screen);
        SDL_Quit();
    }

    return 0;
}
