#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EVaultType.h"
#include "VaultComponent.generated.h"

class ACharacter;
class UCapsuleComponent;
class UCameraComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UVaultComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UVaultComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

	/* INPUT */
	void TryVault();

	UFUNCTION(BlueprintCallable, Category = "Vault")
	bool IsVaulting() const;


protected:
	/* INIT */
	void InitializeVault();

	/* TRACE */
	bool CheckVaultableObstacle(FHitResult& Hit, float& OutHeight, float& OutDepth);

	/* DECISION */
	EVault DecideVaultType(float Height, float Depth) const;

	/* APPLY */
	void ApplyTraversal(float DeltaTime);

	/* MOVEMENT */
	void PerformVault(float DeltaTime);
	void PerformMantle(float DeltaTime);
	void MoveCharacter(float DeltaTime, float Speed);

	/* CAMERA */
	void ApplyCameraOffset(float DeltaTime);

	/* FINISH */
	void FinishTraversal();



protected:
	/* OWNER */
	UPROPERTY()
	ACharacter* OwnerCharacter = nullptr;

	UPROPERTY()
	UCapsuleComponent* Capsule = nullptr;

	UPROPERTY()
	UCameraComponent* Camera = nullptr;

	/* STATE */
	UPROPERTY(BlueprintReadOnly, Category = "Vault")
	bool bIsTraversing = false;




	UPROPERTY()
	bool bCameraOffsetActive = false;

	UPROPERTY(VisibleAnywhere)
	EVault CurrentVaultType = EVault::None;

	/* LOCATIONS */
	UPROPERTY()
	FVector StartLocation;

	UPROPERTY()
	FVector TargetLocation;

	/* CAMERA */
	UPROPERTY()
	FVector CameraStartOffset;

	UPROPERTY()
	FVector CameraVaultOffset;

	/* TUNING */
	UPROPERTY(EditAnywhere, Category = "Vault|Trace")
	float TraceDistance = 120.f;

	UPROPERTY(EditAnywhere, Category = "Vault|Trace")
	float TraceRadius = 25.f;

	UPROPERTY(EditAnywhere, Category = "Vault|Limits")
	float MinHeight = 40.f;

	UPROPERTY(EditAnywhere, Category = "Vault|Limits")
	float MaxVaultHeight = 120.f;

	UPROPERTY(EditAnywhere, Category = "Vault|Limits")
	float MaxMantleHeight = 180.f;

	UPROPERTY(EditAnywhere, Category = "Vault|Limits")
	float MaxVaultDepth = 120.f;

	UPROPERTY(EditAnywhere, Category = "Vault|Movement")
	float InterpSpeed = 10.f;

	UPROPERTY(EditAnywhere, Category = "Vault|Camera")
	float CameraInterpSpeed = 12.f;


	UPROPERTY(VisibleAnywhere, Category = "Vault")
	float VaultHeight;

	UPROPERTY(VisibleAnywhere, Category = "Vault")
	float VaultDepth;

	UPROPERTY(VisibleAnywhere, Category = "Vault")
	float ObstacleTopHeight;


};
