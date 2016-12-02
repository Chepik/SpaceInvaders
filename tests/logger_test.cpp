#include "list"
#include "gtest/gtest.h"

#include "logger.hpp"
#include "alien.hpp"
#include "space_ship.hpp"
#include "obstacle.hpp"
#include "bullet_manager.hpp"
#include "space.hpp"
#include "except.hpp"


// Check output with help of Instance(). Test SetLogLevel().
TEST(logger_test, logger_1)
{
  Logger::SetLogLevel(LogLevel::trace);

  Logger::Instance(LogLevel::trace) << "trace\n";

  Logger::Instance(LogLevel::debug) << "debug\n";

  Logger::Instance(LogLevel::info) << "info\n";

  Logger::Instance(LogLevel::warning) << "warning\n";

  Logger::Instance(LogLevel::fatal) << "fatal\n";
}

// Check output with help of Instance(). Test SetLogLevel().
TEST(logger_test, logger_2)
{
  Logger::SetLogLevel(LogLevel::info);

  Logger::Instance(LogLevel::trace) << "trace\n";

  Logger::Instance(LogLevel::debug) << "debug\n";

  Logger::Instance(LogLevel::info) << "info\n";

  Logger::Instance(LogLevel::warning) << "warning\n";

  Logger::Instance(LogLevel::fatal) << "fatal\n";
}

// Check output with help of Instance(). Test SetLogLevel().
TEST(logger_test, logger_3)
{
  Logger::SetLogLevel(LogLevel::fatal);

  Logger::Instance(LogLevel::trace) << "trace\n";

  Logger::Instance(LogLevel::debug) << "debug\n";

  Logger::Instance(LogLevel::info) << "info\n";

  Logger::Instance(LogLevel::warning) << "warning\n";

  Logger::Instance(LogLevel::fatal) << "fatal\n";
}

// Check output with help of Instance(). Print multiple messages.
TEST(logger_test, logger_4)
{
  Logger::SetLogLevel(LogLevel::info);

  Logger::Instance(LogLevel::fatal) << "info" << " warning" << " fatal\n";
}

// Check output with help of Instance(). Print multiple messages.
TEST(logger_test, logger_5)
{
  Logger::SetLogLevel(LogLevel::info);

  Logger::Instance(LogLevel::info) << 5 << "\n";
  Logger::Instance(LogLevel::info) << std::list<int>{2, 3, 4} << "\n";;
  Logger::Instance(LogLevel::info) << std::vector<Bullet>(3, Bullet()) << "\n";;
  Logger::Instance(LogLevel::info) << Alien() << SpaceShip() << Space(15, 14) << "\n";;
  Logger::Instance(LogLevel::info) << Alien() << SpaceShip() << Space(15, 14) << "\n";;
}

// Check output with help of Instance(). Print multiple messages.
TEST(logger_test, logger_6)
{
  Logger::SetLogLevel(LogLevel::info);

  Logger::Log(LogLevel::trace, "Hello");
  Logger::Log(LogLevel::fatal, "World");
}

// Check WrongLogLevelException.
TEST(logger_test, logger_7)
{
  try
  {
      Logger::GetLogLevelAsString(LogLevel::fatal);
  }
  catch (WrongLogLevelException)
  {

  }
}

// Check output a funtion name, line number and file name.
TEST(logger_test, logger_8)
{
  Logger::SetLogLevel(LogLevel::info);

  Logger::Log(LogLevel::info, "Hello", __func__, std::to_string(__LINE__), __FILE__);
}

// Check output with help of Log().
TEST(logger_test, logger_9)
{
  Logger::SetLogLevel(LogLevel::info);

  LOG_MESSAGE(LogLevel::info, "Hello");
}

// Check output with help of Log().
TEST(logger_test, logger_10)
{
  Logger::SetLogLevel(LogLevel::info);

  LOG_MESSAGE(LogLevel::info, "World");
}

// Check output with help of Log().
TEST(logger_test, logger_11)
{
  Logger::SetLogLevel(LogLevel::info);

  LOG_MESSAGE(LogLevel::info, "Print a message only");
}

// Check output with help of Log().
TEST(logger_test, logger_12)
{
  Logger::SetLogLevel(LogLevel::info);

  Logger::SetPrintFunctionName(true);
  Logger::SetPrintLineNumber(true);

  LOG_MESSAGE(LogLevel::info, "Print a function name and line number");
}

// Check output with help of Log().
TEST(logger_test, logger_13)
{
  Logger::SetLogLevel(LogLevel::info);

  Logger::SetPrintFunctionName(true);
  Logger::SetPrintLineNumber(true);
  Logger::SetPrintFileName(true);

  LOG_MESSAGE(LogLevel::info, "Print a function name, line number and file name");
}

// Check output with help of Instance().
TEST(logger_test, logger_14)
{
  Logger::SetLogLevel(LogLevel::info);

  Logger::SetPrintFunctionName(true);
  Logger::SetPrintLineNumber(true);
  Logger::SetPrintFileName(true);

  Logger::Instance(LogLevel::info, __func__, std::to_string(__LINE__), __FILE__) << "a message" << "\n";
}

// Check output with help of Instance().
TEST(logger_test, logger_15)
{
  Logger::SetLogLevel(LogLevel::info);

  Logger::SetPrintFunctionName(false);
  Logger::SetPrintLineNumber(false);
  Logger::SetPrintFileName(false);

  LOG(LogLevel::info) << "Print a message only" << "\n";
}

// Check output with help of Instance().
TEST(logger_test, logger_16)
{
  Logger::SetLogLevel(LogLevel::info);

  Logger::SetPrintFunctionName(true);
  Logger::SetPrintLineNumber(true);
  Logger::SetPrintFileName(false);

  LOG(LogLevel::info) << "Print a function name and line number" << "\n";
}

// Check output with help of Instance().
TEST(logger_test, logger_17)
{
  Logger::SetLogLevel(LogLevel::info);

  Logger::SetPrintFunctionName(true);
  Logger::SetPrintLineNumber(true);
  Logger::SetPrintFileName(true);

  LOG(LogLevel::info) << "Print a function name, line number and file name" << "\n";
}

// Check output with help of Instance().
TEST(logger_test, logger_18)
{
  Logger::SetLogLevel(LogLevel::info);

  Logger::SetPrintToFile("out.txt", true);

  LOG(LogLevel::info) << "Print to a file" << std::endl;
  LOG(LogLevel::info) << "Print to a file" << std::endl;
  LOG(LogLevel::info) << "Print to a file" << std::endl;
}

TEST(logger_test, logger_19)
{
  Logger::SetLogLevel(LogLevel::info);

  Logger::SetPrintToFile("out.txt", true);

  Logger::SetPrintFunctionName(true);
  Logger::SetPrintLineNumber(true);
  Logger::SetPrintFileName(true);

  LOG(LogLevel::info) << "Print to a file" << std::endl;
  LOG(LogLevel::info) << "Print to a file" << std::endl;
  LOG(LogLevel::info) << "Print to a file" << std::endl;
}
