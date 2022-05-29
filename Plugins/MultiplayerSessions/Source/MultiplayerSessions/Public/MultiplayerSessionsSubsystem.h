// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MultiplayerSessionsSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiSession_OnCreateSessionCompleteSignature, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiSession_OnDestroySessionCompleteSignature, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiSession_OnStartSessionCompleteSignature, bool, bWasSuccessful);

DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiSession_OnFindSessionsCompleteSignature, bool bWasSuccessful, const TArray<FOnlineSessionSearchResult>& SessionResults);
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiSession_OnJoinSessionCompleteSignature, EOnJoinSessionCompleteResult::Type Result);

UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UMultiplayerSessionsSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	void CreateSession(int32 MaxPublicConnections, const FString& MatchType);
	void DestroySession();

	void FindSessions(int32 MaxSearchResults);
	void JoinSession(const FOnlineSessionSearchResult& DesiredSession);

	void StartSession();

public:
	/** Public Subsystem Interface Delegates Start */
	FMultiSession_OnCreateSessionCompleteSignature OnCreateSessionCompleteDelegate;
	FMultiSession_OnDestroySessionCompleteSignature OnDestroySessionCompleteDelegate;

	FMultiSession_OnFindSessionsCompleteSignature OnFindSessionsCompleteDelegate;
	FMultiSession_OnJoinSessionCompleteSignature OnJoinSessionCompleteDelegate;

	FMultiSession_OnStartSessionCompleteSignature OnStartSessionCompleteDelegate;
	/** Public Subsystem Interface Delegates End */

private:
	void InitializeSessionInterface();
	void InitializeDelegates();
	
	/** Internal Callbacks Start */
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSucccessful);

	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);
	/** Internal Callbacks End */

private:
	/** Interface for session management services */
	IOnlineSessionPtr SessionInterface;

	/** The session settings for the last created online session */
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;

	/** The state of the last request for finding sessions */
	TSharedPtr<FOnlineSessionSearch> LastSessionSearchRequest;
	
	/** Online Session Interface Delegates Start */
	FDelegateHandle OnCreateSessionCompleteHandle;
	FDelegateHandle OnDestroySessionCompleteHandle;

	FDelegateHandle OnFindSessionsCompleteHandle;
	FDelegateHandle OnJoinSessionCompleteHandle;

	FDelegateHandle OnStartSessionCompleteHandle;
	/** Online Session Interface Delegates End */

	uint8 bCreateSessionOnDestroy : 1;
	int32 LastNumPublicConnections;
	FString LastMatchType;
};
