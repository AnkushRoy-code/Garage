#ifndef INCLUDE_COMMON_SDLEXCEPTION_H_
#define INCLUDE_COMMON_SDLEXCEPTION_H_

#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <exception>
#include <string>

class SDL_Exception final : public std::exception
{
  private:
    std::string msg;

  public:
    explicit SDL_Exception(const std::string &Msg) :
        msg("SDL_Error: " + Msg + '\n' + SDL_GetError())
    {
    }

    [[nodiscard]] const char *what() const noexcept override
    {
        return msg.c_str();
    }
};

#endif  // INCLUDE_COMMON_SDLEXCEPTION_H_
