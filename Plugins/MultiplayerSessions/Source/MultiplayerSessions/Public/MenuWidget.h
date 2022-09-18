// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MenuWidget.generated.h"

class UMultiplayerSessionsSubsystem;
class UButton;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButton> HostButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButton> JoinButton;

private:
	TWeakObjectPtr<UMultiplayerSessionsSubsystem> MultiplayerSessionsSubsystemPtr;

	int32 MaxPublicConnections;
	FString MatchType;
	FString PathToLobby;

public:
	UMenuWidget(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm="InMatchType"))
	void SetUpMenu(UPARAM(DisplayName="Max Public Connections") int32 InMaxPublicConnections = 4,
	               UPARAM(DisplayName="Match Type") const FString& InMatchType = TEXT("FreeForAll"),
	               UPARAM(DisplayName="Path to Lobby") const FString& InPathToLobby = TEXT("/Game/ThirdPerson/Maps/RemMap"));

protected:
	virtual bool Initialize() override;
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	UFUNCTION()
	void OnCreateSessionComplete(bool bWasSuccessful);
	UFUNCTION()
	void OnDestroySessionComplete(bool bWasSuccessful);
	UFUNCTION()
	void OnStartSessionComplete(bool bWasSuccessful);

	void OnFindSessionsComplete(bool bWasSuccessful, const TArray<FOnlineSessionSearchResult>& SessionResults);
	void OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result);

private:
	void TearDownMenu();

	void SetButtonsEnabled(bool bShouldEnable);

private:
	UFUNCTION()
	void OnHostButtonClicked();

	UFUNCTION()
	void OnJoinButtonClicked();
};
