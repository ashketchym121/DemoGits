#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UItemBase;

/* ================= INVENTORY ENTRY ================= */
USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UItemBase> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Quantity;

	FInventoryItem()
	{
		ItemClass = nullptr;
		Quantity = 0;
	}

	FInventoryItem(TSubclassOf<UItemBase> InItem, int32 InQuantity)
	{
		ItemClass = InItem;
		Quantity = InQuantity;
	}
};

/* ================= INVENTORY COMPONENT ================= */

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GITS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventoryItem> Items;

	/* SERVER ONLY */
	void AddItem(TSubclassOf<UItemBase> ItemClass, int32 Quantity = 1);

	/* Debug helper */
	void PrintInventory() const;

protected:
	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

