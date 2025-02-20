#ifndef INCLUDE_COMMON_SDLEXCEPTION_H_
#define INCLUDE_COMMON_SDLEXCEPTION_H_

#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <exception>
#include <string>

/**
 * @brief Exception class for handling SDL errors.
 * 
 * This class extends std::exception and provides a mechanism to capture
 * and report SDL-specific errors, including the message provided by SDL_GetError().
 */
class SDL_Exception final : public std::exception
{
  private:
    std::string msg;  ///< Stores the error message.

  public:
    /**
     * @brief Constructs an SDL_Exception with a custom message.
     *
     * The provided message is concatenated with the current SDL error message.
     *
     * @param Msg A custom message describing the error.
     */
    explicit SDL_Exception(const std::string &Msg) :
        msg("SDL_Error: " + Msg + '\n' + SDL_GetError())
    {
    }

    /**
     * @brief Returns the error message.
     *
     * @return A C-style string containing the error message.
     */
    [[nodiscard]] const char *what() const noexcept override
    {
        return msg.c_str();
    }
};

#endif  // INCLUDE_COMMON_SDLEXCEPTION_H_
