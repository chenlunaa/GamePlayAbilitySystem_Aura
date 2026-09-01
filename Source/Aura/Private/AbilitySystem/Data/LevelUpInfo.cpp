// Copyright Druid Mechanics


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP) const
{
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		// 等级为1对应Index为1的数据
		if (LevelUpInformation.Num() - 1 <= Level) return Level;
		
		if (XP >= LevelUpInformation[Level].LevelUpRequirement)
		{
			Level++;
		}
		else
		{
			bSearching = false;
		}
	}
	return Level;
}
