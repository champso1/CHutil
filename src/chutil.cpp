#include "chutil/chutil.hpp"

#include <array>
#include <vector>

#ifdef __WIN32__
    #define WIN32_LEAN_AND_MEAN
    #define UNICODE
    #include <windows.h>
    #include <tchar.h>
    #include <strsafe.h>
#else
    #include <unistd.h>
    #include <sys/wait.h>
#endif

namespace chutil
{
    void run_command(std::string_view command)
	{
#ifdef __WIN32__
		std::vector<wchar_t> cmd(command.begin(), command.end());
		
	    STARTUPINFO start_info{};
		start_info.cb = sizeof(STARTUPINFO);
		start_info.hStdError = GetStdHandle(STD_OUTPUT_HANDLE);
		start_info.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		start_info.dwFlags |= STARTF_USESTDHANDLES;

		PROCESS_INFORMATION proc_info{};
		
		BOOL success = CreateProcess(nullptr,
		    cmd.data(),
			nullptr, nullptr,
			TRUE,
			0,
			nullptr, nullptr,
			&start_info,
			&proc_info);
		if (!success) {
			std::array<wchar_t, 256> buf{};
			DWORD err = GetLastError();
			log(LOG_ERROR, "WIN32", "Failed to create child process ({})", err);
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, GetLastError(), LANG_SYSTEM_DEFAULT, buf.data(), buf.size(), nullptr);
			std::string buf_str(buf.begin(), buf.end());
			log(LOG_INFO, "WIN32", "Reason for above error: {}", buf_str);
			exit(EXIT_FAILURE);
		}
		WaitForSingleObject(proc_info.hProcess, INFINITE);
		CloseHandle(proc_info.hProcess);
		CloseHandle(proc_info.hThread);

#else
	    pid_t pid = fork();
		if (pid == -1)
			log(LOG_ERROR, "POSIX", "Failed to create child process.");

		if (pid == 0) {
		    execl("/bin/bash", "bash", "-c", command.data(), nullptr);
			_exit(127);
		}

		waitpid(pid, nullptr, 0);
#endif
	}
	
	std::string run_command_with_stdout(std::string_view command)
	{
#ifdef __WIN32__
		std::vector<wchar_t> cmd(command.begin(), command.end());
		
		SECURITY_ATTRIBUTES sec_attr{};
		sec_attr.nLength = sizeof(SECURITY_ATTRIBUTES);
		sec_attr.bInheritHandle = TRUE;

		HANDLE read_pipe, write_pipe;
		if (!CreatePipe(&read_pipe, &write_pipe, &sec_attr, 0)) {
			DWORD err = GetLastError();
			log(LOG_ERROR, "WIN32", "Failed to create pipe for child process ({}).", err);
		}

	    STARTUPINFO start_info{};
		start_info.cb = sizeof(STARTUPINFO);
		start_info.hStdError = write_pipe;
		start_info.hStdOutput = write_pipe;
		start_info.dwFlags |= STARTF_USESTDHANDLES;

		PROCESS_INFORMATION proc_info{};
		
		BOOL success = CreateProcess(nullptr,
		    cmd.data(),
			nullptr, nullptr,
			TRUE,
			0,
			nullptr, nullptr,
			&start_info,
			&proc_info);
		if (!success) {
			std::array<wchar_t, 256> buf{};
			DWORD err = GetLastError();
			log(LOG_ERROR, "WIN32", "Failed to create child process ({})", err);
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, GetLastError(), LANG_SYSTEM_DEFAULT, buf.data(), buf.size(), nullptr);
			std::string buf_str(buf.begin(), buf.end());
			log(LOG_INFO, "WIN32", "Reason for above error: {}", buf_str);
			CloseHandle(read_pipe);
			CloseHandle(write_pipe);
			exit(EXIT_FAILURE);
		}
		CloseHandle(write_pipe);

		std::array<char, 4096> buffer{};
		std::string output{};
		DWORD bytes_read;
		while(ReadFile(read_pipe, buffer.data(), buffer.size(), &bytes_read, nullptr))
			output.append(buffer.begin(), buffer.end());

		WaitForSingleObject(proc_info.hProcess, INFINITE);

		CloseHandle(read_pipe);
		CloseHandle(proc_info.hProcess);
		CloseHandle(proc_info.hThread);

		return output;
#else
		int pipefd[2]{};
		if (pipe(pipefd) == -1)
			log(ERROR, "(POSIX)", "Failed to create pipe for child process.");

		pid_t pid = fork();
		if (pid == -1)
			log(ERROR, "(POSIX)", "Failed to create child process.");

		if (pid == 0) {
			close(pipefd[0]);
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[1]);

			execl("/bin/bash", "bash", "-c", command.data(), nullptr);
			_exit(127);
		}

		close(pipefd[1]);

		std::array<char, 4096> buffer{};
		std::string output{};
		ssize_t n;

		while ((n = read(pipefd[0], buffer.data(), buffer.size())) > 0)
			output.append_range(buffer);

		close(pipefd[0]);
		waitpid(pid, nullptr, 0);

		return output;
#endif
	}
}
