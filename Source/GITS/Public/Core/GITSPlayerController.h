#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "GITSPlayerController.generated.h"

// Forward declarations
class UInputMappingContext;
class UInputAction;
class AGITSCharacter;

UCLASS()
class GITS_API AGITSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

protected:
	/* ===== Input Mapping Context ===== */

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* IMC_Player;

	/* ===== Input Actions (BP Assets) ===== */

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Look;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Sprint;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Interact;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_ToggleCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Crouch;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_ToggleInventory;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Jump;


private:
	/* Cached Character */
	UPROPERTY()
	AGITSCharacter* GITSCharacter;

	/* ===== Input Callbacks ===== */

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void SprintStarted(const FInputActionValue& Value);
	void SprintCompleted(const FInputActionValue& Value);

	void Interact(const FInputActionValue& Value);
	void ToggleCamera(const FInputActionValue& Value);
	void ToggleCrouch(const FInputActionValue& Value);
	void ToggleInventory(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
};
