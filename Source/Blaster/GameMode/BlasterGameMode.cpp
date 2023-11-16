// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameMode.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Blaster/PlayerState/BlasterPlayerState.h"

void ABlasterGameMode::PlayerEliminated(ABlasterCharacter* ElimmedCharacter, ABlasterPlayerController* VictimController,
                                        ABlasterPlayerController* AttackerController)
{
	ABlasterPlayerState* AttackerPlayerState = AttackerController ? Cast<ABlasterPlayerState>(AttackerController->PlayerState) : nullptr;
	ABlasterPlayerState* VictimPlayerState = VictimController ? Cast<ABlasterPlayerState>(VictimController->PlayerState) : nullptr;
	if(AttackerPlayerState && AttackerPlayerState != VictimPlayerState)
	{
		AttackerPlayerState->AddToScore(1.f);
	}
	if(VictimPlayerState)
	{
		VictimPlayerState->AddToDefeats(1);
	}
	if(ElimmedCharacter)
	{
		ElimmedCharacter->Elim();
	}
}

void ABlasterGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if (ElimmedController)
	{
		TArray<AActor*>AllPlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), AllPlayerStarts);
 
		TArray<AActor*>Characters;
		UGameplayStatics::GetAllActorsOfClass(this, ABlasterCharacter::StaticClass(), Characters);
 
		TArray<float>StockedDistances;
 
		for (int i = 0; i < AllPlayerStarts.Num(); i++)
		{
			float MinDistance = (AllPlayerStarts[i]->GetActorLocation() - Characters[0]->GetActorLocation()).Size();
			for (int j = 1; j < Characters.Num(); j++)
			{
				float Distance = (AllPlayerStarts[i]->GetActorLocation() - Characters[j]->GetActorLocation()).Size();
				if (Distance < MinDistance)
				{
					MinDistance = Distance;
				}
			}
			StockedDistances.Add(MinDistance);
		}
 
		float MaxDistance = StockedDistances[0];
		int32 Selection = 0;
 
		for (int i = 1; i < StockedDistances.Num(); i++)
		{
			if (MaxDistance < StockedDistances[i])
			{
				MaxDistance = StockedDistances[i];
				Selection = i;
			}
		}
		
		//int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1); //Random version
		RestartPlayerAtPlayerStart(ElimmedController, AllPlayerStarts[Selection]);
	}
}
