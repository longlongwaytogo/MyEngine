#pragma once

#if defined(GFX_ENABLE_LOGGING)
    #include <spdlog/spdlog.h>
    #include <spdlog/sinks/stdout_color_sinks.h>
    #include <spdlog/sinks/basic_file_sink.h>
#endif

namespace gfx {

#if defined(GFX_ENABLE_LOGGING)
    #define GFX_LOG_INFO(...) SPDLOG_INFO(__VA_ARGS__)
    #define GFX_LOG_WARN(...) SPDLOG_WARN(__VA_ARGS__)
    #define GFX_LOG_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
    #define GFX_LOG_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
    #define GFX_LOG_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#else
    #define GFX_LOG_INFO(...) ((void)0)
    #define GFX_LOG_WARN(...) ((void)0)
    #define GFX_LOG_ERROR(...) ((void)0)
    #define GFX_LOG_DEBUG(...) ((void)0)
    #define GFX_LOG_TRACE(...) ((void)0)
#endif

void initLogging();
void shutdownLogging();

}
