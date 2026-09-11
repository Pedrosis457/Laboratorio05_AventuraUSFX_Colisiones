// LABORATORIO 04 - Movimiento 8: zigzag de esquinas marcadas.

#pragma once

#include "CoreMinimal.h"
#include "Plataforma.h"
#include "PMZigzag.generated.h"

/** Avanza en zigzag: cambia de direccion de golpe, sin curvas. */
UCLASS()
class AVENTURAUSFX_API APMZigzag : public APlataforma
{
	GENERATED_BODY()

public:
	APMZigzag();

protected:
	virtual void Mover(float DeltaTime) override;

	/** Frecuencia del vaiven transversal: es lo que produce el zigzag. */
	UPROPERTY(EditAnywhere, Category = "Plataforma|Zigzag")
	float FrecuenciaTransversal;
};
