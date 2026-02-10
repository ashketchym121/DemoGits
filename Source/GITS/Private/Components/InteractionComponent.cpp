#include "Components/InteractionComponent.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"

#include "Player/GITSCharacter.h"
#include "Interfaces/InteractableInterface.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	TraceDistance = 300.f;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AGITSCharacter>(GetOwner());
}

/* ================= ENTRY ================= */

void UInteractionComponent::TryInteract()
{
	if (!OwnerCharacter)
	{
		return;
	}

	// Client → Server
	if (!OwnerCharacter->HasAuthority())
	{
		Server_TryInteract();
		return;
	}

	// Server direct
	PerformInteractionTrace();
}

/* ================= SERVER ================= */

void UInteractionComponent::Server_TryInteract_Implementation()
{
	PerformInteractionTrace();
}

/* ================= TRACE LOGIC ================= */

void UInteractionComponent::PerformInteractionTrace()
{
	if (!OwnerCharacter)
	{
		return;
	}

	// === TRACE FROM CHARACTER EYES ===
	FVector Start;
	FRotator ViewRotation;
	OwnerCharacter->GetActorEyesViewPoint(Start, ViewRotation);

	const FVector End = Start + (ViewRotation.Vector() * TraceDistance);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		Params
	);

#if WITH_EDITOR
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.f, 0, 1.f);
#endif

	if (!bHit)
	{
		return;
	}

	AActor* HitActor = HitResult.GetActor();
	if (!IsValid(HitActor))
	{
		return;
	}

	// === INTERFACE CHECK ===
	if (HitActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		IInteractableInterface::Execute_Interact(HitActor, OwnerCharacter);
	}
}
