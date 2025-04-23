#include "hive.h"
int main()
{
    hive::Init();

    HIVE_LOG_INFO("Hello World");

    hive::Shutdown();
}
