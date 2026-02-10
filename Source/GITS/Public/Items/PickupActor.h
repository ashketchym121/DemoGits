#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interfaces/InteractableInterface.h"
#include "PickupActor.generated.h"

class UStaticMeshComponent;
class UItemBase;
class AGITSCharacter;

UCLASS()
class GITS_API APickupActor : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	APickupActor();

protected:
	virtual void BeginPlay() override;

	/* ===== Components ===== */

	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	UStaticMeshComponent* Mesh;

	/* ===== Item Data ===== */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	TSubclassOf<UItemBase> ItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	int32 Quantity;

public:
	/* ===== Interface ===== */

	virtual void Interact_Implementation(ACharacter* Interactor) override;
};
