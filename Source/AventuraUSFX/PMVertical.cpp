// LABORATORIO 04 - Movimiento 2: ascensor, sube y baja.

#include "PMVertical.h"

APMVertical::APMVertical()
{
	Tipo = TEXT("PMVertical");
	Amplitud = 480.0f;
	Frecuencia = 1.1f;

	ConfigurarApariencia(FVector(2.20f, 2.20f, 0.35f), FLinearColor(0.95f, 0.75f, 0.10f));
}

void APMVertical::Mover(float /*DeltaTime*/)
{
	// (0.5 + 0.5*sen) mantiene la altura entre 0 y Amplitud: nunca baja del ancla.
	const float Altura = (0.5f + 0.5f * FMath::Sin(TiempoEnMovimiento * Frecuencia + Desfase)) * Amplitud;

	SetActorLocation(Ancla + FVector(0.0f, 0.0f, Altura));
}
