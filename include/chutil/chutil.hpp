#ifndef __CHUTIL_HPP
#define __CHUTIL_HPP

#include <string_view>
#include <string>
#include <format>
#include <print>
#include <cstdlib>

using uint = unsigned;

namespace chutil
{
	// colors for printing to the terminal
	inline constexpr char const* const ANSI_COLOR_RED =         "\x1b[31m";
	inline constexpr char const* const ANSI_COLOR_GREEN =       "\x1b[32m";
	inline constexpr char const* const ANSI_COLOR_YELLOW =      "\x1b[33m";
	inline constexpr char const* const ANSI_COLOR_BLUE =        "\x1b[34m";
	inline constexpr char const* const ANSI_COLOR_MAGENTA =     "\x1b[35m";
	inline constexpr char const* const ANSI_COLOR_CYAN =        "\x1b[36m";
	inline constexpr char const* const ANSI_COLOR_RESET =       "\x1b[0m";
	
	enum LogType : uint
	{
		LOG_DEBUG = 0,
		LOG_INFO,
		LOG_WARNING,
		LOG_ERROR,
		LOG_ERROR_NOQUIT,
		LOG_NUM_LOG_TYPES
	};
	static std::array<std::string_view, LOG_NUM_LOG_TYPES> log_string_reps{
		"DEBUG", "INFO", "WARNING", "ERROR", "ERROR"};
	static std::array<std::string_view, LOG_NUM_LOG_TYPES> log_string_colors{
		ANSI_COLOR_GREEN, ANSI_COLOR_RESET, ANSI_COLOR_YELLOW, ANSI_COLOR_RED, ANSI_COLOR_RED};

	template <typename... TArgs>
	void log(uint log_type, std::string_view prefix, std::format_string<TArgs...> fmt_string, TArgs&& ...args)
	{
		std::string log_text = std::vformat(fmt_string.get(), std::make_format_args(std::forward<TArgs>(args)...));
		std::println("{}[{}] {}: {}{}", log_string_colors[log_type], log_string_reps[log_type], prefix, log_text, ANSI_COLOR_RESET);

		if (log_type == LOG_ERROR)
			exit(EXIT_FAILURE);
	}
	
	void run_command(std::string_view command);
	std::string run_command_with_stdout(std::string_view command);
}

#endif // __CHUTIL_HPP
