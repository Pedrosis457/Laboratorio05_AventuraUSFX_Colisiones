// LABORATORIO 04 - Movimiento 9: caminata a destinos sorteados.

#pragma once

#include "CoreMinimal.h"
#include "Plataforma.h"
#include "PMAleatoria.generated.h"

/**
 * Se dirige a un punto sorteado dentro de su zona; al llegar sortea otro.
 * Es el unico movimiento que no sigue una formula fija: nunca repite recorrido.
 */
UCLASS()
class AVENTURAUSFX_API APMAleatoria : public APlataforma
{
	GENERATED_BODY()

public:
	APMAleatoria();

protected:
	virtual void Mover(float DeltaTime) override;

	virtual void SortearParametros() override;

	void ElegirNuevoDestino();

	UPROPERTY(VisibleAnywhere, Category = "Plataforma|Aleatoria")
	FVector Destino;

	UPROPERTY(EditAnywhere, Category = "Plataforma|Aleatoria")
	float DistanciaDeLlegada;
};
