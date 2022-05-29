// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

//-------------------------------------------------------------------------------------------------
UMenuWidget::UMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxPublicConnections = 4;
	MatchType = TEXT("FreeForAll");
}

//-------------------------------------------------------------------------------------------------
void UMenuWidget::SetUpMenu(int32 InMaxPublicConnections, const FString& InMatchType, const FString& InPathToLobby)
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);

	MaxPublicConnections = InMaxPublicConnections;
	MatchType = InMatchType;

	PathToLobby = InPathToLobby + TEXT("?listen");
	bIsFocusable = true;

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		FInputModeUIOnly InputModeData;
		InputModeData.SetWidgetToFocus(TakeWidget());
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PlayerController->SetInputMode(InputModeData);
		PlayerController->SetShowMouseCursor(true);
	}

	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		MultiplayerSessionsSubsystemPtr = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();

		if (UMultiplayerSessionsSubsystem* SessionsSubsystem = MultiplayerSessionsSubsystemPtr.Get())
		{
			SessionsSubsystem->OnCreateSessionCompleteDelegate.AddDynamic(this, &UMenuWidget::OnCreateSessionComplete);
			SessionsSubsystem->OnDestroySessionCompleteDelegate.AddDynamic(this, &UMenuWidget::OnDestroySessionComplete);
			SessionsSubsystem->OnStartSessionCompleteDelegate.AddDynamic(this, &UMenuWidget::OnStartSessionComplete);

			SessionsSubsystem->OnFindSessionsCompleteDelegate.AddUObject(this, &UMenuWidget::OnFindSessionsComplete);
			SessionsSubsystem->OnJoinSessionCompleteDelegate.AddUObject(this, &UMenuWidget::OnJoinSessionComplete);
		}
	}
}

//-------------------------------------------------------------------------------------------------
bool UMenuWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &UMenuWidget::OnHostButtonClicked);
	}

	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &UMenuWidget::OnJoinButtonClicked);
	}

	return true;
}

//-------------------------------------------------------------------------------------------------
void UMenuWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	TearDownMenu();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

//-------------------------------------------------------------------------------------------------
// ReSharper disable once CppMemberFunctionMayBeConst
void UMenuWidget::OnCreateSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 15.f, FColor::Green, FString("Session Created Successfully!"));
		}

		GetWorld()->ServerTravel(PathToLobby);
	}

	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 15.f, FColor::Red, FString("Session Failed to Create..."));
		}

		SetButtonsEnabled(true);
	}
}

//-------------------------------------------------------------------------------------------------
void UMenuWidget::OnDestroySessionComplete(bool bWasSuccessful)
{
}

//-------------------------------------------------------------------------------------------------
void UMenuWidget::OnStartSessionComplete(bool bWasSuccessful)
{
}

void UMenuWidget::OnFindSessionsComplete(bool bWasSuccessful, const TArray<FOnlineSessionSearchResult>& SessionResults)
{
	if (!bWasSuccessful || SessionResults.Num() == 0)
	{
		SetButtonsEnabled(true);
		return;
	}

	if (UMultiplayerSessionsSubsystem* SessionsSubsystem = MultiplayerSessionsSubsystemPtr.Get())
	{
		for (const FOnlineSessionSearchResult& Result : SessionResults)
		{
			FString ResultMatchType;
			Result.Session.SessionSettings.Get(FName("MatchType"), ResultMatchType);

			if (ResultMatchType == MatchType)
			{
				SessionsSubsystem->JoinSession(Result);
				return;
			}
		}
	}

	// This won't execute if a valid session is found
	SetButtonsEnabled(true);
}

//-------------------------------------------------------------------------------------------------
// ReSharper disable once CppMemberFunctionMayBeConst
void UMenuWidget::OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result)
{
	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		SetButtonsEnabled(true);
		return;
	}
	
	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		if (const IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface())
		{
			FString ConnectInfo;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, ConnectInfo);

			if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
			{
				PlayerController->ClientTravel(ConnectInfo, TRAVEL_Absolute);
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------
// ReSharper disable once CppMemberFunctionMayBeConst
void UMenuWidget::TearDownMenu()
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		const FInputModeGameOnly InputModeData;
		PlayerController->SetInputMode(InputModeData);
		PlayerController->SetShowMouseCursor(false);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UMenuWidget::SetButtonsEnabled(bool bShouldEnable)
{
	if (HostButton)
	{
		HostButton->SetIsEnabled(bShouldEnable);
	}

	if (JoinButton)
	{
		JoinButton->SetIsEnabled(bShouldEnable);
	}
}

//-------------------------------------------------------------------------------------------------
// ReSharper disable once CppMemberFunctionMayBeConst
void UMenuWidget::OnHostButtonClicked()
{
	SetButtonsEnabled(false);
	
	if (UMultiplayerSessionsSubsystem* SessionsSubsystem = MultiplayerSessionsSubsystemPtr.Get())
	{
		SessionsSubsystem->CreateSession(MaxPublicConnections, MatchType);
	}
}

//-------------------------------------------------------------------------------------------------
// ReSharper disable once CppMemberFunctionMayBeConst
void UMenuWidget::OnJoinButtonClicked()
{
	SetButtonsEnabled(false);
	
	if (UMultiplayerSessionsSubsystem* SessionsSubsystem = MultiplayerSessionsSubsystemPtr.Get())
	{
		SessionsSubsystem->FindSessions(10000);
	}
}
