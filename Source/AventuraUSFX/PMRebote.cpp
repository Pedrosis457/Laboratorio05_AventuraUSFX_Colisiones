// LABORATORIO 04 - Movimiento 11: rebote con gravedad, como una pelota.

#include "PMRebote.h"

APMRebote::APMRebote()
{
	Tipo = TEXT("PMRebote");
	Gravedad = 1800.0f;
	ImpulsoDeRebote = 900.0f;
	AlturaActual = 0.0f;
	VelocidadVertical = 0.0f;
	Amplitud = 60.0f;   // deriva horizontal mientras rebota
	Frecuencia = 0.9f;

	ConfigurarApariencia(FVector(1.70f, 1.70f, 0.40f), FLinearColor(0.98f, 0.90f, 0.30f));
}

void APMRebote::SortearParametros()
{
	Super::SortearParametros();

	Gravedad *= FMath::FRandRange(0.80f, 1.20f);
	ImpulsoDeRebote *= FMath::FRandRange(0.80f, 1.20f);

	// Arranca en el aire, a distinta altura cada una.
	AlturaActual = FMath::FRandRange(0.0f, 250.0f);
	VelocidadVertical = ImpulsoDeRebote;
}

void APMRebote::Mover(float DeltaTime)
{
	// Integracion simple: la velocidad cae con la gravedad...
	VelocidadVertical -= Gravedad * DeltaTime;
	AlturaActual += VelocidadVertical * DeltaTime;

	// ...y al tocar el suelo del ancla se devuelve el impulso.
	if (AlturaActual <= 0.0f)
	{
		AlturaActual = 0.0f;
		VelocidadVertical = ImpulsoDeRebote * FMath::FRandRange(0.90f, 1.05f);
	}

	const float Deriva = FMath::Sin(TiempoEnMovimiento * Frecuencia + Desfase) * Amplitud;

	SetActorLocation(Ancla + Eje * Deriva + FVector(0.0f, 0.0f, AlturaActual));
}
