// LABORATORIO 04 - Movimiento 2: ascensor, sube y baja.

#pragma once

#include "CoreMinimal.h"
#include "Plataforma.h"
#include "PMVertical.generated.h"

/** Sube y baja sobre su propio sitio, como un ascensor. */
UCLASS()
class AVENTURAUSFX_API APMVertical : public APlataforma
{
	GENERATED_BODY()

public:
	APMVertical();

protected:
	virtual void Mover(float DeltaTime) override;
};
