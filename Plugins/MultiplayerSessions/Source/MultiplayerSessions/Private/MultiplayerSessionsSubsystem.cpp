// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"

//-------------------------------------------------------------------------------------------------
UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
{
	bCreateSessionOnDestroy = false;
}

//-------------------------------------------------------------------------------------------------
void UMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	InitializeSessionInterface();
}

//-------------------------------------------------------------------------------------------------
void UMultiplayerSessionsSubsystem::CreateSession(int32 MaxPublicConnections, const FString& MatchType)
{
	if (!SessionInterface || !GetWorld() || !GetWorld()->GetFirstLocalPlayerFromController())
	{
		return;
	}

	if (SessionInterface->GetNamedSession(NAME_GameSession))
	{
		bCreateSessionOnDestroy = true;
		LastNumPublicConnections = MaxPublicConnections;
		LastMatchType = MatchType;
		
		DestroySession();
		return;
	}

	const auto CreateSessionDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete);
	OnCreateSessionCompleteHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionDelegate);

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings{});
	LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	LastSessionSettings->NumPublicConnections = MaxPublicConnections;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->bUseLobbiesIfAvailable = true;
	LastSessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	LastSessionSettings->BuildUniqueId = 1;

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	const bool bWasSuccessfulRequest = SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings);

	if (!bWasSuccessfulRequest)
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteHandle);
		OnCreateSessionCompleteDelegate.Broadcast(false);
	}
}

//-------------------------------------------------------------------------------------------------
void UMultiplayerSessionsSubsystem::DestroySession()
{
	if (!SessionInterface)
	{
		OnDestroySessionCompleteDelegate.Broadcast(false);
		return;
	}

	const auto DestroySessionDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete);
	OnDestroySessionCompleteHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionDelegate);

	const bool bWasSuccessfulRequest = SessionInterface->DestroySession(NAME_GameSession);
	if (!bWasSuccessfulRequest)
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteHandle);
		OnDestroySessionCompleteDelegate.Broadcast(false);
	}
}

//-------------------------------------------------------------------------------------------------
void UMultiplayerSessionsSubsystem::FindSessions(int32 MaxSearchResults)
{
	if (!SessionInterface || !GetWorld() || !GetWorld()->GetFirstLocalPlayerFromController())
	{
		return;
	}

	const auto FindSessionsDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete);
	OnFindSessionsCompleteHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsDelegate);

	LastSessionSearchRequest = MakeShareable(new FOnlineSessionSearch{});
	LastSessionSearchRequest->MaxSearchResults = MaxSearchResults;
	LastSessionSearchRequest->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	LastSessionSearchRequest->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	const bool bWasSuccessfulRequest = SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearchRequest.ToSharedRef());

	if (!bWasSuccessfulRequest)
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteHandle);
		OnFindSessionsCompleteDelegate.Broadcast(false, TArray<FOnlineSessionSearchResult>{});
	}
}

//-------------------------------------------------------------------------------------------------
void UMultiplayerSessionsSubsystem::JoinSession(const FOnlineSessionSearchResult& DesiredSession)
{
	if (!SessionInterface || !GetWorld() || !GetWorld()->GetFirstLocalPlayerFromController())
	{
		OnJoinSessionCompleteDelegate.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	const auto JoinSessionDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete);
	OnJoinSessionCompleteHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionDelegate);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	const bool bWasSuccessfulRequest = SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, DesiredSession);

	if (!bWasSuccessfulRequest)
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteHandle);
		OnJoinSessionCompleteDelegate.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

//-------------------------------------------------------------------------------------------------
void UMultiplayerSessionsSubsystem::StartSession()
{
}

//-------------------------------------------------------------------------------------------------
void UMultiplayerSessionsSubsystem::InitializeSessionInterface()
{
	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = OnlineSubsystem->GetSessionInterface();
	}
}

//-------------------------------------------------------------------------------------------------
void UMultiplayerSessionsSubsystem::InitializeDelegates()
{
	if (SessionInterface)
	{
		const auto StartSessionDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete);
		OnStartSessionCompleteHandle = SessionInterface->AddOnStartSessionCompleteDelegate_Handle(StartSessionDelegate);
	}
}

//-------------------------------------------------------------------------------------------------
void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteHandle);
	}

	OnCreateSessionCompleteDelegate.Broadcast(bWasSuccessful);
}

//-------------------------------------------------------------------------------------------------
void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSucccessful)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteHandle);
	}

	OnDestroySessionCompleteDelegate.Broadcast(bWasSucccessful);
	
	if (bWasSucccessful && bCreateSessionOnDestroy)
	{
		bCreateSessionOnDestroy = false;
		CreateSession(LastNumPublicConnections, LastMatchType);
	}
}

//-------------------------------------------------------------------------------------------------
void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteHandle);
	}

	if (!LastSessionSearchRequest || LastSessionSearchRequest->SearchResults.Num() == 0)
	{
		OnFindSessionsCompleteDelegate.Broadcast(false, TArray<FOnlineSessionSearchResult>());
		return;
	}

	OnFindSessionsCompleteDelegate.Broadcast(bWasSuccessful, LastSessionSearchRequest->SearchResults);
}

//-------------------------------------------------------------------------------------------------
void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteHandle);
	}

	OnJoinSessionCompleteDelegate.Broadcast(Result);
}

//-------------------------------------------------------------------------------------------------
void UMultiplayerSessionsSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
}

//-------------------------------------------------------------------------------------------------
