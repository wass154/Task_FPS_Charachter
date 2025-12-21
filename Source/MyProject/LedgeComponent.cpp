#include "LedgeComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

ULedgeComponent::ULedgeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	LedgeDistance = 150.f;
	maxLedge = 100.f;
	bIsLedging = false;
	bIsdropped = false;
}

void ULedgeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ULedgeComponent::IntializeLedge()
{
	bIsLedging = false;
	bIsdropped = false;
	LedgePostion = FVector::ZeroVector;
	CurrentPostion = FVector::ZeroVector;
	CurrentRotation = FRotator::ZeroRotator;
}

void ULedgeComponent::TryLedge()
{
	if (!CanLedge()) return;

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	FVector Start = Character->GetActorLocation();
	FVector ForwardVector = Character->GetActorForwardVector();
	FVector End = Start + ForwardVector * LedgeDistance;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);

	// Trace for wall
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
	if (bHit)
	{
		// Check ledge above hit
		FVector LedgeCheckStart = HitResult.ImpactPoint + FVector(0, 0, maxLedge);
		FVector LedgeCheckEnd = HitResult.ImpactPoint - FVector(0, 0, 50.f);

		FHitResult LedgeHit;
		if (!GetWorld()->LineTraceSingleByChannel(LedgeHit, LedgeCheckStart, LedgeCheckEnd, ECC_Visibility, Params))
		{
			// Valid ledge found
			LedgePostion = HitResult.ImpactPoint + FVector(0, 0, 50.f); // slightly above ledge
			StartLedge();
		}
	}

	// Optional: Debug
	 DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f);
}

void ULedgeComponent::StartLedge()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	bIsLedging = true;
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Flying); // disable gravity temporarily

	// Optional: Lock player input here

	// Fake animation movement: smoothly move to ledge position
	CurrentPostion = Character->GetActorLocation();
	CurrentRotation = Character->GetActorRotation();
}

void ULedgeComponent::StopLedge()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	bIsLedging = false;
	bIsdropped = false;
	Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	// Optional: Unlock player input here
}

bool ULedgeComponent::CanLedge()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return false;

	if (bIsLedging) return false;
	if (Character->GetCharacterMovement()->IsFalling()) return true; // can grab while falling
	return false;
}

void ULedgeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsLedging)
	{
		ACharacter* Character = Cast<ACharacter>(GetOwner());
		if (!Character) return;

		// Smooth movement to ledge
		FVector Target = LedgePostion;
		FVector NewLocation = FMath::VInterpTo(Character->GetActorLocation(), Target, DeltaTime, 8.f);
		Character->SetActorLocation(NewLocation);

		// Optional: smooth rotation to face ledge
		FRotator TargetRot = CurrentRotation;
		FRotator NewRot = FMath::RInterpTo(Character->GetActorRotation(), TargetRot, DeltaTime, 8.f);
		Character->SetActorRotation(NewRot);

		// Check if reached ledge fully
		if (FVector::Dist(Character->GetActorLocation(), Target) < 5.f)
		{
			// Pull up or allow drop input
			// Example: auto pull-up
			FVector PullUpPos = Target + FVector(0, 0, 100.f);
			Character->SetActorLocation(PullUpPos);
			StopLedge();
		}
	}
}
