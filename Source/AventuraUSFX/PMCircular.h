// LABORATORIO 04 - Movimiento 3: orbita circular horizontal.

#pragma once

#include "CoreMinimal.h"
#include "Plataforma.h"
#include "PMCircular.generated.h"

/** Gira en circulo alrededor de su ancla, siempre a la misma altura. */
UCLASS()
class AVENTURAUSFX_API APMCircular : public APlataforma
{
	GENERATED_BODY()

public:
	APMCircular();

protected:
	virtual void Mover(float DeltaTime) override;
};
