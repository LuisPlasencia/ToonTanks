// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class TOONTANKS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

	void HandleDestruction();

protected:
// private no puede ser accedido de fuera de la clase pero protected peuden acceder los hijos como el tank
	void RotateTurret(FVector LootAtTarget);

	void Fire();


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComp;   //FORWARD DECLARATION nos ahorra poner el include (ya que solo es un puntero) pero no podremos usarlo (implementation details), 
	// lo incluiremos en el .cpp porque ahi si que le haremos uso. -> INCLUIR HEADERS SOLO DONDE LAS NECESITAMOS
	// La razón es porque los archivos header pueden ser necesitados en otros archivos así que nos interesa que sean lo más ligeros posibles y así aligerar los tiempos de compilación
	// Como regla general usaremos puntero para todo con la excepción de las variables básicas (int, float...) porque pesan menos y nos ahorra todo
	// UPROPERTY macro necesaria para que en el editor podamos modificar los parametros del blueprint (acepta más de uno) (VisibleAnywhere, EditAnywhere, VisibleInstanceOnly(solamente visible cuando se añade al mundo como instancia - no en el default blueprint), VisibleDefaultsOnly, EditDefaultsOnly, EditInstanceOnly, BlueprintReadWrite(event graph), BlueprintReadOnly...)
	// para hacer una variable visible en el event graph necesitamos que sea pública a no ser que incluyamos meta data y así podemos dejarla privada -> UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	// UPROPERTY también puede añadir categoría en el editor a una variable -> Category = "lola"

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;  // UStaticMehsComponent included by default in all actors - no need to forward declare - #pragma once impide que importemos headers innecesarios o que ya tenemos
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> ProjectileClass;   // Como queremos spawnear una clase deribada de un blueprint, necesitamos el template TSubclassOf para tener una referencia a una UClass, la cual puede hacer uso de unreal reflections (hace de puente entre c++ y blueprint) 
	// En resumen, es una variable C++ que representa un tipo de clase UClass basada en el Projectile blueprint que hemos creado (en el editor lo enlazaremos al BP_Projectile)


	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem* DeathParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class USoundBase *DeathSound;  

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UCameraShakeBase> DeathCameraShakeClass;
};
