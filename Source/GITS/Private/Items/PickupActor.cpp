#include "Items/PickupActor.h"

#include "Components/StaticMeshComponent.h"
#include "Items/ItemBase.h"
#include "Player/GITSCharacter.h"
#include "Components/InventoryComponent.h"

APickupActor::APickupActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));

	Quantity = 1;
}

void APickupActor::BeginPlay()
{
	Super::BeginPlay();
}

void APickupActor::Interact_Implementation(ACharacter* Interactor)
{
	AGITSCharacter* Character = Cast<AGITSCharacter>(Interactor);
	if (!Character)
	{
		return;
	}

	// SERVER ONLY
	if (!HasAuthority())
	{
		return;
	}

	if (!ItemClass)
	{
		return;
	}

	UInventoryComponent* Inventory = Character->FindComponentByClass<UInventoryComponent>();
	if (!Inventory)
	{
		return;
	}

	// Create Item Object
	UItemBase* NewItem = NewObject<UItemBase>(this, ItemClass);
	if (!NewItem)
	{
		return;
	}

	Inventory->AddItem(ItemClass, Quantity);

	// Destroy pickup on server (auto replicate)
	Destroy();
	
}
