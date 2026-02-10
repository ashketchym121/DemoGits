#include "Components/StaminaComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"

UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	MaxStamina = 100.f;
	DrainRate = 20.f;
	RegenRate = 15.f;

	CurrentStamina = MaxStamina;
	bIsDraining = false;
}

void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		CurrentStamina = MaxStamina;
	}
}

void UStaminaComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// SERVER ONLY
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}

	if (bIsDraining)
	{
		CurrentStamina = FMath::Clamp(
			CurrentStamina - (DrainRate * DeltaTime),
			0.f,
			MaxStamina
		);
	}
	else
	{
		CurrentStamina = FMath::Clamp(
			CurrentStamina + (RegenRate * DeltaTime),
			0.f,
			MaxStamina
		);
	}
}

bool UStaminaComponent::CanSprint() const
{
	return CurrentStamina > 5.f;
}

void UStaminaComponent::StartDrain()
{
	if (!GetOwner()->HasAuthority()) return;
	bIsDraining = true;
}

void UStaminaComponent::StopDrain()
{
	if (!GetOwner()->HasAuthority()) return;
	bIsDraining = false;
}

float UStaminaComponent::GetCurrentStamina() const
{
	return CurrentStamina;
}

float UStaminaComponent::GetMaxStamina() const
{
	return MaxStamina;
}

void UStaminaComponent::OnRep_CurrentStamina()
{
	// UI later (demo-safe)
}

void UStaminaComponent::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps
) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStaminaComponent, CurrentStamina);
}
