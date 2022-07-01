// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BPE_PauseMenu.generated.h"

class UTextBlock;
class UButton;
class ABPE_PlayerController;

/**
 * 
 */
UCLASS()
class BESTPROJECTEVER_API UBPE_PauseMenu : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(Transient)
	TObjectPtr<ABPE_PlayerController> PlayerControllerReference;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PauseTittleText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ResumeButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> MainMenuButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ShowroomButton;

protected:

	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void ResumeGame();

	/** Enable buttons depending on network role */
	void InitializeButtons();

	/** User server travel if has authority or client travel if it is a client */
	UFUNCTION()
	void TravelToShowroom();

	/** User server travel if has authority or client travel if it is a client */
	UFUNCTION()
	void TravelToMainMenu();
	
public:

	void SetPauseMenuInformation(bool bWasPauseRequestByOwner);
};
