#include <gtest/gtest.h>
#include <src/Common/Sequencer/ss6player_sequencer.h>

TEST(ss6player_sequencer, Basic)
{
	SpriteStudio6::Sequencer::DataStep step;
	{
		step.IndexAnimationPack = 0;
		step.IndexAnimation = 0;
		step.CountPlay = 1;
	}
	ASSERT_TRUE(step.IsValid());
}

