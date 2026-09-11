// ============================================================================
//  LABORATORIO 05 - APLICACION 2 DE COLISIONES:  O V E R L A P  (begin - end)
//
//  La zona de energia es una esfera de colision que responde OVERLAP: no
//  detiene a nadie, las plataformas la atraviesan libremente, pero el motor
//  avisa cuando entran y cuando salen.
//
//      OnComponentBeginOverlap  ->  la plataforma se energiza:
//                                   toma el color de la zona y se mueve al doble
//      OnComponentEndOverlap    ->  la plataforma se apaga:
//                                   recupera su color y su velocidad normal
//
//  Para que el efecto se vea todo el tiempo, la zona BARRE el escenario de un
//  extremo al otro: a su paso va prendiendo plataformas y dejandolas apagadas
//  detras de ella.
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZonaDeEnergia.generated.h"

class USphereComponent;

UCLASS()
class AVENTURAUSFX_API AZonaDeEnergia : public AActor
{
	GENERATED_BODY()

public:
	AZonaDeEnergia();

	virtual void Tick(float DeltaTime) override;

	/**
	 * Configura la barrida. Se llama ANTES del BeginPlay (spawn diferido).
	 *
	 * @param NuevoCentro     centro del recorrido
	 * @param NuevaDireccion  direccion en la que barre (se normaliza)
	 * @param NuevoAlcance    cuanto se aleja del centro hacia cada lado
	 * @param NuevoColor      color que le presta a las plataformas que cubre
	 */
	void ConfigurarBarrida(const FVector& NuevoCentro, const FVector& NuevaDireccion,
		float NuevoAlcance, const FLinearColor& NuevoColor);

	int32 ObtenerPlataformasDentro() const { return PlataformasDentro; }
	int32 ObtenerEntradas() const { return Entradas; }
	int32 ObtenerSalidas() const { return Salidas; }
	FLinearColor ObtenerColor() const { return ColorDeLaZona; }

protected:
	virtual void BeginPlay() override;

	/** EVENTO OVERLAP - BEGIN: algo acaba de entrar en la zona. */
	UFUNCTION()
	void AlEntrar(UPrimitiveComponent* ComponenteDeLaZona, AActor* OtroActor,
		UPrimitiveComponent* OtroComponente, int32 OtroIndice, bool bDeBarrido,
		const FHitResult& Barrido);

	/** EVENTO OVERLAP - END: algo acaba de salir de la zona. */
	UFUNCTION()
	void AlSalir(UPrimitiveComponent* ComponenteDeLaZona, AActor* OtroActor,
		UPrimitiveComponent* OtroComponente, int32 OtroIndice);

	/** LA ESFERA DE COLISION. Responde Overlap: deja pasar y solo avisa. */
	UPROPERTY(VisibleAnywhere, Category = "Zona")
	USphereComponent* Esfera;

	UPROPERTY(EditAnywhere, Category = "Zona")
	float RadioDeLaZona;

	/** Cuanto mas rapido se mueve una plataforma mientras esta adentro. */
	UPROPERTY(EditAnywhere, Category = "Zona")
	float MultiplicadorDeEnergia;

	// ---------------------------------------------------------------- Barrida

	UPROPERTY(EditAnywhere, Category = "Zona|Barrida")
	FVector CentroDeBarrida;

	UPROPERTY(EditAnywhere, Category = "Zona|Barrida")
	FVector DireccionDeBarrida;

	UPROPERTY(EditAnywhere, Category = "Zona|Barrida")
	float AlcanceDeBarrida;

	UPROPERTY(EditAnywhere, Category = "Zona|Barrida")
	float VelocidadDeBarrida;

	float TiempoDeVida;

	// ---------------------------------------------------------------- Estado

	UPROPERTY(VisibleAnywhere, Category = "Zona|Estado")
	int32 PlataformasDentro;

	UPROPERTY(VisibleAnywhere, Category = "Zona|Estado")
	int32 Entradas;

	UPROPERTY(VisibleAnywhere, Category = "Zona|Estado")
	int32 Salidas;

	FLinearColor ColorDeLaZona;
};
