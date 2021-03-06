// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TheFarm51TestCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "LineTrace.h"
#include "InteractableActor.h"
#include "Inventory.h"
#include "WeaponBase.h"
#include "Components/Widget.h"
#include "AHUD.h"
#include "Net/UnrealNetwork.h"

//////////////////////////////////////////////////////////////////////////
// ATheFarm51TestCharacter

ATheFarm51TestCharacter::ATheFarm51TestCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	LineTraceComp = CreateDefaultSubobject<ULineTrace>("LineTraceComponent");
	Inventory = CreateDefaultSubobject<UInventory>("InventoryComponent");
	WeaponEquiped = nullptr;
	bItemInteractable = false;
	ItemInteractionText = FText::FromString("Press F to pickup");
	//GameHUD = CreateDefaultSubobject<AAHUD>("GameHUD");
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATheFarm51TestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATheFarm51TestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATheFarm51TestCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATheFarm51TestCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATheFarm51TestCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ATheFarm51TestCharacter::Interact);

}


void ATheFarm51TestCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATheFarm51TestCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATheFarm51TestCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATheFarm51TestCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ATheFarm51TestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TraceForPickup();
}

void ATheFarm51TestCharacter::TraceForPickup()
{
	FVector Start = GetMesh()->GetBoneLocation(FName("head"));
	FVector End = Start + FollowCamera->GetForwardVector() * 200.0f;
	AActor* Actor = LineTraceComp->LineTraceSingle(Start,End);
	if (Actor)
	{
		if (AInteractableActor* Interactable = Cast<AInteractableActor>(Actor))
		{
			if (Interactable->InteractableType == EInteractableType::Statue)
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit Actor Statue"));
			}
			else if (Interactable->InteractableType == EInteractableType::UE4Statue)
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit Actor UE4Statue"));
			}
			//show hud
			FString ItemName = (TEXT("%s"), *Actor->GetName());
			FString ItemText = FString("Press F to pickup " + ItemName);
			ItemInteractionText = FText::FromString(ItemText);
			bItemInteractable = true;
		}
		if (AWeaponBase* WeaponToPick = Cast<AWeaponBase>(Actor))
		{
			FString ItemName = (TEXT("%s"), *Actor->GetName());
			FString ItemText = FString("Press F to pickup " + ItemName);
			ItemInteractionText = FText::FromString(ItemText);
			bItemInteractable = true;
		}
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor %s"), *Actor->GetName());
	}
	else
	{
		bItemInteractable = false;
	}
}

void ATheFarm51TestCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATheFarm51TestCharacter, Weapon);
	DOREPLIFETIME(ATheFarm51TestCharacter, WeaponEquiped);
}

void ATheFarm51TestCharacter::Interact()
{
	ServerInteract();
}

bool ATheFarm51TestCharacter::ServerInteract_Validate()
{
	return true;
}

void ATheFarm51TestCharacter::ServerInteract_Implementation()
{
	if(Role == ROLE_Authority){
		FVector Start = GetMesh()->GetBoneLocation(FName("head"));
		FVector End = Start + FollowCamera->GetForwardVector() * 200.0f;
		AActor* Actor = LineTraceComp->LineTraceSingle(Start, End);
		if (Actor)
		{
			if (AInteractableActor* Interactable = Cast<AInteractableActor>(Actor))
			{
				Inventory->AddItem(Interactable);
			}
			else if (AWeaponBase* WeaponToPick = Cast<AWeaponBase>(Actor))
			{
				Weapon = WeaponToPick;
				WeaponName = WeaponToPick->DefaultWeaponName;
				On_Rep_WeaponInteracted();
			}
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor %s"), *Actor->GetName());
		}
	}
}

void ATheFarm51TestCharacter::On_Rep_WeaponInteracted()
{
	if (Weapon)
	{
		if (!bWeaponEqupiped) {
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Grip_R"));
			Weapon->SetActorEnableCollision(false);
			bWeaponEqupiped = true;
		}
		else 
		{
			WeaponEquiped = Weapon;
			WeaponEquiped->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName("Back_Socket"));
			WeaponEquiped->SetActorEnableCollision(false);
		}
		
	}
	else
	{

	}
}

UInventory* ATheFarm51TestCharacter::GetInventoryComp()
{
	return Inventory;
}
