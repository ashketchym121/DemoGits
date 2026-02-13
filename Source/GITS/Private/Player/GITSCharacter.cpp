#include "Player/GITSCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/InteractionComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/StaminaComponent.h"

#include "Engine/Engine.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

// ================= CONSTRUCTOR =================

AGITSCharacter::AGITSCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Defaults
	WalkSpeed = 350.f;
	SprintSpeed = 750.f;
	bIsSprinting = false;
	bIsFirstPerson = false;
	bReplicates = true;
	SetReplicateMovement(true);
	GetCharacterMovement()->SetIsReplicated(true);
	// Capsule already exists (ACharacter)

	//Inventory
	bIsInventoryOpen = false;
	InventoryWidget = nullptr;

	// ===== Camera Setup =====
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	// ===== Components =====
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComponent"));

	// ===== Movement =====
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;

	/*--Crouch--*/
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 200.f;

}

// ================= BEGIN PLAY =================

void AGITSCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

// ================= MOVE =================

void AGITSCharacter::HandleMove(const FInputActionValue& Value)
{
	if (!IsLocallyControlled())
	{
		return;
	}
	const FVector2D MovementVector = Value.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

// ================= LOOK =================

void AGITSCharacter::HandleLook(const FInputActionValue& Value)
{
	if (!IsLocallyControlled())
	{
		return;
	}
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

// ================= CROUCH =================

void AGITSCharacter::HandleCrouch(const FInputActionValue& Value)
{
	if (!IsLocallyControlled()) return;
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

// ================= SPRINT =================

void AGITSCharacter::StartSprint()
{
	if (!IsLocallyControlled()) return;
	if (!StaminaComponent) return;

	// CLIENT → SERVER
	if (!HasAuthority())
	{
		Server_StartSprint();
		return;
	}

	// SERVER LOGIC
	if (!StaminaComponent->CanSprint())
	{
		return;
	}

	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	StaminaComponent->StartDrain();
}


void AGITSCharacter::StopSprint()
{
	// CLIENT → SERVER
	if (!HasAuthority())
	{
		Server_StopSprint();
		return;
	}

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	if (StaminaComponent)
	{
		StaminaComponent->StopDrain();
	}
}

void AGITSCharacter::Server_StartSprint_Implementation()
{
	StartSprint();
}

void AGITSCharacter::Server_StopSprint_Implementation()
{
	StopSprint();
}


// ================= INTERACT =================

void AGITSCharacter::HandleInteract(const FInputActionValue& Value)
{
	if (!IsLocallyControlled()) return;
	if (InteractionComponent)
	{
		InteractionComponent->TryInteract();
	}
}

// ================= CAMERA TOGGLE =================

void AGITSCharacter::ToggleCameraMode()
{
	if (!IsLocallyControlled()) return;
	bIsFirstPerson = !bIsFirstPerson;

	if (bIsFirstPerson)
	{
		SpringArm->TargetArmLength = 0.f;
	}
	else
	{
		SpringArm->TargetArmLength = 300.f;
	}
}

// ================= INVENTORY TOGGLE (DEMO) =================

void AGITSCharacter::ToggleInventory()
{
	GEngine->AddOnScreenDebugMessage(
		-1, 1.5f, FColor::Green,
		TEXT("ToggleInventory CALLED")
	);
		if (!IsLocallyControlled())
		{
			return;
		}

		APlayerController* PC = Cast<APlayerController>(GetController());
		if (!PC || !InventoryWidgetClass)
		{
			return;
		}

		/* ================= OPEN ================= */
		if (!bIsInventoryOpen)
		{
			InventoryWidget = CreateWidget<UUserWidget>(PC, InventoryWidgetClass);
			if (!InventoryWidget)
			{
				return;
			}

			InventoryWidget->AddToViewport();

			// Input → UI
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
			InputMode.SetHideCursorDuringCapture(false);

			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = true;

			bIsInventoryOpen = true;
		}
		/* ================= CLOSE ================= */
		else
		{
			if (InventoryWidget)
			{
				InventoryWidget->RemoveFromParent();
				InventoryWidget = nullptr;
			}

			// Back to game
			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = false;

			bIsInventoryOpen = false;
		}
}


