// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->MaxSpeed = 1300.f;
	ProjectileMovementComponent->InitialSpeed = 1300.f;

	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"));
	TrailParticles->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	if(LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	//auto MyOwner = GetOwner();  // usamos auto ya que no sabemos si viene de base pawn o si podría venir de otra clase así que es mas seguro que el compilador decida por nosotros
	AActor* MyOwner = GetOwner();

	if(MyOwner == nullptr)
	{
		Destroy();
		return;
	}

	// Aunque auto sea útil, es mas seguro declararlo con el tipo que esperamos. De todas formas lo podemos usar para saber lo que nos está devolviendo y cambiarlo más adelante
	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();  // devuelve el controlador 
	UClass* DamageTypeClass = UDamageType::StaticClass();  //  solo necesitamos un UClass value que represente al UDamageType ya que no lo vamos a customizar, es básicamente como el TSubclassOf que creamos para el blueprint pero vacío

	// auto MyOwnerInstigator = MyOwner->GetInstigatorController();  // devuelve el controlador 
	// auto DamageTypeClass = UDamageType::StaticClass();  //  solo necesitamos un UClass value que represente al UDamageType ya que no lo vamos a customizar, es básicamente como el TSubclassOf que creamos para el blueprint pero vacío
	
	if(OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
		if(HitParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
		}

		if(HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		}

		if(HitCameraShakeClass)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
		}

	}
	Destroy();
	
	// UE_LOG(LogTemp, Warning, TEXT("OnHit"));
	// UE_LOG(LogTemp, Warning, TEXT("HitComp: %s"), *HitComp->GetName());  // EL ASTERISCO CONVIERTE DE FSTRING A C STYLE STRING 
	// UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s"), *OtherActor->GetName());  	
	// UE_LOG(LogTemp, Warning, TEXT("OtherComp: %s"), *OtherComp->GetName());	
}
