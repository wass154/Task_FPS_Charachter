// Fill out your copyright notice in the Description page of Project Settings.

#include "TraversalBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "MyTraversalCharachter.h"
#include "StaminaComponent.h"
#include "JumpComponent.h"
#include "VaultComponent.h"

float UTraversalBlueprintLibrary::GetCharacterStaminaNormalized(ACharacter* Character)
{
    if (!Character) return 0.f;

    if (AMyTraversalCharachter* MyChar = Cast<AMyTraversalCharachter>(Character))
    {
        return MyChar->MainComponents->StaminaComp->GetStaminaNormalized();
    }

    return 0.f;
}

bool UTraversalBlueprintLibrary::IsCharacterJumping(ACharacter* Character)
{
    if (!Character) return false;

    if (AMyTraversalCharachter* MyChar = Cast<AMyTraversalCharachter>(Character))
    {
        return MyChar->MainComponents->JumpComp->IsJumping();
    }

    return false;
}

bool UTraversalBlueprintLibrary::IsCharacterVaulting(ACharacter* Character)
{
    if (!Character) return false;

    if (AMyTraversalCharachter* MyChar = Cast<AMyTraversalCharachter>(Character))
    {
        return MyChar->MainComponents->VaultComp->IsVaulting();
    }

    return false;
}

