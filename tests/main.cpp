#include <gtest/gtest.h>

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();

	#if (!defined SS6PLAYER_TESTS_WITH_CI_BUILD)
	{
		// wait for enter
		std::cin.get();
	}
	#endif

	return 0;
}

