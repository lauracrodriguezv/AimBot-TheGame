
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BPE_ColorTypeWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API UBPE_ColorTypeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(meta = (BindWidget))
	UImage* RedWeapon;

	UPROPERTY(meta = (BindWidget))
	UImage* YellowWeapon;

	UPROPERTY(meta = (BindWidget))
	UImage* BlueWeapon;

public:

	void OnNextWeaponType();
};
