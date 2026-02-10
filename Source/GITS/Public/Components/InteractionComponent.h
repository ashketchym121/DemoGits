#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class AActor;
class AGITSCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GITS_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

	/* Try Interact entry point (called from Character) */
	void TryInteract();

protected:
	virtual void BeginPlay() override;

	/* ===== Interaction Settings ===== */

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float TraceDistance;

	/* Cached owner */
	UPROPERTY()
	AGITSCharacter* OwnerCharacter;

	/* ===== SERVER RPC ===== */

	UFUNCTION(Server, Reliable)
	void Server_TryInteract();

	/* ===== Internal Logic ===== */

	void PerformInteractionTrace();
};
