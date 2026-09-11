// LABORATORIO 04 - Movimiento 7: gira sobre si misma sin desplazarse.

#pragma once

#include "CoreMinimal.h"
#include "Plataforma.h"
#include "PMRotatoria.generated.h"

/** No se traslada: gira sobre su propio eje y cabecea. */
UCLASS()
class AVENTURAUSFX_API APMRotatoria : public APlataforma
{
	GENERATED_BODY()

public:
	APMRotatoria();

protected:
	virtual void Mover(float DeltaTime) override;

	/** Grados por segundo de cabeceo (la plataforma se voltea despacio). */
	UPROPERTY(EditAnywhere, Category = "Plataforma|Rotatoria")
	float VelocidadDeCabeceo;
};
