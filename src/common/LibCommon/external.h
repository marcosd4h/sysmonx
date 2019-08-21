#pragma once

//Internal defines
#define NOMINMAX
#define SPDLOG_WCHAR_TO_UTF8_SUPPORT

//WSDK
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cctype>
#include <cwctype>
#include <locale>
#include <cctype>
#include <mutex>
#include <exception>
#include <typeinfo>
#include <stdexcept>
#include <ctime>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <thread>
#include <queue>
#include <map>
#include <memory>
#include <atomic>
#include <vector>
#include <map>
#include <random>
#include <iterator>
#include <iterator>
#include <iomanip>
#include <fstream>
#include <cstdarg>
#include <ctime>
#include <krabs.h>
#include <cstdio>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <tlhelp32.h>
#include <versionhelpers.h>
#include <tchar.h>
#include <shlobj.h>
#include <dbghelp.h>
#include <psapi.h>
#include <winternl.h>
#include <wincrypt.h>
#include <wintrust.h>
#include <softpub.h>
#include <shldisp.h>
#include <atlbase.h>
#include <shellapi.h>
#include <windows.h>
#include <wininet.h>
#include <sddl.h>

//Blackbone
#include <BlackBone\Config.h>
#include <BlackBone\Process\Process.h>
#include <BlackBone\PE\PEImage.h>
#include <BlackBone\Misc\Utils.h>
#include <BlackBone\Misc\DynImport.h>
#include <BlackBone\Syscalls\Syscall.h>
#include <BlackBone\Patterns\PatternSearch.h>
#include <BlackBone\Asm\LDasm.h>
#include <BlackBone\localHook\VTableHook.hpp>
#include <BlackBone\Patterns\PatternSearch.h>
#include <BlackBone\Syscalls\Syscall.h>

//Boost 
#include <boost\shared_ptr.hpp>
#include <boost\asio.hpp>
#include <boost\asio\dispatch.hpp>
#include <boost\asio\post.hpp>
#include <boost\asio\spawn.hpp>
#include <boost\asio\strand.hpp>
#include <boost\asio\thread_pool.hpp>
#include <boost\thread\thread.hpp>
#include <boost\bind.hpp>
#include <boost\function.hpp>
#include <boost\noncopyable.hpp>
#include <boost\shared_ptr.hpp>
#include <boost\thread\mutex.hpp>
#include <boost\thread\locks.hpp>
#include <boost\lexical_cast.hpp>
#include <boost\uuid\uuid_io.hpp>
#include <boost\uuid\uuid.hpp>
#include <boost\uuid\random_generator.hpp>
#include <boost\regex.hpp>
#include <boost\algorithm\string.hpp>

//Spdlog
#include <spdlog\spdlog.h>
#include <spdlog\sinks\basic_file_sink.h>
#include <spdlog\sinks\stdout_sinks.h>
#include <spdlog\sinks\stdout_color_sinks.h>

//Sqlite 3
#include <sqlite3.h>

//Cista
#include <cista.hpp>

//PugiXML
#include <pugiconfig.hpp>
#include <pugixml.hpp>

//Misc 
#include <cmdparser.h>
#include <sysmonx.h>