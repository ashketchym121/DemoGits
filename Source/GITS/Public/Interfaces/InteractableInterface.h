#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

UINTERFACE(BlueprintType)
class GITS_API UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class GITS_API IInteractableInterface
{
	GENERATED_BODY()

public:
	/* Called on SERVER */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(ACharacter* Interactor);
};
