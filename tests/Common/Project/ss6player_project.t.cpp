#include <gtest/gtest.h>
#include <src/Common/Project/ss6player_project.h>

TEST(ss6player_project, Basic)
{
	SpriteStudio6::Project project;

	ASSERT_FALSE(project.IsValid());

	ASSERT_EQ(project.CountGetTexture(), -1);
}

