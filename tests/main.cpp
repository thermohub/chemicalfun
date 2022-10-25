
#include <gtest/gtest.h>

#include "tst_parser.h"
#include "tst_elements.h"
#include "tst_generator.h"
#include <spdlog/spdlog.h>

int main(int argc, char *argv[])
{
    auto ar_logger = spdlog::get("chemicalfun");
    ar_logger->set_level(spdlog::level::off);
    spdlog::set_pattern("[%n] [%^%l%$] %v");

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
