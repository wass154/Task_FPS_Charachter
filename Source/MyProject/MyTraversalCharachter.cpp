// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTraversalCharachter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h" // Ensure ACharacter is included
#include "GameFramework/Controller.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AMyTraversalCharachter::AMyTraversalCharachter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* ================= MAIN ================= */
	MainComponents = CreateDefaultSubobject<UMainComponents>(TEXT("MainComponents"));

	/* ================= BODY ================= */
	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(GetCapsuleComponent());
	BodyMesh->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	BodyMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	if (GetMesh())
	{
		BodyMesh->SetupAttachment(GetMesh()); // Attach directly to primary mesh (no socket)
	}
	
	/* ================= CAMERA BOOM (TPS) ================= */
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(BodyMesh);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;

	/* ================= CAMERA ================= */
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(BodyMesh,FName("Head1")) ;

	FollowCamera->bUsePawnControlRotation = false;

	/* ================= INPUT MANAGER ================= */
	InputManager = CreateDefaultSubobject<UInputManager>(TEXT("InputManager"));


	/* ================= INITIAL MODE ================= */
	CurrentCameraMode = DefaultCameraMode;

	//ApplyCameraMode(CurrentCameraMode);

}

#pragma region CameraModeTPS/FPS
void AMyTraversalCharachter::ApplyCameraMode(ECameraMode Mode)
{
	if (!BodyMesh || !CameraBoom || !FollowCamera) return;

	if (Mode == ECameraMode::TPS)
	{
		// TPS: Camera on spring arm
		CameraBoom->SetActive(true);
		CameraBoom->SetVisibility(true);

		FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
		FollowCamera->SetRelativeLocation(FVector::ZeroVector);
		FollowCamera->SetRelativeRotation(FRotator::ZeroRotator);
		CameraBoom->bUsePawnControlRotation = true;
		FollowCamera->bUsePawnControlRotation = false;
	}
	else
	{
		// FPS: Camera directly on body
		CameraBoom->SetActive(false);
		CameraBoom->SetVisibility(false);

		// Attach camera to head socket
		FollowCamera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		FollowCamera->SetupAttachment(BodyMesh, FName("head")); // "head" is the bone name in your skeleton
		FollowCamera->SetRelativeLocation(FVector::ZeroVector);
		FollowCamera->SetRelativeRotation(FRotator::ZeroRotator);

		FollowCamera->bUsePawnControlRotation = true;
	}


	CurrentCameraMode = Mode;
}

/* ================= PUBLIC API ================= */

void AMyTraversalCharachter::SetCameraMode(ECameraMode NewMode)
{
	if (NewMode == CurrentCameraMode) return;
	ApplyCameraMode(NewMode);
}

void AMyTraversalCharachter::ToggleCameraMode()
{
	SetCameraMode(
		CurrentCameraMode == ECameraMode::TPS
		? ECameraMode::FPS
		: ECameraMode::TPS
	);
}
#pragma endregion CameraModeTPS/FPS



// Called when the game starts or when spawned
void AMyTraversalCharachter::BeginPlay()
{
	Super::BeginPlay();
	
	if (!MainComponents)
	{
		UE_LOG(LogTemp, Warning, TEXT("mainComnps was null at runtime. Attempting to find or recover."));
		MainComponents = FindComponentByClass<UMainComponents>();
	}

	if (MainComponents)
	{
		Components.SetComponents(CameraBoom, FollowCamera, GetCharacterMovement());
		
	}


	//ApplyCameraMode(DefaultCameraMode);

}

// Called every frame
void AMyTraversalCharachter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyTraversalCharachter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/*OLD INPUTS SECTION OR NEW */

	if (IsValid(MainComponents->MovementSystem))
	{
		PlayerInputComponent->BindAxis("MoveForward", MainComponents->MovementSystem, &UMovementSystems::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", MainComponents->MovementSystem, &UMovementSystems::MoveRight);
	}

	if (IsValid(MainComponents->StaminaComp))
	{
		PlayerInputComponent->BindAction("Sprint", IE_Pressed, MainComponents->StaminaComp, &UStaminaComponent::StartRunning);
		PlayerInputComponent->BindAction("Sprint", IE_Released, MainComponents->StaminaComp, &UStaminaComponent::StopRunning);
	}



	if (IsValid(MainComponents->CamComp)) 
	{
		PlayerInputComponent->BindAxis("Look", MainComponents->CamComp, &UCamComponent::LookUp);
		PlayerInputComponent->BindAxis("Turn", MainComponents->CamComp, &UCamComponent::Turn);
	}


	if (IsValid(MainComponents->CrouchComp))
	{
		PlayerInputComponent->BindAction("Crouch", IE_Pressed, MainComponents->CrouchComp, &UCrouchComponent::StartCrouch);
		PlayerInputComponent->BindAction("Crouch", IE_Released, MainComponents->CrouchComp, &UCrouchComponent::StopCrouch);
	}


	if (IsValid(MainComponents->SlideComp))
	{
		PlayerInputComponent->BindAction("Slide", IE_Pressed, MainComponents->SlideComp, &USlideComponent::StartSlide);
	}

	if (IsValid(MainComponents->VaultComp))
	{
		PlayerInputComponent->BindAction("Vault", IE_Pressed, MainComponents->VaultComp, &UVaultComponent::TryVault);

	}


	if (IsValid(MainComponents->LedgeComp))
	{
		PlayerInputComponent->BindAction("Ledge", IE_Pressed, MainComponents->LedgeComp, &ULedgeComponent::TryLedge);

	}
	
	if (IsValid(MainComponents->JumpComp))
	{
		PlayerInputComponent->BindAction("Jump", IE_Pressed, MainComponents->JumpComp, &UJumpComponent::StartJump);
		PlayerInputComponent->BindAction("Jump", IE_Released, MainComponents->JumpComp, &UJumpComponent::StopJump);


	}



	if (!InputManager) return;


	InputManager->IntiliazeInput();
	InputManager->AddMappingContext(InputManager->DefaultMappingContext);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked< UEnhancedInputComponent>(PlayerInputComponent))
	{

		EnhancedInputComponent->BindAction(InputManager->TryTestVault, ETriggerEvent::Started, MainComponents->VaultComp, &UVaultComponent::TryVault);
	}

	


	

}

