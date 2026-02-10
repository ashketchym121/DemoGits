#include "Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Items/ItemBase.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UInventoryComponent::AddItem(TSubclassOf<UItemBase> ItemClass, int32 Quantity)
{
	if (!ItemClass || Quantity <= 0)
		return;

	if (!GetOwner() || !GetOwner()->HasAuthority())
		return;

	for (FInventoryItem& Entry : Items)
	{
		if (Entry.ItemClass == ItemClass)
		{
			Entry.Quantity += Quantity;
			PrintInventory();
			return;
		}
	}

	Items.Add(FInventoryItem(ItemClass, Quantity));
	PrintInventory();
}

void UInventoryComponent::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventoryComponent, Items);
}

void UInventoryComponent::PrintInventory() const
{
	if (!GEngine) return;

	GEngine->AddOnScreenDebugMessage(
		-1, 3.f, FColor::Cyan,
		TEXT("===== INVENTORY =====")
	);

	for (const FInventoryItem& Entry : Items)
	{
		if (!Entry.ItemClass) continue;

		const UItemBase* DefaultItem = Entry.ItemClass->GetDefaultObject<UItemBase>();
		if (!DefaultItem) continue;

		const FString Line = FString::Printf(
			TEXT("%s x%d"),
			*DefaultItem->ItemName.ToString(),
			Entry.Quantity
		);

		GEngine->AddOnScreenDebugMessage(
			-1, 3.f, FColor::Green,
			Line
		);
	}
}