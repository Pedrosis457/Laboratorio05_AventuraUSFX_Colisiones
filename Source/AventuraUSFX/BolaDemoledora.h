// ============================================================================
//  LABORATORIO 05 - APLICACION 1 DE COLISIONES:  B L O C K
//
//  La bola demoledora es una esfera con fisica real. Su canal de colision
//  responde BLOCK contra las plataformas, y las plataformas responden BLOCK
//  contra ella: ninguna de las dos puede atravesar a la otra.
//
//  De esa colision bloqueante salen dos cosas:
//      1. la respuesta fisica: la bola rebota y sale despedida
//      2. el evento OnComponentHit, que solo existe cuando hay BLOCK
//         (si la respuesta fuera Overlap, este evento NO se dispararia)
//
//  Cada golpe danha la plataforma; al tercero, la plataforma se rompe.
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BolaDemoledora.generated.h"

class UStaticMeshComponent;
class UMaterialInstanceDynamic;

UCLASS()
class AVENTURAUSFX_API ABolaDemoledora : public AActor
{
	GENERATED_BODY()

public:
	ABolaDemoledora();

	virtual void Tick(float DeltaTime) override;

	/** La lanza desde un punto alto con un empujon horizontal sorteado. */
	void Lanzar(const FVector& PuntoDeSalida);

	int32 ObtenerGolpes() const { return GolpesDados; }
	int32 ObtenerRoturas() const { return PlataformasRotas; }

protected:
	virtual void BeginPlay() override;

	/**
	 * EVENTO DE LA COLISION BLOCK.
	 * Lo dispara el motor cuando dos cuerpos que se bloquean llegan a tocarse.
	 */
	UFUNCTION()
	void AlGolpear(UPrimitiveComponent* ComponenteGolpeador, AActor* OtroActor,
		UPrimitiveComponent* OtroComponente, FVector ImpulsoNormal, const FHitResult& Golpe);

	UPROPERTY(VisibleAnywhere, Category = "Bola")
	UStaticMeshComponent* Esfera;

	/** Altura desde la que se la vuelve a soltar cuando se cae del escenario. */
	UPROPERTY(EditAnywhere, Category = "Bola")
	float AlturaDeSalida;

	/** Debajo de esta altura se considera que se salio del mundo. */
	UPROPERTY(EditAnywhere, Category = "Bola")
	float AlturaDeRescate;

	/** Empujon horizontal con el que se la lanza. */
	UPROPERTY(EditAnywhere, Category = "Bola")
	float EmpujeInicial;

	/** Zona sobre la que se la vuelve a soltar (mitad del ancho del escenario). */
	UPROPERTY(EditAnywhere, Category = "Bola")
	float RadioDeCaida;

	UPROPERTY(VisibleAnywhere, Category = "Bola")
	int32 GolpesDados;

	UPROPERTY(VisibleAnywhere, Category = "Bola")
	int32 PlataformasRotas;

	UPROPERTY()
	UMaterialInstanceDynamic* MaterialDinamico;
};
