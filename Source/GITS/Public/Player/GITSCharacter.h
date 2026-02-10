#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GITSCharacter.generated.h"

// Forward declarations
class USpringArmComponent;
class UCameraComponent;
class UInteractionComponent;
class UInventoryComponent;
class UStaminaComponent;

// Forward declaration UI
class UUserWidget;
class UInventoryComponent;

UCLASS()
class GITS_API AGITSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGITSCharacter();


protected:
	virtual void BeginPlay() override;

	/* ================= COMPONENTS ================= */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaminaComponent* StaminaComponent;

	/* ===== INVENTORY UI ===== */

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	UPROPERTY()
	UUserWidget* InventoryWidget;

	bool bIsInventoryOpen;

	/* ================= MOVEMENT ================= */

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float WalkSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SprintSpeed;

	bool bIsSprinting;
	bool bIsFirstPerson;

protected:
	/* ================= SPRINT RPCs ================= */

	UFUNCTION(Server, Reliable)
	void Server_StartSprint();

	UFUNCTION(Server, Reliable)
	void Server_StopSprint();

public:
	/* ================= INPUT HANDLERS ================= */

	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);
	void HandleCrouch(const FInputActionValue& Value);
	void HandleInteract(const FInputActionValue& Value);

	void StartSprint();
	void StopSprint();

	void ToggleCameraMode();

	/* Inventory Toggle (called from PlayerController) */
	void ToggleInventory(); // demo stub

	//getters
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return Camera; }

};
