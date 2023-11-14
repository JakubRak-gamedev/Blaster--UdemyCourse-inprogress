// Fill out your copyright notice in the Description page of Project Settings.


#include "Casing.h"


ACasing::ACasing()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>("CasingMesh");
	SetRootComponent(CasingMesh);
	CasingMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	CasingMesh->SetSimulatePhysics(true);
	CasingMesh->SetEnableGravity(true);
	ShellEjectionImpulse = 10.f;
}


void ACasing::BeginPlay()
{
	Super::BeginPlay();
	CasingMesh->AddImpulse(GetActorForwardVector() * ShellEjectionImpulse);
	SetLifeSpan(10.f);
}



