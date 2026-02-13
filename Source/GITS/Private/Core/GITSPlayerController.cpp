// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GITSPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"

#include "Player/GITSCharacter.h"


void AGITSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// ONLY LOCAL CLIENT SHOULD ADD INPUT
	if (!IsLocalController())
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();

		if (IMC_Player)
		{
			Subsystem->AddMappingContext(IMC_Player, 0);
		}
	}
}

void AGITSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
			EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AGITSPlayerController::Move);
			EIC->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AGITSPlayerController::Look);
			EIC->BindAction(IA_Sprint, ETriggerEvent::Started, this, &AGITSPlayerController::SprintStarted);
			EIC->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &AGITSPlayerController::SprintCompleted);
			EIC->BindAction(IA_Interact, ETriggerEvent::Triggered, this, &AGITSPlayerController::Interact);
            EIC->BindAction(IA_ToggleCamera, ETriggerEvent::Completed, this, &AGITSPlayerController::ToggleCamera);
			EIC->BindAction(IA_Crouch, ETriggerEvent::Triggered, this, &AGITSPlayerController::ToggleCrouch);
			EIC->BindAction(IA_ToggleInventory, ETriggerEvent::Completed, this, &AGITSPlayerController::ToggleInventory);
			EIC->BindAction(IA_Jump, ETriggerEvent::Started,this, &AGITSPlayerController::Jump);
	}
}

void AGITSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	GITSCharacter = Cast<AGITSCharacter>(InPawn);

	UE_LOG(LogTemp, Warning,
		TEXT("OnPossess | Pawn=%s | Local=%d"),
		*GetNameSafe(InPawn),
		IsLocalController());
}


void AGITSPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	GITSCharacter = nullptr;
}

/* ================= INPUT FORWARDING ================= */

void AGITSPlayerController::Move(const FInputActionValue& Value)
{
	if (!GITSCharacter)
	{
		GITSCharacter = Cast<AGITSCharacter>(GetPawn());
	}

	if (GITSCharacter)
	{
		GITSCharacter->HandleMove(Value);
	}
}

void AGITSPlayerController::Look(const FInputActionValue& Value)
{
	if (GITSCharacter)
	{
		GITSCharacter->HandleLook(Value);
	}
}

void AGITSPlayerController::SprintStarted(const FInputActionValue& Value)
{
	if (GITSCharacter)
	{
		GITSCharacter->StartSprint();
	}
}

void AGITSPlayerController::SprintCompleted(const FInputActionValue& Value)
{
	if (GITSCharacter)
	{
		GITSCharacter->StopSprint();
	}
}

void AGITSPlayerController::Interact(const FInputActionValue& Value)
{
	if (GITSCharacter)
	{
		GITSCharacter->HandleInteract(Value);
	}
}

void AGITSPlayerController::ToggleCamera(const FInputActionValue& Value)
{
	if (GITSCharacter)
	{
		GITSCharacter->ToggleCameraMode();
	}
}

void AGITSPlayerController::ToggleCrouch(const FInputActionValue& Value)
{
	if (GITSCharacter)
	{
		GITSCharacter->HandleCrouch(Value);
	}
}

void AGITSPlayerController::Jump(const FInputActionValue& Value)
{
	if (GITSCharacter)
	{
		GITSCharacter->Jump();
	}
}

// ================= INVENTORY =================

void AGITSPlayerController::ToggleInventory(const FInputActionValue& Value)
{
	if (GITSCharacter)
	{
		GITSCharacter->ToggleInventory();
	}
}