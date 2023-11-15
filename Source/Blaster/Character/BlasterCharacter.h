// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Blaster/Interfaces/InteractWithCrosshairsInterface.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "BlasterCharacter.generated.h"

class ABlasterPlayerController;
class UCombatComponent;
class UWidgetComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class AWeapon;
class UAnimMontage;
class AController;

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:

	ABlasterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	void PlayFireMontage(bool bAiming);
	void PlayElimMontage();
	
	virtual void OnRep_ReplicatedMovement() override;
	
	UFUNCTION(NetMulticast, Reliable)
	void Elim();

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* BlasterContext;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* EquipAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* AimAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* FireAction;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EquipButtonPressed();
	void CrouchButtonPressed();
	void AimButtonPressed();
	void AimButtonReleased();
	virtual void Jump() override;
	void FireButtonPressed();
	void FireButtonReleased();
	void CalculateAO_Pitch();

	void AimOffset(float DeltaTime);
	void SimProxiesTurn();
	
	void PlayHitReactMontage();

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,AController* InstigatorController, AActor* DamageCauser);

	void UpdateHUDHealth();
private:
	UPROPERTY(VisibleAnywhere, Category="Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category="Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* OverheadWidget;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	AWeapon* OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	UPROPERTY(VisibleAnywhere)
	UCombatComponent* Combat;

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;
	
	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);

	UPROPERTY(EditAnywhere, Category="Combat")
	UAnimMontage* FireWeaponMontage;

	UPROPERTY(EditAnywhere, Category="Combat")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category="Combat")
	UAnimMontage* ElimMontage;
	
	void HideCameraIfCharacterClose();

	UPROPERTY(EditAnywhere)
	float CameraThreshold = 200.f;

	bool bRotateRootBone;
	float TurnThreshold = 45.f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;
	float CalculateSpeed();

	/*
	 * Player health
	 */
	UPROPERTY(EditAnywhere, Category="Player Stats")
	float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category="Player Stats")
	float Health = 100.f;

	UFUNCTION()
	void OnRep_Health();

	ABlasterPlayerController* BlasterPlayerController;

	bool bElimmed = false;
public:
	void SetOverlappingWeapon(AWeapon* Weapon);
	bool isWeaponEquipped();
	bool isAiming();

	FORCEINLINE float GetAO_Yaw() const {return AO_Yaw;};
	FORCEINLINE float GetAO_Pitch() const {return AO_Pitch;};
	AWeapon* GetEquippedWeapon();
	FORCEINLINE ETurningInPlace GetTurningInPlace() const {return TurningInPlace;};
	FVector GetHitTarget() const;
	FORCEINLINE UCameraComponent* GetFollowCamera() const {return FollowCamera;};
	FORCEINLINE bool ShouldRotateRootBone() const {return bRotateRootBone;};
	FORCEINLINE bool IsElimmed() const {return bElimmed;};
};




