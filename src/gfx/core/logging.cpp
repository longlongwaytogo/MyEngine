#include <gfx/core/logging.h>

namespace gfx {

#if defined(GFX_ENABLE_LOGGING)
void initLogging() {
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    consoleSink->set_level(spdlog::level::trace);
    consoleSink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%t] %v");
    
    auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/gfx.log", true);
    fileSink->set_level(spdlog::level::trace);
    fileSink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] %v");
    
    std::vector<spdlog::sink_ptr> sinks{consoleSink, fileSink};
    auto logger = std::make_shared<spdlog::logger>("gfx", sinks.begin(), sinks.end());
    logger->set_level(spdlog::level::debug);
    spdlog::register_logger(logger);
    spdlog::set_default_logger(logger);
}

void shutdownLogging() {
    spdlog::shutdown();
}
#else
void initLogging() {}
void shutdownLogging() {}
#endif

}
