#include "StaminaWidget.h"
#include "Components/Image.h"
#include "StaminaComponent.h"
#include "MovementSystems.h"
#include "Components/ProgressBar.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyTraversalCharachter.h"
#include "TimerManager.h"

void UStaminaWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeCharacter();
	PlayAnimation(FadeOut);
}

void UStaminaWidget::InitializeCharacter()
{
	OwnerCharacter = Cast<ACharacter>(GetOwningPlayerPawn());
	if (!OwnerCharacter) return;

	TraversalCharachter = Cast<AMyTraversalCharachter>(GetOwningPlayerPawn());
	if (!TraversalCharachter) return;

	StaminaComponent = TraversalCharachter->MainComponents->StaminaComp;
}

void UStaminaWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!OwnerCharacter || !StaminaComponent || !StaminaProgressBar)
	{
		return;
	}

	UpdateStamina(InDeltaTime);
	UpdateVisibility();

}

void UStaminaWidget::UpdateStamina(float DeltaTime)
{
	
	float CurrentStamina = StaminaComponent->CurrentStamina;
	
	float MaxStamina = StaminaComponent->MaxStamina;

	 float TargetPercent = CurrentStamina / MaxStamina;



	SmoothedPercent = FMath::FInterpTo(SmoothedPercent, TargetPercent, DeltaTime, 8.f);
	StaminaProgressBar->SetPercent(SmoothedPercent);
	
	


}

void UStaminaWidget::UpdateVisibility()
{
	const float Speed = OwnerCharacter->GetVelocity().Size();

	if (Speed > 300.f)
	{
		if (!bIsVisible && FadeIN)
		{
			PlayAnimation(FadeIN);
			bIsVisible = true;
		}
	}
	else
	{
		if (bIsVisible && FadeOut)
		{
			PlayAnimation(FadeOut);
			bIsVisible = false;
		}
	}
}
