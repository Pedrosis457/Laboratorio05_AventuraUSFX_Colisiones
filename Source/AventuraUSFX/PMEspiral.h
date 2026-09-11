// LABORATORIO 04 - Movimiento 5: espiral que se abre y se cierra.

#pragma once

#include "CoreMinimal.h"
#include "Plataforma.h"
#include "PMEspiral.generated.h"

/** Gira mientras su radio crece y decrece: una espiral que respira. */
UCLASS()
class AVENTURAUSFX_API APMEspiral : public APlataforma
{
	GENERATED_BODY()

public:
	APMEspiral();

protected:
	virtual void Mover(float DeltaTime) override;

	virtual void SortearParametros() override;

	/** Cada cuanto se abre y se cierra la espiral. */
	UPROPERTY(EditAnywhere, Category = "Plataforma|Espiral")
	float FrecuenciaRadial;
};
