#include <gmock/gmock.h>
#include "FunctionalTestEnvironment.h"

#include "ara/core/initialization.h"


/*!
 * \brief Start point to run functional tests
 *
 */
int
main(int argc, char* argv[])
{
     if (!ara::core::Initialize()) {
        // No interaction with ARA is possible here since initialization failed
        std::cerr << "ara runtime initialization failed";
        return EXIT_FAILURE;
    }

    ::testing::InitGoogleTest(&argc, argv);

    ::testing::Environment* const globalTestsEnvoronment __attribute__((unused)) =
        ::testing::AddGlobalTestEnvironment(new FunctionalTestsEnvironment);

    return RUN_ALL_TESTS();
}