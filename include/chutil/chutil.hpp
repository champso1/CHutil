#ifndef __CHUTIL_HPP
#define __CHUTIL_HPP

#include <string_view>
#include <string>
#include <format>
#include <print>

using uint = unsigned;

namespace chutil
{
	enum LogType : uint
	{
		DEBUG = 0,
		INFO,
		WARNING,
		ERROR,
		NUM_LOG_TYPES
	};
	static std::array<std::string_view, NUM_LOG_TYPES> log_string_reps{"DEBUG", "INFO", "WARNING", "ERROR"};

	template <typename... TArgs>
	void log(uint log_type, std::string_view prefix, std::format_string<TArgs...> fmt_string, TArgs&& ...args)
	{
		std::string log_text = std::vformat(fmt_string.get(), std::make_format_args(std::forward<TArgs>(args)...));
		std::println("[{}] {}: {}", log_string_reps[log_type], prefix, log_text);
	}
	
	void run_command(std::string_view command);
	std::string run_command_with_stdout(std::string_view command);
}

#endif // __CHUTIL_HPP
