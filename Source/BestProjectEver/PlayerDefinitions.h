#pragma once

UENUM(BlueprintType)
enum class EPauseState : uint8
{
	UnPause,
	PausedBySelf,
	PausedByOtherPlayer
};
