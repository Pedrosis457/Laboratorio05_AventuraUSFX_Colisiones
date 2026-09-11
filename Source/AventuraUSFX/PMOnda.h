// LABORATORIO 04 - Movimiento 12: cabalga una ola.

#pragma once

#include "CoreMinimal.h"
#include "Plataforma.h"
#include "PMOnda.generated.h"

/**
 * Se desplaza de ida y vuelta mientras su altura sigue una onda que viaja:
 * la plataforma sube, baja y se inclina como si surfeara.
 */
UCLASS()
class AVENTURAUSFX_API APMOnda : public APlataforma
{
	GENERATED_BODY()

public:
	APMOnda();

protected:
	virtual void Mover(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Plataforma|Onda")
	float AmplitudVertical;

	/** Largo de la ola en centimetros. */
	UPROPERTY(EditAnywhere, Category = "Plataforma|Onda")
	float LongitudDeOnda;

	/** Velocidad con la que la ola viaja bajo la plataforma. */
	UPROPERTY(EditAnywhere, Category = "Plataforma|Onda")
	float VelocidadDeOnda;
};
